//
// Created by neo on 2019-07-19.
//

#include "ShapeResultBloberizer.h"

namespace AtomGraphics {

void ShapeResultBloberizer::FillGlyphs(const ShapeResult *result, float offset) {
    add(result->glyph(), result->fontData().get(), offset);
}

void ShapeResultBloberizer::add(Glyph glyph, const FontData *font_data, float h_offset) {

    if (font_data != pending_font_data_) {
        CommitPendingRun();
# if PLATFORM(IOS)
        CommitPendingBlob();
#endif
        pending_font_data_ = font_data;
    }

    pending_glyphs_.push_back(glyph);
    pending_offsets_.push_back(h_offset);
}

const std::vector<ShapeResultBloberizer::BlobInfo> &ShapeResultBloberizer::Blobs() {
    CommitPendingRun();
    CommitPendingBlob();

    return blobs_;
}

void ShapeResultBloberizer::CommitPendingRun() {
    if (!pending_glyphs_.size()) {
        return;
    }
# if PLATFORM(ANDROID)
    SkPaint run_font;
    run_font.setTextEncoding(SkPaint::kGlyphID_TextEncoding);
    pending_font_data_->SetupPaintFont(&run_font);

    const auto run_size = pending_glyphs_.size();
    const auto &buffer = builder_.AllocRunPosH(run_font, pending_font_data_->paintTypeface(), run_size, 0);


    std::copy(pending_glyphs_.begin(), pending_glyphs_.end(), buffer.glyphs);
    std::copy(pending_offsets_.begin(), pending_offsets_.end(), buffer.pos);
#elif PLATFORM(IOS)
    const auto run_size = pending_glyphs_.size();
    const auto &buffer = builder_.AllocRunPos(pending_font_data_->paintTypeface().CTFont().get(), pending_font_data_->paintTypeface(), run_size);

    std::copy(pending_glyphs_.begin(), pending_glyphs_.end(), buffer.glyphs);

    CGPoint *pos = buffer.pos;
    for (int i = 0; i < run_size; i++) {
        pos[i].x = pending_offsets_[i];
        pos[i].y = 0;
    }

#endif

    builder_run_count_ += 1;
    pending_glyphs_.clear();
    pending_offsets_.clear();
}

void ShapeResultBloberizer::CommitPendingBlob() {
    if (!builder_run_count_)
        return;

    blobs_.emplace_back(builder_.TakeTextBlob());
    builder_run_count_ = 0;
}

}
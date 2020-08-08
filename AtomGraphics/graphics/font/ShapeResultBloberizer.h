//
// Created by neo on 2019-07-19.
//

#ifndef ATOMGRAPHICS_SHAPERESULTBLOBERIZER_H
#define ATOMGRAPHICS_SHAPERESULTBLOBERIZER_H

#include "FontData.h"
#include <vector>
#include <stddef.h>
#include "base/TypeDefine.h"
#include "PaintTextBlobBuilder.h"
#include "graphics/font/Font.h"
#include "PaintTextBlob.h"


namespace AtomGraphics {

class ShapeResultBloberizer {

public:

    void FillGlyphs(const ShapeResult *result, float offset);

    void add(Glyph glyph, const FontData *font_data, float offset);

    struct BlobInfo {
        BlobInfo(scoped_refptr <PaintTextBlob> b)
                : blob(std::move(b)) {}

        scoped_refptr <PaintTextBlob> blob;
    };

    const std::vector <BlobInfo> &Blobs();

private:

    const FontData *pending_font_data_ = nullptr;

    size_t builder_run_count_ = 0;

    PaintTextBlobBuilder builder_;
    std::vector <Glyph> pending_glyphs_;
    std::vector<float> pending_offsets_;

    std::vector <BlobInfo> blobs_;

    void CommitPendingRun();

    void CommitPendingBlob();

};

}

#endif //ATOMGRAPHICS_SHAPERESULTBLOBERIZER_H

//
// Created by neo on 2018/8/29.
//

#include "graphics/font/FontData.h"
#include <skia/core/SkPaint.h>


namespace AtomGraphics {

void FontData::platformInit() {
    SkPaint paint;
    SetupPaintFont(&paint);
    SkPaint::FontMetrics metrics;
    paint.getFontMetrics(&metrics);
    metrics_ = metrics;
}

void FontData::SetupPaintFont(SkPaint *paint) const {
    paint->setAntiAlias(true);
    paint->setLCDRenderText(false);
    paint->setHinting(SkPaint::kNo_Hinting);
    paint->setEmbeddedBitmapText(false);
    paint->setAutohinted(false);
    const float ts = text_size_ >= 0 ? text_size_ : 12;
    paint->setTextSize(SkFloatToScalar(ts));
    paint->setTypeface(paint_typeface_.ToSkTypeface());
    paint->setSubpixelText(true);
}

}
//
// Created by neo on 2019-07-17.
//

#ifndef ATOMGRAPHICS_FONTDATA_H
#define ATOMGRAPHICS_FONTDATA_H


#include "memory/RefCounted.h"
#include "FontStyle.h"
#include "FontMetrics.h"
#include "PaintTypeface.h"

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkPaint.h>

#endif


namespace AtomGraphics {

class FontData : public RefCounted<FontData> {

public:

    FontData(const PaintTypeface &,
             const std::string &family,
             float text_size,
             size_t weight,
             Slant slant);


    const PaintTypeface &paintTypeface() const {
        return paint_typeface_;
    }


    const FontMetrics &fontMetrics() const {
        return metrics_;
    }


#if PLATFORM(ANDROID)

    void SetupPaintFont(SkPaint *paint) const;

#endif


private:

    PaintTypeface paint_typeface_;
    FontMetrics metrics_;
    std::string family_;
    float text_size_;
    size_t weight_;
    Slant slant_;


    void platformInit();

};

}


#endif //ATOMGRAPHICS_FONTDATA_H

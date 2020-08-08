//
// Created by neo on 2019-07-19.
//

#ifndef ATOMGRAPHICS_PAINTTEXTBLOBBUILDER_H
#define ATOMGRAPHICS_PAINTTEXTBLOBBUILDER_H

#include "PaintTextBlob.h"
#include "memory/ScopedRefPtr.h"
#include "ShapeResult.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkTextBlob.h>

#endif

#if PLATFORM(IOS)

#import "graphics/font/ios/TextBlobBuilder.h"

#endif

namespace AtomGraphics {

class PaintTextBlobBuilder {

public:

#if PLATFORM(ANDROID)

    using RunBuffer = SkTextBlobBuilder::RunBuffer;

#endif

#if PLATFORM(IOS)

    using RunBuffer = TextBlobBuilder::RunBuffer;

#endif

    PaintTextBlobBuilder();

    ~PaintTextBlobBuilder();

    scoped_refptr<PaintTextBlob> TakeTextBlob();


#if PLATFORM(ANDROID)

    // These functions pass the calls through to SkTextBlobBuilder, see its
    // interface for details.
    const RunBuffer &AllocRunPosH(const SkPaint &font,
                                  const PaintTypeface &typeface,
                                  int count,
                                  SkScalar y,
                                  const SkRect *bounds = nullptr);

    const RunBuffer &AllocRunPos(const SkPaint &font,
                                 const PaintTypeface &typeface,
                                 int count,
                                 const SkRect *bounds = nullptr);

#endif

#if PLATFORM(IOS)

    const PaintTextBlobBuilder::RunBuffer &AllocRunPos(CTFontRef font, const PaintTypeface &typeface, int count);

#endif


private:
    std::vector<PaintTypeface> typefaces_;

#if PLATFORM(ANDROID)

    SkTextBlobBuilder sk_builder_;

#elif PLATFORM(IOS)

    TextBlobBuilder builder_;

#endif

};

}


#endif //ATOMGRAPHICS_PAINTTEXTBLOBBUILDER_H

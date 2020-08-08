//
// Created by neo on 2019-07-09.
//

#ifndef ATOMGRAPHICS_WORDSHAPER_H
#define ATOMGRAPHICS_WORDSHAPER_H

#include "Font.h"

namespace AtomGraphics {

class WordShaper {

public:

    WordShaper(const Font &font);

    scoped_refptr<const ShapeResult> shapeCharacter(UChar c);

private:

    const Font &font_;

    scoped_refptr<const ShapeResult> shape(UChar unicode);

#if PLATFORM(ANDROID)

    scoped_refptr<const ShapeResult> shapeSingleWord(char *ch, ShapeResult *result, const FontData *font_data);

#elif PLATFORM(IOS)

    scoped_refptr<const ShapeResult> shapeSingleWord(UChar character, ShapeResult *result, const FontData *font_data);

#endif

    scoped_refptr<const ShapeResult> shapeWithSystemFonts(
            const Font & font,
            ShapeResult *result,
            UChar unicode,
            char *c);
};

}


#endif //ATOMGRAPHICS_WORDSHAPER_H

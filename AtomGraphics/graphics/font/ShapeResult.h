//
// Created by neo on 2019-07-09.
//

#ifndef ATOMGRAPHICS_SHAPERESULT_H
#define ATOMGRAPHICS_SHAPERESULT_H

#include "base/TypeDefine.h"
#include "memory/RefCounted.h"
#include "FontData.h"

namespace AtomGraphics {

class ShapeResult : public RefCounted<ShapeResult> {

public:

    Glyph glyph() const {
        return glyph_;
    }

    UChar unicode() const {
        return unicode_;
    }


    const scoped_refptr<const FontData> &fontData() const {
        return primary_font_;
    }

    float getAdvance() const {
        return advance_;
    }

private:
    scoped_refptr<const FontData> primary_font_;

    Glyph glyph_;
    UChar unicode_;
    float advance_;

    friend class WordShaper;

    friend class Font;
};

}


#endif //ATOMGRAPHICS_SHAPERESULT_H

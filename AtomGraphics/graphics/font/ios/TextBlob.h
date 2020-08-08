//
// Created by neo on 2019-08-28.
//

#ifndef ATOMGRAPHICS_TEXTBLOB_H
#define ATOMGRAPHICS_TEXTBLOB_H

#import <CoreText/CoreText.h>
#import "base/TypeDefine.h"
#import "memory/RefCounted.h"

namespace AtomGraphics {

class TextBlob final : public RefCounted<TextBlob> {

public:

    TextBlob(CTFontRef font, int glyphCount, Glyph *glyphs, CGPoint *pos);

    ~TextBlob();

    CTFontRef font() const;

    int glyphCount() const;

    Glyph *glyphs() const;

    CGPoint *pos() const;

private:

    CTFontRef m_font{nullptr};
    int m_glyphCount{0};
    Glyph *m_glyphs;
    CGPoint *m_pos;

};

}

#endif //ATOMGRAPHICS_TEXTBLOB_H

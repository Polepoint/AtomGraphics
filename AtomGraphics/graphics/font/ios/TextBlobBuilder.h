//
// Created by neo on 2019-08-28.
//

#ifndef ATOMGRAPHICS_TEXTBLOBBUILDER_H
#define ATOMGRAPHICS_TEXTBLOBBUILDER_H


#import "TypeDefine.h"
#import "memory/ScopedRefPtr.h"
#import "TextBlob.h"
#import <CoreGraphics/CGGeometry.h>
#import <CoreText/CTFont.h>

namespace AtomGraphics {

class TextBlobBuilder {

public:

    struct RunBuffer {
        Glyph *glyphs;
        CGPoint *pos;
    };

    ~TextBlobBuilder();

    const RunBuffer &allocRunPos(CTFontRef font, int count);

    scoped_refptr<TextBlob> make();

private:

    CTFontRef m_font{nullptr};
    RunBuffer m_currentRunBuffer;
    int m_glyphCount{0};

    void resetBuffer(bool freeBuffer);

};

}


#endif //ATOMGRAPHICS_TEXTBLOBBUILDER_H

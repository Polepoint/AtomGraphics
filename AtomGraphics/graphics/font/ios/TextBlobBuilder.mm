//
// Created by neo on 2019-08-28.
//

#include "TextBlobBuilder.h"


namespace AtomGraphics {

TextBlobBuilder::~TextBlobBuilder() {
    if (m_font) {
        CFRelease(m_font);
    }
}

const TextBlobBuilder::RunBuffer &TextBlobBuilder::allocRunPos(CTFontRef font, int count) {
    resetBuffer(true);
    m_glyphCount = count;
    m_font = font;
    CFRetain(m_font);
    if (count) {
        m_currentRunBuffer.glyphs = static_cast<Glyph *>(malloc(sizeof(Glyph) * count));
        m_currentRunBuffer.pos = static_cast<CGPoint *>(malloc(sizeof(CGPoint) * count));
    }

    return m_currentRunBuffer;
}

scoped_refptr<TextBlob> TextBlobBuilder::make() {
    scoped_refptr<TextBlob> blob = MakeRefCounted<TextBlob>(m_font, m_glyphCount, m_currentRunBuffer.glyphs, m_currentRunBuffer.pos);
    resetBuffer(false);
    return blob;
}

void TextBlobBuilder::resetBuffer(bool freeBuffer) {
    if (m_glyphCount && freeBuffer) {
        free(m_currentRunBuffer.glyphs);
        free(m_currentRunBuffer.pos);
    }

    m_currentRunBuffer.glyphs = nullptr;
    m_currentRunBuffer.pos = nullptr;
    m_glyphCount = 0;
    if (m_font) {
        CFRelease(m_font);
    }
    m_font = nullptr;
}

}
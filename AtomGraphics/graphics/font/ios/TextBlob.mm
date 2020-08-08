//
// Created by neo on 2019-08-28.
//

#include "TextBlob.h"

namespace AtomGraphics {

TextBlob::TextBlob(CTFontRef font, int glyphCount, Glyph *glyphs, CGPoint *pos)
        : m_font(font), m_glyphCount(glyphCount), m_glyphs(glyphs), m_pos(pos) {
    CFRetain(m_font);
}

TextBlob::~TextBlob() {
    CFRelease(m_font);
    if (m_glyphs) {
        free(m_glyphs);
    }
    if (m_pos) {
        free(m_pos);
    }
}

CTFontRef TextBlob::font() const {
    return m_font;
}

int TextBlob::glyphCount() const {
    return m_glyphCount;
}

Glyph *TextBlob::glyphs() const {
    return m_glyphs;
}

CGPoint *TextBlob::pos() const {
    return m_pos;
}

}

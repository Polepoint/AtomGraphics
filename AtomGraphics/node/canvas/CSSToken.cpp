//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "CSSToken.h"
#include <string>

namespace AtomGraphics {

CSSToken::CSSToken(const char *originString, unsigned start, unsigned end)
        : m_originString(originString), m_start(start), m_end(end) {
    truncateWhiteSpace();
    resolve();
}

void CSSToken::truncateWhiteSpace() {
    while (m_start < m_end && m_originString[m_start] == ' ')
        m_start++;

    while (m_start < m_end && m_originString[m_end] == ' ')
        m_end--;
}

void CSSToken::resolve() {
    unsigned pos = m_start;

    if (m_end > 0) {
        if (*m_originString == '-' && m_end > 1 && isNumberChar(m_originString[1])) {
            pos++;
        }
        while (pos <= m_end && isNumberChar(m_originString[pos])) {
            pos++;
        }
    }

    if (pos == m_start) {
        m_tokenType = StringToken;
    } else {
        m_tokenType = NumberToken;
        if (pos == m_end) {
            if (isNumberChar(m_originString[pos])) {
                m_uniType = CSS_NUMBER;
            } else {
                char lastChar = m_originString[pos];
                if (lastChar == '%') {
                    m_uniType = CSS_PERCENTAGE;
                }
            }
        } else {
            if (m_end - pos == 1) {
                if (strncasecmp(m_originString + pos, "px", 2) == 0) {
                    m_uniType = CSS_PX;
                }
            }
        }
    }
}

}
//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CSSTOKEN_H
#define ATOMGRAPHICS_CSSTOKEN_H


#include "CSSTypes.h"

namespace AtomGraphics {

enum CSSTokenType {
    StringToken,
    NumberToken
};

class CSSToken {

public:
    
    CSSToken(const char *originString, unsigned start, unsigned end);
    
    CSSTokenType type() const {
        return m_tokenType;
    }
    
    unsigned int stringStart() const {
        return m_start;
    }
    
    unsigned int stringEnd() const {
        return m_end;
    }
    
    UnitType unitType() const {
        return m_uniType;
    }
    
    const char *originString() const {
        return m_originString;
    }

private:
    
    const char *m_originString{nullptr};
    
    CSSTokenType m_tokenType;
    UnitType m_uniType{CSS_UNKNOWN};
    unsigned m_start;
    unsigned m_end;
    
    void resolve();
    
    void truncateWhiteSpace();
    
    bool isNumberChar(const char c) {
        return c >= '0' && c <= '9';
    }
};

}

#endif //ATOMGRAPHICS_CSSTOKEN_H

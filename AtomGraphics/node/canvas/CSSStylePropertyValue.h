//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CSSSTYLEPROPERTYVALUE_H
#define ATOMGRAPHICS_CSSSTYLEPROPERTYVALUE_H

#include <string>
#include "CSSTypes.h"

namespace AtomGraphics {

class CSSStylePropertyValue final {

public:
    
    CSSStylePropertyValue(const char *stringValue);
    
    CSSStylePropertyValue(double numberValue, UnitType unitType = UnitType::CSS_NUMBER);
    
    CSSStylePropertyValue(const CSSStylePropertyValue &other);
    
    ~CSSStylePropertyValue();
    
    bool isNumber() const {
        return m_isNumber;
    }
    
    bool isString() const {
        return m_isString;
    }
    
    double toNumber() const {
        return m_numberValue;
    }
    
    const char *toString();
    
    UnitType unitType() const {
        return m_unitType;
    }
    
    CSSStylePropertyValue &operator=(const CSSStylePropertyValue &other);
    
    bool operator==(const CSSStylePropertyValue &other);
    
    bool operator!=(const CSSStylePropertyValue &other);

private:
    
    UnitType m_unitType;
    
    char *m_stringValue{nullptr};
    double m_numberValue;

    bool m_isNumber{false};
    bool m_isString{false};
};

}


#endif //ATOMGRAPHICS_CSSSTYLEPROPERTYVALUE_H

//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <cstring>
#include "CSSStylePropertyValue.h"
#include "base/StringConversion.h"

namespace AtomGraphics {

CSSStylePropertyValue::CSSStylePropertyValue(const char *stringValue)
        : m_isString(true) {
    m_stringValue = new char[strlen(stringValue) + 1];
    strcpy(m_stringValue, stringValue);
}

CSSStylePropertyValue::CSSStylePropertyValue(double numberValue, UnitType unitType)
        : m_numberValue(numberValue), m_isNumber(true), m_unitType(unitType) {
    
}

CSSStylePropertyValue::CSSStylePropertyValue(const CSSStylePropertyValue &other) {
    m_isString = other.m_isString;
    m_isNumber = other.m_isNumber;
    m_numberValue = other.m_numberValue;
    m_unitType = other.m_unitType;
    if (m_isString) {
        m_stringValue = new char[strlen(other.m_stringValue) + 1];
        strcpy(m_stringValue, other.m_stringValue);
    }
}


CSSStylePropertyValue::~CSSStylePropertyValue() {
    if (m_stringValue) {
        delete[] m_stringValue;
    }
}

const char *CSSStylePropertyValue::toString() {
    if (m_isNumber && !m_stringValue) {
        char *numberString = StringConversion::doubleToString(m_numberValue);
        if (m_unitType == CSS_NUMBER) {
            m_stringValue = numberString;
        } else if (m_unitType == CSS_PX) {
            size_t length = strlen(numberString) + 3;
            m_stringValue = new char[length];
            m_stringValue[length - 1] = '\0';
            sprintf(m_stringValue, "%spx", numberString);
            delete[] numberString;
        }
    }
    
    return m_stringValue;
}

CSSStylePropertyValue &CSSStylePropertyValue::operator=(const CSSStylePropertyValue &other) {
    m_isString = other.m_isString;
    m_isNumber = other.m_isNumber;
    m_numberValue = other.m_numberValue;
    m_unitType = other.m_unitType;
    if (m_isString) {
        if (m_stringValue) {
            delete[]m_stringValue;
        }
        m_stringValue = new char[strlen(other.m_stringValue) + 1];
        strcpy(m_stringValue, other.m_stringValue);
    }
    return *this;
}

bool CSSStylePropertyValue::operator==(const CSSStylePropertyValue &other) {
    if (m_isNumber && other.m_isNumber) {
        return m_numberValue == other.m_numberValue;
    }
    
    if (m_isString && other.m_isString) {
        return std::strcmp(m_stringValue, other.m_stringValue) == 0;
    }
    
    return false;
}


bool CSSStylePropertyValue::operator!=(const CSSStylePropertyValue &other) {
    return !(*this == other);
}

}

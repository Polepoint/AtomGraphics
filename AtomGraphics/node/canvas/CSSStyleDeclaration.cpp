//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "CSSStyleDeclaration.h"
#include "CSSParser.h"

namespace AtomGraphics {

CSSStyleDeclaration::CSSStyleDeclaration(const CSSStylePropertyChangedCallback propertyChangedCallback, void *userData)
        : m_propertyChangedCallback(propertyChangedCallback),m_userData(userData) {
    
}

CSSStyleDeclaration::~CSSStyleDeclaration() {
    if (m_cssText) {
        delete[]m_cssText;
    }
}

const char *CSSStyleDeclaration::cssStyleText() {
    if (!m_cssText) {
        m_cssText = CSSParser::formatCSSString(*this);
    }
    
    return m_cssText;
}

void CSSStyleDeclaration::setCssStyleText(const char *styleText) {
    CSSParser::parseCSSString(styleText, *this);
}

void CSSStyleDeclaration::setProperty(const std::string &propertyName, const CSSStylePropertyValue &value) {
    auto it = m_properties.find(propertyName);
    if (it == m_properties.end()) {
        m_properties.insert(std::make_pair(propertyName, value));
        if (m_propertyChangedCallback) {
            m_propertyChangedCallback(propertyName.c_str(), value, m_userData);
        }
    } else {
        if (it->second != value) {
            it->second = value;
            if (m_propertyChangedCallback) {
                m_propertyChangedCallback(propertyName.c_str(), value, m_userData);
            }
        }
    }
    resetText();
}

bool CSSStyleDeclaration::getProperty(std::string propertyName, CSSStylePropertyValue **value) {
    auto it = m_properties.find(propertyName);
    if (it == m_properties.end()) {
        return false;
    }
    
    *value = &(it->second);
    
    return true;
}

void CSSStyleDeclaration::removeProperty(std::string propertyName) {
    m_properties.erase(propertyName);
    resetText();
}

void CSSStyleDeclaration::resetText() {
    if (m_cssText) {
        delete[] m_cssText;
        m_cssText = nullptr;
    }
}

}
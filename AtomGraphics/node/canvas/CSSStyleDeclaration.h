//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CSSSTYLEDECLARATION_H
#define ATOMGRAPHICS_CSSSTYLEDECLARATION_H

#include <map>
#include "CSSStylePropertyValue.h"

namespace AtomGraphics {

typedef void (*CSSStylePropertyChangedCallback)(const char *propertyName, const CSSStylePropertyValue &, void *);

class CSSStyleDeclaration final {

public:
    
    CSSStyleDeclaration(const CSSStylePropertyChangedCallback propertyChangedCallback, void *userData);
    
    ~CSSStyleDeclaration();
    
    const char *cssStyleText();
    
    void setCssStyleText(const char *);
    
    void setProperty(const std::string &propertyName, const CSSStylePropertyValue &value);
    
    bool getProperty(std::string propertyName, CSSStylePropertyValue **value);
    
    void removeProperty(std::string propertyName);
    
    size_t length() {
        return m_properties.size();
    }
    
    const std::map<const std::string, CSSStylePropertyValue> &properties() const {
        return m_properties;
    }

private:
    
    char *m_cssText{nullptr};
    std::map<const std::string, CSSStylePropertyValue> m_properties;
    
    CSSStylePropertyChangedCallback m_propertyChangedCallback{nullptr};
    void *m_userData{nullptr};

    void resetText();
    
};

}

#endif //ATOMGRAPHICS_CSSSTYLEDECLARATION_H

//
// Created by neo on 2018/8/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CSSPARSER_H
#define ATOMGRAPHICS_CSSPARSER_H


#include "graphics/font/Font.h"
#include "graphics/font/FontDescription.h"
#include "CSSStyleDeclaration.h"
#include "CSSToken.h"
#include "base/AtomTypes.h"
#include <string>

namespace AtomGraphics {

class CSSParser {

public:

    static bool parseCSSString(const std::string &cssString, FontDescription &fontDescription);

    static std::string fontToCSSConfig(Font &);

    static std::unique_ptr<CSSStylePropertyValue> tokenToCSSPropertyValue(const CSSToken &token);

    static bool parseCSSString(const char *, CSSStyleDeclaration &);

    static char *formatCSSString(const CSSStyleDeclaration &);

    static Color4F parseCSSColor(const char *);

    static bool parseRepetitionType(const char *type, bool &repeatX, bool &repeatY);
};

}


#endif //ATOMGRAPHICS_CSSPARSER_H

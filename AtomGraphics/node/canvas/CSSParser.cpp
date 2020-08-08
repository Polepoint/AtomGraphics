//
// Created by neo on 2018/8/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <vector>
#include <cstdio>
#include "CSSParser.h"
#include "CSSToken.h"
#include "base/StringConversion.h"

namespace AtomGraphics {

#define CSS_STYLE_SEPARATE_TOKEN  ' '

#define CSS_FAMILY_NAME_SEPARATE_TOKEN  ','

inline void splitCSSToken(const char *rawString, std::vector<std::string> &resultTokens, const char separateToken) {
    size_t length = std::strlen(rawString);
    if (length == 0) {
        return;
    }
    char *tmpStr = new char[length + 1];
    int start_pos = 0;
    for (int i = 0; i < length; i++) {
        char c = rawString[i];
        if (c == separateToken) {
            if (i > start_pos) {
                std::strncpy(tmpStr, &rawString[start_pos], static_cast<size_t>(i - start_pos));
                tmpStr[i - start_pos] = '\0';
                resultTokens.push_back(std::string(tmpStr));
            }
            start_pos = i + 1;
        }
    }

    if (start_pos < length) {
        std::strncpy(tmpStr, &rawString[start_pos], static_cast<size_t>(length - start_pos));
        tmpStr[length - start_pos] = '\0';
        resultTokens.push_back(std::string(tmpStr));
    }

    delete[]tmpStr;
}

inline void trimFamilyNameList(std::vector<std::string> &familyNames) {
    for (int i = 0, length = familyNames.size(); i < length; i++) {
        std::string &familyName = familyNames[i];
        int len = familyName.length();
        int startLoc = 0;
        int endLoc = len - 1;
        const char *str = familyName.c_str();
        while (startLoc < len) {
            char c = str[startLoc];
            if (c == ' ' || c == '\'' || c == '"') {
                startLoc++;
            } else {
                break;
            }
        }

        while (endLoc > 0) {
            char c = str[endLoc];
            if (c == ' ' || c == '\'' || c == '"') {
                endLoc--;
            } else {
                break;
            }
        }

        if (startLoc != 0 || endLoc != len - 1) {
            familyNames[i] = familyName.substr(startLoc, endLoc - startLoc + 1);
        }
    }
}


bool CSSParser::parseCSSString(const std::string &cssString, FontDescription &fontDescription) {

    std::vector<std::string> tokens;
    splitCSSToken(cssString.c_str(), tokens, CSS_STYLE_SEPARATE_TOKEN);
    if (tokens.size() == 0) {
        return false;
    }
    auto it = tokens.begin();
    while (it != tokens.end()) {
        std::string token = *it;
        if (token == "normal") {
            fontDescription.setSmallCaps(false);
            fontDescription.setSlant(Slant::kNormal);
            fontDescription.setWeight(400);
        } else if (token == "italic") {
            fontDescription.setSlant(Slant::kItalic);
        } else if (token == "oblique") {
            fontDescription.setSlant(Slant::kOblique);
        } else if (token == "small-caps") {
            fontDescription.setSmallCaps(true);
        } else if (token == "bold" || token == "bolder" || token == "700") {
            fontDescription.setWeight(700);
        } else if (token == "lighter" || token == "200") {
            fontDescription.setWeight(200);
        } else if (token == "100") {
            fontDescription.setWeight(100);
        } else if (token == "300") {
            fontDescription.setWeight(300);
        } else if (token == "400") {
            fontDescription.setWeight(400);
        } else if (token == "500") {
            fontDescription.setWeight(500);
        } else if (token == "600") {
            fontDescription.setWeight(600);
        } else if (token == "800") {
            fontDescription.setWeight(800);
        } else if (token == "900") {
            fontDescription.setWeight(900);
        } else {
            if (token.length() > 2 && token.substr(token.length() - 2, 2) == "px") {
                double numberValue = 0;
                if (StringConversion::stringToNumber(token.substr(0, token.length() - 2).c_str(), token.length() - 2,
                                                     numberValue)) {
                    fontDescription.setFontSize(static_cast<float >(numberValue));
                }
                it++;
                continue;
            }
            std::size_t found = cssString.find_first_of(token);
            if (found != std::string::npos) {
                std::vector<std::string> familyNameFallbackList;
                std::string familyNames = cssString.substr(found, cssString.length() - found);
                splitCSSToken(familyNames.c_str(), familyNameFallbackList, CSS_FAMILY_NAME_SEPARATE_TOKEN);
                trimFamilyNameList(familyNameFallbackList);

                scoped_refptr<SharedFontFamily> fontFamily = SharedFontFamily::Create();
                SharedFontFamily *curr_family = nullptr;
                for (auto family : familyNameFallbackList) {
                    if (!curr_family) {
                        curr_family = fontFamily.get();
                    } else {
                        scoped_refptr<SharedFontFamily> newFamily = SharedFontFamily::Create();
                        curr_family->AppendFamily(newFamily);
                        curr_family = newFamily.get();
                    }

                    curr_family->SetFamily(family);
                }

                fontDescription.SetFamily(*fontFamily.get());
                break;
            }
        }

        it++;
    }


    return true;
}


std::string CSSParser::fontToCSSConfig(Font &font) {
    if (!font.cachedCSSConfig().empty()) {
        return font.cachedCSSConfig();
    }

    const FontDescription &description = font.fontDescription();

    std::string cssConfig = "";
    bool needSeparator = false;
    if (description.slant() == Slant::kItalic) {
        cssConfig = "italic";
        needSeparator = true;
    } else if (description.slant() == Slant::kOblique) {
        cssConfig = "oblique";
        needSeparator = true;
    }

    if (description.isSmallCaps()) {
        if (needSeparator) {
            cssConfig += " ";
        }
        cssConfig += "small-caps";
        needSeparator = true;
    }

    if (description.Weight() != 400) {
        if (needSeparator) {
            cssConfig += " ";
        }
        char weightStr[5] = {0};
        sprintf(weightStr, "%lld", static_cast<long long int>(description.Weight()));
        cssConfig += weightStr;
        needSeparator = true;
    }

    if (needSeparator) {
        cssConfig += " ";
    }

    char fontSizeStr[5] = {0};
    sprintf(fontSizeStr, "%f", description.getFontSize());

    cssConfig += fontSizeStr;
    cssConfig += "px ";
    const FontFamily *family = &description.Family();
    while (family) {
        auto familyName = family->Family();
        cssConfig += familyName;
        family = family->Next();
        if (family) {
            cssConfig += ',';
        }
    }

    font.cacheCSSConfig(cssConfig);

    return cssConfig;
}

std::unique_ptr<CSSStylePropertyValue> CSSParser::tokenToCSSPropertyValue(const CSSToken &token) {
    const char *string = token.originString();
    if (token.type() == NumberToken) {
        double value;
        switch (token.unitType()) {
            case CSS_NUMBER:
                StringConversion::stringToNumber(string + token.stringStart(),
                                                 token.stringEnd() - token.stringStart() + 1, value);
                return std::unique_ptr<CSSStylePropertyValue>(new CSSStylePropertyValue(value));
            case CSS_PX:
                StringConversion::stringToNumber(string + token.stringStart(),
                                                 token.stringEnd() - token.stringStart() - 1, value);
                return std::unique_ptr<CSSStylePropertyValue>(new CSSStylePropertyValue(value, CSS_PX));
            default:
                return nullptr;
        }
    } else {
        size_t strLength = token.stringEnd() - token.stringStart() + 1;
        char *propertyStrValue = new char[strLength + 1];
        propertyStrValue[strLength] = '\0';
        std::memcpy(propertyStrValue, string + token.stringStart(), strLength);
        auto value_ptr = std::unique_ptr<CSSStylePropertyValue>(new CSSStylePropertyValue(propertyStrValue));
        delete[] propertyStrValue;
        return value_ptr;
    }
}

void consumeWhiteSpace(const char *string, unsigned &start, size_t length) {
    while (start < length && string[start] == ' ')
        start++;
}

enum ConsumeResult {
    Normal,
    No_Result_Reach_End,
    No_Result_Empty_Token
};

ConsumeResult consume(const char *string, unsigned &start, unsigned &end, size_t length, const char separateToken) {
    consumeWhiteSpace(string, start, length);
    end = start;
    while (end < length && string[end] != separateToken)
        end++;


    if (end == length) {
        return No_Result_Reach_End;
    } else if (start == end) {
        return No_Result_Empty_Token;
    } else {
        end--;
        return Normal;
    }
}

void consumeBracket(const char *string, unsigned &start, unsigned &end, size_t length) {
    consumeWhiteSpace(string, start, length);
    while (start < length && string[start] != '(')
        start++;

    if (start == length) {
        end = start;
        return;
    }

    end = start;
    while (end < length && string[end] != ')')
        end++;

    if (end == length) {
        end = start;
    }
}

template<class Enumerator>
void consumeBracketElements(const char *string, size_t length, Enumerator enumerator) {
    //(aa,bb,cc)
    unsigned startPos = 1;
    unsigned endPos = startPos;
    int index = 0;
    while (endPos < length) {
        ConsumeResult result = consume(string, startPos, endPos, length, ',');
        bool nextLoop = false;
        if (result == ConsumeResult::Normal) {
            CSSToken token(string, startPos, endPos);
            nextLoop = enumerator(index, result, &token);
        } else if (result == ConsumeResult::No_Result_Empty_Token) {
            nextLoop = enumerator(index, result, nullptr);
        } else {
            if (startPos == endPos - 1) {// "(    )"
                return;
            }
            CSSToken token(string, startPos, endPos - 1);
            enumerator(index, result, &token);
        }

        if (!nextLoop) {
            return;
        }
        startPos = endPos + 2;
        endPos = startPos + 1;
        index++;
    }
}


bool parseSemicolonSign(const char *expression, size_t strLength, unsigned &semicolonSignPos) {
    if (!strLength) {
        return false;
    }

    semicolonSignPos = 0;
    while (semicolonSignPos < strLength && expression[semicolonSignPos] != ':') {
        semicolonSignPos++;
    }

    return semicolonSignPos != strLength;

}

bool CSSParser::parseCSSString(const char *string, CSSStyleDeclaration &declaration) {
    unsigned start = 0, end = 0;
    size_t stringLength = std::strlen(string);
    while (start < stringLength) {
        ConsumeResult consumeResult = consume(string, start, end, stringLength, ';');

        if (consumeResult == ConsumeResult::No_Result_Reach_End) {
            return false;
        } else if (consumeResult == ConsumeResult::No_Result_Empty_Token) {
            start = end + 1;
            continue;
        }

        unsigned semicolonPos = 0;
        if (!parseSemicolonSign(string + start, end - start, semicolonPos)) {
            return false;
        }

        if (semicolonPos == end - start) {
            return false;
        }

        CSSToken lValueToken(string, start, start + semicolonPos - 1);

        CSSToken rValueToken(string, start + semicolonPos + 1, end);

        if (lValueToken.type() != StringToken) {
            return false;
        }

        char *propertyName = new char[semicolonPos];
        propertyName[semicolonPos - 1] = '\0';
        std::memcpy(propertyName, string + lValueToken.stringStart(),
                    lValueToken.stringEnd() - lValueToken.stringStart() + 1);

        auto propertyValue = tokenToCSSPropertyValue(rValueToken);

        if (propertyValue) {
            declaration.setProperty(propertyName, *propertyValue);
        }

        delete[]propertyName;

        start = end + 1;
    }

    return true;
}


char *CSSParser::formatCSSString(const CSSStyleDeclaration &declaration) {
    std::string formatStr = "";
    bool needAppendPrefix = false;
    auto &properties = declaration.properties();
    for (auto it : properties) {
        if (needAppendPrefix) {
            formatStr += "; ";
        }
        formatStr += it.first;
        formatStr += ":";
        if (it.second.isString()) {
            formatStr += "\"";
            formatStr += it.second.toString();
            formatStr += "\"";
        } else {
            formatStr += it.second.toString();
        }
        needAppendPrefix = true;
    }

    char *result = new char[formatStr.length()];
    sprintf(result, "%s", formatStr.c_str());
    return result;
}


bool parseHexStringValueToColor4F(const char *value, Color4F &color) {
    char str[] = "ffffff";
    int length = (int) strlen(value);
    if (length == 4) {// #f0f format
        str[0] = str[1] = value[3];
        str[2] = str[3] = value[2];
        str[4] = str[5] = value[1];
        unsigned int hex =
                (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
        color = Color4F((hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                        (hex >> 16) / 255.0f, 1.0);
    } else if (length == 7) {// #ff00ff format
        str[0] = value[5];
        str[1] = value[6];
        str[2] = value[3];
        str[3] = value[4];
        str[4] = value[1];
        str[5] = value[2];
        unsigned int hex =
                (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
        color = Color4F((hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                        (hex >> 16) / 255.0f, 1.0);
    } else {
        return false;
    }

    return true;
}

bool parseRGBStringValueToColor4F(const char *value, Color4F &color) {
    int length = (int) strlen(value);
    int current = 0;
    float components[4] = {0, 0, 0, 1.f};
    for (int i = 4; i < length && current < 4; i++) {
        if (current == 3) {
            // If we have an alpha component, copy the rest of the wide
            // string into a char array and use atof() to parse it.
            char alpha[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            for (int j = 0; i + j < length - 1 && j < 7; j++) {
                alpha[j] = value[i + j];
            }
            components[current] = (float) atof(alpha);
            current++;
        } else if (isdigit(value[i])) {
            components[current] = (components[current] * 10 + (value[i] - '0'));
        } else if (value[i] == ',' || value[i] == ')') {
            components[current] /= 255.0f;
            current++;
        }
    }
    color = Color4F(components[0], components[1], components[2], components[3]);

    return true;
}

static double calcHue(double temp1, double temp2, double hueVal) {
    if (hueVal < 0.0)
        hueVal += 6.0;
    else if (hueVal >= 6.0)
        hueVal -= 6.0;
    if (hueVal < 1.0)
        return temp1 + (temp2 - temp1) * hueVal;
    if (hueVal < 3.0)
        return temp2;
    if (hueVal < 4.0)
        return temp1 + (temp2 - temp1) * (4.0 - hueVal);
    return temp1;
}


Color4F makeRGBAFromHSLA(double hue, double saturation, double lightness, double alpha) {
    const double scaleFactor = nextafter(256.0, 0.0);

    if (!saturation) {
        int greyValue = static_cast<int>(lightness * scaleFactor);
        return Color4F(greyValue, greyValue, greyValue, static_cast<int>(alpha * scaleFactor));
    }

    double temp2 = lightness <= 0.5 ? lightness * (1.0 + saturation) : lightness + saturation - lightness * saturation;
    double temp1 = 2.0 * lightness - temp2;

    return Color4F(static_cast<float>(calcHue(temp1, temp2, hue + 2.0)),
                   static_cast<float>(calcHue(temp1, temp2, hue)),
                   static_cast<float>(calcHue(temp1, temp2, hue - 2.0)),
                   static_cast<float>(alpha));
}

bool parseHSLStringValueToColor4F(const char *value, Color4F &color) {
    size_t strLength = strlen(value);

    unsigned startPos = 0;
    unsigned endPos = strLength - 1;
    consumeBracket(value, startPos, endPos, strLength);
    if (startPos == endPos || endPos - startPos <= 1) {
        return false;
    }

    double colorArray[3];
    double alpha = 1;

    const char *bracketStr = value + startPos;

    consumeBracketElements(
            bracketStr,
            endPos - startPos,
            [&colorArray, bracketStr, &alpha](int index, ConsumeResult consumeResult, const CSSToken *token) -> bool {
                switch (index) {
                    case 0: {
                        double angleInDegrees = 0;
                        if (consumeResult == ConsumeResult::No_Result_Reach_End) {
                            return false;
                        } else if (consumeResult == ConsumeResult::No_Result_Empty_Token) {
                            angleInDegrees = 0;
                        } else {
                            if (token->type() == CSSTokenType::NumberToken) {
                                StringConversion::stringToNumber(bracketStr + token->stringStart(),
                                                                 token->stringEnd() - token->stringStart() + 1, angleInDegrees);
                            }
                        }
                        colorArray[0] = fmod(fmod(angleInDegrees, 360.0) + 360.0, 360.0) / 60.0;
                    }

                        break;
                    case 1:
                        if (consumeResult == ConsumeResult::No_Result_Reach_End) {
                            return false;
                        }

                    case 2: {
                        if (consumeResult == ConsumeResult::No_Result_Empty_Token) {
                            colorArray[index] = 0;
                        } else {
                            double percentage = 0;
                            if (token->unitType() == UnitType::CSS_PERCENTAGE) {
                                StringConversion::stringToNumber(bracketStr + token->stringStart(),
                                                                 token->stringEnd() - token->stringStart(), percentage);
                                colorArray[index] = percentage / 100.0;
                            }
                        }
                    }
                        break;
                    case 3: {
                        if (token) {
                            if (token->type() == CSSTokenType::NumberToken) {
                                StringConversion::stringToNumber(bracketStr + token->stringStart(),
                                                                 token->stringEnd() - token->stringStart() + 1, alpha);
                            }
                        }

                    }
                        break;
                    default:
                        break;
                }

                return true;
            }
    );

    color = makeRGBAFromHSLA(colorArray[0], colorArray[1], colorArray[2], alpha);

    return true;

}

Color4F CSSParser::parseCSSColor(const char *colorString) {
    static std::map<std::string, Color4F> colorMap;
    if (colorMap.empty()) {
        colorMap.insert(std::pair<std::string, Color4F>(
                "black", Color4F(0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "silver", Color4F(192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "gray", Color4F(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "white", Color4F(255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "maroon", Color4F(128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "red", Color4F(255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "purple", Color4F(128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "fuchsia", Color4F(255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "green", Color4F(0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lime", Color4F(0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "olive", Color4F(128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "yellow", Color4F(255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "navy", Color4F(0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "blue", Color4F(0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "teal", Color4F(0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "aqua", Color4F(0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "aliceblue", Color4F(240.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "antiquewhite",
                Color4F(250.0f / 255, 235.0f / 255, 215.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "aqua", Color4F(0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "aquamarine", Color4F(127.0f / 255, 255.0f / 255, 212.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "azure", Color4F(240.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "beige", Color4F(245.0f / 255, 245.0f / 255, 220.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "bisque", Color4F(255.0f / 255, 228.0f / 255, 196.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "black", Color4F(0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "blanchedalmond",
                Color4F(255.0f / 255, 235.0f / 255, 205.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "blue", Color4F(0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "blueviolet", Color4F(138.0f / 255, 43.0f / 255, 226.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "brown", Color4F(165.0f / 255, 42.0f / 255, 42.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "burlywood", Color4F(222.0f / 255, 184.0f / 255, 135.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "cadetblue", Color4F(95.0f / 255, 158.0f / 255, 160.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "chartreuse", Color4F(127.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "chocolate", Color4F(210.0f / 255, 105.0f / 255, 30.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "coral", Color4F(255.0f / 255, 127.0f / 255, 80.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "cornflowerblue",
                Color4F(100.0f / 255, 149.0f / 255, 237.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "cornsilk", Color4F(255.0f / 255, 248.0f / 255, 220.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "crimson", Color4F(220.0f / 255, 20.0f / 255, 60.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "cyan", Color4F(0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkblue", Color4F(0.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkcyan", Color4F(0.0f / 255, 139.0f / 255, 139.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkgoldenrod",
                Color4F(184.0f / 255, 134.0f / 255, 11.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkgray", Color4F(169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkgreen", Color4F(0.0f / 255, 100.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkgrey", Color4F(169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkkhaki", Color4F(189.0f / 255, 183.0f / 255, 107.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkmagenta", Color4F(139.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkolivegreen",
                Color4F(85.0f / 255, 107.0f / 255, 47.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkorange", Color4F(255.0f / 255, 140.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkorchid", Color4F(153.0f / 255, 50.0f / 255, 204.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkred", Color4F(139.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darksalmon", Color4F(233.0f / 255, 150.0f / 255, 122.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkseagreen",
                Color4F(143.0f / 255, 188.0f / 255, 143.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkslateblue", Color4F(72.0f / 255, 61.0f / 255, 139.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkslategray", Color4F(47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkslategrey", Color4F(47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkturquoise", Color4F(0.0f / 255, 206.0f / 255, 209.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "darkviolet", Color4F(148.0f / 255, 0.0f / 255, 211.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "deeppink", Color4F(255.0f / 255, 20.0f / 255, 147.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "deepskyblue", Color4F(0.0f / 255, 191.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "dimgray", Color4F(105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "dimgrey", Color4F(105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "dodgerblue", Color4F(30.0f / 255, 144.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "firebrick", Color4F(178.0f / 255, 34.0f / 255, 34.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "floralwhite", Color4F(255.0f / 255, 250.0f / 255, 240.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "forestgreen", Color4F(34.0f / 255, 139.0f / 255, 34.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "fuchsia", Color4F(255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "gainsboro", Color4F(220.0f / 255, 220.0f / 255, 220.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "ghostwhite", Color4F(248.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "gold", Color4F(255.0f / 255, 215.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "goldenrod", Color4F(218.0f / 255, 165.0f / 255, 32.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "gray", Color4F(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "green", Color4F(0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "greenyellow", Color4F(173.0f / 255, 255.0f / 255, 47.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "grey", Color4F(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "honeydew", Color4F(240.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "hotpink", Color4F(255.0f / 255, 105.0f / 255, 180.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "indianred", Color4F(205.0f / 255, 92.0f / 255, 92.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "indigo", Color4F(75.0f / 255, 0.0f / 255, 130.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "ivory", Color4F(255.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "khaki", Color4F(240.0f / 255, 230.0f / 255, 140.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lavender", Color4F(230.0f / 255, 230.0f / 255, 250.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lavenderblush",
                Color4F(255.0f / 255, 240.0f / 255, 245.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lawngreen", Color4F(124.0f / 255, 252.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lemonchiffon",
                Color4F(255.0f / 255, 250.0f / 255, 205.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightblue", Color4F(173.0f / 255, 216.0f / 255, 230.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightcoral", Color4F(240.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightcyan", Color4F(224.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightgoldenrodyellow",
                Color4F(250.0f / 255, 250.0f / 255, 210.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightgray", Color4F(211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightgreen", Color4F(144.0f / 255, 238.0f / 255, 144.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightgrey", Color4F(211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightpink", Color4F(255.0f / 255, 182.0f / 255, 193.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightsalmon", Color4F(255.0f / 255, 160.0f / 255, 122.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightseagreen",
                Color4F(32.0f / 255, 178.0f / 255, 170.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightskyblue",
                Color4F(135.0f / 255, 206.0f / 255, 250.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightslategray",
                Color4F(119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightslategrey",
                Color4F(119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightsteelblue",
                Color4F(176.0f / 255, 196.0f / 255, 222.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lightyellow", Color4F(255.0f / 255, 255.0f / 255, 224.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "lime", Color4F(0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "limegreen", Color4F(50.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "linen", Color4F(250.0f / 255, 240.0f / 255, 230.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "magenta", Color4F(255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "maroon", Color4F(128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumaquamarine",
                Color4F(102.0f / 255, 205.0f / 255, 170.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumblue", Color4F(0.0f / 255, 0.0f / 255, 205.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumorchid", Color4F(186.0f / 255, 85.0f / 255, 211.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumpurple",
                Color4F(147.0f / 255, 112.0f / 255, 219.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumseagreen",
                Color4F(60.0f / 255, 179.0f / 255, 113.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumslateblue",
                Color4F(123.0f / 255, 104.0f / 255, 238.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumspringgreen",
                Color4F(0.0f / 255, 250.0f / 255, 154.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumturquoise",
                Color4F(72.0f / 255, 209.0f / 255, 204.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mediumvioletred",
                Color4F(199.0f / 255, 21.0f / 255, 133.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "midnightblue", Color4F(25.0f / 255, 25.0f / 255, 112.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mintcream", Color4F(245.0f / 255, 255.0f / 255, 250.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "mistyrose", Color4F(255.0f / 255, 228.0f / 255, 225.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "moccasin", Color4F(255.0f / 255, 228.0f / 255, 181.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "navajowhite", Color4F(255.0f / 255, 222.0f / 255, 173.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "navy", Color4F(0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "oldlace", Color4F(253.0f / 255, 245.0f / 255, 230.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "olive", Color4F(128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "olivedrab", Color4F(107.0f / 255, 142.0f / 255, 35.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "orange", Color4F(255.0f / 255, 165.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "orangered", Color4F(255.0f / 255, 69.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "orchid", Color4F(218.0f / 255, 112.0f / 255, 214.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "palegoldenrod",
                Color4F(238.0f / 255, 232.0f / 255, 170.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "palegreen", Color4F(152.0f / 255, 251.0f / 255, 152.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "paleturquoise",
                Color4F(175.0f / 255, 238.0f / 255, 238.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "palevioletred",
                Color4F(219.0f / 255, 112.0f / 255, 147.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "papayawhip", Color4F(255.0f / 255, 239.0f / 255, 213.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "peachpuff", Color4F(255.0f / 255, 218.0f / 255, 185.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "peru", Color4F(205.0f / 255, 133.0f / 255, 63.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "pink", Color4F(255.0f / 255, 192.0f / 255, 203.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "plum", Color4F(221.0f / 255, 160.0f / 255, 221.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "powderblue", Color4F(176.0f / 255, 224.0f / 255, 230.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "purple", Color4F(128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "red", Color4F(255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "rosybrown", Color4F(188.0f / 255, 143.0f / 255, 143.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "royalblue", Color4F(65.0f / 255, 105.0f / 255, 225.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "saddlebrown", Color4F(139.0f / 255, 69.0f / 255, 19.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "salmon", Color4F(250.0f / 255, 128.0f / 255, 114.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "sandybrown", Color4F(244.0f / 255, 164.0f / 255, 96.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "seagreen", Color4F(46.0f / 255, 139.0f / 255, 87.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "seashell", Color4F(255.0f / 255, 245.0f / 255, 238.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "sienna", Color4F(160.0f / 255, 82.0f / 255, 45.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "silver", Color4F(192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "skyblue", Color4F(135.0f / 255, 206.0f / 255, 235.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "slateblue", Color4F(106.0f / 255, 90.0f / 255, 205.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "slategray", Color4F(112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "slategrey", Color4F(112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "snow", Color4F(255.0f / 255, 250.0f / 255, 250.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "springgreen", Color4F(0.0f / 255, 255.0f / 255, 127.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "steelblue", Color4F(70.0f / 255, 130.0f / 255, 180.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "tan", Color4F(210.0f / 255, 180.0f / 255, 140.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "teal", Color4F(0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "thistle", Color4F(216.0f / 255, 191.0f / 255, 216.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "tomato", Color4F(255.0f / 255, 99.0f / 255, 71.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "turquoise", Color4F(64.0f / 255, 224.0f / 255, 208.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "violet", Color4F(238.0f / 255, 130.0f / 255, 238.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "wheat", Color4F(245.0f / 255, 222.0f / 255, 179.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "white", Color4F(255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "whitesmoke", Color4F(245.0f / 255, 245.0f / 255, 245.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "yellow", Color4F(255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "yellowgreen", Color4F(154.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "transparent", Color4F(0, 0, 0, 0.0f)));
        colorMap.insert(std::pair<std::string, Color4F>(
                "transparent_white", Color4F(1.f, 1.f, 1.f, 0.f)));
    }

    if (colorString == nullptr) {
        return Color4F::BLACK;
    }
    std::map<std::string, Color4F>::const_iterator iter =
            colorMap.find(colorString);
    if (iter != colorMap.end()) {
        return iter->second;
    }
    int length = (int) strlen(colorString);
    if (length < 4) {
        return Color4F::BLACK;
    }

    Color4F color;
    if (*colorString == '#') {
        if (!parseHexStringValueToColor4F(colorString, color)) {
            return Color4F::BLACK;
        }
    } else {
        if (strncasecmp(colorString, "rgb", 3) == 0) {
            if (!parseRGBStringValueToColor4F(colorString, color)) {
                return Color4F::BLACK;
            }
        } else if (strncasecmp(colorString, "hsl", 3) == 0) {
            if (!parseHSLStringValueToColor4F(colorString, color)) {
                return Color4F::BLACK;
            }
        }
    }

    return color;

}


bool CSSParser::parseRepetitionType(const char *type, bool &repeatX, bool &repeatY) {
    if (!strlen(type) || strcasecmp(type, "repeat") == 0) {
        repeatX = true;
        repeatY = true;
        return true;
    } else if (strcasecmp(type, "no-repeat") == 0) {
        repeatX = false;
        repeatY = false;
        return true;
    } else if (strcasecmp(type, "repeat-x") == 0) {
        repeatX = true;
        repeatY = false;
        return true;
    } else if (strcasecmp(type, "repeat-y") == 0) {
        repeatX = false;
        repeatY = true;
        return true;
    }

    return false;
}

}
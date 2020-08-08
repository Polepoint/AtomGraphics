//
// Created by neo on 2020/4/23.
//

#include "StringUtils.h"

namespace AtomGraphics {

template<typename CharacterType>
inline bool isSpace(CharacterType character) {
    return character <= ' ' && (character == ' ' || character == '\n' || character == '\t' || character == '\r' || character == '\f');
}

std::string StringUtils::stripLeadingAndTrailingSpaces(const std::string &source) {
    const char *characters = source.data();
    unsigned long length = source.length();

    unsigned numLeadingSpaces = 0;
    unsigned numTrailingSpaces = 0;

    for (; numLeadingSpaces < length; ++numLeadingSpaces) {
        if (!isSpace(characters[numLeadingSpaces]))
            break;
    }

    if (numLeadingSpaces == length)
        return source.empty() ? source : std::string();

    for (; numTrailingSpaces < length; ++numTrailingSpaces) {
        if (!isSpace(characters[length - numTrailingSpaces - 1]))
            break;
    }

    return source.substr(numLeadingSpaces, length - numTrailingSpaces);
}

}
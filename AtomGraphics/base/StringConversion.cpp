//
// Created by neo on 2018/12/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <stdio.h>
#include <cstring>
#include "StringConversion.h"

namespace AtomGraphics {

inline bool isNumberChar(const char c) {
    return c >= '0' && c <= '9';
}

bool StringConversion::stringToNumber(const char *numberToken, size_t length, double &result) {
    if (!length) {
        return false;
    }

    int pos = 0;
    bool decimal = false;
    int integerValue = 0;
    double decimalValue = 0;
    int decimalPos = 1;
    bool negative = numberToken[0] == '-';
    if (negative) {
        pos++;
    }
    while (pos < length) {
        const char c = numberToken[pos];
        if (isNumberChar(c)) {
            if (decimal) {
                decimalPos *= 10;
                decimalValue += 1.0 * (c - '0') / decimalPos;
            } else {
                integerValue *= 10;
                integerValue += c - '0';
            }
            pos++;
        } else if (c == '.' && !decimal) {
            decimal = true;
            pos++;
        } else {
            return false;
        }
    }

    result = integerValue + decimalValue;
    if (negative) {
        result *= -1;
    }
    return true;
}

char *StringConversion::doubleToString(double number) {
    char tmp[64] = {'\0'};
    sprintf(tmp, "%g", number);
    size_t length = strlen(tmp) + 1;
    char *string = new char[length];
    string[length - 1] = '\0';
    sprintf(string, "%s", tmp);
    return string;
}

}
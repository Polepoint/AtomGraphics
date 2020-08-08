//
// Created by neo on 2018/8/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <iterator>
#include <cstring>
#include <algorithm>
#include "ATString.h"

namespace AtomGraphics {

namespace base {

inline bool isASCII(char character) {
    return !(character & ~0x7F);
}

inline long lastASCIIPosition(char *source, size_t length) {
    char *start = source;
    char *end = source + length;
    while (start < end) {
        if (!isASCII(*start)) {
            break;
        }
        start++;
    }

    return start - source;
}

inline int decodeNonASCIISequence(uint8_t *source, int &step, size_t maxLength) {
    uint8_t first = source[0];
    if (first <= 0xDF && first >= 0xC2) {
        if (maxLength < 2) {
            step = 1;
            return -1;
        }
        if (source[1] < 0x80 || source[1] > 0xBF) {
            step = 1;
            return -1;
        }

        step = 2;
        return ((source[0] << 6) + source[1]) - 0x00003080;
    } else if (first <= 0xEF && first >= 0xE0) {
        if (maxLength < 3) {
            step = 1;
            return -1;
        }
        switch (first) {
            case 0xE0:
                if (source[1] < 0xA0 || source[1] > 0xBF) {
                    step = 1;
                    return -1;
                }
                break;
            case 0xED:
                if (source[1] < 0x80 || source[1] > 0x9F) {
                    step = 1;
                    return -1;
                }
                break;
            default:
                if (source[1] < 0x80 || source[1] > 0xBF) {
                    step = 1;
                    return -1;
                }
        }

        step = 3;
        return ((source[0] << 12) + (source[1] << 6) + source[2]) - 0x000E2080;
    } else if (first >= 0xF0 && first <= 0xF4) {
        switch (first) {
            case 0xF0:
                if (source[1] < 0x90 || source[1] > 0xBF) {
                    step = 1;
                    return -1;
                }
                break;
            case 0xF4:
                if (source[1] < 0x80 || source[1] > 0x8F) {
                    step = 1;
                    return -1;
                }
                break;
            default:
                if (source[1] < 0x80 || source[1] > 0xBF) {
                    step = 1;
                    return -1;
                }
        }
        if (source[2] < 0x80 || source[2] > 0xBF) {
            step = 2;
            return -1;
        }
        if (source[3] < 0x80 || source[3] > 0xBF) {
            step = 3;
            return -1;
        }

        step = 4;
        return ((source[0] << 18) + (source[1] << 12)
                + (source[2] << 6) + source[3]) - 0x03C82080;
    }

    return -1;
}

inline UChar *decodeFromUTF8(char *source, size_t dataLength, unsigned int &stringLength) {
    if (dataLength == 0) {
        stringLength = 0;
        return new UChar{'\0'};
    }

    char *start = source;
    const char *end = source + dataLength;
    UChar *result = new UChar[dataLength + 1];
    unsigned int decodePos = 0;
    while (start < end) {
        if (isASCII(*start)) {
            long asciiPos = lastASCIIPosition(start, end - start);
            if (asciiPos == end - start) {
                while (start < end) {
                    result[decodePos++] = static_cast<UChar>(*start);
                    start++;
                }
                result[dataLength] = '\0';
                break;
            } else {
                for (int i = 0; i < asciiPos; i++) {
                    result[decodePos++] = static_cast<UChar>(*start);
                    start++;
                }
            }
        } else {
            int step = 0;
            int c = decodeNonASCIISequence(reinterpret_cast<uint8_t *>(start),
                                           step, end - start);
            if (c == -1) {
                start += step;
                continue;
            }
            result[decodePos++] = static_cast<UChar>(c);
            start += step;
        }
    }

    stringLength = decodePos;
    return result;
}

// latin-1
//    inline char *encodeToUTF8(UChar *source, unsigned length, char *&buffer, size_t bufferSize) {
//        const UChar *charactersEnd = source + length;
//        char *bufferEnd = buffer + bufferSize;
//        while (source < charactersEnd) {
//            ConversionResult result = convertUTF16ToUTF8(&source, charactersEnd, &buffer,
//                    bufferEnd, true);
//            if (result != targetExhausted && result != conversionOK) {
//                if (0xD800 <= *characters && *characters <= 0xDFFF) {
//                    if ((buffer + 3) <= bufferEnd) {
//                        putUTF8Triple(buffer, replacementCharacter);
//                        ++source;
//                    } else {
//                        return nullptr;
//                    }
//                }
//            }
//        }
//        return nullptr;
//    }

String String::s_emptyString("", 0);

String::String(const char *buffer, size_t bufferSize) {
    if (bufferSize == 0) {
        m_uCharData = new UChar{'\0'};
        m_is8Bit = true;
    }

    m_dataLength = bufferSize + 1;
    m_data = new char[m_dataLength];
    memcpy(m_data, buffer, bufferSize);
    ((char *) m_data)[bufferSize] = '\0';

    long lastAsciiPos = lastASCIIPosition(static_cast<char *>(m_data), bufferSize);
    if (lastAsciiPos == bufferSize) {
        m_is8Bit = true;
        m_data8 = static_cast<char *>(m_data);
        m_uCharData = decodeFromUTF8(static_cast<char *>(m_data), bufferSize, m_length);
    } else {
        m_is8Bit = false;
        m_uCharData = decodeFromUTF8(static_cast<char *>(m_data), bufferSize, m_length);
        m_data16 = m_uCharData;
    }
}

//    String::String(const UChar *chars, size_t length) {
//        if (length > 0) {
//            m_is8Bit = false;
//            UChar *copiedChars = new UChar[length + 1];
//            memcpy(copiedChars, chars, length * sizeof(UChar));
//            copiedChars[length] = '\0';
//            m_data16 = copiedChars;
//            m_uCharData = copiedChars;
//        }
//    }

String::String(const String &other) {
    this->m_length = other.m_length;
    this->m_data8 = other.m_data8;
    this->m_data16 = other.m_data16;
    this->m_dataLength = other.m_dataLength;
    this->m_is8Bit = other.m_is8Bit;
    if (m_data) {
        delete[] (char *) m_data;
        m_data = new char[m_dataLength];
        memcpy(m_data, other.m_data, m_dataLength);
    }
    if (this->m_uCharData) {
        delete[] this->m_uCharData;
        this->m_uCharData = nullptr;
    }
    if (other.m_uCharData) {
        std::memcpy(this->m_uCharData, other.m_uCharData, m_length * sizeof(UChar));
    }
}

String::~String() {
    if (m_uCharData) {
        delete[] m_uCharData;
    }
    if (m_data) {
        delete[] (char *) m_data;
    }
}

size_t String::find(CodeUnitMatchFunction matchFunction) {
    if (is8Bit()) {
        char *c = std::find_if(m_data8, m_data8 + m_length + 1, matchFunction);
        if (c == m_data8 + m_length + 1) {
            return NotFound;
        }

        return c - m_data8;
    } else {
        UChar *c = std::find_if(m_data16, m_data16 + m_length + 1, matchFunction);
        if (c == m_data16 + m_length + 1) {
            return NotFound;
        }

        return c - m_data16;
    }
}

//String String::substring(unsigned start, unsigned length) const {
//    if (start >= m_length)
//        return *empty();
//    unsigned maxLength = m_length - start;
//    if (length >= maxLength) {
//        if (!start)
//            return *this;
//        length = maxLength;
//    }
//    if (is8Bit())
//        return String(m_data8 + start, length);
//
//    return String(m_data16 + start, length);
//}

// Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
// into the first byte, depending on how many bytes follow.  There are
// as many entries in this table as there are UTF-8 sequence types.
// (I.e., one byte sequence, two byte... etc.). Remember that sequencs
// for *legal* UTF-8 will be 4 or fewer bytes total.
static const unsigned char firstByteMark[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

const UChar replacementCharacter = 0xFFFD;

void convertUTF16ToUTF8(const UChar source, char *target) {
    UChar32 ch;
    unsigned short bytesToWrite = 0;
    const UChar32 byteMask = 0xBF;
    const UChar32 byteMark = 0x80;
    ch = static_cast<unsigned short>(source);
    // If we have a surrogate pair, convert to UChar32 first.
    if ((ch >= 0xD800 && ch <= 0xDBFF) || (ch >= 0xDC00 && ch <= 0xDFFF)) {
        return;
    }
    // Figure out how many bytes the result will require
    if (ch < (UChar32) 0x80) {
        bytesToWrite = 1;
    } else if (ch < (UChar32) 0x800) {
        bytesToWrite = 2;
    } else if (ch < (UChar32) 0x10000) {
        bytesToWrite = 3;
    } else if (ch < (UChar32) 0x110000) {
        bytesToWrite = 4;
    } else {
        bytesToWrite = 3;
        ch = replacementCharacter;
    }

    target += bytesToWrite;

    switch (bytesToWrite) { // note: everything falls through.
        case 4:
            *--target = (char) ((ch | byteMark) & byteMask);
            ch >>= 6;
            [[fallthrough]];
        case 3:
            *--target = (char) ((ch | byteMark) & byteMask);
            ch >>= 6;
            [[fallthrough]];
        case 2:
            *--target = (char) ((ch | byteMark) & byteMask);
            ch >>= 6;
            [[fallthrough]];
        case 1:
            *--target = (char) (ch | firstByteMark[bytesToWrite]);
        default:
            break;
    }
}

}

}

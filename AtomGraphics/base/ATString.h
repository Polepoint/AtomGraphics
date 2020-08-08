//
// Created by neo on 2018/8/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_STRING_H
#define ATOMGRAPHICS_STRING_H

#include "TypeDefine.h"
#include <cstring>

namespace AtomGraphics {

namespace base {

using CodeUnitMatchFunction = bool (*)(UChar);

void convertUTF16ToUTF8(const UChar source, char *target);

class String final {

public:

    /**
     * create string with data buffer and buffer size
     * @param buffer    character buffer
     * @param bufferSize    buffer size
     */
    String(const char *buffer, size_t bufferSize);

//        String(const UChar *, size_t length);

    String(const String &other);

    virtual ~String();

    unsigned int length() const {
        return m_length;
    }

    bool is8Bit() const {
        return m_is8Bit;
    }

    const void *data() const {
        return m_data;
    }

    const char *characters8() const {
        if (m_is8Bit) {
            return m_data8;
        }
        return nullptr;
    }

    const UChar *characters16() const {
        if (m_is8Bit) {
            return nullptr;
        }
        return m_data16;
    }

    const UChar *toUCharData() const {
        return m_uCharData;
    };

private:
    static String s_emptyString;
public:

    static String *empty() { return reinterpret_cast<String *>(&s_emptyString); }

    size_t find(CodeUnitMatchFunction);

//    String substring(unsigned start, unsigned length) const;

private:
    unsigned m_length{0};
    size_t m_dataLength{0};
    bool m_is8Bit{false};
    void *m_data{nullptr};
    char *m_data8{nullptr};
    UChar *m_data16{nullptr};
    UChar *m_uCharData{nullptr};
};

};

};


#endif //ATOMGRAPHICS_STRING_H

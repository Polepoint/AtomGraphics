//
// Created by neo on 2020/4/24.
//

#ifndef ATOMGRAPHICS_SHAREDBUFFER_H
#define ATOMGRAPHICS_SHAREDBUFFER_H


#include "memory/RefCounted.h"

namespace AtomGraphics {

class SharedBuffer final : public RefCounted<SharedBuffer> {

public:

    SharedBuffer(void *ptr = nullptr, unsigned int bufferSize = 0)
            : m_ptr(ptr), m_bufferSize(bufferSize) {}

    ~SharedBuffer();

    void copyData(const void *dataPtr, unsigned bufferSize);

    void *data() const {
        return m_ptr;
    }

    unsigned int bufferSize() const {
        return m_bufferSize;
    }

private:
    void *m_ptr{nullptr};
    unsigned m_bufferSize{0};

    void freeData();
};

}


#endif //ATOMGRAPHICS_SHAREDBUFFER_H

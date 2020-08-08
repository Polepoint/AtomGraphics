//
// Created by neo on 2020/4/24.
//

#include <cstring>
#include <cstdlib>
#include "SharedBuffer.h"

namespace AtomGraphics {

SharedBuffer::~SharedBuffer() {
    freeData();
}

void SharedBuffer::copyData(const void *dataPtr, unsigned int bufferSize) {
    if (m_ptr) {
        freeData();
    }

    m_bufferSize = bufferSize;
    m_ptr = malloc(sizeof(uint8_t) * bufferSize);
    memcpy(m_ptr, dataPtr, static_cast<size_t>(bufferSize));
}

void SharedBuffer::freeData() {
    if (!m_ptr) {
        return;
    }
    free(m_ptr);
    m_ptr = nullptr;
}

}
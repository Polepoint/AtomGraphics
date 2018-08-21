//
// Created by woo on 2018/4/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "base/AtomTypes.h"

namespace AtomGraphics {
    class AtomCanvasImageData {
    public:
        AtomCanvasImageData(int mWidth, int mHeight);

        int getWidth() const;

        int getHeight() const;

        int *getData() const;

        void setData(int *mData);

    private:
        int mWidth;
        int mHeight;
        int *mData;
    };
}

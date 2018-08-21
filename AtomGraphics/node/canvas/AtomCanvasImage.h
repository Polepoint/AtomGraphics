//
// Created by woo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//
#include "base/AtomTypes.h"


namespace AtomGraphics {
    static int autoIncreasedId = 1;

    class AtomCanvasImage {

    public:

        AtomCanvasImage();

        float getWidth() const;

        float getHeight() const;

        const std::string &getSrc() const;

        void setWidth(float width);

        void setHeight(float height);

        void setSrc(const std::string &src);

        int getId() const;

    private:
        float mWidth;
        float mHeight;
        std::string mSrc;
        int mId;
    };

}
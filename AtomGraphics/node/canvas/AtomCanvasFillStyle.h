//
// Created by woo on 2018/4/19.
// Copyright (c) 2018 neo. All rights reserved.
//



#include "base/AtomTypes.h"
#include "AtomCanvasImage.h"

namespace AtomGraphics {
    class AtomCanvasFillStyle {
    public:
        enum Style {
            STYLE_UNDEFINED,
            STYLE_PATTERN,
            STYLE_LINEAR_GRADIENT,
            STYLE_RADIAL_GRADIENT,
        };

        AtomCanvasFillStyle(Style mStyle) : mStyle(mStyle) {
        }


        virtual int getTextureListID() {
            return -1;
        }

        virtual AtomCanvasFillStyle *clone() {
            return nullptr;
        }

        bool isPattern() {
            return mStyle == STYLE_PATTERN;
        }

        bool isLinearGradient() {
            return mStyle == STYLE_LINEAR_GRADIENT;
        }

        bool isRadialGradient() {
            return mStyle == STYLE_RADIAL_GRADIENT;
        }

        bool isDefault() {
            return mStyle == STYLE_UNDEFINED;
        }

        int getStyle() {
            return mStyle;
        }

        virtual ~AtomCanvasFillStyle() {
        }

    private:
        Style mStyle;
    };

    class AtomCanvasPattern : public AtomCanvasFillStyle {
    public:
        AtomCanvasPattern(const std::string &mPattern, const AtomCanvasImage *mImage)
                : AtomCanvasFillStyle(STYLE_PATTERN), mPattern(mPattern), mImage(mImage) {
        }

        virtual ~AtomCanvasPattern() {
            mPattern.clear();
        }

        const std::string &getPattern() const {
            return mPattern;
        }

        int getTextureListId() const {
            return mImage->getId();
        }

        const std::string &getImageSrc() const {
            return mImage->getSrc();
        }

        AtomCanvasFillStyle *clone() {
            AtomCanvasPattern *ptr = new AtomCanvasPattern(mPattern, mImage);
            return ptr;
        }

    private:
        std::string mPattern;
        const AtomCanvasImage *mImage;
    };

    class AtomCanvasLinearGradient : public AtomCanvasFillStyle {
    public:
        static const int MAX_COLOR_STOP_NUM = 5;
        struct ColorStop {
            float pos;
            std::string color;
        };

        typedef struct {
            float x;
            float y;
        } AtomPoint;

        AtomCanvasLinearGradient(const AtomPoint &startPos, const AtomPoint &endPos) :
                AtomCanvasFillStyle(STYLE_LINEAR_GRADIENT), mStartPos(startPos), mEndPos(endPos), mStopCount(0) {
        }

        bool addColorStop(float pos, const std::string &color) {
            if (mStopCount < MAX_COLOR_STOP_NUM) {
                mStops[mStopCount].pos = pos;
                mStops[mStopCount].color = color;
                mStopCount++;
                return true;
            }
            return false;
        }

        AtomCanvasFillStyle *clone() {
            AtomCanvasLinearGradient *ptr = new AtomCanvasLinearGradient(mStartPos, mEndPos);
            //todo add color stop
            *ptr = *this;
            return ptr;
        }

        const AtomPoint &getStartPos() const {
            return mStartPos;
        }

        const AtomPoint &getEndPos() const {
            return mEndPos;
        }

        int getStopCount() const {
            return mStopCount;
        }

        const ColorStop *getColorStop(int index) const {
            if (index >= 0 && index < mStopCount) {
                return &mStops[index];
            }
            return nullptr;
        }

    private:
        AtomPoint mStartPos, mEndPos;
        int mStopCount;
        ColorStop mStops[MAX_COLOR_STOP_NUM];
    };

    class AtomCanvasRadialGradient : public AtomCanvasFillStyle {
    public:
        static const int MAX_COLOR_STOP_NUM = 5;
        struct ColorStop {
            float pos;
            std::string color;
        };

        AtomCanvasRadialGradient(const float *startPos, const float *endPos) :
                AtomCanvasFillStyle(STYLE_RADIAL_GRADIENT), mStopCount(0) {
            mStartPos[0] = startPos[0];
            mStartPos[1] = startPos[1];
            mStartPos[2] = startPos[2];

            mEndPos[0] = endPos[0];
            mEndPos[1] = endPos[1];
            mEndPos[2] = endPos[2];
        }

        bool addColorStop(float pos, const std::string &color) {
            if (mStopCount < MAX_COLOR_STOP_NUM) {
                mStops[mStopCount].pos = pos;
                mStops[mStopCount].color = color;
                mStopCount++;
                return true;
            }
            return false;
        }

        AtomCanvasFillStyle *clone() {
            AtomCanvasRadialGradient *ptr = new AtomCanvasRadialGradient(mStartPos, mEndPos);
            //todo add color stop
            *ptr = *this;
            return ptr;
        }

        const float *getStartPos() const {
            return mStartPos;
        }

        const float *getEndPos() const {
            return mEndPos;
        }

        int getStopCount() const {
            return mStopCount;
        }

        const ColorStop *getColorStop(int index) const {
            if (index >= 0 && index < mStopCount) {
                return &mStops[index];
            }
            return nullptr;
        }
    private:
        float mStartPos[3], mEndPos[3];
        int mStopCount;
        ColorStop mStops[MAX_COLOR_STOP_NUM];
    };
}
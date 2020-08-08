//
// Created by neo on 2019/1/3.
//

#ifndef ATOMGRAPHICS_STROKEDATA_H
#define ATOMGRAPHICS_STROKEDATA_H


#include <skia/core/SkPathEffect.h>
#include <skia/core/SkPaint.h>
#include "graphics/DashArray.h"
#include "graphics/GraphicsTypes.h"

namespace AtomGraphics {

class StrokeData final {

public:
    StrokeData()
            : style_(kSolidStroke),
              thickness_(0),
              line_cap_(SkPaint::kDefault_Cap),
              line_join_(SkPaint::kDefault_Join),
              miter_limit_(4) {}

    StrokeData &operator=(const StrokeData &other) {
        style_ = other.style_;
        thickness_ = other.thickness_;
        line_cap_ = other.line_cap_;
        line_join_ = other.line_join_;
        miter_limit_ = other.miter_limit_;
        dash_ = other.dash_;

        return *this;
    }

    StrokeStyle Style() const { return style_; }

    void SetStyle(StrokeStyle style) { style_ = style; }

    float Thickness() const { return thickness_; }

    void SetThickness(float thickness) { thickness_ = thickness; }

    void SetLineCap(SkPaint::Cap cap) { line_cap_ = cap; }

    void SetLineJoin(SkPaint::Join join) { line_join_ = join; }

    float MiterLimit() const { return miter_limit_; }

    void SetMiterLimit(float miter_limit) { miter_limit_ = miter_limit; }

    void SetLineDash(const DashArray &, float);

    // Sets everything on the paint except the pattern, gradient and color.
    // If a non-zero length is provided, the number of dashes/dots on a
    // dashed/dotted line will be adjusted to start and end that length with a
    // dash/dot. If non-zero, dash_thickness is the thickness to use when
    // deciding on dash sizes. Used in border painting when we stroke thick
    // to allow for clipping at corners, but still want small dashes.
    void SetupPaint(SkPaint *,
                    const int length = 0,
                    const int dash_thickess = 0) const;

    // Setup any DashPathEffect on the paint. See SetupPaint above for parameter
    // information.
    void SetupPaintDashPathEffect(SkPaint *,
                                  const int path_length = 0,
                                  const int dash_thickness = 0) const;

    // Determine whether a stroked line should be drawn using dashes. In practice,
    // we draw dashes when a dashed stroke is specified or when a dotted stroke
    // is specified but the line width is too small to draw circles.
    static bool StrokeIsDashed(float width, StrokeStyle);

    // The length of the dash relative to the line thickness for dashed stroking.
    // A different dash length may be used when dashes are adjusted to better
    // fit a given length path. Thin lines need longer dashes to avoid
    // looking like dots when drawn.
    static float DashLengthRatio(float thickness) {
        return thickness >= 3 ? 2.0f : 3.0f;
    }

    // The length of the gap between dashes relative to the line thickness for
    // dashed stroking. A different gap may be used when dashes are adjusted to
    // better fit a given length path. Thin lines need longer gaps to avoid
    // looking like a continuous line when drawn.
    static float DashGapRatio(float thickness) {
        return thickness >= 3 ? 1.0f : 2.0f;
    }

    // Return a dash gap size that places dashes at each end of a stroke that is
    // strokeLength long, given preferred dash and gap sizes. The gap returned is
    // the one that minimizes deviation from the preferred gap length.
    static float SelectBestDashGap(float stroke_length,
                                   float dash_length,
                                   float gap_length);

private:
    StrokeStyle style_;
    float thickness_;
    SkPaint::Cap line_cap_;
    SkPaint::Join line_join_;
    float miter_limit_;
    sk_sp<SkPathEffect> dash_;
};

};


#endif //ATOMGRAPHICS_STROKEDATA_H

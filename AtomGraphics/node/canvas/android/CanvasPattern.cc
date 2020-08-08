//
// Created by neo on 2019/2/22.
//

#include <skia/core/SkShader.h>
#include <skia/core/SkPictureRecorder.h>
#include <skia/core/SkCanvas.h>
#include "graphics/Pattern.h"
#include "graphics/Image.h"

namespace AtomGraphics {

void Pattern::applyToPaint(SkPaint &flags) {
    if (!m_cachedShader)
        m_cachedShader = createShader();

    flags.setShader(m_cachedShader);
}


sk_sp<SkShader> Pattern::createShader() {
    if (!m_tileImage) {
        return SkShader::MakeColorShader(SK_ColorTRANSPARENT);
    }

    if (m_repeatX && m_repeatY) {
        // Fast path: for repeatXY we just return a shader from the original image.
        return m_tileImage->getSkImage()->makeShader(SkShader::kRepeat_TileMode,
                                                     SkShader::kRepeat_TileMode,
                                                     &SkMatrix::I());
    }

    // Skia does not have a "draw the tile only once" option. Clamp_TileMode
    // repeats the last line of the image after drawing one tile. To avoid
    // filling the space with arbitrary pixels, this workaround forces the
    // image to have a line of transparent pixels on the "repeated" edge(s),
    // thus causing extra space to be transparent filled.
    SkShader::TileMode tile_mode_x =
            m_repeatX ? SkShader::kRepeat_TileMode : SkShader::kClamp_TileMode;
    SkShader::TileMode tile_mode_y =
            m_repeatY ? SkShader::kRepeat_TileMode : SkShader::kClamp_TileMode;
    int border_pixel_x = m_repeatX ? 0 : 1;
    int border_pixel_y = m_repeatY ? 0 : 1;

    // Create a transparent image 2 pixels wider and/or taller than the
    // original, then copy the orignal into the middle of it.
    const SkRect tile_bounds =
            SkRect::MakeWH(m_tileImage->width() + 2 * border_pixel_x,
                           m_tileImage->height() + 2 * border_pixel_y);
    SkPictureRecorder recorder;
    auto *canvas = recorder.beginRecording(tile_bounds);

    SkPaint paint;
    paint.setBlendMode(SkBlendMode::kSrc);
    canvas->drawImage(m_tileImage->getSkImage(), border_pixel_x, border_pixel_y, &paint);

    SkMatrix adjusted_matrix = SkMatrix::I();
    adjusted_matrix.postTranslate(-border_pixel_x, -border_pixel_y);

    // Note: we specify kFixedScale to lock-in the resolution (for 1px padding in
    // particular).
    auto image = SkImage::MakeFromPicture(
            recorder.finishRecordingAsPicture(),
            SkISize::Make(tile_bounds.width(), tile_bounds.height()),
            nullptr, nullptr,
            SkImage::BitDepth::kU8,
            SkColorSpace::MakeSRGB());
    return image->makeShader(
            tile_mode_x,
            tile_mode_y,
            &adjusted_matrix);
}

}
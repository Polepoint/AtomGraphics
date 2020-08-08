//
// Created by neo on 2019-07-16.
//

#ifndef ATOMGRAPHICS_PAINTTYPEFACE_H
#define ATOMGRAPHICS_PAINTTYPEFACE_H

#include <string>

#include "FontDescription.h"
#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkFontStyle.h>
#include <skia/core/SkTypeface.h>

#endif

#if PLATFORM(IOS)

#include <CoreText/CoreText.h>
#include "RetainPtr.h"

#endif

namespace AtomGraphics {

class PaintTypeface {

public:

    PaintTypeface() = default;

    PaintTypeface(const PaintTypeface &other);

    static PaintTypeface FromFamilyNameAndDescription(
            const std::string &family_name,
            const FontDescription &description);

    PaintTypeface &operator=(const PaintTypeface &other);

    operator bool() const;

#if PLATFORM(ANDROID)

    const sk_sp <SkTypeface> &ToSkTypeface() const {return sk_typeface_;}

#endif

#if PLATFORM(IOS)

    const RetainPtr<CTFontRef> &CTFont() const { return font_; }

#endif

private:

    std::string family_name_;

#if PLATFORM(IOS)

    void CreateCTFont();

    RetainPtr<CTFontRef> font_;
    float font_size_;
    RetainPtr<CTFontDescriptorRef> descriptor_;

#endif

#if PLATFORM(ANDROID)

    void CreateSkTypeface();

    SkFontID sk_id_;
    sk_sp <SkTypeface> sk_typeface_;
    SkFontStyle font_style_;

#endif

};

}


#endif //ATOMGRAPHICS_PAINTTYPEFACE_H

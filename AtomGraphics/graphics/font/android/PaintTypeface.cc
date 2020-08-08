//
// Created by neo on 2019-07-16.
//

#include <skia/core/SkFontMgr.h>
#include <graphics/font/PaintTypeface.h>

#include "graphics/font/PaintTypeface.h"

namespace AtomGraphics {

PaintTypeface PaintTypeface::FromFamilyNameAndDescription(const std::string &family_name,
                                                          const FontDescription &description) {
    PaintTypeface typeface;
    typeface.family_name_ = family_name;
    typeface.font_style_ = description.skiaFontStyle();
    typeface.CreateSkTypeface();
    return typeface;
}

PaintTypeface &PaintTypeface::operator=(const PaintTypeface &other) {
    sk_id_ = other.sk_id_;
    sk_typeface_ = other.sk_typeface_;

    // TODO(crbug.com/785682): Debugging possible ref counting issue.
    if (sk_typeface_) {
        // Since we're copying this object which will do a weak unref in the dtor,
        // ensure to bump the weak ref one more time.
        sk_typeface_->weak_ref();
    }

    family_name_ = other.family_name_;
    font_style_ = other.font_style_;
    return *this;
}

PaintTypeface::operator bool() const {
    return !!sk_typeface_;
}

void PaintTypeface::CreateSkTypeface() {
    // This is a fallthrough in all cases in FontCache::CreateTypeface, so
    // this is done unconditionally. Since we create the typeface upon
    // PaintTypeface creation, this should be safe in all cases.
    auto fm(SkFontMgr::RefDefault());
    sk_typeface_ = fm->legacyMakeTypeface(family_name_.c_str(), font_style_);

    sk_id_ = sk_typeface_ ? sk_typeface_->uniqueID() : 0;

    // TODO(crbug.com/785682): Debugging possible ref counting issue.
    if (sk_typeface_)
        sk_typeface_->weak_ref();
}

}
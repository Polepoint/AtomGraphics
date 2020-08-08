//
// Created by neo on 2019-07-16.
//

#include "PaintTypeface.h"

namespace AtomGraphics {

PaintTypeface PaintTypeface::FromFamilyNameAndDescription(const std::string &family_name, const FontDescription &description) {
    PaintTypeface typeface;
    typeface.family_name_ = family_name;
    typeface.descriptor_ = description.ctFontDescriptor();
    typeface.font_size_ = description.getFontSize();
    typeface.CreateCTFont();
    return typeface;
}


PaintTypeface &PaintTypeface::operator=(const PaintTypeface &other) {
    descriptor_ = other.descriptor_;
    font_ = other.font_;
    family_name_ = other.family_name_;

    return *this;
}

PaintTypeface::operator bool() const {
    return !!font_;
}

void PaintTypeface::CreateCTFont() {
    auto familyName = adoptCF(CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, family_name_.c_str(), kCFStringEncodingUTF8, kCFAllocatorNull));
    auto descriptor = CTFontDescriptorCreateCopyWithFamily(descriptor_.get(), familyName.get());
    if (descriptor) {
        font_ = CTFontCreateWithFontDescriptor(descriptor, font_size_, nullptr);
    }
}

}
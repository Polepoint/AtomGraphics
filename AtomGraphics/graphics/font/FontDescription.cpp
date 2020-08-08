//
// Created by neo on 2019-07-22.
//

#include "FontDescription.h"

namespace AtomGraphics {

FontDescription::FontDescription() {
    m_fontSize = 12;
    weight_ = 400;
    slant_ = Slant::kNormal;
    m_smallCaps = false;
}

bool FontDescription::operator==(const FontDescription &other) const {
    if (this == &other) {
        return true;
    }

    bool sameTraits = m_fontSize == other.m_fontSize
            && weight_ == other.weight_
            && slant_ == other.slant_
            && m_smallCaps == other.m_smallCaps;
    if (!sameTraits) {
        return false;
    }

    const FontFamily *familyList = &family_list_;
    const FontFamily *otherFamilyList = &other.family_list_;

    while (familyList && otherFamilyList) {
        if (familyList->Family() == otherFamilyList->Family()) {
            familyList = familyList->Next();
            otherFamilyList = otherFamilyList->Next();
        } else {
            return false;
        }
    }

    return otherFamilyList == nullptr && familyList == nullptr;
}

const FontFamily *FontDescription::FamilyAt(int index) const {
    const FontFamily *family = &family_list_;
    if (index == 0) {
        return family;
    }

    for (int i = 1; i <= index; i++) {
        family = family->Next();
        if (!family) {
            return nullptr;
        }
    }

    return family;
}


}
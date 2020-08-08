//
// Created by neo on 2019/4/2.
//

#include "graphics/font/FontDescription.h"

namespace AtomGraphics {

SkFontStyle FontDescription::skiaFontStyle() const {
    SkFontStyle::Weight weight;
    if (weight_ >= SkFontStyle::kExtraBlack_Weight) {
        weight = SkFontStyle::kExtraBlack_Weight;
    } else if (weight_ >= SkFontStyle::kBlack_Weight) {
        weight = SkFontStyle::kBlack_Weight;
    } else if (weight_ >= SkFontStyle::kExtraBold_Weight) {
        weight = SkFontStyle::kExtraBold_Weight;
    } else if (weight_ >= SkFontStyle::kBold_Weight) {
        weight = SkFontStyle::kBold_Weight;
    } else if (weight_ >= SkFontStyle::kSemiBold_Weight) {
        weight = SkFontStyle::kSemiBold_Weight;
    } else if (weight_ >= SkFontStyle::kMedium_Weight) {
        weight = SkFontStyle::kMedium_Weight;
    } else if (weight_ >= SkFontStyle::kNormal_Weight) {
        weight = SkFontStyle::kNormal_Weight;
    } else if (weight_ >= SkFontStyle::kLight_Weight) {
        weight = SkFontStyle::kLight_Weight;
    } else if (weight_ >= SkFontStyle::kExtraLight_Weight) {
        weight = SkFontStyle::kExtraLight_Weight;
    } else if (weight_ >= SkFontStyle::kThin_Weight) {
        weight = SkFontStyle::kThin_Weight;
    } else {
        weight = SkFontStyle::kInvisible_Weight;
    }

    SkFontStyle::Slant slant;
    if (slant_ == kItalic) {
        slant = SkFontStyle::Slant::kItalic_Slant;
    } else if (slant_ == kOblique) {
        slant = SkFontStyle::Slant::kOblique_Slant;
    } else {
        slant = SkFontStyle::Slant::kUpright_Slant;
    }

    return SkFontStyle(weight, SkFontStyle::kNormal_Width, slant);
}

}
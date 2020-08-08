//
// Created by neo on 2019-06-28.
//

#include "FontCache.h"
#include "FontDescription.h"
#include "FontFallbackList.h"


namespace AtomGraphics {

FontFallbackList::FontFallbackList(const FontFallbackList &other) {
    font_list_ = other.font_list_;
}

const FontData *FontFallbackList::fontDataAt(const FontDescription &font_description, int index) const {
    if (font_list_.size() > index) {
        return font_list_[index].get();
    }

    for (int i = font_list_.size(); i <= index; i++) {
        const FontFamily *family = font_description.FamilyAt(i);
        if (family) {
            scoped_refptr<FontData> result =
                    FontCache::GetFontCache()->GetFontData(font_description, family->Family());
            if (result) {
                font_list_.push_back(result);
            } else {
                font_list_.push_back(FontCache::GetFontCache()->GetLastResortFallbackFont(font_description));
            }
        } else {
            return nullptr;
        }
    }


    return font_list_[index].get();
}

}

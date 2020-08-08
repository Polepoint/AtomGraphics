//
// Created by neo on 2019-07-17.
//

#ifndef ATOMGRAPHICS_FONTDATACACHE_H
#define ATOMGRAPHICS_FONTDATACACHE_H

#include "FontData.h"
#include <map>
#include "cache/TinyLRUCache.h"
#include "graphics/font/FontCacheKey.h"

namespace AtomGraphics {

const unsigned kCMaxInactiveFontData = 225;

struct FontDataCacheEntry {

    FontDataCacheEntry() = default;

    FontDataCacheEntry(const FontDataCacheEntry &other) {
        font_data_init_ = other.font_data_init_;
        font_data_ = other.font_data_;
    }

    void setFontData(scoped_refptr<FontData> font_data) {
        font_data_ = std::move(font_data);
        font_data_init_ = true;
    }

    bool font_data_init_{false};
    scoped_refptr<FontData> font_data_;
};


class FontDataCache {

public:

    FontDataCacheEntry *getFontData(FontCacheKey key);

private:

    struct FontDataTinyLRUCachePolicy : public TinyLRUCachePolicy<FontCacheKey, FontDataCacheEntry> {

        static FontDataCacheEntry createValueForKey(const FontCacheKey &key) {
            return FontDataCacheEntry();
        }

    };


    TinyLRUCache<FontCacheKey, FontDataCacheEntry, kCMaxInactiveFontData, FontDataTinyLRUCachePolicy> cache_;
};

}


#endif //ATOMGRAPHICS_FONTDATACACHE_H

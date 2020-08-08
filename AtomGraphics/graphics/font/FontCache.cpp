//
// Created by neo on 2019-07-11.
//

#include "FontCache.h"


namespace AtomGraphics {

PaintTypeface FontCache::CreateTypeface(
        const std::string &family_name,
        const FontDescription &font_description) {

    return PaintTypeface::FromFamilyNameAndDescription(family_name, font_description);
}

ShapeCache *FontCache::GetShapeCache(const FallbackListCompositeKey &key) {
    ShapeCache *result = nullptr;
    for (const auto &it : fallback_list_shaper_cache_) {
        if (it.first == key) {
            result = it.second.get();
            break;
        }
    }
    if (!result) {
        result = fallback_list_shaper_cache_.emplace(std::make_pair(key, std::make_unique<ShapeCache>()))
                .first->second.get();
    }

    return result;
}

scoped_refptr<FontData> FontCache::GetFontData(const FontDescription &description, const std::string &family_name) {

    FontCacheKey key = description.CacheKey(family_name);

    FontDataCacheEntry *entry = font_data_cache_.getFontData(key);

    scoped_refptr<FontData> result;

    if (!entry->font_data_init_) {
        result = CreateFontData(description, family_name);
        entry->setFontData(result);
    } else {
        result = entry->font_data_;
    }

    return result;
}

scoped_refptr<FontData> FontCache::CreateFontData(
        const FontDescription &font_description,
        const std::string &family_name) {

    PaintTypeface paint_tf = CreateTypeface(family_name, font_description);

    if (!paint_tf)
        return nullptr;

    scoped_refptr<FontData> font_data = MakeRefCounted<FontData>(
            paint_tf,
            family_name,
            font_description.ComputedSize(),
            font_description.Weight(),
            font_description.getSlant());

    return font_data;
}


}
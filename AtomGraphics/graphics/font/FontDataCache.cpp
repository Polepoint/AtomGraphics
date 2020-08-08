//
// Created by neo on 2019-07-17.
//

#include "FontDataCache.h"

namespace AtomGraphics {

FontDataCacheEntry *FontDataCache::getFontData(FontCacheKey key) {
    FontDataCacheEntry *entry = const_cast<FontDataCacheEntry *>(&cache_.get(key));
    return entry;
}

}
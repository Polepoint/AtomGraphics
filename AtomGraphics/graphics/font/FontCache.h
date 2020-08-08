//
// Created by neo on 2018/12/16.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FONTCACHE_H
#define ATOMGRAPHICS_FONTCACHE_H

#include "FontData.h"
#include "PaintTypeface.h"
#include "ShapeCache.h"
#include "FallbackListCompositeKey.h"
#include "FontDataCache.h"

#include "platform/AtomPlatformConfig.h"
#include "Font.h"

#if PLATFORM(ANDROID)

#include <map>
#include <skia/core/SkFontMgr.h>

#endif

namespace AtomGraphics {

class FontCache {

public:

    static FontCache *GetFontCache() {
        static FontCache *fontCache = nullptr;
        if (!fontCache) {
            fontCache = new FontCache();
        }

        return fontCache;
    }

    ShapeCache *GetShapeCache(const FallbackListCompositeKey &key);

    PaintTypeface CreateTypeface(const std::string &family_name,
                                 const FontDescription &font_description);

    scoped_refptr<FontData> GetFontData(const FontDescription &description, const std::string &family_name);

    scoped_refptr<FontData> GetLastResortFallbackFont(const FontDescription &description);

#if PLATFORM(ANDROID)

    scoped_refptr<FontData> FallbackFontForCharacter(const FontDescription &description, UChar32 lookup_char);

#elif PLATFORM(IOS)

    scoped_refptr <FontData> FallbackFontForCharacter(const Font &font, UChar32 lookup_char);

#endif

private:

    std::map<FallbackListCompositeKey, std::unique_ptr<ShapeCache >> fallback_list_shaper_cache_;

    FontDataCache font_data_cache_;

    scoped_refptr<FontData> CreateFontData(const FontDescription &font_description, const std::string &family_name);

#if PLATFORM(ANDROID)

    std::string GetFamilyNameForCharacter(SkFontMgr *fm, UChar32 c);

#endif

#if PLATFORM(IOS)

    std::string GetFamilyNameForCharacter(UChar32 c, const FontData *primaryFont);

#endif

};

}


#endif //ATOMGRAPHICS_FONTCACHE_H

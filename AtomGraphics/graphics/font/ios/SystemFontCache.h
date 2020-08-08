//
// Created by neo on 2019-09-03.
//

#ifndef ATOMGRAPHICS_SYSTEMFONTCACHE_H
#define ATOMGRAPHICS_SYSTEMFONTCACHE_H


#import <vector>

namespace AtomGraphics {

struct SystemFontCacheEntry {
    std::string m_familyName;
    RetainPtr<CTFontRef> m_font;

    SystemFontCacheEntry(const std::string &family_name, const RetainPtr<CTFontRef> &font) :
            m_familyName(family_name), m_font(font) {};

    SystemFontCacheEntry(const SystemFontCacheEntry &other) {
        m_familyName = other.m_familyName;
        m_font = other.m_font;
    }
};


static const size_t kSystemFontCacheCapacity = 10;

class SystemFontCache {

public:

    void add(SystemFontCacheEntry entry) {
        if (m_cache.size() == kSystemFontCacheCapacity)
            m_cache.erase(m_cache.begin());

        m_cache.push_back(entry);
    }

    void get(std::function<bool(const SystemFontCacheEntry &)> predicate) {
        for (long i = m_cache.size() - 1; i >= 0; i--) {
            const SystemFontCacheEntry &value = m_cache[i];
            if (predicate(value)) {
                if (i != m_cache.size() - 1) {
                    SystemFontCacheEntry entry = std::move(m_cache[i]);
                    m_cache.erase(m_cache.begin() + i);
                    m_cache.push_back(entry);
                }
                return;
            }
        }
    }


private:

    std::vector<SystemFontCacheEntry> m_cache;
};

}


#endif //ATOMGRAPHICS_SYSTEMFONTCACHE_H
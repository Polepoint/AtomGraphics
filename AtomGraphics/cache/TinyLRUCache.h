//
// Created by neo on 2018/7/29.
//

#ifndef ATOMGRAPHICS_TINYLRUCACHE_H
#define ATOMGRAPHICS_TINYLRUCACHE_H

#include <vector>
#include <cstddef>

namespace AtomGraphics {

template<typename KeyType, typename ValueType>

struct TinyLRUCachePolicy {
    static bool isKeyNull(const KeyType &) {
        return false;
    }
    
    static ValueType createValueForNullKey() {
        return {};
    }
    
    static ValueType createValueForKey(const KeyType &) {
        return {};
    }
};

template<typename KeyType, typename ValueType, size_t capacity = 4, typename Policy = TinyLRUCachePolicy<KeyType, ValueType>>
class TinyLRUCache {

public:
    const ValueType &get(const KeyType &key) {
        if (Policy::isKeyNull(key)) {
            static ValueType valueForNull = Policy::createValueForNullKey();
            return valueForNull;
        }
        
        for (size_t i = 0; i < m_cache.size(); ++i) {
            if (m_cache[i].first != key)
                continue;
            
            if (i == m_cache.size() - 1)
                return m_cache[i].second;
            
            // If the entry is not the last one, move it to the end of the cache.
            Entry entry = std::move(m_cache[i]);
            m_cache.erase(m_cache.begin() + i);
            m_cache.push_back(entry);
            return m_cache[m_cache.size() - 1].second;
        }
        
        // m_cache[0] is the LRU entry, so remove it.
        if (m_cache.size() == capacity)
            m_cache.erase(m_cache.begin());
        
        m_cache.push_back(std::make_pair(key, Policy::createValueForKey(key)));
        return m_cache[m_cache.size() - 1].second;
    }

private:
    typedef std::pair<KeyType, ValueType> Entry;
    typedef std::vector<Entry> Cache;
    Cache m_cache;
};
}


#endif //ATOMGRAPHICS_TINYLRUCACHE_H

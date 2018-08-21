//
// Created by neo on 2018/7/29.
//

#include <algorithm>
#include "TinyLRUCache.h"

namespace AtomGraphics {

    template<typename KeyType, typename ValueType, size_t capacity, typename Policy>
    const ValueType &TinyLRUCache<KeyType, ValueType, capacity, Policy>::get(const KeyType &key) {
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
}

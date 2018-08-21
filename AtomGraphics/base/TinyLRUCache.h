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
        static bool isKeyNull(const KeyType &) { return false; }

        static ValueType createValueForNullKey() { return {}; }

        static ValueType createValueForKey(const KeyType &) { return {}; }
    };

    template<typename KeyType, typename ValueType, size_t capacity = 4, typename Policy = TinyLRUCachePolicy<KeyType, ValueType>>
    class TinyLRUCache {

    public:
        const ValueType &get(const KeyType &key);

    private:
        typedef std::pair<KeyType, ValueType> Entry;
        typedef std::vector<Entry> Cache;
        Cache m_cache;
    };
}


#endif //ATOMGRAPHICS_TINYLRUCACHE_H

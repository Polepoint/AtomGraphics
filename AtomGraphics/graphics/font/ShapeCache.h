//
// Created by neo on 2019-07-09.
//

#ifndef ATOMGRAPHICS_SHAPECACHE_H
#define ATOMGRAPHICS_SHAPECACHE_H


#include "ShapeResult.h"
#include <map>
#include "memory/ScopedRefPtr.h"

namespace AtomGraphics {


struct ShapeCacheEntry {

    ShapeCacheEntry() { shape_result_ = nullptr; }

    scoped_refptr<const ShapeResult> shape_result_;
};


class ShapeCache {

public:

    ShapeCacheEntry *add(UChar c, ShapeCacheEntry entry) {
        return &(char_cache_map_.emplace(c, entry).first->second);
    }

private:

    std::map<UChar, ShapeCacheEntry> char_cache_map_;

};

}


#endif //ATOMGRAPHICS_SHAPECACHE_H

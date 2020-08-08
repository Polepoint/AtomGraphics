//
// Created by neo on 2019-07-10.
//

#ifndef ATOMGRAPHICS_FALLBACKLISTCOMPOSITEKEY_H
#define ATOMGRAPHICS_FALLBACKLISTCOMPOSITEKEY_H

#include "FontCacheKey.h"
#include "FontDescription.h"
#include <vector>

namespace AtomGraphics {

class FallbackListCompositeKey {

public:

    FallbackListCompositeKey(const FontDescription &font_description)
            : computed_size_(font_description.ComputedSize()) {
        setting_ = font_description.slant_ &
                (font_description.m_smallCaps) << 2 &
                (font_description.weight_) << 3;
    }

    FallbackListCompositeKey(const FallbackListCompositeKey &other) {
        computed_size_ = other.computed_size_;
        setting_ = other.setting_;
        font_cache_keys_ = other.font_cache_keys_;
    }

    void add(FontCacheKey key) {
        font_cache_keys_.push_back(key);
    }

    bool operator==(const FallbackListCompositeKey &other) const {
        if (setting_ == other.setting_
                && computed_size_ == other.computed_size_
                && font_cache_keys_.size() == other.font_cache_keys_.size()) {
            return std::equal(font_cache_keys_.begin(), font_cache_keys_.end(),
                              other.font_cache_keys_.begin());
        }

        return false;
    }

    bool operator<(const FallbackListCompositeKey &other) const {
        return !(*this == other);
    }

private:

    unsigned setting_;
    float computed_size_;
    std::vector<FontCacheKey> font_cache_keys_;

};

}


#endif //ATOMGRAPHICS_FALLBACKLISTCOMPOSITEKEY_H

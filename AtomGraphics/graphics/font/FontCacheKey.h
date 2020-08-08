//
// Created by neo on 2019-06-27.
//

#ifndef ATOMGRAPHICS_FONTCACHEKEY_H
#define ATOMGRAPHICS_FONTCACHEKEY_H

#include <string>

namespace AtomGraphics {

class FontCacheKey {

public:

    FontCacheKey(float font_size, unsigned options, const std::string &family)
            : font_size_(font_size),
              options_(options),
              family_(family) {

    }

    FontCacheKey(const FontCacheKey &other) {
        font_size_ = other.font_size_;
        options_ = other.options_;
        family_ = other.family_;
    }

    bool operator==(const FontCacheKey &other) const {
        return font_size_ == other.font_size_
                && options_ == other.options_
                && family_ == other.family_;
    }

    bool operator!=(const FontCacheKey &other) const {
        return !(*this == other);
    }

    bool operator<(const FontCacheKey &other) const {
        return font_size_ < other.font_size_
                || options_ < other.options_
                || family_ < other.family_;
    }


private:
    float font_size_;
    unsigned options_;

    std::string family_;
};

}


#endif //ATOMGRAPHICS_FONTCACHEKEY_H

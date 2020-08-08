//
// Created by neo on 2019-07-11.
//

#ifndef ATOMGRAPHICS_FONTFAMILY_H
#define ATOMGRAPHICS_FONTFAMILY_H

#include <string>
#include "memory/RefCounted.h"

namespace AtomGraphics {

class SharedFontFamily;

class FontFamily {

public:

    FontFamily() = default;

    FontFamily(const FontFamily &other);

    void SetFamily(const std::string &family) { family_ = family; }

    const std::string &Family() const {
        return family_;
    }

    const FontFamily *Next() const;

    void AppendFamily(scoped_refptr<SharedFontFamily>);

    scoped_refptr<SharedFontFamily> ReleaseNext();

private:
    std::string family_;
    scoped_refptr<SharedFontFamily> next_;
};


class SharedFontFamily :
        public FontFamily,
        public RefCounted<SharedFontFamily> {
public:
    static scoped_refptr<SharedFontFamily> Create() {
        return WrapRefCounted(new SharedFontFamily);
    }

private:
    SharedFontFamily() = default;
};

inline const FontFamily *FontFamily::Next() const {
    return next_.get();
}

inline void FontFamily::AppendFamily(scoped_refptr<SharedFontFamily> family) {
    next_ = std::move(family);
}

inline scoped_refptr<SharedFontFamily> FontFamily::ReleaseNext() {
    return std::move(next_);
}


}


#endif //ATOMGRAPHICS_FONTFAMILY_H

//
// Created by neo on 2019-06-28.
//

#ifndef ATOMGRAPHICS_FONTFALLBACKLIST_H
#define ATOMGRAPHICS_FONTFALLBACKLIST_H

#include <string>
#include <vector>
#include "FontData.h"
#include "memory/RefCounted.h"

namespace AtomGraphics {

class FontDescription;

class FontFallbackList {

public:

    FontFallbackList() = default;

    FontFallbackList(const FontFallbackList &other);

    const FontData *fontDataAt(const FontDescription &font_description, int index) const;

private:

    mutable std::vector<scoped_refptr < FontData>> font_list_;
};

}


#endif //ATOMGRAPHICS_FONTFALLBACKLIST_H

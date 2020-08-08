//
// Created by neo on 2019/1/26.
//

#ifndef ATOMGRAPHICS_CLIPLIST_H
#define ATOMGRAPHICS_CLIPLIST_H

#include <skia/core/SkPath.h>
#include <vector>
#include <skia/core/SkCanvas.h>

namespace AtomGraphics {

class ClipList {

public:

    ClipList() = default;

    ClipList(const ClipList &);

    ~ClipList() = default;

    void ClipPath(const SkPath &path, const SkMatrix &ctm);

    void Playback(SkCanvas*) const;

private:
    struct ClipOp {
        SkPath path_;

        ClipOp() = default;

        ClipOp(const ClipOp &) = default;
    };

    std::vector<ClipOp> clip_list_;

};

}


#endif //ATOMGRAPHICS_CLIPLIST_H

//
// Created by neo on 2019/1/26.
//

#include <skia/pathops/SkPathOps.h>
#include "ClipList.h"

namespace AtomGraphics {

ClipList::ClipList(const ClipList &other) : clip_list_(other.clip_list_) {}

void ClipList::ClipPath(const SkPath &path, const SkMatrix &ctm) {
    ClipOp new_clip;
    new_clip.path_ = path;
    new_clip.path_.transform(ctm);
    clip_list_.push_back(new_clip);
}

void ClipList::Playback(SkCanvas *canvas) const {
    for (auto it = clip_list_.begin(); it < clip_list_.end(); it++) {
        canvas->clipPath(it->path_, SkClipOp::kIntersect, true);
    }
}

}

//
// Created by neo on 2019-07-19.
//

#ifndef ATOMGRAPHICS_PAINTTEXTBLOB_H
#define ATOMGRAPHICS_PAINTTEXTBLOB_H


#include "PaintTypeface.h"
#include "memory/RefCounted.h"
#include <vector>
#include "platform/AtomPlatformConfig.h"

#if PLATFORM(IOS)

#include "graphics/font/ios/TextBlob.h"

#endif

#if PLATFORM(ANDROID)

#include <skia/core/SkTextBlob.h>

#endif

namespace AtomGraphics {

class PaintTextBlob : public RefCounted<PaintTextBlob> {

public:

    PaintTextBlob() = default;

#if PLATFORM(IOS)

    PaintTextBlob(scoped_refptr<TextBlob> blob, std::vector<PaintTypeface> typefaces);

    const scoped_refptr<TextBlob> &ToTextBlob() const { return blob_; }

#endif

#if PLATFORM(ANDROID)

    PaintTextBlob(sk_sp<SkTextBlob> blob, std::vector<PaintTypeface> typefaces);

    const sk_sp<SkTextBlob> &ToSkTextBlob() const { return sk_blob_; }

#endif

    const std::vector<PaintTypeface> &typefaces() const { return typefaces_; }

    operator bool() const;


private:

#if PLATFORM(IOS)

    scoped_refptr<TextBlob> blob_;

#endif

#if PLATFORM(ANDROID)

    sk_sp<SkTextBlob> sk_blob_;

#endif

    std::vector<PaintTypeface> typefaces_;


};

}


#endif //ATOMGRAPHICS_PAINTTEXTBLOB_H

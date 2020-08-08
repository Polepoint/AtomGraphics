//
// Created by neo on 2020/4/26.
//

#include "DiskCache.h"
#include "base/md5.h"

namespace AtomGraphics {

inline void md5FilePath(const char *str, unsigned long length, char *result) {
    unsigned char decrypt[16] = {0};
    yasm_md5_context context;
    yasm_md5_init(&context);
    unsigned char const *buf = reinterpret_cast<const unsigned char *>(str);
    yasm_md5_update(&context, buf, length);
    yasm_md5_final(decrypt, &context);

    for (size_t i = 0; i != 16; ++i) {
        sprintf(result, "%02x", decrypt[i]);
        result += 2;
    }
}

CachedImageHandle DiskCache::resourceForRequest(const CachedImageRequest &request) {
    const std::string &url = request.url();
    char decryptStr[33] = {0};

    md5FilePath(url.c_str(), url.size(), decryptStr);
    scoped_refptr<SharedBuffer> buffer = dataBufferFromFile(decryptStr);

    if (buffer) {
        CachedImageHandle resource(new CachedImage(request));
        resource->setData(buffer);
        return resource;
    } else {
        return CachedImageHandle();
    }
}

void DiskCache::add(const CachedImageHandle &request) {
    const std::string &url = request->url();
    char decryptStr[33] = {0};

    md5FilePath(url.c_str(), url.size(), decryptStr);
    saveBufferDataToFile(decryptStr, request->data());
}

}
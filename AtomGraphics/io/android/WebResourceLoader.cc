//
// Created by neo on 2020/5/2.
//

#include "android/WebResourceLoaderHandler.h"
#include "io/WebResourceLoader.h"
#include "io/ResourceRequest.h"

namespace AtomGraphics {

void WebResourceLoader::loadResource(
        ResourceRequest &request,
        std::function<void(scoped_refptr<SharedBuffer>,
                           float, float,
                           ImageBitmapConfiguration configuration)> completeCallback) {

    WebResourceLoaderHandler::loadResource(
            request.url(),
            [completeCallback](void *ptr,
                               uint32_t bufferSize,
                               uint32_t width,
                               uint32_t height) -> void {

                scoped_refptr <SharedBuffer> sharedBuffer;
                if (ptr) {
                    sharedBuffer = MakeRefCounted<SharedBuffer>();
                    sharedBuffer->copyData(ptr, bufferSize);
                }
                completeCallback(sharedBuffer, width, height, ImageBitmapConfiguration());
            });
}

}
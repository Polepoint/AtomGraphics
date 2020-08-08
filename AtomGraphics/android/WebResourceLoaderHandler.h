//
// Created by neo on 2020/5/3.
//

#ifndef ATGPROJECT_WEBRESOURCELOADERHANDLER_H
#define ATGPROJECT_WEBRESOURCELOADERHANDLER_H

#include <jni.h>
#include <string>

namespace AtomGraphics {

class WebResourceLoaderHandler {

public:

    static void loadResource(const std::string &url,
                             const std::function<void(void *,
                                                      uint32_t,
                                                      uint32_t,
                                                      uint32_t)> &completeCallback);

    void complete(void *bytes, uint32_t bufferSize, uint32_t width, uint32_t height);

private:

    WebResourceLoaderHandler(
            const std::function<void(void *, uint32_t, uint32_t, uint32_t)> &completeCallback)
            : m_completeCallback(completeCallback) {
    }


    std::function<void(void *, uint32_t, uint32_t, uint32_t)> m_completeCallback;
};

}

#endif //ATGPROJECT_WEBRESOURCELOADERHANDLER_H

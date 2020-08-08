//
// Created by neo on 2019-09-28.
//

#ifndef ATOMGRAPHICS_NATIVEEXECUTIONENVIRONMENTJNI_H
#define ATOMGRAPHICS_NATIVEEXECUTIONENVIRONMENTJNI_H


#include <functional>
#include "graphics/DisplayRefreshMonitorClient.h"

namespace AtomGraphics {

class NativeExecutionEnvironmentJNI {

public:

    void requestFrame(JNIEnv *env, jobject jRef);

private:

    class FrameMonitorClient final : public DisplayRefreshMonitorClient {

    public:

        FrameMonitorClient();

        ~FrameMonitorClient();

        void displayRefreshFired() override;

        void schedule() {
            m_scheduled = true;
        }

        bool scheduling() {
            return m_scheduled;
        }

        std::function<void()> m_callbackFunction;
    };

    std::unique_ptr<FrameMonitorClient> m_frameMonitorClient;
};

}

#endif //ATOMGRAPHICS_NATIVEEXECUTIONENVIRONMENTJNI_H

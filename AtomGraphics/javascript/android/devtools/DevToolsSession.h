//
// Created by neo on 2019-08-21.
//

#ifndef ATOMGRAPHICS_DEVTOOLSSESSION_H
#define ATOMGRAPHICS_DEVTOOLSSESSION_H

#include <v8/v8-inspector.h>

namespace AtomGraphics {

namespace JS {

namespace Debug {

class DevToolsSession : public v8_inspector::V8Inspector::Channel {

public:

    virtual void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message);

    virtual void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message);

    virtual void flushProtocolNotifications();
};

}

}

}


#endif //ATOMGRAPHICS_DEVTOOLSSESSION_H

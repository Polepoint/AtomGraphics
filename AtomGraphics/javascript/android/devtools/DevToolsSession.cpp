//
// Created by neo on 2019-08-21.
//

#include "DevToolsSession.h"

namespace AtomGraphics {

namespace JS {

namespace Debug {


void DevToolsSession::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) {

}

void DevToolsSession::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) {

}

void DevToolsSession::flushProtocolNotifications() {

}

}

}

}
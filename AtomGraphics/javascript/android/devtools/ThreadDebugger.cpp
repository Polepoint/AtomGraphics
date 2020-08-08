//
// Created by neo on 2019-08-20.
//

#include "javascript/v8/V8JSContext.h"
#include "javascript/v8/V8JavaScriptInterface.h"
#include "javascript/v8/V8Platform.h"
#include "base/Clock.h"
#include "ThreadDebugger.h"

namespace AtomGraphics {

namespace JS {

namespace Debug {

static ThreadDebugger *instance_;

const static int kDefaultContextGroupID = -1;

const char kDefaultContextName[] = "AtomGraphics-V8Context";

ThreadDebugger::ThreadDebugger(v8::Isolate *isolate) :
        isolate_(isolate),
        v8_inspector_(v8_inspector::V8Inspector::create(isolate, this)) {

}

ThreadDebugger *ThreadDebugger::Instance() {
    if (!instance_) {
        instance_ = new ThreadDebugger(V8Platform::SharedPlatform()->getIsolate());
    }
    return instance_;
}

void ThreadDebugger::ContextCreated(v8::Local<v8::Context> context) {
    v8_inspector::V8ContextInfo context_info(
            context, kDefaultContextGroupID,
            v8_inspector::StringView((uint8_t *) kDefaultContextName,
                                     std::strlen(kDefaultContextName)));
    v8_inspector_->contextCreated(context_info);
}

void ThreadDebugger::runMessageLoopOnPause(int contextGroupId) {
    pause_ = true;
}

void ThreadDebugger::quitMessageLoopOnPause() {
    pause_ = false;
}

void ThreadDebugger::runIfWaitingForDebugger(int contextGroupId) {

}

void ThreadDebugger::muteMetrics(int contextGroupId) {

}

void ThreadDebugger::unmuteMetrics(int contextGroupId) {

}

void ThreadDebugger::beginUserGesture() {

}

void ThreadDebugger::endUserGesture() {

}

std::unique_ptr<v8_inspector::StringBuffer> ThreadDebugger::valueSubtype(v8::Local<v8::Value> local) {
    return V8InspectorClient::valueSubtype(local);
}

bool ThreadDebugger::formatAccessorsAsProperties(v8::Local<v8::Value> local) {
    return V8InspectorClient::formatAccessorsAsProperties(local);
}

bool ThreadDebugger::isInspectableHeapObject(v8::Local<v8::Object> local) {
    return V8InspectorClient::isInspectableHeapObject(local);
}

v8::Local<v8::Context> ThreadDebugger::ensureDefaultContextInGroup(int contextGroupId) {
    return V8JavaScriptInterface::SharedInterface()->context()->getGlobalContext().Get(isolate_);
}

void ThreadDebugger::beginEnsureAllContextsInGroup(int contextGroupId) {

}

void ThreadDebugger::endEnsureAllContextsInGroup(int contextGroupId) {

}

void ThreadDebugger::installAdditionalCommandLineAPI(v8::Local<v8::Context> local, v8::Local<v8::Object> local1) {

}

void ThreadDebugger::consoleAPIMessage(int contextGroupId, v8::Isolate::MessageErrorLevel level,
                                       const v8_inspector::StringView &message, const v8_inspector::StringView &url,
                                       unsigned lineNumber, unsigned columnNumber, v8_inspector::V8StackTrace *trace) {

}

v8::MaybeLocal<v8::Value> ThreadDebugger::memoryInfo(v8::Isolate *isolate, v8::Local<v8::Context> local) {
    return V8InspectorClient::memoryInfo(isolate, local);
}

void ThreadDebugger::consoleTime(const v8_inspector::StringView &title) {

}

void ThreadDebugger::consoleTimeEnd(const v8_inspector::StringView &title) {

}

void ThreadDebugger::consoleTimeStamp(const v8_inspector::StringView &title) {

}

void ThreadDebugger::consoleClear(int contextGroupId) {

}

double ThreadDebugger::currentTimeMS() {
    return Clock::now() / 1000.0;
}

void ThreadDebugger::startRepeatingTimer(double d,
                                         v8_inspector::V8InspectorClient::TimerCallback callback,
                                         void *data) {
}

void ThreadDebugger::cancelTimer(void *data) {

}

bool ThreadDebugger::canExecuteScripts(int contextGroupId) {
    return V8InspectorClient::canExecuteScripts(contextGroupId);
}

void ThreadDebugger::maxAsyncCallStackDepthChanged(int depth) {

}

}

}

}

//
// Created by neo on 2019-08-20.
//

#ifndef ATOMGRAPHICS_THREADDEBUGGER_H
#define ATOMGRAPHICS_THREADDEBUGGER_H


#include <v8/v8-inspector.h>

namespace AtomGraphics {

namespace JS {

namespace Debug {

class ThreadDebugger : public v8_inspector::V8InspectorClient {

public:

    ThreadDebugger(v8::Isolate *);

    static ThreadDebugger *Instance();

    void ContextCreated(v8::Local<v8::Context> context);

    void runMessageLoopOnPause(int contextGroupId) override;

    void quitMessageLoopOnPause() override;

    void runIfWaitingForDebugger(int contextGroupId) override;

    void muteMetrics(int contextGroupId) override;

    void unmuteMetrics(int contextGroupId) override;

    void beginUserGesture() override;

    void endUserGesture() override;

    std::unique_ptr<v8_inspector::StringBuffer> valueSubtype(v8::Local<v8::Value> local) override;

    bool formatAccessorsAsProperties(v8::Local<v8::Value> local) override;

    bool isInspectableHeapObject(v8::Local<v8::Object> local) override;

    v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

    void beginEnsureAllContextsInGroup(int contextGroupId) override;

    void endEnsureAllContextsInGroup(int contextGroupId) override;

    void installAdditionalCommandLineAPI(v8::Local<v8::Context> local, v8::Local<v8::Object> local1) override;

    void consoleAPIMessage(int contextGroupId, v8::Isolate::MessageErrorLevel level,
                           const v8_inspector::StringView &message, const v8_inspector::StringView &url,
                           unsigned lineNumber, unsigned columnNumber,
                           v8_inspector::V8StackTrace *trace) override;

    v8::MaybeLocal<v8::Value> memoryInfo(v8::Isolate *isolate, v8::Local<v8::Context> local) override;

    void consoleTime(const v8_inspector::StringView &title) override;

    void consoleTimeEnd(const v8_inspector::StringView &title) override;

    void consoleTimeStamp(const v8_inspector::StringView &title) override;

    void consoleClear(int contextGroupId) override;

    double currentTimeMS() override;

    void startRepeatingTimer(double d, TimerCallback callback, void *data) override;

    void cancelTimer(void *data) override;

    bool canExecuteScripts(int contextGroupId) override;

    void maxAsyncCallStackDepthChanged(int depth) override;

private:

    v8::Isolate *isolate_;
    std::unique_ptr<v8_inspector::V8Inspector> v8_inspector_;
    bool pause_{false};

};

}

}

}


#endif //ATOMGRAPHICS_THREADDEBUGGER_H

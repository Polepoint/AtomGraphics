//
// Created by neo on 2018/7/29.
//

#include <javascript/ScriptTimer.h>
#include <javascript/ScriptAnimationFrameController.h>
#include <graphics/GraphicsPageContext.h>
#include "JSTimerAPI.h"

using namespace AtomGraphics;

namespace AtomGraphicsJSC {

    void JSTimerAPI::InitJSTimerAPIs(JSContext *context, JSObject *windowRef, GraphicsPageContext *pageContext) {
        JSFunctionObjectCallback setTimeoutCallback = [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                         JSObject *arguments[]) {
            JSObject *timerIDRef = nullptr;
            if (argumentCount < 1) {
                return timerIDRef;
            }

            JSObject *function = arguments[0];
            long delay = 0;
            if (argumentCount >= 2) {
                delay = arguments[1]->toDouble();
            }

            long timerID = ScriptTimer::CreateTimerID(delay, false, [thisObject, function] {
                function->callAsFunction(thisObject);
            });

            timerIDRef = new JSObject(ctx, timerID);

            return timerIDRef;
        };

        JSFunctionObjectCallbackWithoutReturn clearTimerCallback = [](JSContext *ctx, JSObject *thisObject,
                                                                      size_t argumentCount, JSObject *arguments[]) {
            if (argumentCount > 0) {
                JSObject *arg0 = arguments[0];
                if (arg0->isNumber()) {
                    long timerID = arguments[0]->toDouble();
                    ScriptTimer::RemoveTimerID(timerID);
                }
            }
        };

        windowRef->setProperty("setTimeout", setTimeoutCallback);

        windowRef->setProperty("clearTimeout", clearTimerCallback);

        //setInterval clearInterval
        JSFunctionObjectCallback setIntervalCallback = [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                          JSObject *arguments[]) {
            JSObject *timerIDRef = nullptr;
            if (argumentCount < 2) {
                return timerIDRef;
            }

            JSObject *function = arguments[0];
            long delay = arguments[1]->toDouble();

            long timerID = ScriptTimer::CreateTimerID(delay, true, [thisObject, function] {
                function->callAsFunction(thisObject);
            });

            timerIDRef = new JSObject(ctx, timerID);

            return timerIDRef;
        };

        windowRef->setProperty("setInterval", setIntervalCallback);
        windowRef->setProperty("clearInterval", clearTimerCallback);

        ScriptAnimationFrameController *animationFrameController =
                ScriptAnimationFrameController::ControllerForContext(pageContext);

        JSFunctionObjectCallback requestAnimationFrameCallback = [animationFrameController, pageContext](JSContext *ctx,
                                                                                                         JSObject *thisObject,
                                                                                                         size_t argumentCount,
                                                                                                         JSObject *arguments[]) {
            JSObject *requestID = nullptr;
            if (argumentCount > 0) {
                JSObject *functionObj = arguments[0];

                if (functionObj->isFunction()) {
                    long callbackID = animationFrameController->addCallback(
                            new RequestAnimationCallback([thisObject, functionObj]() {
                                functionObj->callAsFunction(thisObject);
                            }));
                    requestID = new JSObject(ctx, callbackID);
                    pageContext->flushController()->scheduleLayerFlush();
                }
            }

            return requestID;
        };

        windowRef->setProperty("requestAnimationFrame", requestAnimationFrameCallback);


        windowRef->setProperty("cancelAnimationFrame", [animationFrameController](JSContext *ctx, JSObject *thisObject,
                                                                                  size_t argumentCount,
                                                                                  JSObject *arguments[]) -> void {
            if (argumentCount > 0) {
                long callbackID = arguments[0]->toDouble();
                animationFrameController->cancelCallback(callbackID);
            }
        });
    }
}
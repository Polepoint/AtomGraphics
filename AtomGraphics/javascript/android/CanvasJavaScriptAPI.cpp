//
// Created by neo on 2018/7/29.
//

#include "CanvasJavaScriptAPI.h"
#include "jsbridge/JSObject.h"
#include "node/canvas/AtomCanvasContext2D.h"
#include "CanvasContextJavaScriptAPI.h"

using namespace AtomGraphics;
using namespace AtomGraphicsJSC;

inline void OnCanvasSetProperty(JSContext *context, Node *node, std::string propertyName, JSObject *value) {
    if (propertyName == "width") {
        Rect frame = node->getFrame();
        node->setFrame(Rect(frame.origin.x, frame.origin.y,
                            static_cast<float>(value->toDouble()), frame.size.height));
    } else if (propertyName == "height") {
        Rect frame = node->getFrame();
        node->setFrame(Rect(frame.origin.x, frame.origin.y,
                            frame.size.width, static_cast<float>(value->toDouble())));
    }
}

inline JSObject *OnCanvasGetProperty(JSContext *context, Node *node, std::string propertyName) {
    JSObject *value = nullptr;
    double height = node->getFrame().size.height;
    double width = node->getFrame().size.width;
    if (propertyName == "width") {
        value = new JSObject(context, width);
    } else if (propertyName == "height") {
        value = new JSObject(context, height);
    }
    return value;
}

namespace AtomGraphicsJSC {

    JSObject *CanvasJavaScriptAPI::JSObjectForCanvasNode(JSContext *context, CanvasNode *node) {
        JSObject *nodeObj = JSObject::getCachedObject(context);
        if (!nodeObj) {
            nodeObj = new JSObject(
                    context, node,
                    [node](JSContext *ctx, const char *propertyName, JSObject *value) {
                        OnCanvasSetProperty(ctx, node, std::string(propertyName), value);
                    }, [node](JSContext *ctx, const char *propertyName) {
                        return OnCanvasGetProperty(ctx, node, std::string(propertyName));
                    });
            CanvasJavaScriptAPI::InitCanvasAPIs(context, nodeObj);
        }

        return nodeObj;
    }

    void CanvasJavaScriptAPI::InitCanvasAPIs(JSContext *context, JSObject *rootNode) {

        CanvasNode *canvasNode = static_cast<CanvasNode *>(rootNode->toObjectPointer());
        CanvasContext2D *context2d = canvasNode->getContext2d();
        JSObject *jsCanvasContext = CanvasContextJavaScriptAPI::JSObjectForCanvasContext(context, context2d);

        JSFunctionObjectCallback callback = [jsCanvasContext](JSContext *ctx, JSObject *thisObject,
                                                              size_t argumentCount, JSObject *arguments[]) {
            std::string contextType;
            JSObject *returnValue = nullptr;

            if (argumentCount == 0) {
                contextType = "2d";
            } else {
                const JSObject *value = arguments[0];
                contextType = value->toString();
            }
            if (contextType == "2d") {
                returnValue = jsCanvasContext;
            }

            return returnValue;
        };

        rootNode->setProperty("getContext", callback);
    }
}
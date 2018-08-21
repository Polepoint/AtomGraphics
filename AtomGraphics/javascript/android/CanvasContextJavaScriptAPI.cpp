//
// Created by neo on 2018/7/29.
//

#include "base/AtomConvert.h"
#include "CanvasContextJavaScriptAPI.h"
#include "jsbridge/JSObject.h"
#include "node/canvas/AtomCanvasContext2D.h"

using namespace AtomGraphics;
using namespace AtomGraphicsJSC;

inline void OnContext2dSetProperty(JSContext *context, CanvasContext2D *context2d, const char *propertyName,
                                   JSObject *value) {

    std::string propertyNameStr = propertyName;
    if (propertyNameStr == "fillStyle") {
        if (value->isString()) {
            Color4F colorStyle = StrValueToColor4F(value->toString());
            context2d->setFillStyle(colorStyle);
        } else if (value->isObject()) {
            AtomCanvasFillStyle *style = ((AtomCanvasFillStyle *) value->toObjectPointer());
            if (style != nullptr) {
                if (style->isPattern()) {
                    context2d->setFillStyle((AtomCanvasPattern *) style);
                } else if (style->isLinearGradient()) {
                    context2d->setFillStyle((AtomCanvasLinearGradient *) style);
                } else if (style->isRadialGradient()) {
                    context2d->setFillStyle((AtomCanvasRadialGradient *) style);
                }
            }
        }
    } else if (propertyNameStr == "shadowBlur") {
        double blur = value->toDouble();
        context2d->setShadowBlur(blur);
    } else if (propertyNameStr == "shadowColor") {
        Color4F shadowColor = StrValueToColor4F(value->toString());
        context2d->setShadowColor(shadowColor);
    } else if (propertyNameStr == "shadowOffsetX") {
        double x = value->toDouble();
        context2d->setShadowOffsetX(x);
    } else if (propertyNameStr == "shadowOffsetY") {
        double y = value->toDouble();
        context2d->setShadowOffsetY(y);
    } else if (propertyNameStr == "strokeStyle") {
        if (value->isString()) {
            Color4F colorStyle = StrValueToColor4F(value->toString());
            context2d->setStrokeStyle(colorStyle);
        }
    } else if (propertyNameStr == "lineWidth") {
        if (value->isNumber()) {
            context2d->setLineWidth(value->toDouble());
        }

    } else if (propertyNameStr == "lineCap") {
        if (value->isString()) {
            context2d->setLineCap(value->toString());
        }
    } else if (propertyNameStr == "lineJoin") {
        if (value->isString()) {
            context2d->setLineJoin(value->toString());
        }
    } else if (propertyNameStr == "miterLimit") {
        if (value->isNumber()) {
            context2d->setMiterLimit(value->toDouble());
        }
    } else if (propertyNameStr == "globalAlpha") {
        if (value->isNumber()) {
            context2d->setGlobalAlpha(value->toDouble());
        }
    } else if (propertyNameStr == "globalCompositeOperation") {
        if (value->isString()) {
            context2d->setGlobalCompositeOperation(value->toString());
        }
    } else if (propertyNameStr == "font") {
        if (value->isString()) {
            context2d->setFont(value->toString());
        }
    } else if (propertyNameStr == "textAlign") {
        if (value->isString()) {
            context2d->setTextAlign(value->toString());
        }
    } else if (propertyNameStr == "textBaseline") {
        if (value->isString()) {
            context2d->setTextBaseline(value->toString());
        }
    }
}

inline JSObject *OnContext2dGetProperty(JSContext *context, CanvasContext2D *context2d, const char *propertyName) {
    JSObject *value = nullptr;
    return value;
}

namespace AtomGraphicsJSC {

    JSObject *CanvasContextJavaScriptAPI::JSObjectForCanvasContext(JSContext *context,
                                                                   AtomGraphics::CanvasContext2D *context2d) {
        JSObject *contextJSObj = JSObject::getCachedObject(context2d);
        if (!contextJSObj) {
            contextJSObj = new JSObject(context, context2d,
                                        [context2d](JSContext *ctx, const char *propertyName, JSObject *value) {
                                            OnContext2dSetProperty(ctx, context2d, propertyName, value);
                                        }, [context2d](JSContext *ctx, const char *propertyName) {
                        return OnContext2dGetProperty(ctx, context2d, propertyName);
                    });
            InitContextAPIs(contextJSObj);
            return contextJSObj;
        }

        return contextJSObj;
    }

    void CanvasContextJavaScriptAPI::InitContextAPIs(JSObject *canvasContext2dJS) {

        canvasContext2dJS->setProperty("beginPath", [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                       JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->beginPath();
        });

        canvasContext2dJS->setProperty("stroke", [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                    JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->stroke();
        });


        canvasContext2dJS->setProperty("fill", [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                  JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->fill();
        });

        canvasContext2dJS->setProperty("moveTo", [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                    JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->moveTo(arguments[0]->toDouble(), arguments[1]->toDouble());
        });

        canvasContext2dJS->setProperty("closePath", [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                       JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->closePath();
        });

        canvasContext2dJS->setProperty("lineTo", [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                    JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->lineTo(arguments[0]->toDouble(), arguments[1]->toDouble());
        });

        canvasContext2dJS->setProperty("clip", [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                                  JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->clip();
        });

        canvasContext2dJS->setProperty("quadraticCurveTo", [](JSContext *ctx,
                                                              JSObject *thisObject,
                                                              size_t argumentCount,
                                                              JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->quadraticCurveTo(arguments[0]->toDouble(),
                                              arguments[1]->toDouble(),
                                              arguments[2]->toDouble(),
                                              arguments[3]->toDouble());
        });

        canvasContext2dJS->setProperty("bezierCurveTo", [](JSContext *ctx,
                                                           JSObject *thisObject,
                                                           size_t argumentCount,
                                                           JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->bezierCurveTo(arguments[0]->toDouble(),
                                           arguments[1]->toDouble(),
                                           arguments[2]->toDouble(),
                                           arguments[3]->toDouble(),
                                           arguments[4]->toDouble(),
                                           arguments[5]->toDouble());
        });

        canvasContext2dJS->setProperty("arc", [](JSContext *ctx,
                                                 JSObject *thisObject,
                                                 size_t argumentCount,
                                                 JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            if (argumentCount >= 5) {
                canvasContext2d->arc(arguments[0]->toDouble(),
                                     arguments[1]->toDouble(),
                                     arguments[2]->toDouble(),
                                     arguments[3]->toDouble(),
                                     arguments[4]->toDouble(),
                                     argumentCount >= 6 ? arguments[5]->toBoolean() : false
                );
            }
        });

        canvasContext2dJS->setProperty("arcTo", [](JSContext *ctx,
                                                   JSObject *thisObject,
                                                   size_t argumentCount,
                                                   JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->arcTo(arguments[0]->toDouble(),
                                   arguments[1]->toDouble(),
                                   arguments[2]->toDouble(),
                                   arguments[3]->toDouble(),
                                   arguments[4]->toDouble()
            );
        });


        canvasContext2dJS->setProperty("rect", [](JSContext *ctx,
                                                  JSObject *thisObject,
                                                  size_t argumentCount,
                                                  JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->setRect(arguments[0]->toDouble(),
                                     arguments[1]->toDouble(),
                                     arguments[2]->toDouble(),
                                     arguments[3]->toDouble()
            );
        });

        canvasContext2dJS->setProperty("fillRect", [](JSContext *ctx,
                                                      JSObject *thisObject,
                                                      size_t argumentCount,
                                                      JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->fillRect(arguments[0]->toDouble(),
                                      arguments[1]->toDouble(),
                                      arguments[2]->toDouble(),
                                      arguments[3]->toDouble()
            );
        });
        canvasContext2dJS->setProperty("strokeRect", [](JSContext *ctx,
                                                        JSObject *thisObject,
                                                        size_t argumentCount,
                                                        JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->strokeRect(arguments[0]->toDouble(),
                                        arguments[1]->toDouble(),
                                        arguments[2]->toDouble(),
                                        arguments[3]->toDouble()
            );
        });

        canvasContext2dJS->setProperty("clearRect", [](JSContext *ctx,
                                                       JSObject *thisObject,
                                                       size_t argumentCount,
                                                       JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->clearRect(arguments[0]->toDouble(),
                                       arguments[1]->toDouble(),
                                       arguments[2]->toDouble(),
                                       arguments[3]->toDouble()
            );
        });

        canvasContext2dJS->setProperty("scale", [](JSContext *ctx,
                                                   JSObject *thisObject,
                                                   size_t argumentCount,
                                                   JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->scale(arguments[0]->toDouble(),
                                   arguments[1]->toDouble()
            );
        });

        canvasContext2dJS->setProperty("rotate", [](JSContext *ctx,
                                                    JSObject *thisObject,
                                                    size_t argumentCount,
                                                    JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->rotate(arguments[0]->toDouble());
        });

        canvasContext2dJS->setProperty("transform", [](JSContext *ctx,
                                                       JSObject *thisObject,
                                                       size_t argumentCount,
                                                       JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->transform(arguments[0]->toDouble(),
                                       arguments[1]->toDouble(),
                                       arguments[2]->toDouble(),
                                       arguments[3]->toDouble(),
                                       arguments[4]->toDouble(),
                                       arguments[5]->toDouble());
        });

        canvasContext2dJS->setProperty("translate", [](JSContext *ctx,
                                                       JSObject *thisObject,
                                                       size_t argumentCount,
                                                       JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->translate(arguments[0]->toDouble(), arguments[1]->toDouble());
        });

        canvasContext2dJS->setProperty("setTransform", [](JSContext *ctx,
                                                          JSObject *thisObject,
                                                          size_t argumentCount,
                                                          JSObject *arguments[]) {
            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->setTransform(arguments[0]->toDouble(),
                                          arguments[1]->toDouble(),
                                          arguments[2]->toDouble(),
                                          arguments[3]->toDouble(),
                                          arguments[4]->toDouble(),
                                          arguments[5]->toDouble());
        });

        canvasContext2dJS->setProperty("fillText",
                                       (JSFunctionObjectCallbackWithoutReturn) ([](JSContext *ctx,
                                                                                   JSObject *thisObject,
                                                                                   size_t argumentCount,
                                                                                   JSObject *arguments[]) {
                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           double maxWidth = 0;
                                           if (argumentCount == 4) {
                                               maxWidth = arguments[3]->toDouble();
                                           }
                                           canvasContext2d->fillText(arguments[0]->toString(),
                                                                     arguments[1]->toDouble(),
                                                                     arguments[2]->toDouble(),
                                                                     maxWidth);

                                       }));


        canvasContext2dJS->setProperty("strokeText",
                                       (JSFunctionObjectCallbackWithoutReturn) ([](JSContext *ctx,
                                                                                   JSObject *thisObject,
                                                                                   size_t argumentCount,
                                                                                   JSObject *arguments[]) {
                                           double maxWidth = 0;
                                           if (argumentCount == 4) {
                                               maxWidth = arguments[3]->toDouble();
                                           }

                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           canvasContext2d->strokeText(arguments[0]->toString(),
                                                                       arguments[1]->toDouble(),
                                                                       arguments[2]->toDouble(),
                                                                       maxWidth);
                                       }));


        canvasContext2dJS->setProperty("measureText", (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                                     JSObject *thisObject,
                                                                                     size_t argumentCount,
                                                                                     JSObject *arguments[]) {
            JSObject *value = nullptr;
            if (argumentCount == 0) {
                return value;
            }

            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            JSObject *textMetrics = new JSObject(ctx);
            textMetrics->setProperty("width",
                                     new JSObject(ctx, canvasContext2d->measureText(arguments[0]->toString())));
            return textMetrics;
        }));


        canvasContext2dJS->setProperty("createLinearGradient",
                                       (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                      JSObject *thisObject,
                                                                      size_t argumentCount,
                                                                      JSObject *arguments[]) {
                                           JSObject *value = nullptr;
                                           if (argumentCount == 0) {
                                               return value;
                                           }

                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           JSObject *jsObject = CreateLinearGradient(ctx,
                                                                                     canvasContext2d,
                                                                                     arguments[0]->toDouble(),
                                                                                     arguments[1]->toDouble(),
                                                                                     arguments[2]->toDouble(),
                                                                                     arguments[3]->toDouble());
                                           return jsObject;
                                       }));

        canvasContext2dJS->setProperty("createPattern",
                                       (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                      JSObject *thisObject,
                                                                      size_t argumentCount,
                                                                      JSObject *arguments[]) {
                                           JSObject *value = nullptr;
                                           if (argumentCount == 0) {
                                               return value;
                                           }

                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           //TODO
                                           AtomCanvasImage *atomCanvasImage = new AtomCanvasImage();
                                           atomCanvasImage->setWidth(arguments[1]->toDouble());
                                           atomCanvasImage->setHeight(arguments[2]->toDouble());
                                           atomCanvasImage->setSrc(arguments[3]->toString());
                                           return new JSObject(ctx, canvasContext2d->createPattern(
                                                   arguments[0]->toString(),
                                                   atomCanvasImage));
                                       }));

        canvasContext2dJS->setProperty("createRadialGradient",
                                       (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                      JSObject *thisObject,
                                                                      size_t argumentCount,
                                                                      JSObject *arguments[]) {
                                           JSObject *value = nullptr;
                                           if (argumentCount == 0) {
                                               return value;
                                           }

                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           JSObject *radialGradient = CreateRadialGradient(ctx,
                                                                                           canvasContext2d,
                                                                                           arguments[0]->toDouble(),
                                                                                           arguments[1]->toDouble(),
                                                                                           arguments[2]->toDouble(),
                                                                                           arguments[3]->toDouble(),
                                                                                           arguments[4]->toDouble(),
                                                                                           arguments[5]->toDouble());
                                           return radialGradient;
                                       }));


        /**
         * drawImage参数为一个Node对象
         */
        canvasContext2dJS->setProperty("drawImage",
                                       (JSFunctionObjectCallbackWithoutReturn) ([](JSContext *ctx,
                                                                                   JSObject *thisObject,
                                                                                   size_t argumentCount,
                                                                                   JSObject *arguments[]) {
                                           if (argumentCount > 0) {
                                               CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());

                                               JSObject *nodeOb = arguments[0];
                                               CanvasNode *node = ((CanvasNode *) nodeOb->toObjectPointer());
                                               canvasContext2d->drawImage(node);
                                           }
                                       }));


        canvasContext2dJS->setProperty("createImageData",
                                       (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                      JSObject *thisObject,
                                                                      size_t argumentCount,
                                                                      JSObject *arguments[]) {
                                           JSObject *value = nullptr;
                                           if (argumentCount == 0) {
                                               return value;
                                           }

                                           std::string script = "_contextCreateImageData(";
                                           script.append(arguments[0]->toString());
                                           script.append(",");
                                           script.append(arguments[1]->toString());
                                           script.append(")");
                                           JSValueRef jsObject = ctx->evaluateScript(script);
                                           JSObject *image = JSObject::ConvertJSValueRefToJSObject(
                                                   ctx, jsObject, nullptr);


                                           return image;
                                       }));

        canvasContext2dJS->setProperty("getImageData",
                                       (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                      JSObject *thisObject,
                                                                      size_t argumentCount,
                                                                      JSObject *arguments[]) {
                                           JSObject *value = nullptr;
                                           if (argumentCount == 0) {
                                               return value;
                                           }

                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           return new JSObject(ctx, canvasContext2d->getImageData(
                                                   arguments[0]->toDouble(),
                                                   arguments[1]->toDouble(),
                                                   arguments[2]->toDouble(),
                                                   arguments[3]->toDouble()));
                                       }));

        /**
         *  TODO putImageData
         */
        canvasContext2dJS->setProperty("putImageData",
                                       (JSFunctionObjectCallbackWithoutReturn) ([](JSContext *ctx,
                                                                                   JSObject *thisObject,
                                                                                   size_t argumentCount,
                                                                                   JSObject *arguments[]) {
//                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                       }));


        canvasContext2dJS->setProperty("save",
                                       (JSFunctionObjectCallbackWithoutReturn) ([](JSContext *ctx,
                                                                                   JSObject *thisObject,
                                                                                   size_t argumentCount,
                                                                                   JSObject *arguments[]) {
                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           canvasContext2d->save();
                                       }));

        canvasContext2dJS->setProperty("restore",
                                       (JSFunctionObjectCallbackWithoutReturn) ([](JSContext *ctx,
                                                                                   JSObject *thisObject,
                                                                                   size_t argumentCount,
                                                                                   JSObject *arguments[]) {
                                           CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
                                           canvasContext2d->restore();
                                       }));

        canvasContext2dJS->setProperty("createEvent", (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                                     JSObject *thisObject,
                                                                                     size_t argumentCount,
                                                                                     JSObject *arguments[]) {
            JSObject *value = nullptr;
            if (argumentCount == 0) {
                return value;
            }

            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            canvasContext2d->createEvent();
        }));

        canvasContext2dJS->setProperty("getContext", (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                                    JSObject *thisObject,
                                                                                    size_t argumentCount,
                                                                                    JSObject *arguments[]) {
            JSObject *value = nullptr;
            if (argumentCount == 0) {
                return value;
            }

            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            return JSObject::CreateObject(ctx, canvasContext2d->getContext());
        }));

        canvasContext2dJS->setProperty("toDataURL", (JSFunctionObjectCallback) ([](JSContext *ctx,
                                                                                   JSObject *thisObject,
                                                                                   size_t argumentCount,
                                                                                   JSObject *arguments[]) {
            JSObject *value = nullptr;
            if (argumentCount == 0) {
                return value;
            }

            CanvasContext2D *canvasContext2d = static_cast<CanvasContext2D *>(thisObject->toObjectPointer());
            return new JSObject(ctx, canvasContext2d->toDataURL());
        }));

    }

    JSObject *CanvasContextJavaScriptAPI::CreateLinearGradient(JSContext *ctx, CanvasContext2D *context2D,
                                                               float x0, float y0, float x1, float y1) {
        //TODO cache for LinearGradient Object
        //TODO JSObject应该增加weakReference object, 像线性渐变这类对象，不用长期存在
        JSObject *jsObject = new JSObject(ctx, context2D->createLinearGradient(x0, y0, x1, y1));

        InitLinearGradientAPIs(jsObject);
        return jsObject;
    }

    JSObject *CanvasContextJavaScriptAPI::CreateRadialGradient(JSContext *ctx, CanvasContext2D *context2D,
                                                               float x0, float y0, float r0, float x1,
                                                               float y1, float r1) {
        JSObject *radialGradient =
                new JSObject(ctx, context2D->createRadialGradient(x0, y0, r0, x1, y1, r1));

        InitRadialGradientAPIs(radialGradient);
        return radialGradient;
    }

    void CanvasContextJavaScriptAPI::InitLinearGradientAPIs(JSObject *gradient) {
        JSFunctionObjectCallbackWithoutReturn addColorStopCallback = [](JSContext *ctx,
                                                                        JSObject *thisObject,
                                                                        size_t argumentCount,
                                                                        JSObject *arguments[]) {
            AtomCanvasLinearGradient *atomCanvasLinearGradient = static_cast<AtomCanvasLinearGradient *>(thisObject->toObjectPointer());
            atomCanvasLinearGradient->addColorStop(arguments[0]->toDouble(),
                                                   arguments[1]->toString());
        };

        gradient->setProperty("addColorStop", addColorStopCallback);
    }

    void CanvasContextJavaScriptAPI::InitRadialGradientAPIs(JSObject *gradient) {
        JSFunctionObjectCallbackWithoutReturn addColorStopCallback = [](JSContext *ctx,
                                                                        JSObject *thisObject,
                                                                        size_t argumentCount,
                                                                        JSObject *arguments[]) {
            AtomCanvasRadialGradient *atomCanvasRadialGradient = static_cast<AtomCanvasRadialGradient *>(thisObject->toObjectPointer());
            atomCanvasRadialGradient->addColorStop(arguments[0]->toDouble(),
                                                   arguments[1]->toString());
        };

        gradient->setProperty("addColorStop", addColorStopCallback);
    }
}
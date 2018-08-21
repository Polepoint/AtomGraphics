//
// Created by neo on 2018/7/30.
//

#include "android/AtomGraphicsJNIHelper.h"
#include "node/canvas/AtomGCanvasContext2D.h"

namespace AtomGraphics {

    void GCanvasContext2D::bindImageTexture(const std::string &src, int imageId) {

    }

    std::string &GCanvasContext2D::getImageDataResult(const std::string &componentId) {
        std::string imageStr = "";
        return imageStr;
    }


    float GCanvasContext2D::measureText(const std::string &text) {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        jclass canvas_node_class = _gCanvasNode->getClassJObject();
        jmethodID methodID = env->GetStaticMethodID(canvas_node_class, "measureText",
                                                    "(Ljava/lang/String;Ljava/lang/String;)I");
        jstring textJStr = env->NewStringUTF(text.c_str());
        jstring fontConfig = env->NewStringUTF(_currentFont.c_str());
        jint result = env->CallStaticIntMethod(canvas_node_class, methodID, textJStr, fontConfig);
        env->DeleteLocalRef(textJStr);
        env->DeleteLocalRef(fontConfig);
        return result;
    }
}
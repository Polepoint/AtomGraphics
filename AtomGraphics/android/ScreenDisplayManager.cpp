//
// Created by neo on 2019/3/13.
//

#include <jni.h>
#include "base/android/DeviceAndroid.h"
#include "ScreenDisplayManager.h"
#include "AtomGraphicsJNIHelper.h"

namespace AtomGraphics {

ScreenDisplayManager *ScreenDisplayManager::Singleton() {
    static ScreenDisplayManager *manager;
    if (!manager) {
        manager = new ScreenDisplayManager();
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz_display_manager = AtomGraphicsBase::FindClass(
                env, "com/neo/atomgraphics/display/ScreenDisplayManager");

        jmethodID method_id = env->GetStaticMethodID(clazz_display_manager, "onNativeSideCreated", "(J)V");
        jlong nativePtr = reinterpret_cast<intptr_t>(manager);
        env->CallStaticVoidMethod(clazz_display_manager, method_id, nativePtr);
        env->DeleteLocalRef(clazz_display_manager);
    }

    return manager;
}

void ScreenDisplayManager::addDisplay(const ScreenDisplay &display) {
    auto iter = getDisplayByID(display.id());
    if (iter != m_displayList.end()) {
        return;
    }

    m_displayList.push_back(display);
}

void ScreenDisplayManager::removeDisplay(int64_t displayId) {
    auto iter = getDisplayByID(displayId);
    if (iter == m_displayList.end()) {
        return;
    }

    m_displayList.erase(iter);
}

ScreenDisplayManager::ScreenDisplayList::iterator ScreenDisplayManager::getDisplayByID(int64_t display_id) {
    auto it = m_displayList.begin();
    while (it != m_displayList.end()) {
        if (it->id() == display_id) {
            return it;
        }
        it++;
    }

    return m_displayList.end();
}


void ScreenDisplayManager::updateDisplay(int sdkDisplayId,
                                         int width,
                                         int height,
                                         float dipScale,
                                         int bitsPerPixel,
                                         int bitsPerComponent) {

    auto iter = getDisplayByID(sdkDisplayId);
    if (iter == m_displayList.end()) {
        addDisplay(ScreenDisplay(sdkDisplayId));
        iter = getDisplayByID(sdkDisplayId);
    }

    iter->setBounds(IntRect(0, 0, static_cast<int>(ceil(width / dipScale)), static_cast<int>(ceil(height / dipScale))));

    iter->setSizeInPixels(IntSize(width, height));
    iter->setDeviceScaleFactor(dipScale);
    iter->setColorDepth(bitsPerPixel);
    iter->setDepthPerComponent(bitsPerComponent);

    DeviceAndroid::SetDeviceScaleFactor(dipScale);

}

ScreenDisplay ScreenDisplayManager::getPrimaryDisplay() {
    if (m_displayList.size()) {
        return *m_displayList.begin();
    } else {
        return ScreenDisplay(0xFF, IntRect(0, 0, 100, 100));
    }
}

}

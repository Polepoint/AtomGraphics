//
// Created by neo on 2019/1/8.
//

#include <android/native_window_jni.h>
#include <memory>
#include "thread/GraphicsThread.h"
#include "thread/UIThread.h"
#include "gpu/GLResourceManager.h"
#include "graphics/PlatformTileLayer.h"
#include "graphics/Transaction.h"
#include "AtomGraphicsJNIHelper.h"
#include "ContentView.h"
#include "ContentViewUpdateDispatcher.h"
#include "ScreenDisplay.h"
#include "ScreenDisplayManager.h"
#include "TransactionApplier.h"

namespace AtomGraphics {

ContentView::ContentView(jobject jObj)
        : m_jRef(jObj),
          m_compositingManager(new CompositingManager) {

}

ContentView::~ContentView() = default;

void ContentView::ref() {
    m_refCount++;
}

void ContentView::deref() {
    if (!(--m_refCount)) {
        if (UIThread::IsMainThread()) {
            delete this;
        } else {
            UIThread::UIThreadTaskRunner()->postTask([this] {
                delete this;
            });
        }
    }
}

void ContentView::destroy() {
    m_resourceProvider.reset();
    GraphicsThread::DispatchOnGraphicsQueue([this]() {
        if (this->m_page) {
            GPUResourceManager::sharedManger()->removePageResource(this->m_page->pageID());
            this->m_page.reset();
        }
        deref();
    });
}

void ContentView::surfaceCreated(jobject surface, int width, int height) {

    if (surface) {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        m_window = ANativeWindow_fromSurface(env, surface);
        m_windowChanged = true;
    }

    if (m_contentSize.width != width || m_contentSize.height != height) {
        m_contentSize = IntSize(width, height);
    }

    m_resourceProvider.reset();
    m_glSurfaceAvailable = true;
    updateVisibleBounds(m_visibleRect.origin.x,
                        m_visibleRect.origin.y,
                        m_visibleRect.size.width,
                        m_visibleRect.size.height);
}

void ContentView::surfaceChanged(jobject surface, int width, int height) {
    surfaceCreated(surface, width, height);
}

void ContentView::surfaceDestroyed() {
    m_glSurfaceAvailable = false;
}

void ContentView::updateVisibleBounds(int x, int y, int width, int height) {
    IntRect visibleRect(x, y, width, height);

    if (m_visibleRect.equals(visibleRect) && !m_windowChanged) {
        return;
    }

    m_visibleRect = visibleRect;
    ref();
    GraphicsThread::DispatchOnGraphicsQueue([this, visibleRect]() {
        updateVisibleContentRects(visibleRect);
        updateVisibility(!visibleRect.isEmpty());
        deref();
    });
}

void ContentView::updateVisibility(bool visible) {
    ref();
    GraphicsThread::DispatchOnGraphicsQueue([this, visible] {
        if (this->m_visible != visible) {
            this->m_visible = visible;
            if (this->m_page) {
                this->m_page->setVisible(visible);
            }
        }
        deref();
    });
}


void ContentView::ensureGPUResource() {
    if (!m_windowChanged || !m_glSurfaceAvailable) {
        return;
    }

    scoped_refptr <GLResource> resource = GPUResourceManager::sharedManger()->resourceForPage(pageID());
    if (resource) {
        resource->updateWindow(m_window);
    } else {
        GPUResourceManager::sharedManger()->initializePageResource(pageID(), m_window);
        ContentViewUpdateDispatcher::SharedInstance()->contentViewReadyToRender(this);
    }

    m_windowChanged = false;
}

void ContentView::updateVisibleContentRects(FloatRect visibleRect) {
    float deviceScaleFactor = Device::DeviceScaleFactor();
    FloatRect pageBounds = {{0, 0}, this->m_contentSize};
    pageBounds.scale(1 / deviceScaleFactor);
    FloatRect scaledVisibleRect = visibleRect;
    scaledVisibleRect.scale(1 / deviceScaleFactor);

    GraphicsPageViewInfo info(pageBounds, scaledVisibleRect, deviceScaleFactor, 1);

    this->page()->updateVisibleContentRects(info);
    if (!this->m_didFinishFirstTransaction) {
        this->page()->rootLayer()->getPlatformLayer()->setNeedsDisplay();
    }

    ensureGPUResource();

    JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
    jclass clazz_ContentView = env->GetObjectClass(m_jRef);
    jmethodID methodID = env->GetMethodID(clazz_ContentView, "onViewportChange", "(IIII)V");
    jint jX = static_cast<jint>(visibleRect.origin.x);
    jint jY = static_cast<jint>(visibleRect.origin.y);
    jint jWidth = static_cast<jint>(visibleRect.size.width);
    jint jHeight = static_cast<jint>(visibleRect.size.height);
    env->CallVoidMethod(m_jRef, methodID, jX, jY, jWidth, jHeight);
    env->DeleteLocalRef(clazz_ContentView);
}

void ContentView::applyTransaction(const Transaction &transaction) {
    if (rootCompositingView()) {
        TransactionApplier::applyTransaction(transaction, m_compositingManager.get());
        bool needSwapBuffers = false;
        SkPictureRecorder recorder;
        SkCanvas *canvas = recorder.beginRecording(m_contentSize.width, m_contentSize.height);
        float deviceScaleFactor = Device::DeviceScaleFactor();
        canvas->scale(deviceScaleFactor, deviceScaleFactor);
        rootCompositingView()->commitImage(canvas, SkMatrix::I(), &needSwapBuffers);
        if (needSwapBuffers) {
            canvas->flush();
            auto pic = recorder.finishRecordingAsPicture();
            resourceProvider()->playback(pic);
            resourceProvider()->swapBuffers();
        }
    }
    finishApplyTransaction();
}

void ContentView::finishApplyTransaction() {
    if (!m_didFinishFirstTransaction) {
        m_didFinishFirstTransaction = true;
        jobject jRef = m_jRef;
        UIThread::UIThreadTaskRunner()->postTask([jRef]() {
            JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
            jclass clazz_ContentView = env->GetObjectClass(jRef);
            jmethodID methodID = env->GetMethodID(clazz_ContentView, "didFinishInitialization", "()V");
            env->CallVoidMethod(jRef, methodID);
            env->DeleteLocalRef(clazz_ContentView);
        });
    }
}

GraphicsPage *ContentView::page() {
    if (!m_page) {
        m_page = std::make_unique<GraphicsPage>();
        m_page->setRootLayer(layer());
        m_page->setVisible(m_visible);
    }

    return m_page.get();
}

GraphicsLayer *ContentView::layer() {
    if (!m_layer) {
        m_layer = std::make_unique<GraphicsLayer>(std::make_unique<PlatformTileLayer>());
    }

    return m_layer.get();
}

CompositingView *ContentView::rootCompositingView() {
    if (!m_rootCompositingView) {
        m_rootCompositingView = m_compositingManager->createViewWithID(
                layer()->getPlatformLayer()->getLayerID());
    }

    return m_rootCompositingView.get();
}

ResourceProvider *ContentView::resourceProvider() {
    if (!m_resourceProvider) {
        m_resourceProvider = ResourceProvider::CreateResourceProvider(
                m_window,
                true,
                GPUResourceManager::sharedManger()->resourceForPage(pageID()));
    }

    return m_resourceProvider.get();
}

}
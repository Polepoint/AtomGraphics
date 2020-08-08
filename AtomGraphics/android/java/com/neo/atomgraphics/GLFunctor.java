package com.neo.atomgraphics;

import android.graphics.Canvas;
import android.os.Build;

import java.lang.reflect.Method;

public class GLFunctor {

    private static Class sDisplayCanvasClass;
    private static Method sMethod_drawGLFunctor;
    private long mNativePtr;
    private Runnable mFunctorReleasedCallback;
    private int mRefCount;

    private static final boolean sSupportFunctorReleasedCallback =
            (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N && Build.VERSION.SDK_INT <= Build.VERSION_CODES.P);


    static {
        if (sSupportFunctorReleasedCallback) {
            try {
                sDisplayCanvasClass = Class.forName("android.view.DisplayListCanvas");
                sMethod_drawGLFunctor = sDisplayCanvasClass.getMethod("drawGLFunctor2", long.class, Runnable.class);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    GLFunctor(long contentViewPtr) {
        mNativePtr = nativeCreate(contentViewPtr);

        if (sSupportFunctorReleasedCallback) {
            mFunctorReleasedCallback = new Runnable() {
                @Override
                public void run() {
                    GLFunctor.this.removeReference();
                }
            };
        }

        addReference();
    }

    void requestDraw(Canvas canvas) {
        if (sSupportFunctorReleasedCallback && sDisplayCanvasClass != null) {
            if (canvas.getClass() == sDisplayCanvasClass || sDisplayCanvasClass.isAssignableFrom(canvas.getClass())) {
                try {
                    sMethod_drawGLFunctor.invoke(canvas, mNativePtr, mFunctorReleasedCallback);
                    addReference();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void addReference() {
        ++mRefCount;
    }

    private void removeReference() {
        if (--mRefCount == 0) {
            // When |mRefCount| decreases to zero, the functor is neither attached to a view, nor
            // referenced from the render tree, and so it is safe to delete the HardwareRenderer
            // instance to free up resources because the current state will not be drawn again.
            nativeDestroy(mNativePtr);
        }
    }

    public void destroy() {
        removeReference();
        nativeRemoveContentView(mNativePtr);
    }

    private native long nativeCreate(long contentViewPtr);

    private native void nativeDestroy(long nativePtr);

    private native void nativeRemoveContentView(long nativePtr);

}

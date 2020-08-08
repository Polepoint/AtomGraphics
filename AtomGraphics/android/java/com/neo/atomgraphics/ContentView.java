package com.neo.atomgraphics;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.*;
import android.os.Build;
import android.support.annotation.NonNull;
import android.view.*;
import android.widget.FrameLayout;

import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.reflect.PositionUpdateListener;
import com.neo.atomgraphics.reflect.ReflectUtils;

@SuppressLint("ViewConstructor")
public class ContentView extends FrameLayout implements ViewTreeObserver.OnPreDrawListener {

    public interface Listener {

        void didFinishInitialization();

        void notifyViewportChanged(int x, int y, int width, int height);
    }

    private GLFunctor mGlFunctor;

    private RenderNode mRenderNode;
    private PositionUpdateListener mPositionUpdateListener;

    private long mNativePtr = 0;

    private Rect mVisibleRect = new Rect();

    private Listener mListener;

    private SurfaceTexture mSurfaceTexture;
    private Surface mSurface;
    private int mSurfaceWidth;
    private int mSurfaceHeight;
    private boolean mDestroyed;
    private boolean mDeferTextureRelease;
    private boolean mIsViewVisible;
    private boolean mNeedUpdateNativeVisibleRect;


    public ContentView(Context context, Listener listener) {
        super(context);
        this.setBackgroundColor(Color.TRANSPARENT);
        mListener = listener;
        mNativePtr = nativeInit();
        TextureView textureView = new TextureView(context);
        textureView.setOpaque(false);
        textureView.setSurfaceTextureListener(new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surfaceTexture, int width, int height) {
                mSurfaceTexture = surfaceTexture;
                mSurface = new Surface(surfaceTexture);
                mSurfaceWidth = width;
                mSurfaceHeight = height;

                nativeSurfaceCreated(mNativePtr, mSurface, width, height);
            }

            @Override
            public void onSurfaceTextureSizeChanged(SurfaceTexture surfaceTexture, int width, int height) {
                if (mSurfaceTexture != surfaceTexture || mSurfaceWidth != width || mSurfaceHeight != height) {
                    mSurface = new Surface(surfaceTexture);
                    mSurfaceWidth = width;
                    mSurfaceHeight = height;
                    nativeSurfaceChanged(mNativePtr, mSurface, width, height);
                }
            }

            @Override
            public boolean onSurfaceTextureDestroyed(SurfaceTexture surfaceTexture) {
                if (mSurface != null) {
                    mSurfaceTexture = surfaceTexture;
                    if (mNativePtr != 0) {
                        nativeSurfaceDestroyed(mNativePtr);
                    }
                    mDeferTextureRelease = true;
                    return false;
                }

                return true;
            }

            @Override
            public void onSurfaceTextureUpdated(SurfaceTexture surfaceTexture) {
                if (mSurfaceTexture != surfaceTexture) {
                    mSurfaceTexture = surfaceTexture;
                    mSurface = new Surface(surfaceTexture);
                    nativeSurfaceChanged(mNativePtr, mSurface, mSurfaceWidth, mSurfaceHeight);
                }
            }
        });

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            mRenderNode = new RenderNode(getClass().getName());
            mPositionUpdateListener = new PositionUpdateListener() {

                @Override
                public void positionChanged(long frameNumber, int left, int top, int right, int bottom) {
                    ContentView.this.updateVisibleRect();
                }

                @Override
                public void positionLost(long frameNumber) {
                    ContentView.this.updateVisibleRect();
                }
            };
            ReflectUtils.addPositionUpdateListener(mRenderNode, mPositionUpdateListener);
        } else {
            mGlFunctor = new GLFunctor(mNativePtr);
        }
        addView(textureView, LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);

        this.getViewTreeObserver().addOnPreDrawListener(this);
    }

    @Override
    protected void onVisibilityChanged(@NonNull View changedView, int visibility) {
        super.onVisibilityChanged(changedView, visibility);
        if (mDestroyed) {
            return;
        }

        boolean visible = visibility == View.VISIBLE;
        if (mIsViewVisible != visible) {
            mIsViewVisible = visible;
            nativeSetViewVisibility(mNativePtr, mIsViewVisible);
        }
    }

    @Override
    protected void onWindowVisibilityChanged(int visibility) {
        super.onWindowVisibilityChanged(visibility);
    }

    @CalledByNative
    private void didFinishInitialization() {
        if (mListener != null) {
            mListener.didFinishInitialization();
        }
    }

    @CalledByNative
    private void onViewportChange(int x, int y, int width, int height) {
        if (mListener != null) {
            mListener.notifyViewportChanged(x, y, width, height);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        this.updateVisibleRect();
        if (canvas.isHardwareAccelerated()) {
            this.requestDraw(canvas);
        }
    }

    private void updateVisibleRect() {
        Rect visibleRect = new Rect();
        if (!this.getLocalVisibleRect(visibleRect)) {
            visibleRect.setEmpty();
        }

        if (mVisibleRect.equals(visibleRect)) {
            return;
        }

        mVisibleRect = visibleRect;

        mNeedUpdateNativeVisibleRect = true;

        if (mListener != null) {
            mListener.notifyViewportChanged(mVisibleRect.left, mVisibleRect.top, mVisibleRect.width(), mVisibleRect.height());
        }

        this.postInvalidate();
    }

    long getPageID() {
        return nativeGetPageID(mNativePtr);
    }

    public void destroy() {
        this.getViewTreeObserver().removeOnPreDrawListener(this);

        if (mGlFunctor != null) {
            mGlFunctor.destroy();
        }
        if (mRenderNode != null) {
            ReflectUtils.removePositionUpdateListener(mRenderNode, mPositionUpdateListener);
        }
        nativeDestroy(mNativePtr);
        mNativePtr = 0;
        mDestroyed = true;
        if (mDeferTextureRelease && mSurfaceTexture != null) {
            mSurfaceTexture.release();
        }

        mSurface = null;
    }

    private void requestDraw(Canvas canvas) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            canvas.drawRenderNode(mRenderNode);
        } else {
            mGlFunctor.requestDraw(canvas);
        }
    }

    @Override
    public boolean onPreDraw() {
        if (mNeedUpdateNativeVisibleRect) {
            nativeUpdateVisibleRect(mNativePtr, mVisibleRect.left, mVisibleRect.top, mVisibleRect.width(), mVisibleRect.height());
        }
        mNeedUpdateNativeVisibleRect = false;

        return true;
    }

    private native long nativeInit();

    private native void nativeDestroy(long nativePtr);

    private native long nativeGetPageID(long nativePtr);

    private native void nativeSurfaceCreated(long nativePtr, Surface surface, int width, int height);

    private native void nativeSurfaceDestroyed(long nativePtr);

    private native void nativeSurfaceChanged(long nativePtr, Surface surface, int width, int height);

    private native void nativeUpdateVisibleRect(long nativePtr, int x, int y, int width, int height);

    private native void nativeSetViewVisibility(long nativePtr, boolean visible);

}

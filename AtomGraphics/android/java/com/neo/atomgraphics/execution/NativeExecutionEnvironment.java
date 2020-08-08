package com.neo.atomgraphics.execution;

import android.content.Context;
import android.util.ArrayMap;
import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.node.canvas.CanvasNode;

public abstract class NativeExecutionEnvironment implements ExecutionEnvironment {

    public abstract class FrameCallback {
        private long mID;
        private boolean mCanceled;

        abstract void onFrame();
    }

    private long mPtr;
    private ArrayMap<Long, FrameCallback> mFrameCallbacks;
    private CanvasNode mRootNode;
    private long mRequestIDCount;
    private boolean mRequestingFrame;

    public NativeExecutionEnvironment() {
        mPtr = nativeConstructor();
        mFrameCallbacks = new ArrayMap<>();
    }

    public long requestAnimationFrame(FrameCallback frameCallback) {

        long requestID = mRequestIDCount++;
        mFrameCallbacks.put(requestID, frameCallback);
        if (!mRequestingFrame) {
            mRequestingFrame = true;
            nativeRequestFrame(mPtr);
        }
        return requestID;
    }

    public void cancelAnimationFrame(long requestID) {
        FrameCallback callback = mFrameCallbacks.get(requestID);
        if (callback != null) {
            callback.mCanceled = true;
        }
    }

    @Override
    public void onExecution(Context context, long pageID) {
        long nodePtr = nativeGetRootCanvasNodeByID(pageID);
        mRootNode = new CanvasNode(nodePtr);
    }

    protected abstract void onExecution(CanvasNode node);

    @Override
    public void destroy() {

    }

    public CanvasNode getRootNode() {
        return mRootNode;
    }

    @CalledByNative
    private void doFrame() {
        mRequestingFrame = false;
        ArrayMap<Long, FrameCallback> executingCallback = mFrameCallbacks;
        mFrameCallbacks.clear();
        for (FrameCallback callback : executingCallback.values()) {
            if (!callback.mCanceled)
                callback.onFrame();
        }
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        nativeDestructor(mPtr);
    }

    private native long nativeConstructor();

    private native void nativeDestructor(long ptr);

    private native void nativeRequestFrame(long ptr);

    private native long nativeGetRootCanvasNodeByID(long pageID);

}

package com.neo.atomgraphics.node.canvas;

import android.graphics.Point;
import com.neo.atomgraphics.node.Node;

public class CanvasNode extends Node {

    private long mPtr;
    private CanvasRenderingContext2D mContext;
    private boolean mShouldReleaseNative;

    public CanvasNode() {
        mPtr = nativeConstructor();
        mShouldReleaseNative = true;
    }

    public CanvasNode(long prt) {
        this.mPtr = prt;
    }

    long getNativePtr() {
        return mPtr;
    }

    public CanvasRenderingContext2D getContext() {
        if (mContext == null) {
            mContext = new CanvasRenderingContext2D(nativeGetCanvasContext(mPtr));
        }

        return mContext;
    }

    public Point getBoundsSize() {
        return new Point();
    }

    public void setBoundsSize(Point boundsSize) {
        mContext = null;
    }

    public Point getContentSize() {
        return new Point();
    }

    public void setContentSize(Point size) {
        mContext = null;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        if (mShouldReleaseNative) {
            nativeDestructor(mPtr);
        }
    }

    private native long nativeConstructor();

    private native void nativeDestructor(long ptr);

    private native long nativeGetCanvasContext(long ptr);

}

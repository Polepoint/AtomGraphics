package com.neo.atomgraphics.node.canvas;

public class ImageData {

    private long mPtr;
    private int width;
    private int height;

    public ImageData(long ptr) {
        this.mPtr = ptr;
        if (ptr > 0) {
            width = nativeGetWidth(ptr);
            height = nativeGetHeight(ptr);
        }
    }

    long nativePtr() {
        return mPtr;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        if (mPtr != 0) {
            nativeDestructor(mPtr);
        }
    }

    private native void nativeDestructor(long ptr);

    private native int nativeGetWidth(long ptr);

    private native int nativeGetHeight(long ptr);

}

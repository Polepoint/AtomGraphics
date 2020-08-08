package com.neo.atomgraphics.node.canvas;

public class Gradient {

    private long mPtr;

    public Gradient(float x0, float y0, float x1, float y1) {
        mPtr = nativeConstructor(x0, y0, x1, y1);
    }

    public Gradient(float x0, float y0, float r0, float x1, float y1, float r1) {
        mPtr = nativeConstructor(x0, y0, r0, x1, y1, r1);
    }

    long nativePtr() {
        return mPtr;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        nativeDestructor(mPtr);
    }

    private native long nativeConstructor(float x0, float y0, float x1, float y1);

    private native long nativeConstructor(float x0, float y0, float r0, float x1, float y1, float r1);

    private native void nativeDestructor(long ptr);
}

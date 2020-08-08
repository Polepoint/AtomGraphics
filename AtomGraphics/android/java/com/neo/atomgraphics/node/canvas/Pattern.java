package com.neo.atomgraphics.node.canvas;

public class Pattern {

    private long mPtr;

    public Pattern(CanvasNode node, boolean repeatX, boolean repeatY) {
        mPtr = nativeConstructor(node.getNativePtr(), repeatX, repeatY);
    }

    long nativePtr() {
        return mPtr;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        nativeDestructor(mPtr);
    }

    private native long nativeConstructor(long nodePtr, boolean repeatX, boolean repeatY);

    private native void nativeDestructor(long ptr);
}

package com.neo.atomgraphics.javascript;

import com.neo.atomgraphics.base.CalledByNative;

public class JSMessageCallback {
    private long mNativePtr;

    @CalledByNative
    public JSMessageCallback(long nativePtr) {
        mNativePtr = nativePtr;
    }

    public void callback(String data) {
        if (mNativePtr != 0) {
            nativeCallback(mNativePtr, data);
            mNativePtr = 0;
        }
    }

    private native void nativeCallback(long nativePtr, String data);

}
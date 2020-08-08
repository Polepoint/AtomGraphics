package com.neo.atomgraphics;

import android.content.Context;
import com.neo.atomgraphics.base.ContextUtils;

public class GraphicsEnvironment {

    static {
        System.loadLibrary("atomgraphics");
    }

    private static boolean sInitialized;

    public static void initialize(Context applicationContext) {
        if (!sInitialized) {
            ContextUtils.initApplicationContext(applicationContext);
            nativeInitGraphicsEnv();
            sInitialized = true;
        }
    }

    private static native void nativeInitGraphicsEnv();
}

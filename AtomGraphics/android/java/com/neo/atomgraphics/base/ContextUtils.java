package com.neo.atomgraphics.base;

import android.content.Context;

public class ContextUtils {
    private static Context sApplicationContext;

    public static Context getApplicationContext() {
        return sApplicationContext;
    }

    public static void initApplicationContext(Context appContext) {
        if (sApplicationContext != null && sApplicationContext != appContext) {
            throw new RuntimeException("Attempting to set multiple global application contexts.");
        }
        initJavaSideApplicationContext(appContext);
    }

    private static void initJavaSideApplicationContext(Context appContext) {
        if (appContext == null) {
            throw new RuntimeException("Global application context cannot be set to null.");
        }
        sApplicationContext = appContext;
        initNativeContext(sApplicationContext);
    }

    private static native void initNativeContext(Context context);
}

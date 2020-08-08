package com.neo.atomgraphics.thread;

import android.os.Handler;
import android.os.Looper;

public class UIThread {

    private static Handler sMainHandler;

    public static void dispatch(Runnable runnable) {
        synchronized (UIThread.class) {
            if (sMainHandler == null) {
                sMainHandler = new Handler(Looper.getMainLooper());
            }
        }

        sMainHandler.post(runnable);
    }
}

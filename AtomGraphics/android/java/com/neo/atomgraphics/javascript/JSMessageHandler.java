package com.neo.atomgraphics.javascript;

import com.neo.atomgraphics.base.CalledByNative;

public interface JSMessageHandler {

    @CalledByNative
    void onMessage(String data, JSMessageCallback callback);

}
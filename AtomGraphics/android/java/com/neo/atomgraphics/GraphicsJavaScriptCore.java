package com.neo.atomgraphics;

import android.content.res.AssetManager;

import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.base.ContextUtils;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public class GraphicsJavaScriptCore {

    private long mNativeJavaScriptInterface;
    private static GraphicsJavaScriptCore sJavaScriptCore;

    public static GraphicsJavaScriptCore globalJavaScriptCore() {
        if (sJavaScriptCore == null) {
            sJavaScriptCore = new GraphicsJavaScriptCore();
        }

        return sJavaScriptCore;
    }

    private GraphicsJavaScriptCore() {
        mNativeJavaScriptInterface = nativeCreateJavaScriptInterface();
    }

    public long getNativePtr() {
        return mNativeJavaScriptInterface;
    }

    public void runScript(String script) {
        this.nativeLoadJavaScript(mNativeJavaScriptInterface, script);
    }

    @CalledByNative
    public String readScriptFile(String filename) {
        String scriptString = "";
        AssetManager assetManager = ContextUtils.getApplicationContext().getAssets();
        try {
            InputStream inputStream = assetManager.open(filename);
            InputStreamReader streamReader = new InputStreamReader(inputStream, StandardCharsets.UTF_8);
            char[] input = new char[inputStream.available()];
            int bufferSize = streamReader.read(input);
            streamReader.close();
            scriptString = new String(input, 0, bufferSize);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return scriptString;
    }

    public void reset() {
        nativeReset(mNativeJavaScriptInterface);
    }

    private native long nativeCreateJavaScriptInterface();

    private native void nativeLoadJavaScript(long nativeJavaScriptInterface, String script);

    private native void nativeReset(long nativeJavaScriptInterface);

}

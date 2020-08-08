package com.neo.atomgraphics.javascript;

import android.content.Context;
import com.neo.atomgraphics.GraphicsJavaScriptCore;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public class JSPageScope {
    private long mNativePtr;

    private static String sScriptContent = "";

    public JSPageScope(Context context, long pageID) {
        if (sScriptContent.isEmpty()) {
            try {
                InputStream inputStream = context.getAssets().open("pageScope.js");
                InputStreamReader streamReader = new InputStreamReader(inputStream, StandardCharsets.UTF_8);
                char input[] = new char[inputStream.available()];
                int bufferSize = streamReader.read(input);
                streamReader.close();
                sScriptContent = new String(input, 0, bufferSize);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        mNativePtr = this.nativeInit(pageID, sScriptContent, GraphicsJavaScriptCore.globalJavaScriptCore().getNativePtr());
    }

    public void evaluateScript(String script) {
        nativeEvaluateScript(mNativePtr, script, GraphicsJavaScriptCore.globalJavaScriptCore().getNativePtr());
    }

    public void callJSHandler(String handlerName, String data) {
        nativeCallJSHandler(mNativePtr, handlerName, data, GraphicsJavaScriptCore.globalJavaScriptCore().getNativePtr());
    }

    public void registerNativeHandler(String handlerName, JSMessageHandler handler) {
        nativeRegisterNativeHandler(mNativePtr, handlerName, handler, GraphicsJavaScriptCore.globalJavaScriptCore().getNativePtr());
    }

    public void destroy() {
        nativeDestroy(mNativePtr, GraphicsJavaScriptCore.globalJavaScriptCore().getNativePtr());
    }

    private native long nativeInit(long pageID, String scriptContent, long javaScriptCorePtr);

    private native void nativeDestroy(long nativePtr, long javaScriptCorePtr);

    private native void nativeEvaluateScript(long nativePtr, String script, long javaScriptCorePtr);

    private native void nativeCallJSHandler(long nativePtr, String handlerName, String data, long javaScriptCorePtr);

    private native void nativeRegisterNativeHandler(long nativePtr, String handlerName, JSMessageHandler handler, long javaScriptCorePtr);

}

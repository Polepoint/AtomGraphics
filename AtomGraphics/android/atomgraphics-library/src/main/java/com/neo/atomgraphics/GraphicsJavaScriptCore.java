package com.neo.atomgraphics;

import android.content.res.AssetManager;

import com.neo.atomgraphics.graphics.GraphicsPageContext;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

public class GraphicsJavaScriptCore {

    private static Map<GraphicsPageContext, GraphicsJavaScriptCore> contextBindJavaScriptCoreMap;

    private long mNativeJavaScriptInterface;

    private GraphicsPageContext mPageContext;

    public static GraphicsJavaScriptCore globalJavaScriptCore(GraphicsPageContext pageContext) {
        if (contextBindJavaScriptCoreMap == null) {
            contextBindJavaScriptCoreMap = new HashMap<GraphicsPageContext, GraphicsJavaScriptCore>();
        }

        GraphicsJavaScriptCore javaScriptCore = contextBindJavaScriptCoreMap.get(pageContext);
        if (javaScriptCore == null) {
            javaScriptCore = new GraphicsJavaScriptCore(pageContext);
            contextBindJavaScriptCoreMap.put(pageContext, javaScriptCore);
        }

        return javaScriptCore;
    }

    private GraphicsJavaScriptCore(GraphicsPageContext pageContext) {
        mPageContext = pageContext;
        mNativeJavaScriptInterface = createNativeJavaScriptInterface(pageContext.getNativePageContextRef());
        this.preloadCustomScript();
    }

    private void preloadCustomScript() {
        this.runScriptFile("zrender.js");
    }

    public void runScript(String script) {
        this.loadJavaScript(mNativeJavaScriptInterface, script);
    }

    public void runScriptFile(String filename) {
        AssetManager assetManager = mPageContext.getContext().getAssets();
        try {
            InputStream inputStream = assetManager.open(filename);
            InputStreamReader streamReader = new InputStreamReader(inputStream, "utf-8");
            char input[] = new char[inputStream.available()];
            int bufferSize = streamReader.read(input);
            streamReader.close();
            String scriptString = new String(input, 0, bufferSize);
            this.runScript(scriptString);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private native long createNativeJavaScriptInterface(long pageContextRef);

    private native void loadJavaScript(long nativeJavaScriptInterface, String script);

}

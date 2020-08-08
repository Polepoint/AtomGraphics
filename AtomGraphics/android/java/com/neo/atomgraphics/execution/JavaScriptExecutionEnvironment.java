package com.neo.atomgraphics.execution;

import android.content.Context;
import com.neo.atomgraphics.javascript.JSMessageHandler;
import com.neo.atomgraphics.javascript.JSPageScope;
import com.neo.atomgraphics.javascript.PageScopeCommandQueue;

public class JavaScriptExecutionEnvironment implements ExecutionEnvironment {
    private JSPageScope mPageScope;
    private PageScopeCommandQueue mPageScopeCommandQueue;

    public JavaScriptExecutionEnvironment() {
        mPageScopeCommandQueue = new PageScopeCommandQueue();
    }

    @Override
    public void onExecution(Context context, long pageID) {
        mPageScope = new JSPageScope(context, pageID);
        if (!mPageScopeCommandQueue.isEmpty()) {
            mPageScopeCommandQueue.flushCommand(mPageScope);
        }
    }

    public void evaluateScript(String script) {
        if (mPageScope == null) {
            mPageScopeCommandQueue.addCommand(PageScopeCommandQueue.Type_EvaluateScript, script, null);
        } else {
            mPageScope.evaluateScript(script);
        }
    }

    public void callJSHandler(String handlerName) {
        callJSHandler(handlerName, null);
    }

    public void callJSHandler(String handlerName, String params) {
        if (mPageScope == null) {
            mPageScopeCommandQueue.addCommand(PageScopeCommandQueue.Type_CallJSHandler, handlerName, params);
        } else {
            mPageScope.callJSHandler(handlerName, params);
        }
    }

    public void registerNativeHandler(String handlerName, JSMessageHandler handler) {
        if (mPageScope == null) {
            mPageScopeCommandQueue.addCommand(PageScopeCommandQueue.Type_RegisterNativeHandler, handlerName, handler);
        } else {
            mPageScope.registerNativeHandler(handlerName, handler);
        }
    }

    @Override
    public void destroy() {
        if (mPageScope != null) {
            mPageScope.destroy();
        }
    }
}

package com.neo.atomgraphics.graphics;

import android.content.Context;

import java.util.HashSet;
import java.util.Set;

public class GraphicsPageContext {

    static private Set<GraphicsPageContext> graphicsPageHashSet;

    private long mNativePageContextRef;
    private Context mContext;

    /**
     * Different activities have different Contexts, different DisplayLinks, different JavascriptInterfaces.
     *
     * @param context The Context the GraphicsView is running in.
     * @return GraphicsPageContext Wrapping the Context and Native Graphics Page Context
     */
    public static GraphicsPageContext GraphicsPageContextForContext(Context context) {
        GraphicsPageContext targetPageContext = null;
        if (graphicsPageHashSet == null) {
            graphicsPageHashSet = new HashSet<GraphicsPageContext>();
        }

        for (GraphicsPageContext pageContext : graphicsPageHashSet) {
            if (pageContext.mContext == context) {
                targetPageContext = pageContext;
                break;
            }
        }

        if (targetPageContext == null) {
            targetPageContext = new GraphicsPageContext(context);
            graphicsPageHashSet.add(targetPageContext);
        }

        return targetPageContext;
    }

    public static GraphicsPageContext GraphicsPageContextForNativeRef(long nativePageContextRef) {
        GraphicsPageContext targetPageContext = null;
        if (graphicsPageHashSet == null) {
            graphicsPageHashSet = new HashSet<GraphicsPageContext>();
            return null;
        }

        for (GraphicsPageContext pageContext : graphicsPageHashSet) {
            if (pageContext.mNativePageContextRef == nativePageContextRef) {
                targetPageContext = pageContext;
                break;
            }
        }

        return targetPageContext;
    }

    public GraphicsPageContext(Context context) {
        mContext = context;
        mNativePageContextRef = createNativePageContext(mContext);
    }

    public long getNativePageContextRef() {
        return mNativePageContextRef;
    }

    public Context getContext() {
        return mContext;
    }

    private native long createNativePageContext(Context nativeContext);

}

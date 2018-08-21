package com.neo.atomgraphics.graphics;

public class GraphicsPage {
    private GraphicsPageContext mPageContext;
    private long mNativeGraphicsPageRef;

    public GraphicsPage(GraphicsPageContext pageContext, GraphicsLayer rootLayer) {
        mPageContext = pageContext;
        mNativeGraphicsPageRef = createNativeGraphicsPage(pageContext.getNativePageContextRef(), rootLayer.getNativeGraphicsLayerRef());
    }

    public GraphicsPageContext getPageContext() {
        return mPageContext;
    }

    public void release() {
        this.release(mNativeGraphicsPageRef);
    }

    private native void release(long nativeGraphicsPageRef);

    private native long createNativeGraphicsPage(long graphicsPageContextRef, long rootLayerRef);

}

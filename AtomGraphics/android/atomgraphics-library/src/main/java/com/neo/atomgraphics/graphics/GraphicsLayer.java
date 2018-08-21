package com.neo.atomgraphics.graphics;

import com.neo.atomgraphics.node.Node;

public class GraphicsLayer {

    private long mNativeGraphicsLayerRef;

    public GraphicsLayer(PlatformLayer platformLayer, Node rootNode) {
        mNativeGraphicsLayerRef = createNativeGraphicsLayer(platformLayer.getNativePlatformLayerRef(), rootNode.getNativeNodeRef(), layerType());
    }

    public long getNativeGraphicsLayerRef() {
        return mNativeGraphicsLayerRef;
    }

    protected String layerType() {
        return "default";
    }

    private native long createNativeGraphicsLayer(long platformLayerRef, long rootNodeRef, String layerType);
}

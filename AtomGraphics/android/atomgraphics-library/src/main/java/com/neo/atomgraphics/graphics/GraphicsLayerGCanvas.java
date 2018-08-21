package com.neo.atomgraphics.graphics;

import com.neo.atomgraphics.node.Node;

public class GraphicsLayerGCanvas extends GraphicsLayer {

    public GraphicsLayerGCanvas(PlatformLayerTextureView platformLayer, Node rootNode) {
        super(platformLayer, rootNode);
    }

    @Override
    protected String layerType() {
        return "gcanvas";
    }
}

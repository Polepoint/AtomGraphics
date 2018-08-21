package com.neo.atomgraphics.graphics;

import com.neo.atomgraphics.GCanvasTextureView;

public class PlatformLayerTextureView extends PlatformLayer {

    public PlatformLayerTextureView(GCanvasTextureView textureView) {
        mNativePlatformLayerRef = createNativePlatformLayerRef(textureView);
    }

    private native long createNativePlatformLayerRef(GCanvasTextureView textureView);
}

package com.neo.atomgraphics.node;

import com.neo.atomgraphics.base.CalledByNative;
import com.taobao.gcanvas.GCanvasJNI;

public class GCanvasNode extends CanvasNode {

    @Override
    protected String getNodeType() {
        return "gcanvas";
    }

    @CalledByNative
    private static int measureText(String text, String fontConfig) {
        return GCanvasJNI.measureText(text, fontConfig);
    }
}

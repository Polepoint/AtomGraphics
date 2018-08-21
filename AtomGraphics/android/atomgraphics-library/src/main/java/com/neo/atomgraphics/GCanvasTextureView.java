package com.neo.atomgraphics;

import android.graphics.SurfaceTexture;
import android.util.AttributeSet;
import android.view.TextureView;

import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.graphics.GraphicsPageContext;
import com.taobao.gcanvas.GCanvasJNI;
import com.taobao.gcanvas.surface.GTextureView;

import java.util.UUID;

public class GCanvasTextureView extends GTextureView implements TextureView.SurfaceTextureListener {

    private GraphicsPageContext mPageContext;

    public GCanvasTextureView(GraphicsPageContext context) {
        this(context, null);
    }

    public GCanvasTextureView(GraphicsPageContext context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public GCanvasTextureView(GraphicsPageContext context, AttributeSet attrs, int defStyleAttr) {
        super(context.getContext(), UUID.randomUUID().toString(), attrs, defStyleAttr);
        mPageContext = context;
        this.addSurfaceTextureListener(this);
    }

    @CalledByNative
    public void renderCommand(String renderCommand) {
        GCanvasJNI.render(1, this.getCanvasKey(), renderCommand);
    }


    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        String canvasKey = this.getCanvasKey();
//        GCanvasJNI.setDevicePixelRatio(canvasKey, getResources().getDisplayMetrics().density);
        GCanvasJNI.setDevicePixelRatio(canvasKey, 1);
        GCanvasJNI.setContextType(canvasKey, 0);
        GCanvasJNI.setLogLevel("debug");
        GraphicsJavaScriptCore.globalJavaScriptCore(mPageContext).runScriptFile("main-android.js");
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        return true;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {

    }
}

package com.neo.atomgraphics;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.widget.LinearLayout;

import com.neo.atomgraphics.graphics.GraphicsLayer;
import com.neo.atomgraphics.graphics.GraphicsLayerGCanvas;
import com.neo.atomgraphics.graphics.GraphicsPage;
import com.neo.atomgraphics.graphics.GraphicsPageContext;
import com.neo.atomgraphics.graphics.PlatformLayerTextureView;
import com.neo.atomgraphics.node.GCanvasNode;

public class AtomGraphicsView extends LinearLayout {
    private GCanvasTextureView mGCanvasTextureView;
    private GraphicsPage mGraphicsPage;
    private GraphicsPageContext mPageContext;
    private GraphicsLayer mRootLayer;
    private GCanvasNode mRootNode;

    public AtomGraphicsView(Context context) {
        this(context, null);
    }

    public AtomGraphicsView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public AtomGraphicsView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.initGraphicsEnv();
        this.setOrientation(LinearLayout.VERTICAL);
        mPageContext = GraphicsPageContext.GraphicsPageContextForContext(context);

        mGCanvasTextureView = new GCanvasTextureView(mPageContext);
        mGCanvasTextureView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

        mRootNode = new GCanvasNode();
        mRootLayer = new GraphicsLayerGCanvas(new PlatformLayerTextureView(mGCanvasTextureView), mRootNode);
        mGraphicsPage = new GraphicsPage(mPageContext, mRootLayer);
        addView(mGCanvasTextureView);
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        super.onLayout(changed, l, t, r, b);
        if (changed) {
            mRootNode.setFrame(0, 0, r - l, b - t);
        }
    }

    public void reload() {
        GraphicsJavaScriptCore.globalJavaScriptCore(mPageContext).runScript("redraw()");
    }

    private native void initGraphicsEnv();
}

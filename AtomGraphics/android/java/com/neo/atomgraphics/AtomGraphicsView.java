package com.neo.atomgraphics;

import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.widget.FrameLayout;

import com.neo.atomgraphics.base.ContextUtils;
import com.neo.atomgraphics.execution.ExecutionEnvironment;

public class AtomGraphicsView extends FrameLayout {
    private ContentView mContentView;
    private ExecutionEnvironment mExecutionEnvironment;
    private boolean mInitialized;
    private Rect mViewport;

    public AtomGraphicsView(Context context) {
        this(context, null);
    }

    public AtomGraphicsView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public AtomGraphicsView(final Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.setBackgroundColor(Color.TRANSPARENT);
        mViewport = new Rect();

        mContentView = new ContentView(context, new ContentView.Listener() {
            @Override
            public void didFinishInitialization() {
                mInitialized = true;
                if (mExecutionEnvironment != null) {
                    mExecutionEnvironment.onExecution(context, mContentView.getPageID());
                }
            }

            @Override
            public void notifyViewportChanged(int x, int y, int width, int height) {
                if (mViewport.left != x
                        || mViewport.top != y
                        || mViewport.width() != width
                        || mViewport.height() != height) {

                    mViewport.set(x, y, x + width, y + height);
                    onViewportChange(x, y, width, height);
                }
            }
        });

        addView(mContentView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
    }


    public void onViewportChange(int x, int y, int width, int height) {

    }

    public void destroy() {
        if (mExecutionEnvironment != null) {
            mExecutionEnvironment.destroy();
        }
        mContentView.destroy();
    }

    public void setExecutionEnvironment(ExecutionEnvironment executionEnvironment) {
        this.mExecutionEnvironment = executionEnvironment;
        if (mInitialized) {
            this.mExecutionEnvironment.onExecution(ContextUtils.getApplicationContext(), mContentView.getPageID());
        }
    }
}

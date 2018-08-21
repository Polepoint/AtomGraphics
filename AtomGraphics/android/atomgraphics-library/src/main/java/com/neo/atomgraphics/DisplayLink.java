package com.neo.atomgraphics;

import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.graphics.GraphicsPageContext;

public class DisplayLink {
    private long mNativeDisplayLinkRef;
    private final VSyncMonitor mVSyncMonitor;
    private boolean mVSyncPaused;

    @CalledByNative
    DisplayLink(long nativeDisplayLinkRef, long nativePageContext) {
        mNativeDisplayLinkRef = nativeDisplayLinkRef;
        GraphicsPageContext pageContext = GraphicsPageContext.GraphicsPageContextForNativeRef(nativePageContext);
        if (pageContext != null) {
            mVSyncPaused = true;
            VSyncMonitor.Listener vSyncListener = new VSyncMonitor.Listener() {
                @Override
                public void onVSync(VSyncMonitor monitor, long vsyncTimeMicros) {
                    if (mVSyncPaused) {
                        return;
                    }
                    onDisplayVSync(vsyncTimeMicros);
                }
            };
            this.mVSyncMonitor = new VSyncMonitor(pageContext.getContext(), vSyncListener);
        } else {
            this.mVSyncMonitor = null;
        }
    }

    @CalledByNative
    public void schedule() {
        if (mVSyncPaused) {
            mVSyncPaused = false;
            mVSyncMonitor.requestUpdate();
        }
    }

    private void onDisplayVSync(long vsyncTimeMicros) {
        mVSyncPaused = this.nativeOnVSync(mNativeDisplayLinkRef, vsyncTimeMicros);
        if (!mVSyncPaused) {
            mVSyncMonitor.requestUpdate();
        }
    }

    private native boolean nativeOnVSync(long nativeDisplayLinkRef, long vsyncTimeMicros);
}

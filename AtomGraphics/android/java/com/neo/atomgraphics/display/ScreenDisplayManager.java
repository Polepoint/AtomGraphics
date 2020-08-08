package com.neo.atomgraphics.display;

import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.display.DisplayManager;
import android.hardware.display.DisplayManager.*;
import android.util.SparseArray;
import android.view.Display;
import android.view.WindowManager;
import com.neo.atomgraphics.base.CalledByNative;
import com.neo.atomgraphics.base.ContextUtils;

public class ScreenDisplayManager {

    private interface DisplayListenerBackend {

        /**
         * Starts to listen for display changes. This will be called
         * when the first observer is added.
         */
        void startListening();

        /**
         * Toggle the accurate mode if it wasn't already doing so. The backend
         * will keep track of the number of times this has been called.
         */
        void startAccurateListening();

        /**
         * Request to stop the accurate mode. It will effectively be stopped
         * only if this method is called as many times as
         * startAccurateListening().
         */
        void stopAccurateListening();
    }

    @SuppressLint("NewApi")
    private class DisplayListenerBackendImpl
            implements DisplayListenerBackend, DisplayListener {

        // DisplayListenerBackend implementation:

        @Override
        public void startListening() {
            getDisplayManager().registerDisplayListener(this, null);
        }

        @Override
        public void startAccurateListening() {
            // Always accurate. Do nothing.
        }

        @Override
        public void stopAccurateListening() {
            // Always accurate. Do nothing.
        }

        // DisplayListener implementation:

        @Override
        public void onDisplayAdded(int sdkDisplayId) {
            // ScreenDisplay is added lazily on first use. This is to workaround corner case
            // bug where DisplayManager.getDisplay(sdkDisplayId) returning null here.
        }

        @Override
        public void onDisplayRemoved(int sdkDisplayId) {
            // Never remove the primary display.
            if (sdkDisplayId == mMainSdkDisplayId) return;

            ScreenDisplay screenDisplay = mIdMap.get(sdkDisplayId);
            if (screenDisplay == null) return;

            if (mNativePointer != 0) nativeRemoveDisplay(mNativePointer, sdkDisplayId);
            mIdMap.remove(sdkDisplayId);
        }

        @Override
        public void onDisplayChanged(int sdkDisplayId) {
            ScreenDisplay screenDisplay = mIdMap.get(sdkDisplayId);
            Display display = getDisplayManager().getDisplay(sdkDisplayId);
            // Note display null check here is needed because there appear to be an edge case in
            // screen display code, similar to onDisplayAdded.
            if (screenDisplay != null && display != null) {
                screenDisplay.updateFromDisplay(display);
            }
        }
    }

    private static ScreenDisplayManager sScreenDisplayManager;


    private long mNativePointer;
    private DisplayListenerBackend mBackend;
    private int mMainSdkDisplayId;
    private final SparseArray<ScreenDisplay> mIdMap = new SparseArray<>();

    @CalledByNative
    private static void onNativeSideCreated(long nativePointer) {
        if (sScreenDisplayManager == null) {
            initialManagerInstance(nativePointer);
        } else {
            sScreenDisplayManager.mNativePointer = nativePointer;
        }
    }

    static ScreenDisplayManager getInstance() {
        if (sScreenDisplayManager == null) {
            // Split between creation and initialization to allow for calls from ScreenDisplay to
            // reference sScreenDisplayManager during initialize().
            initialManagerInstance(null);
        }
        return sScreenDisplayManager;
    }

    private static void initialManagerInstance(Long nativePointer) {
        sScreenDisplayManager = new ScreenDisplayManager();
        if (nativePointer != null) {
            sScreenDisplayManager.mNativePointer = nativePointer;
        }

        sScreenDisplayManager.initialize();
    }

    private void initialize() {
        mBackend = new DisplayListenerBackendImpl();
        // Make sure the display map contains the built-in primary display.
        // The primary display is never removed.
        Display display = getDisplayManager().getDisplay(Display.DEFAULT_DISPLAY);

        // Android documentation on Display.DEFAULT_DISPLAY suggests that the above
        // method might return null. In that case we retrieve the default display
        // from the application context and take it as the primary display.
        if (display == null) display = getDefaultDisplayForContext(getContext());

        mMainSdkDisplayId = display.getDisplayId();
        addDisplay(display); // Note this display is never removed.

        mBackend.startListening();
    }

    public static Display getDefaultDisplayForContext(Context context) {
        WindowManager windowManager =
                (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        return windowManager.getDefaultDisplay();
    }

    private static DisplayManager getDisplayManager() {
        return (DisplayManager) getContext().getSystemService(Context.DISPLAY_SERVICE);
    }

    private static Context getContext() {
        return ContextUtils.getApplicationContext();
    }

    private ScreenDisplay addDisplay(Display display) {
        int sdkDisplayId = display.getDisplayId();
        ScreenDisplay screenDisplay = new ScreenDisplay(display.getDisplayId());
        assert mIdMap.get(sdkDisplayId) == null;
        mIdMap.put(sdkDisplayId, screenDisplay);
        screenDisplay.updateFromDisplay(display);
        return screenDisplay;
    }

    void updateDisplayOnNativeSide(ScreenDisplay display) {
        if (mNativePointer == 0) return;
        nativeUpdateDisplay(mNativePointer,
                display.getDisplayId(),
                display.getDisplayWidth(),
                display.getDisplayHeight(),
                display.getDipScale(),
                display.getBitsPerPixel(),
                display.getBitsPerComponent());
    }

    private native void nativeUpdateDisplay(long nativeDisplayManager, int sdkDisplayId,
                                            int width, int height, float dipScale, int bitsPerPixel,
                                            int bitsPerComponent);

    private native void nativeRemoveDisplay(long nativeDisplayManager, int sdkDisplayId);

}

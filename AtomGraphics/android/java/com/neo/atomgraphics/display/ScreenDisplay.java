package com.neo.atomgraphics.display;

import android.graphics.PixelFormat;
import android.graphics.Point;
import android.util.DisplayMetrics;
import android.view.Display;

public class ScreenDisplay {

    private final int mDisplayId;
    private Point mSize;
    private float mDipScale;
    private int mBitsPerPixel;
    private int mBitsPerComponent;

    ScreenDisplay(int displayId) {
        mDisplayId = displayId;
        mSize = new Point();
    }


    /**
     * multi-screen ?
     *
     * @return Display id that does not necessarily match the one defined in Android's Display.
     */
    public int getDisplayId() {
        return mDisplayId;
    }

    /**
     * Note: For JB pre-MR1, this can sometimes return values smaller than the actual screen.
     * https://crbug.com/829318
     *
     * @return Display height in physical pixels.
     */
    public int getDisplayHeight() {
        return mSize.y;
    }

    /**
     * Note: For JB pre-MR1, this can sometimes return values smaller than the actual screen.
     *
     * @return Display width in physical pixels.
     */
    public int getDisplayWidth() {
        return mSize.x;
    }

    /**
     * @return A scaling factor for the Density Independent Pixel unit.
     */
    public float getDipScale() {
        return mDipScale;
    }

    /**
     * @return Number of bits per pixel.
     */
    /* package */ int getBitsPerPixel() {
        return mBitsPerPixel;
    }

    /**
     * @return Number of bits per each color component.
     */
    @SuppressWarnings("deprecation")
    /* package */ int getBitsPerComponent() {
        return mBitsPerComponent;
    }

    private void update(Point size, Float dipScale, Integer bitsPerPixel, Integer bitsPerComponent) {
        boolean sizeChanged = size != null && !mSize.equals(size);
        // Intentional comparison of floats: we assume that if scales differ, they differ
        // significantly.
        boolean dipScaleChanged = dipScale != null && mDipScale != dipScale;
        boolean bitsPerPixelChanged = bitsPerPixel != null && mBitsPerPixel != bitsPerPixel;
        boolean bitsPerComponentChanged =
                bitsPerComponent != null && mBitsPerComponent != bitsPerComponent;

        boolean changed = sizeChanged || dipScaleChanged || bitsPerPixelChanged
                || bitsPerComponentChanged;
        if (!changed) return;

        if (sizeChanged) mSize = size;
        if (dipScaleChanged) mDipScale = dipScale;
        if (bitsPerPixelChanged) mBitsPerPixel = bitsPerPixel;
        if (bitsPerComponentChanged) mBitsPerComponent = bitsPerComponent;

        ScreenDisplayManager.getInstance().updateDisplayOnNativeSide(this);
    }


    /**
     * This method returns the bitsPerPixel without the alpha channel, as this is the value expected
     * by Chrome and the CSS media queries.
     */
    @SuppressWarnings("deprecation")
    private static int bitsPerPixel(int pixelFormatId) {
        // For JB-MR1 and above, this is the only value, so we can hard-code the result.
        if (pixelFormatId == PixelFormat.RGBA_8888) return 24;

        PixelFormat pixelFormat = new PixelFormat();
        PixelFormat.getPixelFormatInfo(pixelFormatId, pixelFormat);
        if (!PixelFormat.formatHasAlpha(pixelFormatId)) return pixelFormat.bitsPerPixel;

        switch (pixelFormatId) {
            case PixelFormat.RGBA_1010102:
                return 30;

            case PixelFormat.RGBA_4444:
                return 12;

            case PixelFormat.RGBA_5551:
                return 15;

            case PixelFormat.RGBA_8888:
                assert false;
                // fall through
                // RGBX_8888 does not have an alpha channel even if it has 8 reserved bits at the end.
            case PixelFormat.RGBX_8888:
            case PixelFormat.RGB_888:
            default:
                return 24;
        }
    }


    @SuppressWarnings("deprecation")
    private static int bitsPerComponent(int pixelFormatId) {
        switch (pixelFormatId) {
            case PixelFormat.RGBA_4444:
                return 4;

            case PixelFormat.RGBA_5551:
                return 5;

            case PixelFormat.RGBA_8888:
            case PixelFormat.RGBX_8888:
            case PixelFormat.RGB_888:
                return 8;

            case PixelFormat.RGB_332:
                return 2;

            case PixelFormat.RGB_565:
                return 5;

            // Non-RGB formats.
            case PixelFormat.A_8:
            case PixelFormat.LA_88:
            case PixelFormat.L_8:
                return 0;

            // Unknown format. Use 8 as a sensible default.
            default:
                return 8;
        }
    }


    @SuppressWarnings("deprecation")
    public void updateFromDisplay(Display display) {
        Point size = new Point();
        DisplayMetrics displayMetrics = new DisplayMetrics();
        display.getRealSize(size);
        display.getRealMetrics(displayMetrics);

        // JellyBean MR1 and later always uses RGBA_8888.
        int pixelFormatId = PixelFormat.RGBA_8888;
        this.update(size, displayMetrics.density, bitsPerPixel(pixelFormatId), bitsPerComponent(pixelFormatId));
    }
}

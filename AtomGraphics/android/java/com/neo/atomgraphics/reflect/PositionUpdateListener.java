package com.neo.atomgraphics.reflect;

public abstract class PositionUpdateListener {

    private Object mHiddenListener;

    void setHiddenListener(Object hiddenListener) {
        mHiddenListener = hiddenListener;
    }

    Object getHiddenListener() {
        return mHiddenListener;
    }

    public abstract void positionChanged(long frameNumber, int left, int top, int right, int bottom);

    public abstract void positionLost(long frameNumber);

}

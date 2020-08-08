package com.neo.atomgraphics.node.canvas;

public class Font {

    private String mCSSFont;

    private Slant mSlant;
    private boolean mSmallCaps;
    private int mWeight;
    private int mFontSize;
    private String mFamily;

    public enum Slant {
        kNormal,
        kItalic,
        kOblique,
    }

    public Font() {
        mSmallCaps = false;
        mSlant = Slant.kNormal;
        mWeight = 400;
        mFontSize = 12;
    }

    String createCSSString() {

        if (mCSSFont != null) {
            return mCSSFont;
        }

        mCSSFont = "";
        boolean needSeparator = false;
        if (mSlant == Slant.kItalic) {
            mCSSFont = "italic";
            needSeparator = true;
        } else if (mSlant == Slant.kOblique) {
            mCSSFont = "oblique";
            needSeparator = true;
        }

        if (mSmallCaps) {
            if (needSeparator) {
                mCSSFont += " ";
            }
            mCSSFont += "small-caps";
            needSeparator = true;
        }

        if (mWeight != 400) {
            if (needSeparator) {
                mCSSFont += " ";
            }

            mCSSFont += mWeight;
            needSeparator = true;
        }

        if (needSeparator) {
            mCSSFont += " ";
        }

        return mCSSFont += mFontSize + "px " + mFamily;
    }

    public void setFamily(String family) {
        if (!mFamily.equals(family)) {
            mFamily = family;
            mCSSFont = null;
        }
    }

    public void setFontSize(int fontSize) {
        if (mFontSize != fontSize) {
            mFontSize = fontSize;
            mCSSFont = null;
        }
    }

    public void setSmallCaps(boolean smallCaps) {
        if (mSmallCaps != smallCaps) {
            mSmallCaps = smallCaps;
            mCSSFont = null;
        }
    }

    public void setWeight(int weight) {
        if (mWeight != weight) {
            mWeight = weight;
            mCSSFont = null;
        }
    }

    public void setSlant(Slant slant) {
        if (mSlant != slant) {
            mSlant = slant;
            mCSSFont = null;
        }
    }
}

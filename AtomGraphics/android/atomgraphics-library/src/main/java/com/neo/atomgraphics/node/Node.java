package com.neo.atomgraphics.node;

import android.graphics.Canvas;

import java.util.ArrayList;

public class Node {

    private long mNativeNode;
    private ArrayList<Node> mSubNode;

    public Node() {
        super();
        mSubNode = new ArrayList<>();
        mNativeNode = createNativeNode(getNodeType());
    }

    protected String getNodeType() {
        return "default";
    }

    public long getNativeNodeRef() {
        return this.mNativeNode;
    }

    public void render(Canvas canvas) {

    }

    public void addSubNode(Node node) {
        mSubNode.add(node);
        addSubNode(mNativeNode, node.mNativeNode);
    }

    public void setFrame(int x, int y, int width, int height) {
        setFrame(mNativeNode, x, y, width, height);
    }

    protected native long createNativeNode(String type);

    private native void setFrame(long mNativeNode, int x, int y, int width, int height);

    private native void addSubNode(long mNativeNode, long subNativeNode);
}

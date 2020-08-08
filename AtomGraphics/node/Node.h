//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMNODE_H
#define ATOMNODE_H

#include <vector>
#include <string>
#include "math/AtomMath.h"
#include "math/AtomGeometry.h"
#include "base/AtomTypes.h"


namespace AtomGraphics {

class GraphicsContext;

class GraphicsLayer;

class Node {

public:
    static const int INVALID_TAG = -1;
    
    Node();
    
    virtual ~Node();
    
    virtual bool init();
    
    virtual void setGraphicsLayer(GraphicsLayer *graphicsLayer);
    
    virtual GraphicsLayer *graphicsLayer() {
        return m_graphicsLayer;
    }
    
    virtual void addChild(Node *child);
    
    virtual void addChild(Node *child, int tag);
    
    virtual void addChild(Node *child, const std::string &name);
    
    virtual Node *getChildByTag(int tag) const;
    
    virtual Node *getChildByName(const std::string &name) const;

//    template <typename T>
//    T getChildByTag(int tag) const { return static_cast<T>(getChildByTag(tag)); }
    
    virtual std::vector<Node *> &getChildren() {
        return m_children;
    }
    
    virtual const std::vector<Node *> &getChildren() const {
        return m_children;
    }
    
    virtual void setParent(Node *parent);
    
    virtual Node *getParent() {
        return m_parent;
    }
    
    virtual const Node *getParent() const {
        return m_parent;
    }
    
    virtual void removeFromParent();
    
    virtual void removeFromParentAndCleanup(bool cleanup);
    
    virtual void removeChild(Node *child, bool cleanup = true);
    
    virtual void cleanup();
    
    virtual void removeAllChildren();
    
    virtual void removeAllChildrenWithCleanup(bool cleanup);
    
    virtual int getTag() const;
    
    virtual void setTag(int tag);
    
    virtual const std::string &getName() const;
    
    virtual void setName(const std::string &name);
    
    virtual const Color4F &getBackgroundColor() const;
    
    virtual void setFrame(const FloatRect &frame);
    
    virtual const FloatRect &getFrame() const;
    
    virtual void setBackgroundColor(const Color4F &color);
    
    virtual void willDrawContent();
    
    virtual void drawContent(GraphicsContext *context, const FloatRect &bounds);
    
    virtual void didEndDrawingContent();
    
    static Node *create();

protected:


private:
    void addChildHelper(Node *child, int tag, const std::string &name, bool setTag);

protected:
    
    GraphicsLayer *m_graphicsLayer{nullptr};
    
    bool m_contentSizeDirty;         ///< whether or not the contentSize is dirty
    
    bool m_dirty;
    
    FloatRect m_frame;                 ///< position of the node
    
    mutable Mat4 m_transform;        ///< transform
    
    Node *m_parent;                  ///< weak reference to parent node
    std::vector<Node *> m_children;   ///< array of children nodes
    
    float _globalZOrder;            ///< Global order used to sort the node
    
    std::string m_name;              ///<a string label, an user defined string to identify this node
    size_t m_hashOfName;             ///<hash value of _name, used for speed in getChildByName
    
    int m_tag;                       ///< a tag. Can be any number you assigned just to identify this node
    
    Color4F m_backgroundColor;
};

}

#endif //ATOMNODE_H

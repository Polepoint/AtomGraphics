//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMNODE_H
#define ATOMGRAPHICS_ATOMNODE_H

#include <vector>
#include <string>
#include "math/AtomMath.h"
#include "math/AtomGeometry.h"
#include "AtomRenderer.h"
#include "AtomTypes.h"


namespace atomgraphics {

    class Renderer;

    class Node {

    public:
        static const int INVALID_TAG = -1;

        virtual void addChild(Node *child);

        virtual void addChild(Node *child, int tag);

        virtual void addChild(Node *child, const std::string &name);

        virtual Node *getChildByTag(int tag) const;

        virtual Node *getChildByName(const std::string &name) const;

//    template <typename T>
//    T getChildByTag(int tag) const { return static_cast<T>(getChildByTag(tag)); }

        virtual std::vector<Node *> &getChildren() {
            return _children;
        }

        virtual const std::vector<Node *> &getChildren() const {
            return _children;
        }

        virtual void setParent(Node *parent);

        virtual Node *getParent() {
            return _parent;
        }

        virtual const Node *getParent() const {
            return _parent;
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

        virtual const Color4B &getBackgroundColor() const;

        virtual void setBackgroundColor(const Color4B &color);

//        virtual void draw(Renderer *renderer, const atomgraphics::Mat4 &transform, uint32_t flags);

//        virtual void draw() final;

        void scheduleUpdate(void);

        virtual bool dirty();

        static Node *create();

        Node();

        virtual ~Node();

        virtual bool init();

    protected:


    private:
        void addChildHelper(Node *child, int tag, const std::string &name, bool setTag);

    protected:

        Size _contentSize;
        bool _contentSizeDirty;         ///< whether or not the contentSize is dirty

        bool _dirty;

        Vec2 _position;                 ///< position of the node

        mutable Mat4 _transform;        ///< transform

        Node *_parent;                  ///< weak reference to parent node
        std::vector<Node *> _children;   ///< array of children nodes

        float _globalZOrder;            ///< Global order used to sort the node

        std::string _name;              ///<a string label, an user defined string to identify this node
        size_t _hashOfName;             ///<hash value of _name, used for speed in getChildByName

        int _tag;                       ///< a tag. Can be any number you assigned just to identify this node

        Color4B _backgroundColor;
    };

}

#endif //ATOMGRAPHICS_ATOMNODE_H

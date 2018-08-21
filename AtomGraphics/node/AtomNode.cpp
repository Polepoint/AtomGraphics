//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <new>
#include "AtomNode.h"

#define ATOM_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)

namespace AtomGraphics {

    Node *Node::create() {
        Node *ret = new(std::nothrow) Node();
        if (ret && ret->init()) {
//        ret->autorelease();
        } else {
            ATOM_SAFE_DELETE(ret);
        }
        return ret;
    }

    bool Node::init() {
        return true;
    }

    Node::Node() {
        _dirty = true;
    }

    Node::~Node() {

    }

    void Node::setGraphicsLayer(GraphicsLayer *graphicsLayer) {
        _graphicsLayer = graphicsLayer;
        for (auto child:_children) {
            child->setGraphicsLayer(graphicsLayer);
        }
    }


    void Node::addChild(Node *child) {

        ATASSERT(child != nullptr, "Argument must be non-nil");
        this->addChild(child, child->_name);
    }

    void Node::addChild(Node *child, int tag) {
        ATASSERT(child != nullptr, "Argument must be non-nil");
        ATASSERT(child->_parent == nullptr, "child already added. It can't be added again");

        addChildHelper(child, tag, "", true);
    }

    void Node::addChild(Node *child, const std::string &name) {
        ATASSERT(child != nullptr, "Argument must be non-nil");
        ATASSERT(child->_parent == nullptr, "child already added. It can't be added again");

        addChildHelper(child, INVALID_TAG, name, false);
    }


    void Node::addChildHelper(Node *child, int tag, const std::string &name, bool setTag) {
        auto assertNotSelfChild([this, child]() -> bool {
            for (Node *parent(getParent()); parent != nullptr;
                 parent = parent->getParent())
                if (parent == child)
                    return false;

            return true;
        });
        (void) assertNotSelfChild;

        ATASSERT(assertNotSelfChild(),
                "A node cannot be the child of his own children");

        if (_children.empty()) {
            _children.reserve(4);
        }

        _children.push_back(child);

        if (setTag)
            child->setTag(tag);
        else
            child->setName(name);

        child->setParent(this);
        child->setGraphicsLayer(_graphicsLayer);
    }

    void Node::setParent(Node *parent) {
        _parent = parent;
    }

    Node *Node::getChildByTag(int tag) const {
        ATASSERT(tag != Node::INVALID_TAG, "Invalid tag");

        for (const auto child : _children) {
            if (child && child->_tag == tag)
                return child;
        }
        return nullptr;
    }

    Node *Node::getChildByName(const std::string &name) const {
        ATASSERT(!name.empty(), "Invalid name");

        std::hash<std::string> h;
        size_t hash = h(name);

        for (const auto &child : _children) {
            // Different strings may have the same hash code, but can use it to compare first for speed
            if (child->_hashOfName == hash && child->_name.compare(name) == 0)
                return child;
        }
        return nullptr;
    }

    void Node::removeFromParent() {

    }

    void Node::removeFromParentAndCleanup(bool cleanup) {

    }

    void Node::removeChild(Node *child, bool cleanup /* = true */) {
        // explicit nil handling
        if (_children.empty()) {
            return;
        }

        if (cleanup) {
            child->cleanup();
        }

        ssize_t index = -1;
        auto iter = std::find(_children.begin(), _children.end(), child);
        if (iter != _children.end())
            index = iter - _children.begin();
        auto it = std::next(_children.begin(), index);
        _children.erase(it);
        child->setParent(nullptr);
        child->setGraphicsLayer(nullptr);
    }


    void Node::cleanup() {
        //clean the running and queued renderer
        for (const auto &child: _children)
            child->cleanup();
    }

    void Node::removeAllChildren() {
        this->removeAllChildrenWithCleanup(true);
    }

    void Node::removeAllChildrenWithCleanup(bool cleanup) {
        // not using detachChild improves speed here
        for (const auto &child : _children) {
//        // IMPORTANT:
//        //  -1st do onExit
//        //  -2nd cleanup
//        if(_running)
//        {
//            child->onExitTransitionDidStart();
//            child->onExit();
//        }

            if (cleanup) {
                child->cleanup();
            }
            child->setParent(nullptr);
            child->setGraphicsLayer(nullptr);
        }

        _children.clear();
    }

    int Node::getTag() const {
        return _tag;
    }

    void Node::setTag(int tag) {
        _tag = tag;
    }

    const std::string &Node::getName() const {
        return _name;
    }

    void Node::setName(const std::string &name) {
        _name = name;
        std::hash<std::string> h;
        _hashOfName = h(name);
    }

    const Color4F &Node::getBackgroundColor() const {
        return _backgroundColor;
    }

    void Node::setBackgroundColor(const Color4F &color) {
        _backgroundColor = color;
    }

    const Rect &Node::getFrame() const {
        return _frame;
    }

    void Node::setFrame(const Rect &frame) {
        if (!_frame.equals(frame)) {
            _frame = frame;
            _dirty = true;
        }
    }

    void Node::draw(GraphicsContext *context) {
        if (this->_dirty) {
//            painter->fillRect(this->_position.x, this->_position.y, this->_contentSize.width, this->_contentSize.height, this->_backgroundColor);

            auto &children = this->_children;
            for (const auto &child : children) {
                child->draw(context);
            }

            this->_dirty = false;
        }
    }
//
//    void Node::draw() {
//    auto renderer = _director->getRenderer();
//    draw(renderer, _modelViewTransform, true);
//    }

    void Node::scheduleUpdate() {
        if (_graphicsLayer && _dirty) {
            _graphicsLayer->setLayerContentsDirty(true);
            _graphicsLayer->scheduleLayerFlush();
        }
    }

    bool Node::dirty() {
        return _dirty;
    }

    void Node::setDirty(bool dirty) {
        if (dirty ^ _dirty) {
            _dirty = dirty;
            if (dirty) {
                auto &children = this->_children;
                for (const auto &child : children) {
                    child->setDirty(dirty);
                }
            }
        }
    }

}


//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <new>
#include "Node.h"
#include "graphics/GraphicsLayer.h"

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
    m_dirty = true;
}

Node::~Node() {

}

void Node::setGraphicsLayer(GraphicsLayer *graphicsLayer) {
    m_graphicsLayer = graphicsLayer;
    for (auto child:m_children) {
        child->setGraphicsLayer(graphicsLayer);
    }
}


void Node::addChild(Node *child) {
    
    ATASSERT(child != nullptr, "Argument must be non-nil");
    this->addChild(child, child->m_name);
}

void Node::addChild(Node *child, int tag) {
    ATASSERT(child != nullptr, "Argument must be non-nil");
    ATASSERT(child->m_parent == nullptr, "child already added. It can't be added again");
    
    addChildHelper(child, tag, "", true);
}

void Node::addChild(Node *child, const std::string &name) {
    ATASSERT(child != nullptr, "Argument must be non-nil");
    ATASSERT(child->m_parent == nullptr, "child already added. It can't be added again");
    
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
    
    if (m_children.empty()) {
        m_children.reserve(4);
    }
    
    m_children.push_back(child);
    
    if (setTag)
        child->setTag(tag);
    else
        child->setName(name);
    
    child->setParent(this);
    child->setGraphicsLayer(m_graphicsLayer);
}

void Node::setParent(Node *parent) {
    m_parent = parent;
}

Node *Node::getChildByTag(int tag) const {
    ATASSERT(tag != Node::INVALID_TAG, "Invalid tag");
    
    for (const auto child : m_children) {
        if (child && child->m_tag == tag)
            return child;
    }
    return nullptr;
}

Node *Node::getChildByName(const std::string &name) const {
    ATASSERT(!name.empty(), "Invalid name");
    
    std::hash<std::string> h;
    size_t hash = h(name);
    
    for (const auto &child : m_children) {
        // Different strings may have the same hash code, but can use it to compare first for speed
        if (child->m_hashOfName == hash && child->m_name.compare(name) == 0)
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
    if (m_children.empty()) {
        return;
    }
    
    if (cleanup) {
        child->cleanup();
    }
    
    ssize_t index = -1;
    auto iter = std::find(m_children.begin(), m_children.end(), child);
    if (iter != m_children.end())
        index = iter - m_children.begin();
    auto it = std::next(m_children.begin(), index);
    m_children.erase(it);
    child->setParent(nullptr);
    child->setGraphicsLayer(nullptr);
}


void Node::cleanup() {
    //clean the running and queued renderer
    for (const auto &child: m_children)
        child->cleanup();
}

void Node::removeAllChildren() {
    this->removeAllChildrenWithCleanup(true);
}

void Node::removeAllChildrenWithCleanup(bool cleanup) {
    // not using detachChild improves speed here
    for (const auto &child : m_children) {
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
    
    m_children.clear();
}

int Node::getTag() const {
    return m_tag;
}

void Node::setTag(int tag) {
    m_tag = tag;
}

const std::string &Node::getName() const {
    return m_name;
}

void Node::setName(const std::string &name) {
    m_name = name;
    std::hash<std::string> h;
    m_hashOfName = h(name);
}

const Color4F &Node::getBackgroundColor() const {
    return m_backgroundColor;
}

void Node::setBackgroundColor(const Color4F &color) {
    m_backgroundColor = color;
}

const FloatRect &Node::getFrame() const {
    return m_frame;
}

void Node::setFrame(const FloatRect &frame) {
    if (!m_frame.equals(frame)) {
        m_frame = frame;
        m_dirty = true;
    }
}

void Node::willDrawContent() {

}

void Node::drawContent(GraphicsContext *context, const FloatRect &bounds) {
    if (this->m_dirty) {
        this->m_dirty = false;
    }
}

void Node::didEndDrawingContent() {

}

}


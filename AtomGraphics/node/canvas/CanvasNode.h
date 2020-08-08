//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASNODE_H
#define ATOMCANVASNODE_H

#include "node/Node.h"
#include "WebGLRenderingContext.h"

typedef enum {
    CanvasContextType2D,
    CanvasContextTypeWebGL,
} CanvasContextType;

namespace AtomGraphics {

class CanvasRenderingContext2D;

class CanvasNode : public Node {

public:

    CanvasNode();

    virtual ~CanvasNode(){

    };

    virtual std::shared_ptr<CanvasRenderingContext2D> getContext2d();

    void setFrame(const FloatRect &frame) override;

    const FloatSize &contentSize() const;

    virtual float contentWidth() const;

    virtual void setContentWidth(float width);

    virtual float contentHeight() const;

    virtual void setContentHeight(float height);


protected:

    FloatSize m_contentSize;
    float m_deviceScaleFactor{1};

    std::shared_ptr<CanvasRenderingContext2D> m_canvasContext2d;

    virtual std::shared_ptr<CanvasRenderingContext2D> createContext2d() = 0;
};

}


#endif //ATOMCANVASNODE_H

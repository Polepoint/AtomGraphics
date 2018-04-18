//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGCANVASNODE_H
#define ATOMGCANVASNODE_H


#include "AtomCanvasNode.h"
#include "AtomGCanvasContext2d.h"

namespace AtomGraphics {

    class GCanvasNode : public CanvasNode {


    public:
        CanvasContext2d *getContext2d() override;

        void setPosition(const Vec2 &position) override;

        void setContentSize(const Size &contentSize) override;

        GCanvasNode(const void *container);

        void setContextType(int type);

    private:
        std::string _componentId;
        GCanvasContext2d *_gcanvasContext2d;
    };
}

#endif //ATOMGCANVASNODE_H
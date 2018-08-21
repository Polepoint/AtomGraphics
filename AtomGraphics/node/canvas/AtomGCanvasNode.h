//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGCANVASNODE_H
#define ATOMGCANVASNODE_H


#include "AtomCanvasNode.h"
#include "AtomGCanvasContext2D.h"

namespace AtomGraphics {

    class GCanvasContext2D;

    class GCanvasNode : public CanvasNode {

    public:

        GCanvasNode();

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

        static jclass getClassJObject();

        static void setClassJObject(jclass classObj);

#endif

        CanvasContext2D *getContext2d() override;

        void draw(GraphicsContext *context) override;

    private:
        GCanvasContext2D *m_gcanvasContext2d;
        CanvasContextType m_currentContextType;

        friend class GCanvasContext2D;
    };
}

#endif //ATOMGCANVASNODE_H
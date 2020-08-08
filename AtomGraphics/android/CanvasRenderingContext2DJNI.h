//
// Created by neo on 2019-09-28.
//

#ifndef ATOMGRAPHICS_CANVASRENDERINGCONTEXT2DJNI_H
#define ATOMGRAPHICS_CANVASRENDERINGCONTEXT2DJNI_H

#include "node/canvas/CanvasRenderingContext2D.h"

namespace AtomGraphics {

class CanvasRenderingContext2DJNI {

public:

    CanvasRenderingContext2DJNI(std::shared_ptr<CanvasRenderingContext2D> context)
            : m_context(std::move(context)) {

    };

    CanvasRenderingContext2D *getContext() const {
        return m_context.get();
    }

private:

    std::shared_ptr<CanvasRenderingContext2D> m_context;
};

}

#endif //ATOMGRAPHICS_CANVASRENDERINGCONTEXT2DJNI_H

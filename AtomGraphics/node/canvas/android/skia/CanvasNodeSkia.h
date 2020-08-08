//
// Created by neo on 2018/9/1.
//

#ifndef ATOMGRAPHICS_CANVASNODESKIA_H
#define ATOMGRAPHICS_CANVASNODESKIA_H

#include "node/canvas/CanvasNode.h"
#include "CanvasRenderingContext2DSkia.h"

namespace AtomGraphics {

class CanvasRenderingContext2DSkia;

class CanvasNodeSkia : public CanvasNode {

public:

    void setFrame(const FloatRect &frame) override;

    void drawContent(GraphicsContext *context, const FloatRect &bounds) override;

    void willDrawContent() override;

    void didEndDrawingContent() override;

private:

    std::shared_ptr<CanvasRenderingContext2D> createContext2d() override;

    CanvasRenderingContext2DSkia *context2DSkia();

    friend CanvasRenderingContext2DSkia;
};

}


#endif //ATOMGRAPHICS_CANVASNODESKIA_H

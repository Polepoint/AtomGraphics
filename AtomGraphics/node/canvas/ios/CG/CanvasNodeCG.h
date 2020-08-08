//
// Created by neo on 2018/4/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMCANVASNODECG_H
#define ATOMGRAPHICS_ATOMCANVASNODECG_H

#import "CanvasNode.h"
#import "CanvasRenderingContext2DCG.h"

namespace AtomGraphics {

class CanvasRenderingContext2DCG;

class CanvasNodeCG final : public CanvasNode {

public:
    
    ~CanvasNodeCG() {};
    
    void drawContent(GraphicsContext *context, const FloatRect &) override;
    
    void setFrame(const FloatRect &frame) override;
    
    void willDrawContent() override;
    
    void didEndDrawingContent() override;

private:
    
    std::shared_ptr<CanvasRenderingContext2D> createContext2d() override;
    
    CanvasRenderingContext2DCG *context2DCG();

    friend class CanvasRenderingContext2DCG;
};

}


#endif //ATOMGRAPHICS_ATOMCANVASNODECG_H

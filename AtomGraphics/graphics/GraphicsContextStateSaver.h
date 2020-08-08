//
// Created by neo on 2018/8/26.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CONTEXTSTATESAVER_H
#define ATOMGRAPHICS_CONTEXTSTATESAVER_H

#import "GraphicsContext.h"

namespace AtomGraphics {

class GraphicsContextStateSaver {

public:
    
    GraphicsContextStateSaver(GraphicsContext *context, bool doSave = true);
    
    ~GraphicsContextStateSaver();
    
    void save();
    
    void restore();
    
    bool didSave() const {
        return m_saved;
    }


private:
    
    bool m_saved{false};
    GraphicsContext *m_context;
};

}


#endif //ATOMGRAPHICS_CONTEXTSTATESAVER_H

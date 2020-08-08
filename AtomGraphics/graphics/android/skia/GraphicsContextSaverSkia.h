//
// Created by neo on 2019/4/5.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTEXTSAVERSKIA_H
#define ATOMGRAPHICS_GRAPHICSCONTEXTSAVERSKIA_H

#include "GraphicsContextSkia.h"

namespace AtomGraphics {

class GraphicsContextSaverSkia final {

public:

    GraphicsContextSaverSkia(GraphicsContextSkia *context);

    ~GraphicsContextSaverSkia();

private:
    GraphicsContextSkia *m_context{nullptr};
    int m_saveCount;
};

}


#endif //ATOMGRAPHICS_GRAPHICSCONTEXTSAVERSKIA_H

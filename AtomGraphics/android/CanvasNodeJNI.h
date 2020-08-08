//
// Created by neo on 2019-09-28.
//

#ifndef ATOMGRAPHICS_CANVASNODEJNI_H
#define ATOMGRAPHICS_CANVASNODEJNI_H

#include "node/canvas/CanvasNode.h"

namespace AtomGraphics {

class CanvasNodeJNI {

public:

    CanvasNodeJNI(long pageID, std::shared_ptr<Node> node) : m_pageID(pageID), m_node(std::move(node)) {}

    CanvasNode *getNode() const {
        return static_cast<CanvasNode *>(m_node.get());
    }


private:
    long m_pageID;
    std::shared_ptr<Node> m_node;
};

}

#endif //ATOMGRAPHICS_CANVASNODEJNI_H

//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

/**
 * GraphicsPage定义为一个Graphics的最外层容器, 也是GraphicsLayer的容器
 */

#ifndef ATOMGRAPHICS_GRAPHICSPAGE_H
#define ATOMGRAPHICS_GRAPHICSPAGE_H

#include <set>
#include <string>
#include "GraphicsLayer.h"
#include "GraphicsContentFlushController.h"
#include "GraphicsPageContext.h"

namespace AtomGraphics {

    class GraphicsContext;

    class GraphicsPageContext;

    class GraphicsLayer;

    class GraphicsPage {

    public:

        GraphicsPage(GraphicsPageContext *context, GraphicsLayer *rootLayer);

        virtual ~GraphicsPage();

        long pageID() const;

        /**
         * 通知更新
         */
        void schedulePageFlush();

        /**
         * Graphics内容刷新结束
         */
        void didUpdate();

        /**
         * 准备即将更新用的内容
         */
        void buildPendingFlushContexts();


        GraphicsContext *takePendingFlushContext();

        /**
         * 通知layer通过backingStore更新layer状态
         * 对CALayer来说，这个一步就是layer.contents = CGImage
         */
        void updateLayerContents();

        GraphicsLayer *rootLayer() const;

        void release();

    private:
        GraphicsPageContext *m_pageContext;
        long m_pageID;
        bool m_needsFlush{false};
        bool m_pendingToFlush{false};
        bool m_updating{false};
        GraphicsLayer *m_rootLayer{nullptr};

        friend class GraphicsContentFlushController;
    };
}


#endif //ATOMGRAPHICS_GRAPHICSPAGE_H

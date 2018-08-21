//
// Created by neo on 2018/7/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_DISPLAYLINKHANDLERCA_H
#define ATOMGRAPHICS_DISPLAYLINKHANDLERCA_H

#import "DisplayLink.h"
#import <QuartzCore/CADisplayLink.h>

@class CADisplayLinkHandler;

namespace AtomGraphics {

    class DisplayLinkCA : public DisplayLink {

    public:
        DisplayLinkCA(GraphicsContentFlushController *flushController);

        void schedule() override;

        void pause() override;

    private:
        CADisplayLinkHandler *m_displayLinkHandler;
    };
}


#endif //ATOMGRAPHICS_DISPLAYLINKHANDLERCA_H

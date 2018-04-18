//
// Created by neo on 2018/4/17.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMMESSAGECLIENT_H
#define ATOMGRAPHICS_ATOMMESSAGECLIENT_H

namespace AtomGraphics {

    class MessageReceiver {

    public:
        virtual void didReceiveMessage();
    };
}


#endif //ATOMGRAPHICS_ATOMMESSAGECLIENT_H

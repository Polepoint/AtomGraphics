//
// Created by neo on 2018/4/16.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CONNECTION_H
#define ATOMGRAPHICS_CONNECTION_H

#include <string>
#include <map>
#include <dispatch/dispatch.h>
#import "AtomConnectionMessage.h"

namespace AtomGraphics {

    class Connection {

    public:
        Connection();

        void open();

        void sendMessage(std::string receiverName, ConnectionMessage *message);

        static Connection *SharedConnection();

    private:
        dispatch_queue_t _dispatchQueue;
    };

}

#endif //ATOMGRAPHICS_CONNECTION_H

//
// Created by neo on 2018/4/16.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomConnection.h"


namespace AtomGraphics {

    void Connection::open() {

    }

    void Connection::sendMessage(std::string receiverName, ConnectionMessage *message) {
        dispatch_async(_dispatchQueue, [receiverName, message] {

        });
    }

    Connection::Connection() {
        _dispatchQueue = dispatch_queue_create("neo.AtomGraphics.IPC.Connection", DISPATCH_QUEUE_SERIAL);
    }

    Connection *Connection::SharedConnection() {
        static Connection *sharedConnection = nullptr;

        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            if (sharedConnection) {
                sharedConnection = new Connection();
            }
        });

        return sharedConnection;
    }
}
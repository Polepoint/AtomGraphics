//
// Created by neo on 2018/4/17.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMCONNECTIONMESSAGE_H
#define ATOMGRAPHICS_ATOMCONNECTIONMESSAGE_H


#import <experimental/string>

namespace AtomGraphics {

    class ConnectionMessage {

    public:
        const std::string &getMessageName() const;

    private:

        std::string _name;

    };
}


#endif //ATOMGRAPHICS_ATOMCONNECTIONMESSAGE_H

//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMTIMERBASE_H
#define ATOMGRAPHICS_ATOMTIMERBASE_H


namespace AtomGraphics {

    class TimerBase {

    public:

        virtual void startOneShot(double delay) {
        };

        virtual void stop(double delay) {
        };

        virtual void stop() {
        };

        virtual void restart() {
        };

        virtual bool isActive() {
            return false;
        };

    };
}


#endif //ATOMGRAPHICS_ATOMTIMERBASE_H

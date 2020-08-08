//
// Created by neo on 2018/8/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TEXTMETRICS_H
#define ATOMGRAPHICS_TEXTMETRICS_H


namespace AtomGraphics {

class TextMetrics {

public:

    float getWidth() const;

    void setWidth(float width);

private:
    float m_width{0};
};

}


#endif //ATOMGRAPHICS_TEXTMETRICS_H

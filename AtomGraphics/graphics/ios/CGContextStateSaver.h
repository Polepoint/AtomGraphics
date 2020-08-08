//
// Created by neo on 2018/12/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CGCONTEXTSTATESAVER_H
#define ATOMGRAPHICS_CGCONTEXTSTATESAVER_H


#include <CoreGraphics/CoreGraphics.h>

namespace AtomGraphics {

class CGContextStateSaver {

public:
    
    CGContextStateSaver(CGContextRef context, bool saveAndRestore = true)
            : m_context(context), m_saveAndRestore(saveAndRestore) {
        if (m_saveAndRestore)
            CGContextSaveGState(m_context);
    }
    
    ~CGContextStateSaver() {
        if (m_saveAndRestore)
            CGContextRestoreGState(m_context);
    }
    
    void save() {
        CGContextSaveGState(m_context);
        m_saveAndRestore = true;
    }
    
    void restore() {
        CGContextRestoreGState(m_context);
        m_saveAndRestore = false;
    }
    
    bool didSave() const {
        return m_saveAndRestore;
    }

private:
    CGContextRef m_context;
    bool m_saveAndRestore;
};

}


#endif //ATOMGRAPHICS_CGCONTEXTSTATESAVER_H

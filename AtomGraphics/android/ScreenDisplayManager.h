//
// Created by neo on 2019/3/13.
//

#ifndef ATOMGRAPHICS_WINDOWDISPLAYMANAGER_H
#define ATOMGRAPHICS_WINDOWDISPLAYMANAGER_H

#include <vector>
#include "ScreenDisplay.h"

namespace AtomGraphics {

class ScreenDisplayManager {

public:

    using ScreenDisplayList = std::vector<ScreenDisplay>;

    static ScreenDisplayManager *Singleton();

    void addDisplay(const ScreenDisplay &display);

    void removeDisplay(int64_t displayId);

    void updateDisplay(int sdkDisplayId,
                       int width, int height, float dipScale,
                       int bitsPerPixel, int bitsPerComponent);

    ScreenDisplay getPrimaryDisplay();

private:

    std::vector<ScreenDisplay> m_displayList;

    ScreenDisplayManager() {}

    ScreenDisplayList::iterator getDisplayByID(int64_t display_id);

};

}


#endif //ATOMGRAPHICS_WINDOWDISPLAYMANAGER_H

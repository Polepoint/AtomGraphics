//
// Created by neo on 2019/2/13.
//

#ifndef ATOMGRAPHICS_COMPOSITINGVIEW_H
#define ATOMGRAPHICS_COMPOSITINGVIEW_H

#include "graphics/TransactionProperties.h"
#include "graphics/android/ResourceProvider.h"
#include "math/IntRect.h"
#include <vector>

namespace AtomGraphics {

class CompositingView {

public:

    CompositingView(long viewID);

    long viewID() const {
        return m_viewID;
    };

    void setBounds(const IntRect &bounds);

    void setPosition(const FloatPoint &position);

    void setContentsScale(float contentsScale);

    void displayImage(sk_sp<SkImage> image);

    void addSubview(std::shared_ptr<CompositingView> subview);

    void removeSubview(CompositingView *subview);

    void removeFromSuperView();

    CompositingView *superview() const {
        return m_superview;
    }

    const std::vector<std::shared_ptr<CompositingView>> &subviews() const {
        return m_subviews;
    };

    void commitImage(SkCanvas *canvas, const SkMatrix &matrix, bool *needSwapBuffers);

private:
    long m_viewID;

    FloatRect m_bounds;
    FloatPoint m_position;
    float m_contentsScale{1};

    sk_sp<SkImage> m_displayingImage;

    std::vector<std::shared_ptr<CompositingView>> m_subviews;
    CompositingView *m_superview{nullptr};
};

}


#endif //ATOMGRAPHICS_COMPOSITINGVIEW_H

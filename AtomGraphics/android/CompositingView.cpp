//
// Created by neo on 2019/2/13.
//

#include "graphics/android/ResourceProvider.h"
#include "CompositingView.h"

namespace AtomGraphics {

CompositingView::CompositingView(long viewID)
        : m_viewID(viewID) {

}

void CompositingView::setBounds(const AtomGraphics::IntRect &bounds) {
    if (m_bounds != bounds) {
        m_bounds = bounds;
    }
}

void CompositingView::setPosition(const FloatPoint &position) {
    m_position = position;
}

void CompositingView::setContentsScale(float contentsScale) {
    m_contentsScale = contentsScale;
}

void CompositingView::displayImage(sk_sp<SkImage> image) {
    m_displayingImage = image;
}

void CompositingView::addSubview(std::shared_ptr<CompositingView> subview) {
    auto it = std::find(m_subviews.begin(), m_subviews.end(), subview);
    if (it != m_subviews.end()) {
        return;
    }

    subview.get()->m_superview = this;
    m_subviews.push_back(subview);
}

void CompositingView::removeSubview(CompositingView *subview) {
    auto it = m_subviews.begin();
    for (; it != m_subviews.end(); it++) {
        if ((*it).get() == subview)break;
    }

    if (it == m_subviews.end()) {
        return;
    }

    subview->m_superview = nullptr;
    m_subviews.erase(it);
}

void CompositingView::removeFromSuperView() {
    if (m_superview) {
        m_superview->removeSubview(this);
    }
}

void CompositingView::commitImage(SkCanvas *canvas, const SkMatrix &matrix, bool *needSwapBuffers) {
    if (m_displayingImage) {
        canvas->save();
        float x = matrix.getTranslateX();
        float y = matrix.getTranslateY();
        canvas->translate(x, y);
        SkPaint paint;
        paint.setBlendMode(SkBlendMode::kSrc);
        canvas->drawImageRect(m_displayingImage, m_displayingImage->bounds(), m_bounds, &paint);
        canvas->restore();
        m_displayingImage.reset();
        *needSwapBuffers = true;
    }

    if (m_subviews.size()) {
        for (auto subview: m_subviews) {
            SkMatrix m = matrix;
            FloatPoint position = subview->m_position;
            m.setTranslate(position.x, position.y);
            subview->commitImage(canvas, m, needSwapBuffers);
        }
    }

}

}

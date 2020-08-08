#import "Gradient.h"
#import "ColorCG.h"


namespace AtomGraphics {

void Gradient::platformDestroy() {
    CGGradientRelease(m_gradient);
    m_gradient = nullptr;
}

CGGradientRef Gradient::platformGradient() {
    if (m_gradient)
        return m_gradient;
    
    sortStopsIfNecessary();
    
    unsigned numStops = m_stops.size();
    
    std::vector<CGFloat> locations;
    std::vector<CGFloat> colorComponents;
    
    for (const auto &stop : m_stops) {
        colorComponents.push_back(stop.color.r);
        colorComponents.push_back(stop.color.g);
        colorComponents.push_back(stop.color.b);
        colorComponents.push_back(stop.color.a);
        locations.push_back(stop.offset);
    }
    
    m_gradient = CGGradientCreateWithColorComponents(sRGBColorSpaceRef(), colorComponents.data(), locations.data(), numStops);
    
    return m_gradient;
}

void Gradient::paint(CGContextRef platformContext) {
    CGGradientDrawingOptions extendOptions = kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation;
    CGGradientRef gradient = platformGradient();
    
    if (m_isLinear) {
        CGContextDrawLinearGradient(platformContext, gradient, m_point0, m_point1, extendOptions);
    } else {
        CGContextDrawRadialGradient(platformContext, gradient, m_point0, m_startRadius, m_point1, m_endRadius, extendOptions);
    }
}


}
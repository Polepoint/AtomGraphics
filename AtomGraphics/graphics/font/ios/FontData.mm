//
// Created by neo on 2018/8/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "FontData.h"

namespace AtomGraphics {

void FontData::platformInit() {
    paint_typeface_;
    auto &font = paint_typeface_.CTFont();
    CGFloat capHeight = CTFontGetCapHeight(font.get());
    CGFloat lineGap = CTFontGetLeading(font.get());
    CGFloat ascent = CTFontGetAscent(font.get());
    CGFloat descent = CTFontGetDescent(font.get());
    float lineSpacing = lroundf(ascent) + lroundf(descent) + lroundf(lineGap);

    lineGap = ceilf(lineGap);
    lineSpacing = ceil(ascent) + ceil(descent) + lineGap;
    ascent = ceilf(ascent);
    descent = ceilf(descent);
    CGFloat xHeight = CTFontGetXHeight(font.get());


    metrics_.setAscent(ascent);
    metrics_.setDescent(descent);
    metrics_.setCapHeight(capHeight);
    metrics_.setLineGap(lineGap);
    metrics_.setXHeight(xHeight);
    metrics_.setLineSpacing(lineSpacing);
}

}

//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AtomGCanvasContext2D.h"
#import <GCanvas/GCVFont.h>

namespace AtomGraphics {

    void GCanvasContext2D::bindImageTexture(const std::string &src, int imageId) {
//        [[AtomGraphicsGCanvasModule sharedModule] bindImageTexture:src imageId:imageId comonentId:_canvasNode->_componentId];
    }

    std::string &GCanvasContext2D::getImageDataResult(const std::string &componentId) {
        std::string str = "";
        return str;
    }

    float GCanvasContext2D::measureText(const std::string &text) {
        float width = 0;
        GCVFont *curFont = [GCVFont sharedInstance];
        [curFont resetWithFontStyle:_currentFont.data() isStroke:false deviceRatio:_devicePixelRatio];

        NSString *string = [[NSString alloc] initWithBytes:text.data() length:std::strlen(text.data()) encoding:NSUTF8StringEncoding];
        NSAttributedString *attributes = [[NSAttributedString alloc] initWithString:string attributes:@{(id) kCTFontAttributeName: (__bridge id) curFont.mainFont}];
        width = attributes.size.width;

        return width;
    }

}
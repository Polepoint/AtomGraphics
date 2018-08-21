//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICSVIEW_H
#define ATOMGRAPHICSVIEW_H

#import <UIKit/UIKit.h>
#import "GraphicsPageContext.h"

@interface AtomGraphicsView : UIView {

}

+ (AtomGraphics::GraphicsPageContext *)PageContext;

- (void)reloadGraphics;

@end


#endif //ATOMGRAPHICSVIEW_H

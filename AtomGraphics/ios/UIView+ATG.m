//
// Created by neo on 2020/3/5.
//

#import "UIView+ATG.h"


@implementation UIView (ATG)

- (UIView *)scrollerSuperview {
    UIView *superView = [self superview];
    while (superView && ![superView isKindOfClass:[UIScrollView class]]) {
        superView = [superView superview];
    }

    return superView;
}


@end
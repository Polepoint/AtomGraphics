//
// Created by neo on 2020/5/4.
//

#import "GraphicsConfiguration.h"


@implementation GraphicsConfiguration {

}

+ (instancetype)globalConfiguration {
    static GraphicsConfiguration *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [GraphicsConfiguration new];
    });

    return instance;
}


@end
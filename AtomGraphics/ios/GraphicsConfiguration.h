//
// Created by neo on 2020/5/4.
//

#import <Foundation/Foundation.h>


@interface GraphicsConfiguration : NSObject

@property(atomic, strong) NSURL *baseURL;

+ (instancetype)globalConfiguration;

@end
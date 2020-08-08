//
// Created by Amazing on 2018/12/6.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, AGResizeMode) {
    RCTResizeModeCover = UIViewContentModeScaleAspectFill,
    RCTResizeModeContain = UIViewContentModeScaleAspectFit,
    RCTResizeModeStretch = UIViewContentModeScaleToFill,
    RCTResizeModeCenter = UIViewContentModeCenter,
    RCTResizeModeRepeat = -1, // Use negative values to avoid conflicts with iOS enum values.
};

@interface ImageCache : NSObject

+ (instancetype)sharedSingleton;

- (UIImage *)imageForUrl:(NSString *)url size:(CGSize)size scale:(CGFloat)scale resizeMode:(AGResizeMode)resizeMode responseDate:(NSString *)responseDate;

- (void)addImageToCache:(UIImage *)image URL:(NSString *)url size:(CGSize)size scale:(CGFloat)scale resizeMode:(AGResizeMode)resizeMode responseDate:(NSString *)responseDate;
@end
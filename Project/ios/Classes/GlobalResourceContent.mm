//
// Created by neo on 2019-09-05.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "GlobalResourceContent.h"


@implementation GlobalResourceContent {

}

+ (NSString *)tadpolesJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"tadpoles" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)loadingJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"loading" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)loading2JSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"loading2" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)flowerJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"flower" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)heartJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"heart" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)colorfulTextJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"colorful-text" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}


+ (NSString *)clockJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"clock" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)hanabiJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"hanabi" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)pixelsGraphicJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"pixels-graphic" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}

+ (NSString *)galaxyJSContent {
    static NSString *content;
    if (!content) {
        NSString *chartFile = [[NSBundle mainBundle] pathForResource:@"galaxy" ofType:@"js"];
        content = [NSString stringWithContentsOfFile:chartFile encoding:NSUTF8StringEncoding error:nil];
    }

    return content;
}



@end
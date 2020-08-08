//
// Created by neo on 2019/9/30.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "JSExecutionChecker.h"


@implementation JSExecutionChecker {

}

+ (void)checkFunctionExecutionResult:(JSValue *)result {
#if DEBUG
    if (!result.isUndefined) {
        NSLog(@"AtomGraphics - JS evaluate result: %@", result);
    }
#endif
}


@end
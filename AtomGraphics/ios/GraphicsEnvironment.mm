//
// Created by neo on 2019-09-05.
//

#import "DeviceIOS.h"
#import "GraphicsEnvironment.h"
#import "UIThread.h"


@implementation GraphicsEnvironment {

}

static BOOL sEnvironmentInitialized;

+ (void)initializeEnvironment {
    if (sEnvironmentInitialized) {
        return;
    }

    sEnvironmentInitialized = YES;
    AtomGraphics::UIThread::InitUIThread();
    AtomGraphics::DeviceIOS::InitializePlatform();
}

@end
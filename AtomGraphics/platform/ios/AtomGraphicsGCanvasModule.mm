//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsGCanvasModule.h"
#import "GCanvasModule.h"
#import "AtomGraphicsGCanvasView.h"
#import "AtomGraphicsGCanvasComponent.h"

static AtomGraphicsGCanvasModule *sharedModuleInstance;

@interface AtomGraphicsGCanvasModule () <GCanvasModuleProtocol, GCVImageLoaderProtocol>

@property(nonatomic, strong) GCanvasModule *gcanvasModule;

@property(nonatomic, strong) NSMutableDictionary<NSString *, AtomGraphicsGCanvasComponent *> *registeredCanvasComponents;

@end

@implementation AtomGraphicsGCanvasModule {
    dispatch_queue_t _commitQueue;
}

+ (AtomGraphicsGCanvasModule *)sharedModule {
    if (!sharedModuleInstance) {
        sharedModuleInstance = [AtomGraphicsGCanvasModule new];
    }
    return sharedModuleInstance;
}


- (instancetype)init {
    self = [super init];
    if (self) {
        self.gcanvasModule = [[GCanvasModule alloc] initWithDelegate:self];
        self.gcanvasModule.imageLoader = self;
        _registeredCanvasComponents = [NSMutableDictionary dictionary];
        _commitQueue = dispatch_queue_create("neo.AtomGraphics.CommitCommand", nullptr);
    }

    return self;
}

- (void)registerCanvasView:(AtomGraphicsGCanvasView *)gCanvasView {
    NSString *componentID = gCanvasView.componentId;
    _registeredCanvasComponents[componentID] = [[AtomGraphicsGCanvasComponent alloc] initWithCanvasView:gCanvasView];
    [self.gcanvasModule enable:@{@"componentId": componentID}];
}

- (void)unregisterCanvasView:(NSString *)componentID {
    [_registeredCanvasComponents removeObjectForKey:componentID];
}

- (AtomGraphicsGCanvasView *)getCanvasView:(NSString *)componentID {
    return [_registeredCanvasComponents[componentID] gCanvasView];
}

- (void)addCommand:(NSString *)command componentID:(NSString *)componentID {
    AtomGraphicsGCanvasComponent *component = _registeredCanvasComponents[componentID];
    NSMutableArray *renderCommands = component.renderCommands;
    [renderCommands addObject:command];
    [self flushCountDownAtComponent:component];
}

- (void)flushCountDownAtComponent:(AtomGraphicsGCanvasComponent *)component {
    if (component.isCountingDown) {
        return;
    }

    component.isCountingDown = YES;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_MSEC * 100), _commitQueue, [component] {
        component.isCountingDown = NO;
        if (component.renderCommands.count) {
            NSMutableArray *commands = [component renderCommands];
            NSString *commandLine = [commands componentsJoinedByString:@";"];
            [commands removeAllObjects];
            [[AtomGraphicsGCanvasModule sharedModule].gcanvasModule render:commandLine componentId:component.componentID];
        }
    });
}

- (void)setContextType:(NSUInteger)type componentID:(NSString *)componentID {
    [self.gcanvasModule setContextType:type componentId:componentID];
}

#pragma mark - GCanvasModuleProtocol Methods


- (NSString *)gcanvasModuleInstanceId {
    return [NSString stringWithFormat:@"%p", self];
}

- (id <GCanvasViewProtocol>)gcanvasComponentById:(NSString *)componentId {
    return [_registeredCanvasComponents[componentId] gCanvasView];
}

- (void)dispatchGlobalEvent:(NSString *)event params:(NSDictionary *)params {

}

#pragma mark - GCVImageLoaderProtocol Methods

- (void)loadImage:(NSURL *)url completed:(GCVLoadImageCompletion)completion {
    NSURLSession *session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:[NSURLRequest requestWithURL:url] completionHandler:^(NSData *_Nullable data, NSURLResponse *_Nullable response, NSError *_Nullable error) {
        completion([UIImage imageWithData:data], error, YES, url);
    }];

    [task resume];
}


@end

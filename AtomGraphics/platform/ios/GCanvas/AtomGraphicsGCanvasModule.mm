//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsGCanvasModule.h"
#import "AtomGraphicsGCanvasView.h"
#import "AtomGCanvasLayerBackingStore.h"
#import "AtomContentFlushController.h"

using namespace AtomGraphics;

static AtomGraphicsGCanvasModule *sharedModuleInstance;

@interface AtomGraphicsGCanvasModule () <GCanvasModuleProtocol, GCVImageLoaderProtocol>

@end

@implementation AtomGraphicsGCanvasModule {
    AtomContentFlushController *_flushController;
    std::map<std::string, AtomGCanvasLayerBackingStore *> _registeredBackingStore;
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
        _flushController = new AtomContentFlushController();
    }

    return self;
}

- (void)registerCanvasView:(AtomGraphicsGCanvasView *)gCanvasView {
    NSString *componentID = gCanvasView.componentId;
    _registeredBackingStore[componentID.cString] = new AtomGCanvasLayerBackingStore(gCanvasView);
    [self.gcanvasModule enable:@{@"componentId": componentID}];
}

- (void)unregisterCanvasView:(NSString *)componentID {
    _registeredBackingStore.erase(componentID.cString);
}

- (AtomGraphicsGCanvasView *)getCanvasView:(const std::string &)componentID {
    AtomGCanvasLayerBackingStore *backingStore = _registeredBackingStore[componentID];
    return backingStore->getGCanvasView();
}

- (void)addCommand:(const std::string &)command componentID:(const std::string &)componentID {
    AtomGCanvasLayerBackingStore *backingStore = _registeredBackingStore[componentID];
    if (backingStore->addCommand(command)) {
        _flushController->setBackingStoreToFlush(backingStore);
        _flushController->scheduleLayerFlush();
    }
}

- (void)setContextType:(NSUInteger)type componentID:(NSString *)componentID {
    [self.gcanvasModule setContextType:type componentId:componentID];
}

#pragma mark - GCanvasModuleProtocol Methods


- (NSString *)gcanvasModuleInstanceId {
    return [NSString stringWithFormat:@"%p", self];
}

- (id <GCanvasViewProtocol>)gcanvasComponentById:(NSString *)componentId {
    return _registeredBackingStore[componentId.cString]->getGCanvasView();
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

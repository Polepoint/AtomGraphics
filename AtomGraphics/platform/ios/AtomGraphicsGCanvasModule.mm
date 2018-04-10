//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsGCanvasModule.h"
#import "GCanvasModule.h"
#import "AtomGraphicsGCanvasView.h"

static AtomGraphicsGCanvasModule *sharedModuleInstance;

@interface AtomGraphicsGCanvasModule () <GCanvasModuleProtocol, GCVImageLoaderProtocol>

@property(nonatomic, strong) GCanvasModule *gcanvasModule;

@property(nonatomic, strong) NSMutableDictionary *registeredCanvasViews;

@property(nonatomic, strong) NSMutableDictionary<NSString *, NSMutableArray *> *renderCommands;

@end

@implementation AtomGraphicsGCanvasModule {

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
        _registeredCanvasViews = [NSMutableDictionary dictionary];
        _renderCommands = [NSMutableDictionary dictionary];
    }

    return self;
}

- (void)registerCanvasView:(AtomGraphicsGCanvasView *)gCanvasView {
    NSString *componentID = gCanvasView.componentId;
    _registeredCanvasViews[componentID] = gCanvasView;
    _renderCommands[componentID] = [NSMutableArray array];
    [self.gcanvasModule enable:@{@"componentId": componentID}];
}

- (void)unregisterCanvasView:(NSString *)componentID {
    [_registeredCanvasViews removeObjectForKey:componentID];
    [_renderCommands removeObjectForKey:componentID];
}

- (AtomGraphicsGCanvasView *)getCanvasView:(NSString *)componentID {
    return _registeredCanvasViews[componentID];
}

- (void)addCommand:(NSString *)command componentID:(NSString *)componentID {
    NSMutableArray *renderCommands = _renderCommands[componentID];
    [renderCommands addObject:command];
}

- (void)setContextType:(NSUInteger)type componentID:(NSString *)componentID {
    [self.gcanvasModule setContextType:type componentId:componentID];
}

#pragma mark - GCanvasModuleProtocol Methods


- (NSString *)gcanvasModuleInstanceId {
    return [NSString stringWithFormat:@"%p", self];
}

- (id <GCanvasViewProtocol>)gcanvasComponentById:(NSString *)componentId {
    return _registeredCanvasViews[componentId];
}

- (void)dispatchGlobalEvent:(NSString *)event params:(NSDictionary *)params {
    NSString *componentID = params[@"ref"];
    if (componentID) {
        NSMutableArray *commands = _renderCommands[componentID];
        NSString *commandLine = [commands componentsJoinedByString:@";"];
        [commands removeAllObjects];
        [self.gcanvasModule render:commandLine componentId:componentID];
    }

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

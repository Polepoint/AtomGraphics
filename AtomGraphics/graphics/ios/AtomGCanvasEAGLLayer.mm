//
// Created by neo on 2018/7/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGCanvasEAGLLayer.h"

typedef enum {
    LOG_LEVEL_VERBOSE = -1,
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

static long existContextCount = 0;
static EAGLSharegroup *sharegroup;

@interface AtomGCanvasEAGLLayer ()

@property(nonatomic, copy) NSString *componentID;
@property(nonatomic, strong) EAGLContext *context;
@property(nonatomic, assign) BOOL enterBackground;

@end

@implementation AtomGCanvasEAGLLayer {
    BOOL _glEnvironmentInitialized;
    GLuint _framebuffer;
    GLuint _colorRenderbuffer;
}

+ (void)initialize {
    [super initialize];
#if DEBUG
    [GCanvasPlugin setLogLevel:LOG_LEVEL_DEBUG];
#else
    [GCanvasPlugin setLogLevel:LOG_LEVEL_VERBOSE];
#endif

}


- (instancetype)init {
    self = [super init];
    if (self) {
        self.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking: @NO, kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8};
        self.opaque = NO;
        NSNull *nullValue = [NSNull null];
        self.style = @{
                @"actions": @{
                        @"anchorPoint": nullValue,
                        @"anchorPointZ": nullValue,
                        @"backgroundColor": nullValue,
                        @"borderColor": nullValue,
                        @"borderWidth": nullValue,
                        @"bounds": nullValue,
                        @"contents": nullValue,
                        @"contentsRect": nullValue,
                        @"contentsScale": nullValue,
                        @"cornerRadius": nullValue,
                        @"opacity": nullValue,
                        @"position": nullValue,
                        @"shadowColor": nullValue,
                        @"sublayerTransform": nullValue,
                        @"sublayers": nullValue,
                        @"transform": nullValue,
                        @"zPosition": nullValue
                }
        };

        _componentID = [[NSUUID UUID] UUIDString];
        _plugin = [[GCanvasPlugin alloc] initWithComponentId:_componentID];
        [_plugin setContextType:GCVContextType2D];
        [self initGLContext];
        [self initNotificationObserver];
    }

    return self;
}

- (void)initGLContext {
    if (sharegroup) {
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:sharegroup];
    } else {
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        sharegroup = _context.sharegroup;
    }
    _context.multiThreaded = YES;
    existContextCount++;
}

- (void)initGLEnvironment {

    if (_glEnvironmentInitialized) {
        return;
    }
    [EAGLContext setCurrentContext:_context];
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glGenRenderbuffers(1, &_colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);
    if (![_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:self]) {
        NSLog(@"failed to call context");
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        _glEnvironmentInitialized = NO;
        return;
    } else {
        _glEnvironmentInitialized = YES;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    [self resetPluginBuffer];
}

- (void)dealloc {
    existContextCount--;
    if (existContextCount == 0) {
        sharegroup = nil;
    }
    [_plugin removeGCanvas];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)setContextType:(GCVContextType)type {
    [_plugin setContextType:type];
}


- (void)renderCommand:(NSString *)command {
    if (![command length]) {
        return;
    }

    [_plugin addCommands:command];

    if (_enterBackground) {
        return;
    }
    __weak __typeof(self) weakSelf = self;
    void (^block)() = ^{
        [weakSelf setNeedsDisplay];
    };
    if ([[NSThread currentThread] isMainThread]) {
        block();
    } else {
        dispatch_async(dispatch_get_main_queue(), block);
    }
}

- (void)display {
    [super display];
    if (!_glEnvironmentInitialized) {
        [self initGLEnvironment];
    }
    [EAGLContext setCurrentContext:_context];
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);
    }

#ifdef DEBUG
    GLenum errorInfo = glGetError();
    if (errorInfo != GL_NO_ERROR) {
        NSLog(@"begin OpenGL error: 0x%x", errorInfo);
    }
#endif

    [_plugin execCommands];

    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)resetPluginBuffer {
    if (!_glEnvironmentInitialized) {
        [self initGLEnvironment];
    }

    if (!_glEnvironmentInitialized) {
        return;
    }

    CGFloat devicePixelRatio = self.contentsScale;
    [_plugin setDevicePixelRatio:devicePixelRatio];

    CGRect frame = self.frame;
    [_plugin setFrame:CGRectMake(frame.origin.x, frame.origin.y,
            frame.size.width * devicePixelRatio, frame.size.height * devicePixelRatio)];
    [_plugin setClearColor:self.backgroundColor ? [UIColor colorWithCGColor:self.backgroundColor] : [UIColor clearColor]];
}

- (void)resizeLayerBuffer {
    if (!_glEnvironmentInitialized) {
        return;
    }

    [EAGLContext setCurrentContext:_context];
    glDeleteFramebuffers(1, &_framebuffer);
    glDeleteRenderbuffers(1, &_colorRenderbuffer);
    _glEnvironmentInitialized = NO;
}


#pragma mark - Notification

- (void)initNotificationObserver {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onDidEnterBackgroundNotify)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onWillEnterForegroundNotify)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
}


- (void)onDidEnterBackgroundNotify {
    //TODO enter background
    _enterBackground = YES;
}


- (void)onWillEnterForegroundNotify {
    //TODO enter foreground
    _enterBackground = NO;
}

@end
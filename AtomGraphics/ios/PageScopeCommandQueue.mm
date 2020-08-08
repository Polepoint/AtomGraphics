//
// Created by neo on 2018/12/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "PageScopeCommandQueue.h"
#import "JSPageScope.h"

@interface Command : NSObject

@property(nonatomic, assign) CommandType type;

@property(nonatomic, strong) id data1;

@property(nonatomic, strong) id data2;

+ (instancetype)commandWithType:(CommandType)type data1:(id)data1 data2:(id)data2;

@end

@implementation Command


+ (instancetype)commandWithType:(CommandType)type data1:(id)data1 data2:(id)data2 {
    Command *command = [self new];
    command.type = type;
    command.data1 = data1;
    command.data2 = data2;
    return command;
}


@end

@interface PageScopeCommandQueue ()

@property(nonatomic, strong) NSMutableArray *queue;

@end


@implementation PageScopeCommandQueue {

}

- (instancetype)init {
    self = [super init];
    if (self) {
        _queue = [NSMutableArray array];
    }

    return self;
}


- (void)addCommand:(CommandType)type data1:(id)data1 data2:(id)data2 {
    [_queue addObject:[Command commandWithType:type data1:data1 data2:data2]];
}

- (void)flushCommandToPageScope:(JSPageScope *)scope {
    NSArray *pendingFlushCommand = _queue.copy;
    [_queue removeAllObjects];
    for (Command *command in pendingFlushCommand) {
        switch (command.type) {
            case EvaluateScript:
                [scope evaluateScript:command.data1];
                break;
            case CallJSHandler:
                [scope callJSHandler:command.data1 parameters:command.data2];
                break;
            case RegisterNativeHandler:
                [scope registerNativeHandler:command.data1 handlerCallback:command.data2];
                break;
            default:
                break;
        }
    }
}

- (BOOL)empty {
    return _queue.count == 0;
}

- (void)clear {

}

@end
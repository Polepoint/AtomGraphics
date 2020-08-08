//
// Created by Amazing on 2018/12/6.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "ImageCache.h"
#import <CommonCrypto/CommonDigest.h>

static const NSString *dictoryName = @"atomgraphics-image-cache";

static const NSUInteger FCTMaxCachableDecodedImageSizeInBytes = 1048576 * 30; // 30MB

static id _instance = nil;

@interface ImageCache ()

@property(nonatomic, strong) NSCache *decodedImageCache;

@end

@implementation ImageCache {
}


+ (instancetype)sharedSingleton {
    return [[self alloc] init];
}

+ (instancetype)allocWithZone:(struct _NSZone *)zone {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [super allocWithZone:zone];
    });
    return _instance;
}

- (instancetype)init {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [super init];
    });
    return _instance;
}

//copy在底层 会调用copyWithZone:
- (id)copyWithZone:(NSZone *)zone {
    return _instance;
}

+ (id)copyWithZone:(struct _NSZone *)zone {
    return _instance;
}

+ (id)mutableCopyWithZone:(struct _NSZone *)zone {
    return _instance;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)clearCache {
    [_decodedImageCache removeAllObjects];
}

- (NSCache *)decodedImageCache {
    if (!_decodedImageCache) {
        _decodedImageCache = [NSCache new];
        _decodedImageCache.totalCostLimit = 200 * 1024 * 1024; // 200MB
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(clearCache)
                                                     name:UIApplicationDidReceiveMemoryWarningNotification
                                                   object:nil];
    }
    return _decodedImageCache;
}

- (void)addImageToCache:(UIImage *)image
                 forKey:(NSString *)cacheKey {
    if (!image) {
        return;
    }
    [self cacheImage:image toTempCacheWithCacheKey:cacheKey];
    [self cacheImage:image toDiskCacheWithCacheKey:cacheKey];
}

- (void)cacheImage:(UIImage *)image toTempCacheWithCacheKey:(NSString *)cacheKey {
    CGFloat bytes = image.size.width * image.size.height * image.scale * image.scale * 4;
    [self.decodedImageCache setObject:image
                               forKey:cacheKey
                                 cost:bytes];
}

- (void)cacheImage:(UIImage *)image toDiskCacheWithCacheKey:(NSString *)cacheKey {
    CGFloat bytes = image.size.width * image.size.height * image.scale * image.scale * 4;
    //单个图片不能超过30M
    if (bytes <= FCTMaxCachableDecodedImageSizeInBytes) {
        NSString *imagePath = [[self getCachesDirPath] stringByAppendingPathComponent:cacheKey];
        [UIImagePNGRepresentation(image) writeToFile:imagePath atomically:YES];
    }
}


- (UIImage *)imageForUrl:(NSString *)url
                    size:(CGSize)size
                   scale:(CGFloat)scale
              resizeMode:(AGResizeMode)resizeMode
            responseDate:(NSString *)responseDate {
    NSString *cacheKey = [self getMOD5KeyWithOriginalKey:url];
    NSString *imagePath = [[self getCachesDirPath] stringByAppendingPathComponent:cacheKey];
    if ([self.decodedImageCache objectForKey:cacheKey]) {
        return [self.decodedImageCache objectForKey:cacheKey];
    }
    //磁盘缓存在读出恢复成image对象时，需要注意scale
    UIImage *diskImage = [UIImage imageWithData:[NSData dataWithContentsOfFile:imagePath] scale:scale];
    if (diskImage) {
        [self cacheImage:diskImage toTempCacheWithCacheKey:cacheKey];
    }
    return diskImage;
}

- (void)addImageToCache:(UIImage *)image
                    URL:(NSString *)url
                   size:(CGSize)size
                  scale:(CGFloat)scale
             resizeMode:(AGResizeMode)resizeMode
           responseDate:(NSString *)responseDate {
    NSString *cacheKey = [self getMOD5KeyWithOriginalKey:url];
    return [self addImageToCache:image forKey:cacheKey];
}

- (NSString *)getMOD5KeyWithOriginalKey:(NSString *)originalKey {
    return [[self class] getMD5WithString:originalKey];
}

- (NSString *)getCachesDirPath {
    NSString *cachesDir = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) firstObject];
    cachesDir = [cachesDir stringByAppendingPathComponent:const_cast<NSString *>(dictoryName)];
    [self checkAndSetDirectoryValid:cachesDir];
    return cachesDir;
}

- (void)checkAndSetDirectoryValid:(NSString *)dirctoryRoot {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDir = NO;
    BOOL existed = [fileManager fileExistsAtPath:dirctoryRoot isDirectory:&isDir];
    if (!(isDir && existed)) {
        [fileManager createDirectoryAtPath:dirctoryRoot withIntermediateDirectories:YES attributes:nil error:nil];
    }
}

+ (NSString *)getMD5WithString:(NSString *)string {
    const char *original_str = [string UTF8String];
    unsigned char digist[CC_MD5_DIGEST_LENGTH]; //CC_MD5_DIGEST_LENGTH = 16
    CC_MD5(original_str, (uint) strlen(original_str), digist);
    NSMutableString *outPutStr = [NSMutableString stringWithCapacity:10];
    for (int i = 0; i < CC_MD5_DIGEST_LENGTH; i++) {
        [outPutStr appendFormat:@"%02x", digist[i]];//小写x表示输出的是小写MD5，大写X表示输出的是大写MD5
    }
    return [outPutStr lowercaseString];
}

@end
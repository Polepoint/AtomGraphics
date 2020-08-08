//
// Created by neo on 2020/4/26.
//

#import <cstring>
#import "WebResourceLoader.h"
#import "GraphicsThread.h"
#import "ResourceRequest.h"
#import "GraphicsConfiguration.h"
#import "ImageBuffer.h"

namespace AtomGraphics {

void WebResourceLoader::loadResource(ResourceRequest &request, std::function<void(scoped_refptr<SharedBuffer>, float, float, ImageBitmapConfiguration)> completeCallback) {

    NSURLSession *session = [NSURLSession sharedSession];
    NSString *urlStr = [NSString stringWithUTF8String:request.url().c_str()];
    NSURL *url = [NSURL URLWithString:urlStr];
    if (!url.host) {
        url = [NSURL URLWithString:urlStr relativeToURL:[GraphicsConfiguration globalConfiguration].baseURL];
    }
    NSURLSessionDataTask *task = [session dataTaskWithURL:url completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        bool failed = error != nullptr;
        scoped_refptr<SharedBuffer> sharedBuffer;
        float imageWidth = 0, imageHeight = 0;
        ImageBitmapConfiguration configuration;

        if (!failed && [response.MIMEType hasPrefix:@"image"]) {
            UIImage *image = [UIImage imageWithData:data];
            if (image) {
                CGImageRef imageRef = image.CGImage;
                imageWidth = CGImageGetWidth(imageRef);
                imageHeight = CGImageGetHeight(imageRef);
                configuration.colorSpace = CGImageGetColorSpace(imageRef);
                configuration.bitmapInfo = CGImageGetBitmapInfo(imageRef);
                CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(imageRef));
                const void *buffer = CFDataGetBytePtr(imageData);
                CFIndex bufferLength = CFDataGetLength(imageData);
                sharedBuffer = MakeRefCounted<SharedBuffer>();
                sharedBuffer->copyData(buffer, static_cast<unsigned int>(bufferLength));
                CFRelease(imageData);
            }
        }

        GraphicsThread::DispatchOnGraphicsQueue([completeCallback, sharedBuffer, imageWidth, imageHeight, configuration]() {
            completeCallback(sharedBuffer, imageWidth, imageHeight, configuration);
        });
    }];

    [task resume];
}

}

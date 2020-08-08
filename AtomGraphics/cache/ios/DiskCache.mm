//
// Created by neo on 2020/04/28.
//

#import "DiskCache.h"

namespace AtomGraphics {

const char *DiskCache::kCacheFolderName = "atg_cache";

static NSString *sCacheDirectoryPath;

void DiskCache::checkCachePath() {
    if (!m_cachePathInitialized) {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        sCacheDirectoryPath = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) firstObject];
        sCacheDirectoryPath = [sCacheDirectoryPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%s", kCacheFolderName]];
        BOOL isDir = NO;
        BOOL existed = [fileManager fileExistsAtPath:sCacheDirectoryPath isDirectory:&isDir];
        if (existed) {
            NSError *error;
            [fileManager removeItemAtPath:sCacheDirectoryPath error:&error];
            if (error) {
                //TODO: log error
                return;
            }
        }

        [fileManager createDirectoryAtPath:sCacheDirectoryPath withIntermediateDirectories:YES attributes:nil error:nil];
        m_cachePathInitialized = true;
    }
}

scoped_refptr<SharedBuffer> DiskCache::dataBufferFromFile(const char *file) {
    checkCachePath();

    NSData *data = [[NSFileManager defaultManager] contentsAtPath:[NSString stringWithFormat:@"%@/%s.data", sCacheDirectoryPath, file]];
    scoped_refptr<SharedBuffer> buffer;
    if (data.length) {
        buffer = MakeRefCounted<SharedBuffer>();
        buffer->copyData(data.bytes, static_cast<unsigned int>(data.length));
    }

    return buffer;
}

void DiskCache::saveBufferDataToFile(const char *file, scoped_refptr<SharedBuffer> bufferData) {
    if (!bufferData || !bufferData->bufferSize()) {
        return;
    }

    checkCachePath();

    NSData *data = [NSData dataWithBytes:bufferData->data() length:bufferData->bufferSize()];
    [[NSFileManager defaultManager] createFileAtPath:[NSString stringWithFormat:@"%@/%s.data", sCacheDirectoryPath, file]
                                            contents:data
                                          attributes:nil];
}

}
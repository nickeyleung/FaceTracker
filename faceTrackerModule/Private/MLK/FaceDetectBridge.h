//
//  FaceDetectBridge.h
//  ffmpeg5
//
//  Created by Leung on 2025/3/9.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "MLKitFaceDetection/MLKFaceDetector.h"

NS_ASSUME_NONNULL_BEGIN

@interface FaceDetectBridge : NSObject
+ (void)faceDetectWithSampleBuffer:(CMSampleBufferRef)sample complete: (void (^)(NSArray<MLKFace *> *))complete;
@end

NS_ASSUME_NONNULL_END

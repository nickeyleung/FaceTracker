//
//  FaceDetectBridge.m
//  ffmpeg5
//
//  Created by Leung on 2025/3/9.
//

#import "FaceDetectBridge.h"
#import "Header.h"

@implementation FaceDetectBridge

+ (void)faceDetectWithSampleBuffer:(CMSampleBufferRef)sample complete:(nonnull void (^)(NSArray<MLKFace *> * _Nonnull))complete {
    
    [FaceDetecteManager detectFaceWithSampleBuffer:sample cameraPosition:AVCaptureDevicePositionFront complete:^(NSArray<MLKFace *> * _Nonnull faces) {
            complete(faces);
        }];
}
@end

//
//  FaceTrackerManager.h
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

#import "AVFoundation/AVFoundation.h"
#import <Foundation/Foundation.h>
#import <MLKitFaceDetection/MLKFace.h>

NS_ASSUME_NONNULL_BEGIN

@interface FaceTrackerManager : NSObject

+(id)shared;

- (void)updateStickerImagePath:(NSString *)path;
-(void)detectFace:(CAEAGLLayer *)displayLayer;
- (void)captureOutput:(AVCaptureOutput *)output didOutput:(CMSampleBufferRef)sampleBuffer from:(AVCaptureConnection *)connection;
-(void)updateFace:(MLKFace *)face;
@end

NS_ASSUME_NONNULL_END

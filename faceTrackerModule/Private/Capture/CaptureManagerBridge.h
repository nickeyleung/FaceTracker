//
//  CaptureManagerBridge.h
//  ffmpeg5
//
//  Created by Leung on 2024/9/20.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVCaptureDevice.h>
#import "faceTrackerModule-Swift.h"

NS_ASSUME_NONNULL_BEGIN

@class AVCaptureOutput;
@class AVCaptureConnection;

@protocol CaptureOutputLogic <NSObject>

- (void)captureOutput:(AVCaptureOutput *)output didOutput:(CMSampleBufferRef)sampleBuffer from:(AVCaptureConnection *)connection;

@end

@interface CaptureManagerBridge : NSObject

@property (nonatomic, weak) id<CaptureOutputLogic> delegate;
-(void)setup:(UIView * _Nullable)displayView;
-(void)startCapture;
-(void)stopCapture;
-(AVCaptureDevicePosition)getposition;
-(void)createSamples:(EAGLContext *)context imageBuffer:(CVPixelBufferRef)imageBuffer complete:(void (^)(CVOpenGLESTextureRef, CVOpenGLESTextureRef))completion;
@end

NS_ASSUME_NONNULL_END

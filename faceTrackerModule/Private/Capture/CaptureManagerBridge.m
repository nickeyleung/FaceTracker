//
//  CaptureManagerBridge.m
//  ffmpeg5
//
//  Created by Leung on 2024/9/20.
//

#import "CaptureManagerBridge.h"
//#import "faceTrackerModule-Swift.h"

@interface CaptureManagerBridge() <CaptureLogic>
@property (nonatomic, strong) CaptureManager *capture;
@end

@implementation CaptureManagerBridge

- (void)setup:(UIView *)displayView {
    self.capture = [[CaptureManager alloc] initWithDelegate:self formatType:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange displayView:displayView orientation:(AVCaptureVideoOrientationPortrait)];
}

- (void)captureOutput:(AVCaptureOutput *)output didOutput:(CMSampleBufferRef)sampleBuffer from:(AVCaptureConnection *)connection {
    [_delegate captureOutput:output didOutput:sampleBuffer from:connection];
}

- (void)startCapture {
    [self.capture startCapture];
}

- (void)stopCapture {
    [self.capture stopCapture];
}

- (AVCaptureDevicePosition)getposition {
    return [self.capture getCameraPosition];
}

- (void)createSamples:(EAGLContext *)context imageBuffer:(CVPixelBufferRef)imageBuffer complete:(void (^)(CVOpenGLESTextureRef _Nonnull, CVOpenGLESTextureRef _Nonnull))completion {
    
    [self.capture createSamplesWithContext:context imageBuffer:imageBuffer complete:^(CVOpenGLESTextureRef _Nonnull y, CVOpenGLESTextureRef _Nonnull uv) {
        completion(y, uv);
    }];
}
@end

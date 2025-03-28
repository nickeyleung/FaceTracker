//
//  FaceTrackerManager.m
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

#import "FaceTrackerManager.h"
#import "FaceTrackerBridge.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "CaptureManagerBridge.h"
#import "FaceDetectBridge.h"
#import <GoogleMLKit/MLKit.h>

static id shared = nil;

@interface FaceTrackerManager() <CaptureOutputLogic>
@property (nonatomic, strong) CaptureManagerBridge *capture;
@property (nonatomic, strong) EAGLContext *context;
@property (nonatomic, strong) FaceTrackerBridge *tracker;
@property (nonatomic, strong) CAEAGLLayer *displayLayer;
@end

@implementation FaceTrackerManager

+(id)shared {
    
    if (shared == nil) {
        shared = [[super alloc] init];
        [shared config];
    }
    
    return shared;
}

- (void)config {
    
    self.context = [[EAGLContext alloc] initWithAPI:(kEAGLRenderingAPIOpenGLES3)];
    [EAGLContext setCurrentContext:_context];

    _capture = [[CaptureManagerBridge alloc] init];
    [_capture setup:nil];
    _capture.delegate = self;
    
    [_capture startCapture];
    
    self.tracker = [FaceTrackerBridge shared];

}

- (void)detectFace:(CAEAGLLayer *)displayLayer {
    
    self.displayLayer = displayLayer;
    
    if (self.displayLayer != nil) {
        
        displayLayer.drawableProperties = @{kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8};
        displayLayer.backgroundColor = [[UIColor lightGrayColor] CGColor];
    }
}

- (void)captureOutput:(AVCaptureOutput *)output didOutput:(CMSampleBufferRef)sampleBuffer from:(AVCaptureConnection *)connection {
        
    
    [self displaySampleBuffer:sampleBuffer];
    
    [FaceDetectBridge faceDetectWithSampleBuffer:sampleBuffer complete:^(NSArray<MLKFace *> * _Nonnull faces) {
        
        MLKFace *face = faces.firstObject;
        if (face != nil) {
            dispatch_async(dispatch_get_main_queue(), ^{
                CGFloat width = face.frame.size.width / 2;
                CGFloat height = face.frame.size.height / 2;
                CGFloat centerX = face.frame.origin.x / 2 + width / 2;
                CGFloat centerY = face.frame.origin.y / 2 + height / 2;
                [self.tracker updateSticker:centerX centerY:centerY width:width height:height];
            });
        }
    }];
    

}

- (void)displaySampleBuffer:(CMSampleBufferRef)sampleBuffer {
    
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    
    if (imageBuffer == nil ) {
        return;
    }
    
    [self.capture createSamples:self.context imageBuffer:imageBuffer complete:^(CVOpenGLESTextureRef _Nonnull sampleY, CVOpenGLESTextureRef _Nonnull sampleUV) {
        
        int width = 0;
        int height = 0;
        
        if (self.displayLayer != nil) {
            width = self.displayLayer.frame.size.width;
            height = self.displayLayer.frame.size.height;
        }

        [self.tracker display:sampleY sampleUV:sampleUV width:width heighe:height];
        [self.context presentRenderbuffer:(NSUInteger)GL_RENDERBUFFER];

    }];

    
}

@end

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
@property (nonatomic, strong) MLKFace *face;

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

- (void)updateStickerImagePath:(NSString *)path {
    [self.tracker config2:path];
}

- (void)detectFace:(CAEAGLLayer *)displayLayer {
    
    self.displayLayer = displayLayer;
    
    if (self.displayLayer != nil) {
        
        displayLayer.drawableProperties = @{kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8};
        displayLayer.backgroundColor = [[UIColor lightGrayColor] CGColor];
        [self.context renderbufferStorage:GL_RENDERBUFFER fromDrawable:self.displayLayer];
    }
}

- (void)captureOutput:(AVCaptureOutput *)output didOutput:(CMSampleBufferRef)sampleBuffer from:(AVCaptureConnection *)connection {
        
    
    [self displaySampleBuffer:sampleBuffer];
    
    [FaceDetectBridge faceDetectWithSampleBuffer:sampleBuffer complete:^(NSArray<MLKFace *> * _Nonnull faces) {
        
        self.face = faces.firstObject;

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
        
        if (self.face != nil) {
            CGFloat width = self.face.frame.size.width / [[UIScreen mainScreen] scale] * 1.1;
            CGFloat height = self.face.frame.size.height / [[UIScreen mainScreen] scale] * 1.2;
            CGFloat centerX = self.face.frame.origin.x / [[UIScreen mainScreen] scale] + width / 2;
            CGFloat centerY = self.face.frame.origin.y / [[UIScreen mainScreen] scale] + height / 2;
            CGFloat x = ((centerX / self.displayLayer.frame.size.width) - 0.5 ) * 2;
            CGFloat y = ((centerY / self.displayLayer.frame.size.height) - 0.5 ) * 2 - 0.2;
            [self.tracker updateSticker:x centerY:-y width:width / self.displayLayer.frame.size.width height:height / self.displayLayer.frame.size.height rotateX:-self.face.headEulerAngleX rotateY:-self.face.headEulerAngleY rotateZ:-self.face.headEulerAngleZ];
        }
        [self.context presentRenderbuffer:(NSUInteger)GL_RENDERBUFFER];

    }];

    
}

@end

//
//  FaceTrackerBridge.m
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

#import "FaceTrackerBridge.h"
#import "GLScene.hpp"

@interface FaceTrackerBridge()
@property (nonatomic, assign) GLScene *scene;
@end

@implementation FaceTrackerBridge

static id shared = nil;

+(id)shared {
    
    if (shared == nil) {
        shared = [[super alloc] init];
        [shared config];
    }
    
    return shared;
}

-(void)config {
    
    self.scene = new GLScene();
    
    NSString *pathV = [[NSBundle mainBundle] pathForResource:@"YUV" ofType:@"vsh"];
    NSString *vsh = [[NSString alloc] initWithContentsOfFile:pathV encoding:NSUTF8StringEncoding error:nil];
        
    NSString *pathf= [[NSBundle mainBundle] pathForResource:@"Normal" ofType:@"fsh"];
    NSString *fsh = [[NSString alloc] initWithContentsOfFile:pathf encoding:NSUTF8StringEncoding error:nil];
    
    NSString *stickerPathV = [[NSBundle mainBundle] pathForResource:@"sticker" ofType:@"vsh"];
    NSString *stickerVsh = [[NSString alloc] initWithContentsOfFile:stickerPathV encoding:NSUTF8StringEncoding error:nil];
        
    NSString *stickerPathf= [[NSBundle mainBundle] pathForResource:@"sticker" ofType:@"fsh"];
    NSString *stickerFsh = [[NSString alloc] initWithContentsOfFile:stickerPathf encoding:NSUTF8StringEncoding error:nil];
    
    NSString *path = [[NSBundle mainBundle] resourcePath];
    self.scene->setBundlePath([path UTF8String], [vsh UTF8String], [fsh UTF8String],[stickerVsh UTF8String], [stickerFsh UTF8String]);
}

- (void)updateSticker:(float)centerX centerY:(float)centerY width:(int)width height:(int)height {
    self.scene->updateSticker(centerX, centerY, width, height);
}

- (void)display:(CVOpenGLESTextureRef)sampleY sampleUV:(CVOpenGLESTextureRef)sampleUV width:(int)width heighe:(int)height {
    self.scene->display(sampleY, sampleUV, width, height);

}

@end

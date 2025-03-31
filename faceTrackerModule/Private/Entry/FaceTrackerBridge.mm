//
//  FaceTrackerBridge.m
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

#import "FaceTrackerBridge.h"
#import "GLScene.hpp"
#import <UIKit/UIKit.h>

@interface FaceTrackerBridge()
@property (nonatomic, assign) GLScene *scene;
@end

@implementation FaceTrackerBridge

static id shared = nil;

+(id)shared {
    
    if (shared == nil) {
        shared = [[super alloc] init];
    }
    
    return shared;
}

- (void)config2:(NSString *)imagePath {
    
    self.scene = new GLScene();
    
    NSString *path = [[[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Frameworks"] stringByAppendingPathComponent:@"faceTrackerModule.framework"] stringByAppendingPathComponent:@"Shaders.bundle"];

    NSString *pathV = [path stringByAppendingPathComponent:@"YUV.vsh"];
    NSString *vsh = [[NSString alloc] initWithContentsOfFile:pathV encoding:NSUTF8StringEncoding error:nil];
        
    NSString *pathf= [path stringByAppendingPathComponent:@"Normal.fsh"];
    NSString *fsh = [[NSString alloc] initWithContentsOfFile:pathf encoding:NSUTF8StringEncoding error:nil];
    
    NSString *stickerPathV = [path stringByAppendingPathComponent:@"sticker.vsh"];
    NSString *stickerVsh = [[NSString alloc] initWithContentsOfFile:stickerPathV encoding:NSUTF8StringEncoding error:nil];
        
    NSString *stickerPathf= [path stringByAppendingPathComponent:@"sticker.fsh"];
    NSString *stickerFsh = [[NSString alloc] initWithContentsOfFile:stickerPathf encoding:NSUTF8StringEncoding error:nil];
    
    self.scene->setBundlePath([imagePath UTF8String], [vsh UTF8String], [fsh UTF8String],[stickerVsh UTF8String], [stickerFsh UTF8String]);
}

- (void)updateSticker:(float)centerX centerY:(float)centerY width:(float)width height:(float)height rotateX:(float)rotateX rotateY:(float)rotateY rotateZ:(float)rotateZ {
    self.scene->updateSticker(centerX, centerY, width, height, rotateX, rotateY, rotateZ);
}

- (void)display:(CVOpenGLESTextureRef)sampleY sampleUV:(CVOpenGLESTextureRef)sampleUV width:(int)width heighe:(int)height {
    self.scene->display(sampleY, sampleUV, width, height);

}

@end

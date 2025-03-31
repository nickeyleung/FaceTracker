//
//  FaceTrackerBridge.h
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FaceTrackerBridge : NSObject

+(id)shared;
-(void)config2:(NSString *)imagePath;
- (void)updateSticker:(float)centerX centerY:(float)centerY width:(float)width height:(float)height rotateX:(float)rotateX rotateY:(float)rotateY rotateZ:(float)rotateZ;

-(void)display:(CVOpenGLESTextureRef)sampleY sampleUV:(CVOpenGLESTextureRef)sampleUV width:(int)width heighe:(int)height;

@end

NS_ASSUME_NONNULL_END

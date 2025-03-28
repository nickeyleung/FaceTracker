//
//  FaceTrackerBridge.h
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

#import <CoreVideo/CoreVideo.h>

NS_ASSUME_NONNULL_BEGIN

@interface FaceTrackerBridge : NSObject

+(id)shared;

-(void)updateSticker:(float)centerX centerY:(float)centerY width:(int)width height:(int)height;

-(void)display:(CVOpenGLESTextureRef)sampleY sampleUV:(CVOpenGLESTextureRef)sampleUV width:(int)width heighe:(int)height;

@end

NS_ASSUME_NONNULL_END

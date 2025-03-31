//
//  FaceTrackerManager.h
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

//#import "faceTrackerModule-Swift.h"
#import "AVFoundation/AVFoundation.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FaceTrackerManager : NSObject

+(id)shared;

- (void)updateStickerImagePath:(NSString *)path;
-(void)detectFace:(CAEAGLLayer *)displayLayer;
@end

NS_ASSUME_NONNULL_END

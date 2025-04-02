//
//  FaceTrackerConfigurator.swift
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

import UIKit

public class FaceTrackerConfigurator: NSObject {
    
    private static var manager: FaceTracker = FaceTracker()
    
    public class func detectFace(request: FaceTrackerProtocol) {
        
        guard let layer = request.displayLayer else { return }
        var path = ""
        
        switch request.effectImage {
        case .rabbitSample:
            path = NSString(string: Bundle.main.bundlePath).appendingPathComponent("Frameworks").appending("/faceTrackerModule.framework").appending("/Shaders.bundle").appending("/item_stickerrabit.png")
        case .imagePath(let ipath):
            path = ipath
        }
        manager.updateStickerImagePath(path: path)
        manager.detectFace(layer: layer)
        manager.startCapture()
    }
}

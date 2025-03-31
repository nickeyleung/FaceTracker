//
//  FaceTrackerConfigurator.swift
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

import UIKit
import MyObjcFramework

public class FaceTrackerConfigurator: NSObject {
    
    private static var manager: FaceTrackerManager?
    
    public class func detectFace(request: FaceTrackerProtocol) {
        
        guard let manager = FaceTrackerManager.shared() as? FaceTrackerManager, let layer = request.displayLayer else { return }
        self.manager = manager
        var path = ""
        
        switch request.effectImage {
        case .rabbitSample:
            path = NSString(string: Bundle.main.bundlePath).appendingPathComponent("Frameworks").appending("/faceTrackerModule.framework").appending("/Shaders.bundle").appending("/item_stickerrabit.png")
        case .imagePath(let ipath):
            path = ipath
        }
        manager.updateStickerImagePath(path)
        manager.detectFace(layer)
    }
}

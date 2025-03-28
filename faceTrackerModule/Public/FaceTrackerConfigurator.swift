//
//  FaceTrackerConfigurator.swift
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

import UIKit
import MyObjcFramework

public class FaceTrackerConfigurator: NSObject {
    
    public class func detectFace(request: FaceTrackerProtocol) {
        
        guard let manager = FaceTrackerManager.shared() as? FaceTrackerManager, let layer = request.displayLayer else { return }
        manager.detectFace(layer)
    }
}

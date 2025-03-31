//
//  FaceTrackerProtocol.swift
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

import UIKit

public enum ImagePath {
    case rabbitSample
    case imagePath(path: String)
}

public protocol FaceTrackerProtocol {
    
    var displayLayer: CAEAGLLayer? {get}
    var effectImage: ImagePath {get}
    
}

//
//  FaceTrackerProtocol.swift
//  faceTrackerModule
//
//  Created by Leung on 2025/3/24.
//

import UIKit

@objc public protocol FaceTrackerProtocol {
    
    var displayLayer: CAEAGLLayer? {get}
    var effectImage: UIImage {get}
    
}

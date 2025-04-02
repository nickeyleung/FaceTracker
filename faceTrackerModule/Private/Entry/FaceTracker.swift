//
//  FaceTracker.swift
//  faceTrackerModule
//
//  Created by Leung on 2025/4/1.
//

import UIKit
import AVFoundation
import MyObjcFramework

class FaceTracker: NSObject {
    
    private var captrue: CaptureManager?
    
    public func updateStickerImagePath(path: String) {
        guard let manager = FaceTrackerManager.shared() as? FaceTrackerManager else { return }
        manager.updateStickerImagePath(path)
    }
    
    public func detectFace(layer: CAEAGLLayer) {
        guard let manager = FaceTrackerManager.shared() as? FaceTrackerManager else { return }
        manager.detectFace(layer)
    }
    
    public func startCapture() {
        self.captrue = CaptureManager(delegate: self, formatType: kCVPixelFormatType_420YpCbCr8BiPlanarFullRange, displayView: nil)
        self.captrue?.startCapture()
    }
    
    public func stopCapture() {
        self.captrue?.stopCapture()
    }
}

extension FaceTracker: CaptureLogic {
    func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        
        DispatchQueue.main.async {
            guard let manager = FaceTrackerManager.shared() as? FaceTrackerManager else { return }
            manager.captureOutput(output, didOutput: sampleBuffer, from: connection)
        }

        FaceDetecteManager.detectFace(sampleBuffer: sampleBuffer, cameraPosition: .front) { faces in
            guard let manager = FaceTrackerManager.shared() as? FaceTrackerManager, let face = faces.first else { return }
            manager.update(face)
        }
    }
    
}

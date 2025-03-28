//
//  FaceDetecteManager.swift
//  ffmpeg5
//
//  Created by Leung on 2025/3/5.
//

import UIKit
import MLKitFaceDetection
import MLKitVision
import MLKitCommon
import CoreMedia
import AVFoundation

@objc public class FaceDetecteManager: NSObject {

    static private var detector: FaceDetector?
    
    static private func createDetector() {
        
        let options = FaceDetectorOptions()
        options.performanceMode = .fast
        options.contourMode = .all
        options.classificationMode = .none
        
        detector = FaceDetector.faceDetector(options: options)
    }
    
    static private func imageOrientation(deviceOrientation: UIDeviceOrientation, cameraPosition: AVCaptureDevice.Position) -> UIImage.Orientation {
        switch deviceOrientation {
        case .portrait:
          return cameraPosition == .front ? .leftMirrored : .right
        case .landscapeLeft:
          return cameraPosition == .front ? .downMirrored : .up
        case .portraitUpsideDown:
          return cameraPosition == .front ? .rightMirrored : .left
        case .landscapeRight:
          return cameraPosition == .front ? .upMirrored : .down
        case .faceDown, .faceUp, .unknown:
          return .up
       default:
            return .up
        }
      }
    
    @objc public static func detectFace(sampleBuffer: CMSampleBuffer, cameraPosition: AVCaptureDevice.Position, complete: @escaping ([Face]) -> Void) {
        
        if detector == nil {
            createDetector()
        }
        
        guard let detector = detector else { return }
        
        let visionImg = VisionImage(buffer: sampleBuffer)
        visionImg.orientation = imageOrientation(deviceOrientation: UIDeviceOrientation.landscapeRight, cameraPosition: cameraPosition)
        do {
          let result = try  detector.results(in: visionImg)
            complete(result)
        }catch {
            print(error)
        }

    }
}

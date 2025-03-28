//
//  CaptureManager.swift
//  ffmpeg5
//
//  Created by Leung on 2024/9/20.
//

import UIKit
import AVFoundation

@objc public protocol CaptureLogic {
    @objc func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection)
}

@objc public class CaptureManager: NSObject {

    private var session = AVCaptureSession()
    private var back: AVCaptureDevice?
    private var front: AVCaptureDevice?
    private var current: AVCaptureDevice?
    var delegate: CaptureLogic?
    private var output: AVCaptureOutput?
    
    @objc public   init(delegate: CaptureLogic, formatType: OSType, displayView: UIView?, orientation: AVCaptureVideoOrientation = .portrait) {
        super.init()
        self.delegate = delegate
        setup(type: .video, formatType: formatType, displayView: displayView, orientation: orientation)
    }
    private override init() {
        super.init()
    }
    
    private func setup(type: AVMediaType, formatType: OSType, displayView: UIView?, orientation: AVCaptureVideoOrientation = .portrait) {
        
        session.sessionPreset = .hd1280x720
        
        if let displayView = displayView {
            let displayLayer = AVCaptureVideoPreviewLayer(session: session)
            displayLayer.frame = displayView.bounds
            displayView.layer.addSublayer(displayLayer)
        }
        
        AVCaptureDevice.devices(for: type).forEach { device in
            if device.position == .back {
                back = device
            }else if device.position == .front {
                front = device
            }
            
            if let back = front, let deviceInput = try? AVCaptureDeviceInput(device: back) {

                if session.canAddInput(deviceInput) {
                    session.addInput(deviceInput)
                    current = back
                }
                
                let output = AVCaptureVideoDataOutput()
                let availableTypes = output.availableVideoPixelFormatTypes

                if availableTypes.contains(formatType){
                    output.videoSettings = [kCVPixelBufferPixelFormatTypeKey as String: formatType]
                }else {
                    
                    if availableTypes.contains(kCVPixelFormatType_420YpCbCr8PlanarFullRange) {
                        output.videoSettings = [kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_420YpCbCr8PlanarFullRange]
                    }else if availableTypes.contains(kCVPixelFormatType_420YpCbCr8Planar) {
                        output.videoSettings = [kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_420YpCbCr8Planar]
                    }else if availableTypes.contains(kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
                        output.videoSettings = [kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_420YpCbCr8BiPlanarFullRange]
                    }
                }
                
                output.setSampleBufferDelegate(self, queue: DispatchQueue.global())
                output.alwaysDiscardsLateVideoFrames = false
                
                self.output = output

                
                if session.canAddOutput(output) {
                    
                    session.addOutput(output)
                    output.alwaysDiscardsLateVideoFrames = true
                    let connection = output.connection(with: .video)
                    connection?.videoOrientation = orientation
                }
            }
            
        }
        
    }
    
    @objc public func startCapture() {
        if !session.isRunning {
            DispatchQueue.global().async {
                self.session.startRunning()
            }
        }
    }
    
    @objc public func stopCapture() {
        session.stopRunning()
    }
    
    @objc public func getCameraPosition() -> AVCaptureDevice.Position {
        return current?.position ?? .front
    }
    
    @objc public func createSamples(context: EAGLContext?, imageBuffer: CVPixelBuffer, complete: @escaping (CVOpenGLESTexture, CVOpenGLESTexture) -> Void) {
        
        guard let context = context else { return}
        
        let width = CVPixelBufferGetWidth(imageBuffer)
        let height = CVPixelBufferGetHeight(imageBuffer)
        
        DispatchQueue.main.sync(execute: {
            var cache: CVOpenGLESTextureCache?
            CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, nil, context, nil, &cache)
            
            guard let cache = cache else { return}
            var sampleY: CVOpenGLESTexture?
            CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, cache, imageBuffer, nil, GLenum(GL_TEXTURE_2D), GL_LUMINANCE, GLsizei(width), GLsizei(height), GLenum(GL_LUMINANCE), GLenum(GL_UNSIGNED_BYTE), 0, &sampleY)
            
            var sampleUV: CVOpenGLESTexture?
            CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, cache, imageBuffer, nil, GLenum(GL_TEXTURE_2D), GL_LUMINANCE_ALPHA, GLsizei(width / 2), GLsizei(height / 2), GLenum(GL_LUMINANCE_ALPHA), GLenum(GL_UNSIGNED_BYTE), 1, &sampleUV)
            
            guard let sampleY = sampleY, let sampleUV = sampleUV else { return }
            complete(sampleY, sampleUV)
        })
        

        
    }
}

extension CaptureManager: AVCaptureVideoDataOutputSampleBufferDelegate {
    
    public func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        delegate?.captureOutput(output, didOutput: sampleBuffer, from: connection)
//        print("didOutput, ")
    }
    
    public func captureOutput(_ output: AVCaptureOutput, didDrop sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        print("drop, ", sampleBuffer.attachments)
    }
}

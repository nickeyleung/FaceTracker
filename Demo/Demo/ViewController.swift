//
//  ViewController.swift
//  Demo
//
//  Created by Leung on 2025/3/26.
//

import UIKit
import faceTrackerModule

class ViewController: UIViewController {
    
    var displayLa: CAEAGLLayer = CAEAGLLayer()
    var config: FaceTrackerConfigurator?

    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.view.layer.addSublayer(displayLa)
        displayLa.frame = self.view.bounds
        
        FaceTrackerConfigurator.detectFace(request: self)

    }


}

extension ViewController: FaceTrackerProtocol {
    var effectImage: faceTrackerModule.ImagePath {
        
        if let path = Bundle.main.path(forResource: "item_stickerrabit", ofType: "png") {
            return .imagePath(path: path)
        }else {
            return .rabbitSample
        }
    }
    
    var displayLayer: CAEAGLLayer? {
        self.displayLa
    }

}


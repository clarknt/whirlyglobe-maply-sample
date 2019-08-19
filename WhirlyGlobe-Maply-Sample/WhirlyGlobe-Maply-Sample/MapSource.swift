//
//  MapSource.swift
//  WhirlyGlobe-Maply-Sample
//
//  Created by clarknt on 2019-08-17.
//  Copyright © 2019 clarknt. All rights reserved.
//

import Foundation

enum MapSourceType {
    case local
    case remote
}

protocol MapSource {
    var type: MapSourceType { get }
    var name: String { get }
}

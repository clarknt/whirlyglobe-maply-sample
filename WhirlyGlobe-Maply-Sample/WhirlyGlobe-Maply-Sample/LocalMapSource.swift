//
//  LocalMapSource.swift
//  WhirlyGlobe-Maply-Sample
//
//  Created by clarknt on 2019-08-17.
//  Copyright © 2019 clarknt. All rights reserved.
//

import Foundation

struct LocalMapSource: MapSource {
    var type: MapSourceType { get { return .local } }
    var name: String

    static let suffix = ".mbtiles"
    
    let filename: String
}

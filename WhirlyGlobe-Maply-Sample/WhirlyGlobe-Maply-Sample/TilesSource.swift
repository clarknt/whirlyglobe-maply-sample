//
//  MapSource.swift
//  WhirlyGlobe-Maply-Sample
//
//  Created by clarknt on 2019-08-17.
//  Copyright © 2019 clarknt. All rights reserved.
//

import Foundation

enum TilesSourceType {
    case local
    case remote
}

protocol TilesSource {
    var type: TilesSourceType { get }
    var name: String { get }
}

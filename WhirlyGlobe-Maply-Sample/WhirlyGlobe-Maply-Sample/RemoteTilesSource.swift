//
//  RemoteMapSource.swift
//  WhirlyGlobe-Maply-Sample
//
//  Created by clarknt on 2019-08-17.
//  Copyright © 2019 clarknt. All rights reserved.
//

import Foundation

struct RemoteTilesSource: TilesSource {
    var type: TilesSourceType { get { return .remote } }
    var name: String
    
    let url: String
    let ext: String
    let maxZoom: Int32
}

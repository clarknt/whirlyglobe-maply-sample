/*
 *  MaplyMBTileFetcher.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 9/13/18.
 *  Copyright 2011-2018 mousebird consulting inc
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#import "MaplyTileSourceNew.h"
#import "MaplyCoordinateSystem.h"

/**
    Tile Fetcher that reads from an MBTiles file.
 
    This tile fetcher focuses on a single MBTiles file.  You mate this
    with a QuadImageLoader to do the actual work.

    Will work for image or vector MBTiles files.
  */
@interface MaplyMBTileFetcher : NSObject<MaplyTileFetcher>

/// Initialize with the name of the local MBTiles file
- (nullable instancetype)initWithMBTiles:(NSString *__nonnull)fileName;

/// TileInfo objected needed by a QuadImageLoader
- (nullable NSObject<MaplyTileInfoNew> *)tileInfo;

/// Min zoom read from file
- (int)minZoom;

/// Max zoom read from file
- (int)maxZoom;

/// Coordinate system (probably Spherical Mercator)
- (MaplyCoordinateSystem * __nonnull)coordSys;

@end

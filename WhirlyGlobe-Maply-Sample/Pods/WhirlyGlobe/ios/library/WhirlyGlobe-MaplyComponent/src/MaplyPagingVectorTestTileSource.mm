/*
 *  MaplyPagingVectorTestTileSource.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 6/19/14.
 *  Copyright 2011-2017 mousebird consulting
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

#import "MaplyScreenLabel.h"
#import "MaplyPagingVectorTestTileSource.h"

@implementation MaplyPagingVectorTestTileSource

- (instancetype)initWithCoordSys:(MaplyCoordinateSystem *)coordSys minZoom:(int)minZoom maxZoom:(int)maxZoom
{
    self = [super init];
    if (!self)
        return nil;
    
    _coordSys = coordSys;
    _minZoom = minZoom;
    _maxZoom = maxZoom;
    
    return self;
}

static const int MaxDebugColors = 10;
static const int debugColors[MaxDebugColors] = {0x86812D, 0x5EB9C9, 0x2A7E3E, 0x4F256F, 0xD89CDE, 0x773B28, 0x333D99, 0x862D52, 0xC2C653, 0xB8583D};

- (void)startFetchForTile:(MaplyTileID)tileID forLayer:(MaplyQuadPagingLayer *)layer
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                   ^{
                       // Add in a little delay
                       if (self->_useDelay)
                           usleep(0.215 * 1e6);
                       
                       if (tileID.level > self->_maxZoom)
                       {
                           [layer tileFailedToLoad:tileID];
                       } else {
                           MaplyCoordinate ll,ur;
                           [layer geoBoundsforTile:tileID ll:&ll ur:&ur];
                           MaplyCoordinate center;
                           center.x = (ll.x+ur.x)/2.0;  center.y = (ll.y+ur.y)/2.0;
                           MaplyCoordinate coords[4];
                           double spanX = ur.x - ll.x;
                           double spanY = ur.y - ll.y;
                           coords[0] = MaplyCoordinateMake(ll.x+spanX*0.1,ll.y+spanY*0.1);
                           coords[1] = MaplyCoordinateMake(ll.x+spanX*0.1,ur.y-spanY*0.1);
                           coords[2] = MaplyCoordinateMake(ur.x-spanX*0.1,ur.y-spanY*0.1);
                           coords[3] = MaplyCoordinateMake(ur.x-spanX*0.1,ll.y+spanY*0.1);
                           
                           // Color rectangle with outline
                           int hexColor = debugColors[tileID.level % MaxDebugColors];
                           float red = (((hexColor) >> 16) & 0xFF)/255.0;
                           float green = (((hexColor) >> 8) & 0xFF)/255.0;
                           float blue = (((hexColor) >> 0) & 0xFF)/255.0;
                           UIColor *color = [UIColor colorWithRed:red green:green blue:blue alpha:0.75];
                           MaplyVectorObject *rect = [[MaplyVectorObject alloc] initWithLineString:coords numCoords:4 attributes:nil];
                           MaplyComponentObject *compObj0 = [layer.viewC addVectors:@[rect] desc:
                                                            @{kMaplyFilled: @(true),
                                                              kMaplyColor: color,
                                                              kMaplyEnable: @NO,
                                                              kMaplyDrawPriority: @(kMaplyVectorDrawPriorityDefault+100+tileID.level)
                                                              }
                                                              mode:MaplyThreadCurrent
                                                            ];
                           MaplyComponentObject *compObj1 = [layer.viewC addVectors:@[rect] desc:
                                                             @{kMaplyFilled: @(false),
                                                               kMaplyColor: [UIColor whiteColor],
                                                               kMaplyEnable: @NO,
                                                               kMaplyDrawPriority: @(kMaplyVectorDrawPriorityDefault+101+tileID.level)
                                                               }
                                                                mode:MaplyThreadCurrent
                                                             ];
                           
                           // Label
                           MaplyScreenLabel *label = [[MaplyScreenLabel alloc] init];
                           label.loc = center;
                           label.text = [NSString stringWithFormat:@"%d: (%d,%d)",tileID.level,tileID.x,tileID.y];
                           MaplyComponentObject *compObj2 = [layer.viewC addScreenLabels:@[label] desc:
                                                             @{kMaplyFont: [UIFont systemFontOfSize:18.0],
                                                               kMaplyJustify: @"center",
                                                               kMaplyEnable: @NO,
                                                               kMaplyTextOutlineSize: @(1.0)
                                                               }
                                                                mode:MaplyThreadCurrent
                                                             ];
                           
                           [layer addData:@[compObj0,compObj1,compObj2] forTile:tileID];
                           
                           [layer tileDidLoad:tileID];
                       }
                   });
}

@end

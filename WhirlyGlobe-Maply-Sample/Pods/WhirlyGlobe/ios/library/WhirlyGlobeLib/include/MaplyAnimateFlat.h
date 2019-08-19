/*
 *  MaplyAnimateFlat.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 6/24/13.
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

#import <UIKit/UIKit.h>
#import "WhirlyVector.h"
#import "WhirlyGeometry.h"
#import "MaplyFlatView.h"

/// Maply translation from one location to another.
@interface MaplyAnimateFlat : NSObject<MaplyAnimationDelegate>

/// Kick off the translate/scale to the new extents
/// Assign this to the flat view's delegate and it'll do the rest
- (id)initWithView:(MaplyFlatView *)flatView destWindow:(WhirlyKit::Point2f)window destContentOffset:(WhirlyKit::Point2f)contentOffset howLong:(float)howLong;

@end

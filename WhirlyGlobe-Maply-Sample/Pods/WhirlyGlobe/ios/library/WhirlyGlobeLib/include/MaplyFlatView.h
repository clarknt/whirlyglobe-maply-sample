/*
 *  MaplyFlatView.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 5/2/13.
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

#import "MaplyView.h"

/** The flat view implements a top down orthogonal projection
    which is prefect for doing a straight up 2D map.
    It thinks more like a window in that it's trying to
    display the full extents (as passed in) within a large window
    (also passed in) but only showing a smaller window within that.
    We presume the caller will move that smaller window around, thus
    changing our model and projection matrices.
  */
@interface MaplyFlatView : MaplyView

/// Initialize with the coordinate system we'll use
- (id)initWithCoordAdapter:(WhirlyKit::CoordSystemDisplayAdapter *)coordAdapter;

/// Generate the model view matrix for use by OpenGL.
- (Eigen::Matrix4d)calcModelMatrix;

/// Height above the plane.  Always returns 0.
- (double)heightAboveSurface;

/// Minimum valid height above plane.  Always returns 0.
- (double)minHeightAboveSurface;

/// Maximum valid height above plane.  Always returns 0.
- (double)maxHeightAboveSurface;

/// Set where the middle of the displayed region is.  Current disabled.
- (void)setLoc:(WhirlyKit::Point3d)newLoc;

/// This view tries to display the given extents in display space
@property (nonatomic,assign) WhirlyKit::Mbr extents;

/// Size of the overall window we're simulating
@property (nonatomic,readonly) WhirlyKit::Point2f windowSize;

/// Content offset within the overall window
@property (nonatomic,readonly) WhirlyKit::Point2f contentOffset;

/// Sets the total window size and the region we're looking at within it.
/// This just gets converted to model and projection matrix parameters
- (void)setWindowSize:(WhirlyKit::Point2f)inWindowSize contentOffset:(WhirlyKit::Point2f)inContentOffset;

@end


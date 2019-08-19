/*
 *  MaplyPanDelegateMap.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 1/10/12.
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

#import <Foundation/Foundation.h>
#import <vector>
#import "MaplyView.h"

@class WhirlyKitSceneRendererES;

// Sent out when the pan delegate takes control
#define kPanDelegateDidStart @"WKPanDelegateStarted"
// Sent out when the pan delegate finished (but hands off to momentum)
#define kPanDelegateDidEnd @"WKPanDelegateEnded"

#define kPanDelegateMinTime 0.1

// Bounds check that adjusts the center to try and compensate
bool MaplyGestureWithinBounds(const std::vector<WhirlyKit::Point2d> &bounds,const WhirlyKit::Point3d &loc,UIView *view,WhirlyKitSceneRendererES *sceneRender,MaplyView *testMapView,WhirlyKit::Point3d *newCenter);

// Custom pan gesture recognizer that plays well with scroll views.
@interface MinDelay2DPanGestureRecognizer : UIPanGestureRecognizer {
    // time of start of gesture
    CFTimeInterval startTime;
}

- (void)forceEnd;

@end

@interface MaplyPanDelegate : NSObject <UIGestureRecognizerDelegate>

/// Create a pinch gesture and a delegate and wire them up to the given UIView
+ (MaplyPanDelegate *)panDelegateForView:(UIView *)view mapView:(MaplyView *)mapView useCustomPanRecognizer:(bool)useCustomPanRecognizer;

@property (nonatomic,weak) UIGestureRecognizer *gestureRecognizer;

/// Set the bounding rectangle
- (void)setBounds:(WhirlyKit::Point2d *)bounds;

@end

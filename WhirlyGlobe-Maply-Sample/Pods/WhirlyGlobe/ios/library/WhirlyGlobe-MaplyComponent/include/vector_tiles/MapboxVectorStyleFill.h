/*
 *  MapboxVectorStyleFill.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 2/17/15.
 *  Copyright 2011-2015 mousebird consulting
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

#import "MapboxVectorStyleSet.h"

@interface MapboxVectorFillLayout : NSObject

@property (nonatomic) bool visible;

- (instancetype)initWithStyleEntry:(NSDictionary *)styleEntry styleSet:(MapboxVectorStyleSet *)styleSet viewC:(NSObject<MaplyRenderControllerProtocol> *)viewC;

@end

@interface MapboxVectorFillPaint : NSObject

@property (nonatomic) double opacity;
@property (nonatomic) double opacityBase;
@property (nonatomic,strong) MaplyVectorFunctionStops *opacityFunc;
@property (nonatomic,strong) UIColor *color;
@property (nonatomic,strong) UIColor *outlineColor;

- (instancetype)initWithStyleEntry:(NSDictionary *)styleEntry styleSet:(MapboxVectorStyleSet *)styleSet viewC:(NSObject<MaplyRenderControllerProtocol> *)viewC;

@end

/// @brief The fill (e.g. fill polygon) style
@interface MapboxVectorLayerFill : MaplyMapboxVectorStyleLayer

@property (nonatomic,strong) MapboxVectorFillLayout *layout;
@property (nonatomic,strong) MapboxVectorFillPaint *paint;

- (instancetype)initWithStyleEntry:(NSDictionary *)styleEntry parent:(MaplyMapboxVectorStyleLayer *)refLayer styleSet:(MapboxVectorStyleSet *)styleSet drawPriority:(int)drawPriority viewC:(NSObject<MaplyRenderControllerProtocol> *)viewC;

@end

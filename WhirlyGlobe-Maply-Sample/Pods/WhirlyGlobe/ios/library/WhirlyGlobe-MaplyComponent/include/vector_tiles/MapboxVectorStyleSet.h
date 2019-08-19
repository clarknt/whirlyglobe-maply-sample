/*
 *  MapboxVectorStyleSet.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 2/16/15.
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

#import <Foundation/Foundation.h>
#import "MaplyVectorStyle.h"
#import "MapboxVectorTiles.h"

@class MapboxVectorFilter;
@class MaplyVectorFunctionStops;

/** @brief The Mapbox Vector Style Set parses Mapbox GL Style sheets and turns them into Maply compatible styles.
    @details A style delegate is required by the Mapnik parser to build geometry out of Mapnik vector tiles.  This style delegate can read a Mapbox GL Style sheet and produce compatible styles.
 */
@interface MapboxVectorStyleSet : NSObject <MaplyVectorStyleDelegate>

/// @brief Default settings and scale factor for Mapnik vector geometry.
@property (nonatomic, strong, nonnull) MaplyVectorStyleSettings *tileStyleSettings;

/// @brief The view controller everything will be added to
@property (nonatomic, weak, nullable) NSObject<MaplyRenderControllerProtocol> *viewC;

/// @brief Style name
@property (nonatomic, strong, nullable) NSString *name;

/// @brief Version number from the style
@property (nonatomic) NSUInteger version;

/// @brief Constants from the Style sheet
@property (nonatomic, strong, nullable) NSDictionary *constants;

/// @brief Where we can fetch the sprites
@property (nonatomic, strong, nullable) NSString *spriteURL;

/// @brief Layers parsed from the style sheet
@property (nonatomic, strong, nullable) NSArray *layers;

/// @brief Layers sorted by their ID
@property (nonatomic, strong, nullable) NSDictionary *layersByName;

/// @brief Layers sorted by source layer name
@property (nonatomic, strong, nullable) NSDictionary *layersBySource;

/// @brief Initialize with the style JSON and the view controller
/// @details We'll parse the style JSON passed in and return nil on failure.
/// @details The optional filter can be used to reject layers we won't use
- (id __nullable)initWithJSON:(NSData * __nonnull)styleJSON settings:(MaplyVectorStyleSettings * __nonnull)settings viewC:(NSObject<MaplyRenderControllerProtocol> * __nonnull)viewC filter:(bool (^_Nullable)(NSMutableDictionary * __nonnull))filterBlock;

/// @brief Return an integer value for the given name, taking the constants into account.
- (int)intValue:(NSString * __nonnull)name dict:(NSDictionary * __nullable)dict defVal:(int)defVal;

/// @brief Return a double value for the given name, taking the constants into account
- (double)doubleValue:(NSString * __nonnull)name dict:(NSDictionary * __nullable)dict defVal:(double)devVal;

/// @brief Return a double value for the given name, taking the constants into account
- (double)doubleValue:(id __nonnull)entry defVal:(double)defVal;
    
/// @brief Return a bool for the given name.  True if it matches the onString.  Default if it's missing
- (bool)boolValue:(NSString * __nonnull)name dict:(NSDictionary * __nullable)dict onValue:(NSString * __nonnull)onString defVal:(bool)defVal;

/// @brief Return a string for the given name, taking the constants into account
- (NSString *_Nullable)stringValue:(NSString * __nullable)name dict:(NSDictionary * __nullable)dict defVal:(NSString * __nullable)defVal;

/// @brief Return a color for the given name, taking the constants into account
- (UIColor *_Nullable)colorValue:(NSString * __nullable)name val:(id __nullable )val dict:(NSDictionary *__nullable)dict defVal:(UIColor * __nullable)defVal multiplyAlpha:(bool)multiplyAlpha;

/// @brief Return an array for the given name, taking the constants into account
- (NSArray *_Nullable)arrayValue:(NSString * __nonnull)name dict:(NSDictionary * __nullable)dict defVal:(NSArray * __nullable)defVal;

/// @brief Parse and return a set of stops.  Takes constants into account
- (MaplyVectorFunctionStops * __nullable)stopsValue:(id __nonnull)entry defVal:(id __nullable)defEntry;

/// @brief Scale the color by the given opacity
- (UIColor * __nullable)color:(UIColor * __nonnull)color withOpacity:(double)opacity;

/// @brief Return the integer corresponding to the name.  Basically parse the enumerated type.
- (NSUInteger)enumValue:(NSString * __nonnull)name options:(NSArray * __nonnull)options defVal:(NSUInteger)defVal;

/// @brief Check for and report an unsupported field
- (void)unsupportedCheck:(NSString * __nonnull)field in:(NSString * __nonnull)what styleEntry:(NSDictionary * __nonnull)styleEntry;

/// @brief Check if the given thing is a constant and return its value if it is.  Otherwise just return it.
- (id __nullable)constantSubstitution:(id __nonnull)thing forField:(NSString * __nullable)field;

@end

/** @brief Layer definition from the Style Sheet.
    @details This is a single layer from the Mapbox style sheet.  It's also used to build visible objects.
  */
@interface MaplyMapboxVectorStyleLayer : NSObject<MaplyVectorStyle>
// Note: Need a better base class than MaplyVectorTileStyle

@property (nonatomic,weak,nullable) MapboxVectorStyleSet *styleSet;

/// @brief ID on the layer style entry
@property (nonatomic,nullable,strong) NSString *ident;

/// @brief Source from layer defn
@property (nonatomic,nullable,strong) NSString *source;

/// @brief Source layer from layer defn
@property (nonatomic,nullable,strong) NSString *sourceLayer;

/// @brief Min/max zoom levels
@property (nonatomic) int minzoom,maxzoom;

/// @brief Filter this layer uses to match up to data
@property (nonatomic,nullable,strong) MapboxVectorFilter *filter;

/// @brief DrawPriority based on location in the style sheet
@property (nonatomic) int drawPriority;

/// @brief Initialize with the style sheet and the entry for this layer
+ (id __nullable)VectorStyleLayer:(MapboxVectorStyleSet * __nonnull)styleSet JSON:(NSDictionary * __nonnull)layerDict drawPriority:(int)drawPriority;

/// @brief Base class initialization.  Copies data out of the refLayer
- (id __nullable)initWithStyleEntry:(NSDictionary * __nonnull)styleEntry parent:(MaplyMapboxVectorStyleLayer * __nonnull)refLayer styleSet:(MapboxVectorStyleSet * __nonnull)styleSet drawPriority:(int)drawPriority viewC:(NSObject<MaplyRenderControllerProtocol> * __nonnull)viewC;

/// @brief Unique Identifier for this style
@property(nonatomic,strong) NSString * _Nonnull uuid;

/// @brief Set if this geometry is additive (e.g. sticks around) rather than replacement
@property(nonatomic) bool geomAdditive;

@end

/// @brief Mapbox filter operator types
typedef enum {MBFilterEqual,MBFilterNotEqual,MBFilterGreaterThan,MBFilterGreaterThanEqual,MBFilterLessThan,MBFilterLessThanEqual,MBFilterIn,MBFilterNotIn,MBFilterHas,MBFilterNotHas,MBFilterAll,MBFilterAny,MBFilterNone} MapboxVectorFilterType;

/// @brief Mapbox geometry types
typedef enum {MBGeomPoint,MBGeomLineString,MBGeomPolygon,MBGeomNone} MapboxVectorGeometryType;

/// @brief Filter is used to match data in a layer to styles
@interface MapboxVectorFilter : NSObject

/// @brief The comparison type for this filter
@property (nonatomic) MapboxVectorFilterType filterType;

/// @brief Attribute name for all the types that take two arguments
@property (nonatomic,nullable,strong) NSString *attrName;

/// @brief Set if we're comparing geometry type instead of an attribute
@property (nonatomic) MapboxVectorGeometryType geomType;

/// @brief Attribute value to compare for all the type that take two arguments
@property (nonatomic,nullable,strong) id attrVal;

/// @brief Attribute values for the in and !in operators
@property (nonatomic,nullable,strong) NSArray *attrVals;

/// @brief For All and Any these are the MapboxVectorFilters to evaluate
@property (nonatomic,nullable,strong) NSArray *subFilters;

/// @brief Parse the filter info out of the style entry
- (id _Nullable)initWithArray:(NSArray * __nonnull)styleEntry styleSet:(MapboxVectorStyleSet * __nonnull)styleSet viewC:(NSObject<MaplyRenderControllerProtocol> * __nonnull)viewC;

/// @brief Test a feature's attributes against the filter
- (bool)testFeature:(NSDictionary * __nonnull)attrs tile:(MaplyTileID)tileID viewC:(NSObject<MaplyRenderControllerProtocol> * __nonnull)viewC;

@end

/// @brief A single zoom and value
@interface MaplyVectorFunctionStop : NSObject

/// @brief Zoom level this applies to
@property (nonatomic) double zoom;

/// @brief Value at that zoom level
@property (nonatomic) double val;

/// @brief Could also just be a color
@property (nonatomic,nullable,strong) UIColor *color;

@end

/// @brief These are used to control simple values based on zoom level
@interface MaplyVectorFunctionStops : NSObject

/// @brief Array of function stops as they apply to value.  These are MaplyVectorFunctionStop objects.
@property (nonatomic,strong,nullable) NSArray *stops;

/// @brief Used in exponential calculation
@property (nonatomic,assign) double base;

/// @brief Parse out of a JSON array
- (id _Nullable)initWithArray:(NSArray * __nonnull)dataArray styleSet:(MapboxVectorStyleSet * __nonnull)styleSet viewC:(NSObject<MaplyRenderControllerProtocol> * __nonnull)viewC;

/// @brief Calculate a value given the zoom level
- (double)valueForZoom:(int)zoom;

/// @brief This version returns colors, assuming we're dealing with colors
- (UIColor * _Nonnull)colorForZoom:(int)zoom;

/// @brief Returns the minimum value
- (double)minValue;

/// @brief Returns the maximum value
- (double)maxValue;

@end

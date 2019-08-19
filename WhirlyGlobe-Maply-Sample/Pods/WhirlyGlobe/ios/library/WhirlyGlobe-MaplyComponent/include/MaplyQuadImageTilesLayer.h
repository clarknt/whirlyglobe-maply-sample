/*
 *  MaplyQuadImageTilesLayer.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 5/13/13.
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

#import "MaplyViewControllerLayer.h"
#import "MaplyCoordinateSystem.h"
#import "MaplyTileSource.h"
#import "MaplyRenderController.h"

/** 
    The Maply Quad Image Tiles Layer is for paging image pyramids local or remote.
    
    This layer pages image pyramids.  They can be local or remote, in any coordinate system Maply supports and you provide a MaplyTileSource conformant object to do the actual image tile fetching.
    
    This is the main interface for image pyramid paging and so has a lot of knobs you can twiddle.  The defaults should work fine in most cases.
    
    You probably don't have to implement your own tile source.  Go look at the MaplyRemoteTileSource and MaplyMBTileSource objects, as well as MaplyMultiplexTileSource.  Those will do remote, local, and sources for animation respectively.  There's also MaplyWMSTileSource, but I wouldn't expect to use that.
    @see MaplyRemoteTileSource
    @see MaplyMBTileSource
    @see MaplyMultiplexTileSource
    @see MaplyWMSTileSource
  */
@interface MaplyQuadImageTilesLayer : MaplyViewControllerLayer

/** 
    Initialize with a tile source object.
 
    The initialize expects a tile source.  The tile source can be one of the standard ones listed above, or it can be one of your own that conforms to the MaplyTileSource protocol. The tile source's coordinate system will be used.
 
    @param tileSource This is an object conforming to the MaplyTileSource protocol.  There are several you can pass in, or you can write your own.
 */
- (nullable instancetype)initWithTileSource:(NSObject<MaplyTileSource> *__nonnull)tileSource;

/** 
    Initialize with a coordinate system for the image pyramid and the tile source object.  
    
    The initialize expects a coordinate system (probably MaplySphericalMercator) and a tile source.  The tile source can be one of the standard ones listed above, or it can be one of your own that conforms to the MaplyTileSource protocol.
    
    @param coordSys The coordinate system. This must match what your
            image pyramid is in, or it will look weird.  Very weird.
    
    @param tileSource This is an object conforming to the MaplyTileSource protocol.  There are several you can pass in, or you can write your own.
  */
- (nullable instancetype)initWithCoordSystem:(MaplyCoordinateSystem *__nonnull)coordSys tileSource:(NSObject<MaplyTileSource> *__nonnull)tileSource;

/** 
    Set the active tile source.
    
    If you change this, it will force a reload of all loaded tiles and start fetching from the new tile source.
  */
@property (nonatomic,strong,nonnull) NSObject<MaplyTileSource> *tileSource;

/** 
    Enable/Disable the whole layer.
    
    By default this is on.  If you turn it off, there may be a slight delay before the whole layer disappears.  The layer will keep working, but any geometry will be invisible until you turn it back on.
  */
@property (nonatomic,assign) bool enable;

/** 
    Set the fade for the whole layer.
    
    By default this is 1.0.  The fade values can be changed while the layer is running.  It will blend the layer with the fade value when rendering.  A value of 0.0 will stop rendering the layer.
  */
@property (nonatomic,assign) float fade;

/** 
    The number of simultaneous fetches the layer will attempt at once.
    
    The toolkit loves its dispatch queues and threads.  By default this number is set to 8 or 16, but if you need to constrain it, you can set it lower (or higher!).  If your tile source can't handle multi-thread access, set this to 1.
  */
@property (nonatomic,assign) int numSimultaneousFetches;

/** 
    If set, we'll generate skirts between different levels of details on the globe.
    
    This option makes zero sense in 2D mode, some sense if 3D map mode (if you're doing elevation) and a lot of sense in globe mode.  What it does is generate skirts along the sides of each tile so you can't see between that tile and the next one when it abuts a different level of detail.
    
    Best to set this explicitly.  For a base map (e.g. the first image layer you put down), it should probably be set.  For following layers it should probably not, but specifics can change.  Try it out.
  */
@property (nonatomic,assign) bool handleEdges;

/** 
    Generate pole geometry for projections that don't go all the way to the poles.
    
    This is for spherical mercator with web extents.  That projection doesn't run all the way to the poles, but it gets fairly close.  If set, we close that gap for the north and south poles and generate the remaining geometry.
    
    We'll pull a texture value from the edge of the images, so build your texture accordingly.  A nice flat color along the north and south border is the best idea.  If not, it'll look a little odd, but still okay.
    
    Though this is designed for MaplySphericalMercator, it may work in similar projections.  It's not going to make any sense for, say UTM, but give it a try.
  */
@property (nonatomic,assign) bool coverPoles;

/** 
    Color for polygons that make up the north pole.
    
    If coverPoles is set to true, then this will be the color of the polygons.  If this is not set, we'll pull that color from the textures.
  */
@property (nonatomic,nullable,strong) UIColor *northPoleColor;

/** 
    Color for polygons that make up the south pole.
 
    If coverPoles is set to true, then this will be the color of the polygons.  If this is not set, we'll pull that color from the textures.
 */
@property (nonatomic,nullable,strong) UIColor *southPoleColor;

/** 
    Set the minimum viewer height the layer will be visible at.
    
    This is off by default.  When on the layer will not be visible unless the viewer is above this height.
  */
@property (nonatomic,assign) float minVis;

/** 
    Set the maximum viewer height the layer will be visible at.
    
    This is off by default.  When on the layer will not be visible unless the viewer is below this height.
  */
@property (nonatomic,assign) float maxVis;

/** 
    Controls whether the fetching code runs in a single thread or is spawned asyncronously.
    
    If set, we'll kick off the tile fetches in their own dispatched blocks.  If not set, we'll just do it in the layer thread.
  */
@property (nonatomic,assign) bool asyncFetching;

/** 
    Set the minimum time for an update based on the viewer's location and orientation.
    
    Paging layers watch the viewer to see what it's up to.  When the viewer moves, the layer updates its contents accordingly.  However, the viewer can be moving constantly so we need a way to keep things under control.
    
    This value (in seconds) specifies the minimum time between updates.  In other words, we won't recalculate things more often than this.  Default value is 1/10s.
  */
@property (nonatomic,assign) NSTimeInterval viewUpdatePeriod;

/** 
    Set the minimum movement for an update based on the viewer's location.
    
    This is useful for throttling layer updates based on how far a viewer moves.  This will only kick off a view update if the viewer moves the given distance (in display coordinates).
    
    We do not take orientation into account here, so you'd probably be better looking straight down.  Default is off.
    
    I suggest not using this unless you've already run into the problem this solves.  Specifically that's where you've moving constantly, but in small increments and are burning too much CPU.
    @see viewUpdatePeriod
  */
@property (nonatomic,assign) float minUpdateDist;

/** 
    Have the layer wait until all local tiles are loaded before updating the renderer.
    
    This will have the layer sit on updates until all the local tiles are in.  You won't see the lower levels loading in.  See waitLoadTimeout.
  */
@property (nonatomic,assign) bool waitLoad;

/** 
    The timeout for wait loads.  We can't wait longer than this for local updates to come back.
    
    If waitLoad is on, this is the maximum time we'll wait for local tile fetches to complete.  There's a limit to the volume of scene changes we can let build up int he queue before we have to flush them.
  */
@property (nonatomic,assign) NSTimeInterval waitLoadTimeout;

/** 
    The number of images we're expecting to get per tile.
    
    This is the number of images the layer will ask for per tile.  The default is 1, which is the normal case.  If this is greater than one that typically means we're going to animate between them.
    
    the MaplyTileSource delegate is always expected to provide this many imates.
  */
@property (nonatomic,assign) unsigned int imageDepth;

/** 
    Set the current image we're displaying.
    
    This sets the current image being displayed, and interpolates between it and the next image.  If set to an integer value, you'll get just that image.  If set to a value between integers, you'll get a blend of the two.
    
    This is incompatible with setting an animationPeriod.  Do just one or the other.
   */
@property (nonatomic, assign) float currentImage;

/** 
    If set, we'll use this as the maximum current image value when animating.
    
    By default this is off (-1).  When it's on, we'll consider this the last valid value for currentImage.  This means, when animating, we'll run from 0 to maxCurrentImage.
    
    This is helpful when you have an animation you want to taper off at the end past the last frame.
  */
@property (nonatomic, assign) float maxCurrentImage;

/** 
    The length of time we'll take to switch through all available images (per tile).
    
    If set to non-zero right after layer creation we'll run through all the available images (in each tile) over the given period.  This only makes sense if you've got more than one image per tile.
    
    If you want tighter control use the currentImage property and set your own timer.
  */
@property (nonatomic, assign) float animationPeriod;

/** 
    If set to true, we'll consider the list of images for each tile to be circular when we animate.
    
    When set we'll loop back to the first image when we go past the last.  This is the default.
    
    When not set, we'll run from 0 to maxCurrentImage and then restart.
  */
@property (nonatomic, assign) bool animationWrap;

/** 
    If set, we'll try to fetch frames individually.
    
    When fetching from a data source that has multiple frames we'll fetch each frame individually and allow them to display as we go.
    
    If this is false, we'll force all the frames to load for a given tile before we move on to the next tile.
  */
@property (nonatomic, assign) bool allowFrameLoading;

/** 
    For the case where we're loading individual frames, this sets the order to load them in.
    
    When doing animation and loading frames, we have the option of loading them one by one.  Normally we start from 0 and work our way up, but you can control that order here.
  */
- (void)setFrameLoadingPriority:(NSArray *__nullable)priorities;

/** 
    Include the original z values in the tile geometry for a custom shader.
    
    When generating tiles for the globe we project the coordinates from their local system (probably MaplySphericalMercator) into a display system.  If you wanted the original z values, to say, write a custom shader that maps color to elevation, that data is now missing.
    
    If set, this adds the z values back as a separate vertex attribute called "a_elev" for your shader to pick up.
  */
@property (nonatomic, assign) bool includeElevAttrForShader;

/** 
    If true we'll actuall use the elevation values to modify the mesh.
    
    When this is on we'll modify the mesh to actually use elevation values passed in through the elevation delegate. On is the default, logically enough.
    
    Why would you ever turn it off?  If you wanted a flat (or curved for the globe) mesh with elevation values in their seperate attribute array.  You might want a shading effect rather than actual geometry.
  */
@property (nonatomic, assign) bool useElevAsZ;

/** 
    Requires an elevation chunk for every tile we display.
    
    Elevation data is optional on the globe or map.  If it exists, via the MaplyElevationSource delegate on the view controller, then we'll use it to construct the tile.  This property requires elevation for any tile we display.
    
    What this does is prevent flat tiles from showing up if there's more imagery than elevation.
    
    We do make a distinction between missing tiles and tiles that are simply flat (at zero) in the MaplyElevationDatabase, so ocean will work correctly.
  */
@property (nonatomic, assign) bool requireElev;

/** 
    Color for the tile geometry.
    
    The geometry we create for tiles has an RGBA color.  It's white/full alpha by default, but you can set it here.  You might want to do this if you'd like a semi-transparent layer, sort of a shader of course, where you can do whatever you like.
  */
@property (nonatomic,strong,nullable) UIColor *color;

/** 
    Maximum number of tiles to load in at once.
    
    This is the maximum number of tiles the pager will have loaded into memory at once.  The default is 128 and that's generally good enough.  However, if your tile size is small, you may want to load in more.
    
    Tile loading can get out of control when using elevation data.  The toolkit calculates potential sceen coverage for each tile so elevation data makes all tiles more important.  As a result the system will happily page in way more data than you may want.  The limit becomes important in elevation mode, so leave it at 128 unless you need to change it.
  */
@property (nonatomic) int maxTiles;

/** 
    Tinker with the importance for tiles.  This will cause more or fewer tiles to load
    
    The system calculates an importance for each tile based on its size and location on the screen.  You can mess with those values here.
    
    Any value less than 1.0 will make the tiles less important.  Any value greater than 1.0 will make tiles more important.
  */
@property (nonatomic) float importanceScale;

/** 
    Set the shader name to use for generated tiles.
    
    Shader programs are accessed by name.  When you create a shader and tie it into the scene, you'll have the name.  Use that name here to ensure that all tiles are rendered with that MaplyShader.
    
    Be sure to set this immediately after layer creation.  It can't be changed in the middle.
  */
@property (nonatomic,strong,nullable) NSString *shaderProgramName;

/** 
    Set the (power of two) size of texture atlases the layer will create.
    
    The system makes extensive use of texture atlases for rendering tiles.  Typically we'll only have one or two gigantic textures will all our imagery and a handfull of drawables.  This is what makes the system fast.  Very fast.
    
    This option controls the size of those texture atlases.  It's set to 2048 by default (2048x2048 texels).  If you're going to change it, set it to 1024, but don't go any lower unless you know something we don't.  It must always be a power of 2.
  */
@property (nonatomic) unsigned int texturAtlasSize;

/** 
    Set the image format for the texture atlases (thus the imagery).
    
    OpenGL ES offers us several image formats that are more efficient than 32 bit RGBA, but they're not always appropriate.  This property lets you choose one of them.  The 16 or 8 bit ones can save a huge amount of space and will work well for some imagery, most maps, and a lot of weather overlays.
    
    Be sure to set this at layer creation, it won't do anything later on.
 
| Image Format | Description |
|:-------------|:------------|
| MaplyImageIntRGBA | 32 bit RGBA with 8 bits per channel.  The default. |
| MaplyImageUShort565 | 16 bits with 5/6/5 for RGB and none for A. |
| MaplyImageUShort4444 | 16 bits with 4 bits for each channel. |
| MaplyImageUShort5551 | 16 bits with 5/5/5 bits for RGB and 1 bit for A. |
| MaplyImageUByteRed | 8 bits, where we choose the R and ignore the rest. |
| MaplyImageUByteGreen | 8 bits, where we choose the G and ignore the rest. |
| MaplyImageUByteBlue | 8 bits, where we choose the B and ignore the rest. |
| MaplyImageUByteAlpha | 8 bits, where we choose the A and ignore the rest. |
| MaplyImageUByteRGB | 8 bits, where we average RGB for the value. |
| MaplyImage4Layer8Bit | 32 bits, four channels of 8 bits each.  Just like MaplyImageIntRGBA, but a warning not to do anything too clever in sampling. |
  */
@property (nonatomic) MaplyQuadImageFormat imageFormat;

/** 
    Number of border texels to set up around image tiles.
    
    For matching image tiles along borders in 3D (probably the globe) we resample the image slightly smaller than we get and make up a boundary around the outside.  This number controls that border size.
    
    By default this is 1.  It's safe to set it to 0 for 2D maps and some overlays.
  */
@property (nonatomic) int borderTexel;

/** 
    Control how tiles are indexed, either from the lower left or the upper left.
    
    If set, we'll use the OSM approach (also Google Maps) to y indexing.  That's that default and it's normally what you're run into.
    
    Strictly speaking, TMS addressing (the standard) is flipped the other way.  So if you're tile source looks odd, try setting this to false.
    
    Default value is true.
  */
@property (nonatomic) bool flipY;

/** 
    Use the target zoom level shortcut when possible.
    
    This turns on the target zoom level shortcut as described in targetZoomLevel.  When on we'll calculate tile importance that way, that is based on a target zoom level rather than the more complex screen space calculations.
    
    It's on by default and will activate only when this layer's coordinate system is the same as the display system and there's no view matrix (e.g. tilt) set.
  */
@property (nonatomic) bool useTargetZoomLevel;


/**
 Modify how the importance (screen space) is calculated for any given tile.
 
 This is set by default and when set we use the parent's bounding box to calculate importance for a given tile.
 */
@property (nonatomic) bool useParentTileBounds;

/** 
    Only load a single level at a time.
    
    When set, we'll only load one level of tiles at once.  This is very efficient for memory and fast for loading, but you'll see flashing as you move between levels.
    
    Off by default.
  */
@property (nonatomic) bool singleLevelLoading;

/** 
    Detail the levels you want loaded in target level mode.
    
    The image display can work in one of two modes, quad tree where it loads everything starting from the min level or a target level mode where it just tries to load one or more target levels.  This is the array that controls which levels it will try to load.
    
    We do this so that the user doesn't have to wait for the target level to load.  This can be distracting on large displays with small tiles.  If you use this mode, the layer will load lower levels first, filling in quicker and then load the target level.  This looks much better, but doesn't take as long as the full quad tree based loading.
    
    The layer calculates the optimal target level (for 2D maps, if you're in that mode).  The entries in this array are relative to that level or absolute.  For example [0,-4,-2] means the layer will always try to load levels 0, targetLevel-4 and targetLevel-2, but only the latter two if they make sense.
  */
@property (nonatomic,strong,nullable) NSArray *multiLevelLoads;

/** 
    The target zoom level for this layer given the current view settings.
    
    Calculates the target zoom level for the middle of the screen.
    
    This only makes sense for flat maps that use the same coordinate system we're using in this tile source.  In addition, the viewer can't have a tilt or any non-2D transform in the view matrix.  If it does, this is meaningless, but it'll return a number anyway.
    
    If all those conditions are met then we can say we're only displaying a single zoom level and this is that.
  */
- (int)targetZoomLevel;

/** 
    Set the number of samples for each tile per level.
    
    When sampling the globe without an elevation input, we'll divide each tile into a given number of samples.  By default this is 10x10.  Setting this dictionary will let you control that sampling per level.
    
    Each entry in the dictionary is an NSNumber for the level (-1 for default).  The corresponding value is an NSNumber for the sampling (e.g. 10).
    
    Call this immediately after setting up the layer or it won't take effect.
  */
- (void)setTesselationValues:(NSDictionary * __nonnull)tessDict;

/** 
    Force a full reload of all tiles.
 
    This will notify the system to flush out all the existing tiles and start reloading from the top.  If everything is cached locally (and the MaplyTileSource objects say so) then this should appear instantly.  If something needs to be fetched or it's taking too long, you'll see these page in from the low to the high level.
 
    This is good for tile sources, like weather, that need to be refreshed every so often.
 */
- (void)reload;

/** 
    Pass back the loaded image(s) for a given tile.
    
    If the tile source implements startFetchForTile: then we'll expect it to do the asynchronous loading.  When it's done loading an image, it calls this.
    
    When we're loading just one image per tile, call this with a UIImage or MaplyImageTile. If we're expecting multiple images (see: imageDepth) then pass in a MaplyImageTile that's been set up appropriately.
    
    @param images Either one of UIImage or MaplyPlaceholderImage.
    
    @param tileID The tile we've loaded.
 */
- (bool)loadedImages:(id __nonnull)images forTile:(MaplyTileID)tileID;

/** 
    Pass back the loaded image(s) for a given tile.
    
    If the tile source implements startFetchForTile: then we'll expect it to do the asynchronous loading.  When it's done loading an image, it calls this.
    
    When we're loading just one image per tile, call this with a UIImage or MaplyImageTile. If we're expecting multiple images (see: imageDepth) then pass in a MaplyImageTile that's been set up appropriately.
    
    @param images Either one of UIImage or MaplyPlaceholderImage.
    
    @param tileID The tile we've loaded.
    
    @param frame If we're loading an animation frame by frame, this is the frame ID.
  */
- (bool)loadedImages:(id __nonnull)images forTile:(MaplyTileID)tileID frame:(int)frame;

- (void)loadedElevation:(MaplyElevationChunk *__nonnull)elevChunk forTile:(MaplyTileID)tileID;
- (void)loadedElevation:(MaplyElevationChunk *__nonnull)elevChunk forTile:(MaplyTileID)tileID frame:(int)frame;

/** 
    Pass back an error for a given tile.
    
    If the tile source implements startFetchForTile: then this is how it tells us about a specific failure.
    
    It can also just call loadedImages:forTile: with nil, but this is more helpful.
  */
- (void)loadError:(NSError *__nullable)error forTile:(MaplyTileID)tileID;

/** 
    Pass back an error for a given tile and frame (if we're loading animations).
    
    If the tile source implements startFetchForTile: then this is how it tells us about a specific failure.
    
    It can also just call loadedImages:forTile: with nil, but this is more helpful.
 */
- (void)loadError:(NSError *__nullable)error forTile:(MaplyTileID)tileID frame:(int)frame;

/** 
    Status structures describing which frames are loaded.
    
    Query this to find out which frames are completely loaded into memory and which are not.
    
    This queries the underlying control logic and there is no delegate.  It's polling only.
 */
- (nullable NSArray *)loadedFrames;

/** 
    Return the bounding box for a given tile.
 
    This calculates the bounding box (in geographic) for the given tile.
 */
- (MaplyBoundingBox)geoBoundsForTile:(MaplyTileID)tileID;

/** 
    Return the bounding box for a given tile.
    
    This calculates the bounding box (in geographic) for the given tile.
  */
- (void)geoBoundsForTile:(MaplyTileID)tileID bbox:(MaplyBoundingBox *__nonnull)bbox;

/** 
    Return the bounding box for a given tile.
 
    This calculate the bounding box in local coordinates for the given tile.
 */
- (MaplyBoundingBox)boundsForTile:(MaplyTileID)tileID;

/** 
    Return the bounding box for a given tile.
    
    This calculate the bounding box in local coordinates for the given tile.
  */
- (void)boundsForTile:(MaplyTileID)tileID bbox:(MaplyBoundingBox *__nonnull)bbox;

/** 
    Do a hard reset of the layer.
    
    This will clean out all the layers resources and force it to start loading again.
    
    Call this right after you change a tile source.  This lets you change the tile source to something incompatible with the previous one.
  */
- (void)reset;

@end

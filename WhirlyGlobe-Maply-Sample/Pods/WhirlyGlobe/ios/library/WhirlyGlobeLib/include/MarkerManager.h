/*
 *  MarkerManager.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 7/16/13.
 *  Copyright 2011-2017 mousebird consulting. All rights reserved.
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

#import <math.h>
#import <set>
#import <map>
#import "Identifiable.h"
#import "BasicDrawable.h"
#import "DataLayer.h"
#import "LayerThread.h"
#import "TextureAtlas.h"
#import "SelectionManager.h"
#import "LayoutManager.h"
#import "Scene.h"
#import "BaseInfo.h"

namespace WhirlyKit
{
    /// Default priority for markers.  At the end, basically
    static const int MarkerDrawPriority=10000;
    
    /// Maximum number of triangles we'll stick in a drawable
    static const int MaxMarkerDrawableTris=1<<15/3;
}

namespace WhirlyKit
{
    
/// Marker representation.
/// Used internally to track marker resources
class MarkerSceneRep : public Identifiable
{
public:
    MarkerSceneRep();
    ~MarkerSceneRep() { };

    // Clear the contents out of the scene
    void clearContents(SelectionManager *selectManager,LayoutManager *layoutManager,SimpleIdentity generatorId,SimpleIdentity screenGenId,ChangeSet &changes,NSTimeInterval when);
    
    // Enable/disable marker related features
    void enableContents(SelectionManager *selectManager,LayoutManager *layoutManager,SimpleIdentity generatorId,SimpleIdentity screenGenId,bool enable,ChangeSet &changes);

    SimpleIDSet drawIDs;  // Drawables created for this
    SimpleIDSet selectIDs;  // IDs used for selection
    SimpleIDSet screenShapeIDs;  // IDs for screen space objects
    bool useLayout;  // True if we used the layout manager (and thus need to delete)
    float fadeOut;   // Time to fade away for deletion
};
typedef std::set<MarkerSceneRep *,IdentifiableSorter> MarkerSceneRepSet;
    
}

// Used to pass marker information between threads
@interface WhirlyKitMarkerInfo : WhirlyKitBaseInfo

@property (nonatomic) NSArray *markers;
@property (nonatomic) UIColor *color;
@property (nonatomic) bool screenObject;
@property (nonatomic) float width,height;
@property (nonatomic) WhirlyKit::SimpleIdentity  markerId;
@property (nonatomic) int clusterGroup;

- (id)initWithMarkers:(NSArray *)markers desc:(NSDictionary *)desc;

- (void)parseDesc:(NSDictionary *)desc;

@end

/** WhirlyGlobe Marker
 A single marker object to be placed on the globe.  It will show
 up with the given width and height and be selectable if so desired.
 */
@interface WhirlyKitMarker : NSObject

/// If set, this marker should be made selectable
///  and it will be if the selection layer has been set
@property (nonatomic,assign) bool isSelectable;
/// If the marker is selectable, this is the unique identifier
///  for it.  You should set this ahead of time
@property (nonatomic,assign) WhirlyKit::SimpleIdentity selectID;
/// The location for the center of the marker.
@property (nonatomic,assign) WhirlyKit::GeoCoord loc;
/// Set if we're moving these over time (screen only)
@property (nonatomic,assign) bool hasMotion;
/// Set for animation over time
@property (nonatomic,assign) WhirlyKit::GeoCoord endLoc;
/// Timing for animation, if present
@property (nonatomic,assign) NSTimeInterval startTime,endTime;
/// Color for this marker
@property (nonatomic) UIColor *color;
/// The list of textures to use.  If there's just one
///  we show that.  If there's more than one, we switch
///  between them over the period.
@property (nonatomic,assign) std::vector<WhirlyKit::SimpleIdentity> &texIDs;
@property (nonatomic,assign) bool lockRotation;
/// The height in 3-space (remember the globe has radius = 1.0)
@property (nonatomic,assign) float height;
/// The width in 3-space (remember the globe has radius = 1.0)
@property (nonatomic,assign) float width;
/// Height in screen space to consider for layout
@property (nonatomic,assign) float layoutHeight;
/// Width in screen space to soncider for layout
@property (nonatomic,assign) float layoutWidth;
/// Set if we want a static rotation.  Only matters in screen space
/// This is rotation clockwise from north in radians
@property (nonatomic,assign) float rotation;
/// Offset in points
@property (nonatomic,assign) WhirlyKit::Point2d offset;
/// The period over which we'll switch textures
@property (nonatomic,assign) NSTimeInterval period;
/// For markers with more than one texture, this is the offset
///  we'll use when calculating position within the period.
@property (nonatomic,assign) NSTimeInterval timeOffset;
/// Value to use for the layout engine.  Set to MAXFLOAT by
///  default, which will always display.
@property (nonatomic,assign) float layoutImportance;
/// A list of vertex attributes to apply to the marker
@property (nonatomic,assign) WhirlyKit::SingleVertexAttributeSet &vertexAttrs;

/// Add a texture ID to be displayed
- (void)addTexID:(WhirlyKit::SimpleIdentity)texID;

@end

namespace WhirlyKit
{
    
#define kWKMarkerManager "WKMarkerManager"

/** The Marker Manager is used to create and destroy geometry for 2D and 3D markers.
    It's entirely thread safe except for the 
  */
class MarkerManager : public SceneManager
{
public:
    MarkerManager();
    virtual ~MarkerManager();
    
    /// Add an array of markers, returning the identity that corresponds
    SimpleIdentity addMarkers(NSArray *markers,NSDictionary *desc,ChangeSet &changes);
    
    /// Remove the given set of markers
    void removeMarkers(SimpleIDSet &markerIDs,ChangeSet &changes);
    
    /// Enable/disable markers
    void enableMarkers(SimpleIDSet &markerIDs,bool enable,ChangeSet &changes);
    
    /// Called by the scene once things are set up
    virtual void setScene(Scene *inScene);
    
protected:
    pthread_mutex_t markerLock;
    /// Resources associated with given markers
    MarkerSceneRepSet markerReps;
    /// ID for the marker generator
    WhirlyKit::SimpleIdentity generatorId;
    /// Screen space generator on the render side
    WhirlyKit::SimpleIdentity screenGenId;
};

}

/*
 *  LabelManager.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 7/22/13.
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

#import <math.h>
#import <set>
#import <map>
#import "Identifiable.h"
#import "BasicDrawable.h"
#import "DataLayer.h"
#import "LayerThread.h"
#import "TextureAtlas.h"
#import "SelectionManager.h"
#import "LayoutLayer.h"
#import "LabelRenderer.h"

namespace WhirlyKit
{
    
/// Default for label draw priority
static const int LabelDrawPriority=1000;

/// Size of one side of the texture atlases built for labels
/// You can also specify this at startup
static const unsigned int LabelTextureAtlasSizeDefault = 512;

}

/** The Single Label represents one label with its text, location,
 and an NSDictionary that can be used to override some attributes.
 In general we don't want to create just one label, we want to
 create a large number of labels at once.  We use an array of
 these single labels to do that.
 */
@interface WhirlyKitSingleLabel : NSObject

/// If set, this marker should be made selectable
///  and it will be if the selection layer has been set
@property (nonatomic,assign) bool isSelectable;
/// If the marker is selectable, this is the unique identifier
///  for it.  You should set this ahead of time
@property (nonatomic,assign) WhirlyKit::SimpleIdentity selectID;
/// The text we want to see
@property (nonatomic,retain) NSString *text;
/// A geolocation for the middle, left or right of the label
///  depending on the justification
@property (nonatomic,assign) WhirlyKit::GeoCoord loc;
/// Set if we're moving these over time (screen only)
@property (nonatomic,assign) bool hasMotion;
/// Set for animation over time
@property (nonatomic,assign) WhirlyKit::GeoCoord endLoc;
/// Timing for animation, if present
@property (nonatomic,assign) NSTimeInterval startTime,endTime;
/// Rotation around the origin
@property (nonatomic,assign) float rotation;
/// Keep a label oriented upright on the screen
@property (nonatomic,assign) bool keepUpright;
/// This dictionary contains overrides for certain attributes
///  for just this label.  Only width, height, icon, text color, and
///  background color supported.
@property (nonatomic,retain) NSDictionary *desc;
/// If non-zero, this is the texture to use as an icon
@property (nonatomic,assign) WhirlyKit::SimpleIdentity iconTexture;
/// If the texture is set and this is non-zero the size of the image
@property (nonatomic,assign) CGSize iconSize;
/// If set, this moves the label if displayed in screen (2D) mode
@property (nonatomic,assign) CGSize screenOffset;
/// If non-null, used to identify a set of labels of which only one should be displayed
@property (nonatomic,assign) std::string uniqueID;

/// This is used to sort out width and height from the defaults.  Pass
///  in the value of one and zero for the other and it will fill in the
///  missing one.
- (bool)calcWidth:(float *)width height:(float *)height defaultFont:(UIFont *)font;

/// This will calculate the real extents in 3D over the globe.
/// Pass in an array of 3 point3f structures for the points and
///  normals.  The corners are returned in counter-clockwise order.
/// This is used for label selection
- (void)calcExtents:(NSDictionary *)topDesc corners:(WhirlyKit::Point3f *)pts norm:(WhirlyKit::Point3f *)norm coordAdapter:(WhirlyKit::CoordSystemDisplayAdapter *)coordAdapter;

/// Slightly more specific version
- (void)calcExtents2:(float)width2 height2:(float)height2 iconSize:(WhirlyKit::Point2f)iconSize justify:(WhirlyKitLabelJustify)justify corners:(WhirlyKit::Point3f *)pts norm:(WhirlyKit::Point3f *)norm iconCorners:(WhirlyKit::Point3f *)iconPts coordAdapter:(WhirlyKit::CoordSystemDisplayAdapter *)coordAdapter;

/// This version is for screen space labels
- (void)calcScreenExtents2:(float)width2 height2:(float)height2 iconSize:(WhirlyKit::Point2f)iconSize justify:(WhirlyKitLabelJustify)justify corners:(WhirlyKit::Point3f *)pts iconCorners:(WhirlyKit::Point3f *)iconPts useIconOffset:(bool)useIconOffset;

@end


namespace WhirlyKit
{
    
#define kWKLabelManager "WKLabelManager"

/** The label manager controls resources for text labels, including construction
    and destruction.  All methods (other than the destructor) are thread safe.
  */
class LabelManager : public SceneManager
{
public:
    LabelManager();
    virtual ~LabelManager();

    /// Add the given set of labels, returning an ID that represents the whole thing
    SimpleIdentity addLabels(NSArray *labels,NSDictionary *desc,ChangeSet &changes);
    
    /// Change visual attributes (just the visibility range)
    void changeLabel(SimpleIdentity labelID,NSDictionary *desc,ChangeSet &changes);
    
    /// Remove the given label(s)
    void removeLabels(SimpleIDSet &labelID,ChangeSet &changes);
    
    /// Enable/disable labels
    void enableLabels(SimpleIDSet labelID,bool enable,ChangeSet &changes);
    
protected:
    pthread_mutex_t labelLock;
    
    /// If set, we're using font textures instead of rendering each piece of text
    bool useFontManager;
    /// Keep track of labels (or groups of labels) by ID for deletion
    WhirlyKit::LabelSceneRepSet labelReps;
    unsigned int textureAtlasSize;
};

}

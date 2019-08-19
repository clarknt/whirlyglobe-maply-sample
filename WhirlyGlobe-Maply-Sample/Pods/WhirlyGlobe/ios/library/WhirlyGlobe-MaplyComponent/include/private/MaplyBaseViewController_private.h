/*
 *  MaplyBaseViewController_private.h
 *  MaplyComponent
 *
 *  Created by Steve Gifford on 12/14/12.
 *  Copyright 2012-2017 mousebird consulting
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

#import "MaplyBaseViewController.h"
#import "MaplyViewControllerLayer_private.h"
#import "MaplyComponentObject_private.h"
#import "WGInteractionLayer_private.h"
#import "PanDelegateFixed.h"
#import "PinchDelegateFixed.h"
#import "MaplyBaseInteractionLayer_private.h"
#import "MaplyVectorObject_private.h"
#import "MaplyShader_private.h"
#import "MaplyActiveObject_private.h"
#import "MaplyCoordinateSystem_private.h"
#import "MaplyCluster.h"
#import "SMCalloutView.h"
#import "Maply3dTouchPreviewDelegate.h"
#import "MaplyRenderController_private.h"
#import "MaplyRemoteTileFetcher.h"

@interface MaplyBaseViewController() <SMCalloutViewDelegate>
{
@public
    MaplyRenderController *renderControl;
    
    WhirlyKitEAGLView *glView;
    
    WhirlyKitLayerThread *baseLayerThread;
    WhirlyKitLayoutLayer *layoutLayer;
    WhirlyKitParticleSystemLayer *partSysLayer;
    NSMutableArray *layerThreads;

    // Layers (and associated data) created for the user
    NSMutableArray *userLayers;
    
    // List of views we're tracking for location
    NSMutableArray *viewTrackers;
    
    // List of annotations we're tracking for location
    NSMutableArray *annotations;
    
    /// A pointer to the 3D view.  The subclasses are keeping points with the right subclass.
    WhirlyKitView *visualView;
    
    /// Active models
    NSMutableArray *activeObjects;
    
    /// The default cluster generator (group 0)
    MaplyBasicClusterGenerator *defaultClusterGenerator;
    
    /// Current draw priority if we're assigning them ourselves
    int layerDrawPriority;
    
    /// Set if we're dumping out performance output
    bool _performanceOutput;
    
    /// Set while we're trying to track foreground/background
    bool wasAnimating;
    
    /// When an annotation comes up we may want to reposition the view.  This works poorly in some cases.
    bool allowRepositionForAnnnotations;
    
    /// Number of simultaneous tile fetcher connections (per tile fetcher)
    int tileFetcherConnections;
  
    /// 3dtouch preview context, so we can remove it.
    id <UIViewControllerPreviewing> previewingContext;
  
    /// Need to keep a ref to this because the system keeps a weak ref
    Maply3dTouchPreviewDelegate *previewTouchDelegate;
    
    /// Shared sampling layers (used for loaders)
    std::vector<MaplyQuadSamplingLayer *> samplingLayers;
    
    /// Shared tile fetcher used by default for loaders
    std::vector<MaplyRemoteTileFetcher *> tileFetchers;
}

/// This is called by the subclasses.  Don't call it yourself.
- (void) clear;

/// LoadSetup is where the Component does all the WhirlyGlobe/Maply specific setup.  If you override this,
///  be sure to call [super loadSetup] first and then do your thing.
- (void) loadSetup;

/// Create the EAGLView
- (void) loadSetup_glView;

/// If you have your own WhirlyGlobeView or MaplyView subclass, set it up here
- (WhirlyKitView *) loadSetup_view;

/// For loading the Maply or Globe view.  The subclasses call this, but you shouldn't
- (WhirlyKit::Scene *) loadSetup_scene;

/// Override this to set up the default lighting scheme (e.g. the shaders).
/// The base class provides an adequate default
- (void) loadSetup_lighting;

/// The base classes fill this in to return their own interaction layer subclass
- (MaplyBaseInteractionLayer *) loadSetup_interactionLayer;

/// Make the renderer's GL context active.  This is used internally.
- (void) useGLContext;

/// Called internally to mark a block of work being done
- (bool) startOfWork;

/// Called internally to end a block of work being done
- (void) endOfWork;

/// Look for a sampling layer that matches the given parameters
/// We'll also keep it around until the user lets us know we're done
- (MaplyQuadSamplingLayer *)findSamplingLayer:(MaplySamplingParams *)params forUser:(NSObject *)userObj;

/// The given user object is done with the given sampling layer.  So we may shut it down.
- (void)releaseSamplingLayer:(MaplyQuadSamplingLayer *)layer forUser:(NSObject *)userObj;

@end

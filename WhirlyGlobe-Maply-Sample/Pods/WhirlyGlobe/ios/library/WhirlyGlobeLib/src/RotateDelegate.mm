/*
 *  RotateDelegate.mm
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 6/10/11.
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

#import "WhirlyVector.h"
#import "RotateDelegate.h"
#import "EAGLView.h"
#import "SceneRendererES.h"
#import "IntersectionManager.h"

using namespace Eigen;
using namespace WhirlyKit;

@implementation WhirlyGlobeRotateDelegate
{
    bool valid;
    WhirlyGlobeView *globeView;
	WhirlyKit::Point3d startOnSphere;
	/// The view transform when we started
	Eigen::Matrix4d startTransform;
    /// Starting point for rotation
    Eigen::Quaterniond startQuat;
    /// Axis to rotate around
//    Eigen::Vector3d axis;
    float startRot;
    double sphereRadius;
}

- (id)initWithGlobeView:(WhirlyGlobeView *)inView
{
	if ((self = [super init]))
	{
		globeView = inView;
        valid = false;
        _rotateAroundCenter = true;
        sphereRadius = 1.0;
	}
	
	return self;
}

+ (WhirlyGlobeRotateDelegate *)rotateDelegateForView:(UIView *)view globeView:(WhirlyGlobeView *)globeView
{
	WhirlyGlobeRotateDelegate *rotateDelegate = [[WhirlyGlobeRotateDelegate alloc] initWithGlobeView:globeView];
    UIRotationGestureRecognizer *rotateRecog = [[UIRotationGestureRecognizer alloc] initWithTarget:rotateDelegate action:@selector(rotateGesture:)];
    rotateRecog.delegate = rotateDelegate;
	[view addGestureRecognizer:rotateRecog];
    rotateDelegate.gestureRecognizer = rotateRecog;
	return rotateDelegate;
}

// We'll let other gestures run
// We're expecting to run as well
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return TRUE;
}

- (void)updateWithCenter:(CGPoint)center touch:(CGPoint)touch glView:(WhirlyKitEAGLView *)glView
{
	WhirlyKitSceneRendererES *sceneRender = glView.renderer;

    if (valid)
    {
//        NSLog(@"Rotate updated");
        Eigen::Quaterniond newRotQuat = startQuat;
        Point3d axis = [globeView currentUp];
        if (_rotateAroundCenter)
        {
            // Figure out where we are now
            // We have to roll back to the original transform with the current height
            //  to get the rotation we want
            Point3d hit;
            Eigen::Quaterniond oldQuat = globeView.rotQuat;
            [globeView setRotQuat:startQuat updateWatchers:false];
            Eigen::Matrix4d curTransform = [globeView calcFullMatrix];
            if ([globeView pointOnSphereFromScreen:center transform:&curTransform
                                         frameSize:Point2f(sceneRender.framebufferWidth/glView.contentScaleFactor,sceneRender.framebufferHeight/glView.contentScaleFactor)
                                               hit:&hit normalized:true radius:sphereRadius])
            {
                // This gives us a direction to rotate around
                // And how far to rotate
                Eigen::Quaterniond endRot;
                endRot.setFromTwoVectors(startOnSphere,hit);
                axis = hit.normalized();
                newRotQuat = startQuat * endRot;
            } else {
                newRotQuat = oldQuat;
            }
        }
        
        // And do a rotation around the pinch
        CGPoint touch0 = touch;
        float dx = touch0.x-center.x,dy=touch0.y-center.y;
        double curRot = atan2(dy, dx);
        double diffRot = curRot-startRot;
        Eigen::AngleAxisd rotQuat(-diffRot,axis);
        newRotQuat = newRotQuat * rotQuat;
        
        [globeView setRotQuat:(newRotQuat) updateWatchers:false];
    }
}

// Called for rotate actions
- (void)rotateGesture:(id)sender
{
	UIRotationGestureRecognizer *rotate = sender;
	WhirlyKitEAGLView  *glView = (WhirlyKitEAGLView  *)rotate.view;
	WhirlyKitSceneRendererES *sceneRender = glView.renderer;
    
    // Turn off rotation if we fall below two fingers
    if ([rotate numberOfTouches] < 2)
    {
        if (valid)
            [[NSNotificationCenter defaultCenter] postNotificationName:kRotateDelegateDidEnd object:globeView];
        valid = false;
        return;
    }

    IntersectionManager *intManager = (IntersectionManager *)sceneRender.scene->getManager(kWKIntersectionManager);

	switch (rotate.state)
	{
		case UIGestureRecognizerStateBegan:
        {
//            NSLog(@"Rotate started");
            [globeView cancelAnimation];

			startTransform = [globeView calcFullMatrix];
            startQuat = [globeView rotQuat];
            valid = true;
            CGPoint startPoint = [rotate locationInView:glView];
            
            // Look for an intersection with grabbable objects
            Point3d interPt;
            double interDist;
            if (intManager->findIntersection(sceneRender, globeView, Point2f(sceneRender.framebufferWidth/glView.contentScaleFactor,sceneRender.framebufferHeight/glView.contentScaleFactor), Point2f(startPoint.x,startPoint.y), interPt, interDist))
            {
                sphereRadius = interPt.norm();
                startOnSphere = interPt.normalized();
                valid = true;
            } else {
                if ([globeView pointOnSphereFromScreen:[rotate locationInView:glView] transform:&startTransform
                                             frameSize:Point2f(sceneRender.framebufferWidth/glView.contentScaleFactor,sceneRender.framebufferHeight/glView.contentScaleFactor)
                                                   hit:&startOnSphere normalized:true])
                    valid = true;
                else
                    valid = false;
            }
            
            CGPoint center = [rotate locationInView:glView];
            CGPoint touch0 = [rotate locationOfTouch:0 inView:glView];
            float dx = touch0.x-center.x,dy=touch0.y-center.y;
            startRot = atan2(dy, dx);
            
            if (valid)
                [globeView cancelAnimation];
            
            [[NSNotificationCenter defaultCenter] postNotificationName:kRotateDelegateDidStart object:globeView];
        }
			break;
		case UIGestureRecognizerStateChanged:
        {
            [globeView cancelAnimation];

            if (valid)
            {
//                NSLog(@"Rotate updated");
                Eigen::Quaterniond newRotQuat = startQuat;
                Point3d axis = [globeView currentUp];
                if (_rotateAroundCenter)
                {
                    // Figure out where we are now
                    // We have to roll back to the original transform with the current height
                    //  to get the rotation we want
                    Point3d hit;
                    Eigen::Quaterniond oldQuat = globeView.rotQuat;
                    [globeView setRotQuat:startQuat updateWatchers:false];
                    Eigen::Matrix4d curTransform = [globeView calcFullMatrix];
                    if ([globeView pointOnSphereFromScreen:[rotate locationInView:glView] transform:&curTransform
                                                 frameSize:Point2f(sceneRender.framebufferWidth/glView.contentScaleFactor,sceneRender.framebufferHeight/glView.contentScaleFactor)
                                                       hit:&hit normalized:true radius:sphereRadius])
                    {
                        // This gives us a direction to rotate around
                        // And how far to rotate
                        Eigen::Quaterniond endRot;
                        endRot.setFromTwoVectors(startOnSphere,hit);
                        axis = hit.normalized();
                        newRotQuat = startQuat * endRot;
                    } else {
                        newRotQuat = oldQuat;
                    }
                }
                
                // And do a rotation around the pinch
                CGPoint center = [rotate locationInView:glView];
                CGPoint touch0 = [rotate locationOfTouch:0 inView:glView];
                float dx = touch0.x-center.x,dy=touch0.y-center.y;
                double curRot = atan2(dy, dx);
                double diffRot = curRot-startRot;
                Eigen::AngleAxisd rotQuat(-diffRot,axis);
                newRotQuat = newRotQuat * rotQuat;
                
                [globeView setRotQuat:(newRotQuat) updateWatchers:true];
            }
        }
			break;
        case UIGestureRecognizerStateFailed:
        case UIGestureRecognizerStateCancelled:
        case UIGestureRecognizerStateEnded:
//            NSLog(@"Rotate ended");
            [[NSNotificationCenter defaultCenter] postNotificationName:kRotateDelegateDidEnd object:globeView];
            valid = false;
            break;
        default:
            break;
	}
}

@end

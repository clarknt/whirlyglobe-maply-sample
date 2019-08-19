/*
 *  AnimateViewMomentum.mm
 *  WhirlyGlobeApp
 *
 *  Created by Steve Gifford on 5/23/11.
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

#import <algorithm>
#import "AnimateViewMomentum.h"

using namespace Eigen;

@implementation AnimateViewMomentumMessage

- (id)initWithGlobeView:(WhirlyGlobeView *)globeView rot:(Quaterniond &)endRot time:(NSTimeInterval)endTime
{
    self = [super init];
    _globeView = globeView;
    _rot = endRot;
    _endTime = endTime;
    
    return self;
}

@end

@implementation AnimateViewMomentum
{
    Eigen::Quaterniond startQuat;
    Eigen::Vector3d axis;
    float maxTime;
    CFTimeInterval startDate;
}

- (id)initWithView:(WhirlyGlobeView *)globeView velocity:(float)inVel accel:(float)inAcc axis:(Vector3f)inAxis northUp:(bool)northUp
{
    if ((self = [super init]))
    {
        _velocity = inVel;
        _acceleration = inAcc;
        _northUp = northUp;
        axis = Vector3d(inAxis.x(),inAxis.y(),inAxis.z());
        startQuat = [globeView rotQuat];
        
        startDate = CFAbsoluteTimeGetCurrent();
        
        // Let's calculate the maximum time, so we know when to stop
        if (_acceleration != 0.0)
        {
            maxTime = 0.0;
            if (_acceleration != 0.0)
                maxTime = -_velocity / _acceleration;
            maxTime = std::max(0.f,maxTime);
            
            if (maxTime == 0.0)
                startDate = 0;
        } else
            maxTime = MAXFLOAT;
        
        // If this is going to end, let people know when
        if (maxTime < MAXFLOAT)
        {
//            Quaterniond endRot = [self rotForTime:maxTime];
//            AnimateViewMomentumMessage *msg = [[AnimateViewMomentumMessage alloc] initWithGlobeView:globeView rot:endRot time:startDate+maxTime];
        }
    }
    
    return self;
}

- (Quaterniond)rotForTime:(NSTimeInterval)sinceStart
{
    // Calculate the offset based on angle
    float totalAng = (_velocity + 0.5 * _acceleration * sinceStart) * sinceStart;
    Eigen::Quaterniond diffRot(Eigen::AngleAxisd(totalAng,axis));
    Eigen::Quaterniond newQuat;
    newQuat = startQuat * diffRot;

    if (_northUp)
    {
        // We'd like to keep the north pole pointed up
        // So we look at where the north pole is going
        Vector3d northPole = (newQuat * Vector3d(0,0,1)).normalized();
        if (northPole.y() != 0.0)
        {
            // We need to know where up (facing the user) will be
            //  so we can rotate around that
            Vector3d newUp = [WhirlyGlobeView prospectiveUp:newQuat];
            
            // Then rotate it back on to the YZ axis
            // This will keep it upward
            float ang = atan(northPole.x()/northPole.y());
            
            // However, the pole might be down now
            // If so, rotate it back up
            if (northPole.y() < 0.0)
                ang += M_PI;
            Eigen::AngleAxisd upRot(ang,newUp);
            
            newQuat = (newQuat * upRot).normalized();
        }
    }

    return newQuat;
}

// Called by the view when it's time to update
- (void)updateView:(WhirlyGlobeView *)globeView
{
    if (startDate == 0.0)
        return;
    
	float sinceStart = CFAbsoluteTimeGetCurrent()-startDate;
    
    if (sinceStart > maxTime)
    {
        // This will snap us to the end and then we stop
        sinceStart = maxTime;
        startDate = 0;
    }
    
    Quaterniond newQuat = [self rotForTime:sinceStart];
    [globeView setRotQuat:newQuat];
    
    // Make sure to cancel the animation *after* we set the new rotation (duh)
    if (startDate == 0)
        [globeView cancelAnimation];
}

@end

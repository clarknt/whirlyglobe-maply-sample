/*
 *  ScreenSpaceDrawable.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 8/24/14.
 *  Copyright 2011-2017 mousebird consulting.
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

#import "BasicDrawable.h"

namespace WhirlyKit
{
    
// Shader name
#define kScreenSpaceShaderName "Screen Space Shader"
#define kScreenSpaceShaderMotionName "Screen Space Shader Motion"
    
/// Construct and return the Screen Space shader program
OpenGLES2Program *BuildScreenSpaceProgram();
OpenGLES2Program *BuildScreenSpaceMotionProgram();

/// Wrapper for building screen space drawables
class ScreenSpaceDrawable : public BasicDrawable
{
public:
    // Construct with or without motion support
    ScreenSpaceDrawable(bool hasMotion,bool hasRotation);
    
    // If we've got a rotation, we set this to keep the image facing upright
    //  probably because it's text.
    void setKeepUpright(bool keepUpright);
    // Time we start counting from for motion
    void setStartTime(NSTimeInterval inStartTime) { startTime = inStartTime; }
    // Time we start counting from for motion
    NSTimeInterval getStartTime() { return startTime; }
    
    // Each vertex has an offset on the screen
    void addOffset(const Point2f &offset);
    void addOffset(const Point2d &offset);

    // Add a direction to the attribute list (for animation)
    void addDir(const Point3f &dir);
    void addDir(const Point3d &dir);
    
    // Add a rotation vector to the attribute list
    void addRot(const Point3f &dir);
    void addRot(const Point3d &dir);

    /// If we have motion we need to force the render to keep rendering
    virtual void updateRenderer(WhirlyKitSceneRendererES *renderer);
    
    /// We override draw so we can set our own values
    virtual void draw(WhirlyKitRendererFrameInfo *frameInfo,Scene *scene);

protected:
    bool motion,rotation;
    bool keepUpright;
    int offsetIndex;
    int dirIndex;
    int rotIndex;
    NSTimeInterval startTime;
};
    
}

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "emess.h"
#import "geocent.h"
#import "geodesic.h"
#import "nad_list.h"
#import "org_proj4_PJ.h"
#import "org_proj4_Projections.h"
#import "pj_list.h"
#import "projects.h"
#import "proj_api.h"
#import "proj_config.h"

FOUNDATION_EXPORT double proj4VersionNumber;
FOUNDATION_EXPORT const unsigned char proj4VersionString[];


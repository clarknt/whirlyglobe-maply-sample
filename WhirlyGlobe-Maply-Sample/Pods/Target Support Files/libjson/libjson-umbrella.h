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

#import "JSONOptions.h"
#import "libjson.h"
#import "_internal/Dependencies/libbase64++/libbase64++.h"
#import "_internal/Dependencies/mempool++/mempool.h"
#import "_internal/Source/internalJSONNode.h"
#import "_internal/Source/JSONAllocator.h"
#import "_internal/Source/JSONChildren.h"
#import "_internal/Source/JSONDebug.h"
#import "_internal/Source/JSONDefs.h"
#import "_internal/Source/JSONDefs/GNU_C.h"
#import "_internal/Source/JSONDefs/Strings_Defs.h"
#import "_internal/Source/JSONDefs/Unknown_C.h"
#import "_internal/Source/JSONDefs/Visual_C.h"
#import "_internal/Source/JSONGlobals.h"
#import "_internal/Source/JSONMemory.h"
#import "_internal/Source/JSONMemoryPool.h"
#import "_internal/Source/JSONNode.h"
#import "_internal/Source/JSONPreparse.h"
#import "_internal/Source/JSONSharedString.h"
#import "_internal/Source/JSONSingleton.h"
#import "_internal/Source/JSONStats.h"
#import "_internal/Source/JSONStream.h"
#import "_internal/Source/JSONValidator.h"
#import "_internal/Source/JSONWorker.h"
#import "_internal/Source/JSON_Base64.h"
#import "_internal/Source/NumberToString.h"

FOUNDATION_EXPORT double libjsonVersionNumber;
FOUNDATION_EXPORT const unsigned char libjsonVersionString[];


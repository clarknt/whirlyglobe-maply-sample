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

#import "Maply3DTouchPreviewDatasource.h"
#import "Maply3dTouchPreviewDelegate.h"
#import "MaplyActiveObject.h"
#import "MaplyAerisTiles.h"
#import "MaplyAnimationTestTileSource.h"
#import "MaplyAnnotation.h"
#import "MaplyAtmosphere.h"
#import "MaplyBaseViewController.h"
#import "MaplyBillboard.h"
#import "MaplyBlankTileSource.h"
#import "MaplyCluster.h"
#import "MaplyColorRampGenerator.h"
#import "MaplyComponent.h"
#import "MaplyComponentObject.h"
#import "MaplyCoordinate.h"
#import "MaplyCoordinateSystem.h"
#import "MaplyElevationDatabase.h"
#import "MaplyElevationSource.h"
#import "MaplyGDALRetileSource.h"
#import "MaplyGeomBuilder.h"
#import "MaplyGeomModel.h"
#import "MaplyIconManager.h"
#import "MaplyImageTile.h"
#import "MaplyLabel.h"
#import "MaplyLAZQuadReader.h"
#import "MaplyLAZShader.h"
#import "MaplyLight.h"
#import "MaplyLocationTracker.h"
#import "MaplyMarker.h"
#import "MaplyMatrix.h"
#import "MaplyMBTileFetcher.h"
#import "MaplyMBTileSource.h"
#import "MaplyMoon.h"
#import "MaplyMultiplexTileSource.h"
#import "MaplyPagingElevationTestTileSource.h"
#import "MaplyPagingVectorTestTileSource.h"
#import "MaplyParticleSystem.h"
#import "MaplyPoints.h"
#import "MaplyQuadImageFrameLoader.h"
#import "MaplyQuadImageLoader.h"
#import "MaplyQuadImageOfflineLayer.h"
#import "MaplyQuadImageTilesLayer.h"
#import "MaplyQuadPagingLayer.h"
#import "MaplyQuadSampler.h"
#import "MaplyQuadTracker.h"
#import "MaplyRemoteTileElevationSource.h"
#import "MaplyRemoteTileFetcher.h"
#import "MaplyRemoteTileSource.h"
#import "MaplyRenderController.h"
#import "MaplyRenderTarget.h"
#import "MaplyScreenLabel.h"
#import "MaplyScreenMarker.h"
#import "MaplyScreenObject.h"
#import "MaplyShader.h"
#import "MaplyShape.h"
#import "MaplySharedAttributes.h"
#import "MaplySphericalQuadEarthWithTexGroup.h"
#import "MaplyStarsModel.h"
#import "MaplySticker.h"
#import "MaplySun.h"
#import "MaplyTexture.h"
#import "MaplyTextureBuilder.h"
#import "MaplyTileSource.h"
#import "MaplyTileSourceNew.h"
#import "MaplyUpdateLayer.h"
#import "MaplyVariableTarget.h"
#import "MaplyVectorObject.h"
#import "MaplyVertexAttribute.h"
#import "MaplyViewController.h"
#import "MaplyViewControllerLayer.h"
#import "MaplyViewTracker.h"
#import "MaplyWMSTileSource.h"
#import "NSData+Zlib.h"
#import "NSDictionary+StyleRules.h"
#import "WGCoordinate.h"
#import "WhirlyGlobeComponent.h"
#import "WhirlyGlobeViewController.h"
#import "GeoJSONSource.h"
#import "MapboxMultiSourceTileInfo.h"
#import "MapboxVectorImageInterpreter.h"
#import "MapboxVectorStyleBackground.h"
#import "MapboxVectorStyleFill.h"
#import "MapboxVectorStyleLine.h"
#import "MapboxVectorStyleRaster.h"
#import "MapboxVectorStyleSet.h"
#import "MapboxVectorStyleSymbol.h"
#import "MapboxVectorTiles.h"
#import "MapboxVectorTilesPagingDelegate.h"
#import "MaplyVectorStyle.h"
#import "MaplyVectorStyleSimple.h"
#import "MaplyVectorTileLineStyle.h"
#import "MaplyVectorTileMarkerStyle.h"
#import "MaplyVectorTilePolygonStyle.h"
#import "MaplyVectorTiles.h"
#import "MaplyVectorTileStyle.h"
#import "MaplyVectorTileTextStyle.h"
#import "MapnikStyle.h"
#import "MapnikStyleRule.h"
#import "MapnikStyleSet.h"
#import "SLDExpressions.h"
#import "SLDOperators.h"
#import "SLDStyleSet.h"
#import "SLDSymbolizers.h"
#import "SLDWellKnownMarkers.h"

FOUNDATION_EXPORT double WhirlyGlobeVersionNumber;
FOUNDATION_EXPORT const unsigned char WhirlyGlobeVersionString[];


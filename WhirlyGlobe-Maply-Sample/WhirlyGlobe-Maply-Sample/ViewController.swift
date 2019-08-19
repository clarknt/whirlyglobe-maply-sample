//
//  ViewController.swift
//  WhirlyGlobe-Maply-Sample
//
//  Created by clarknt on 2019-08-16.
//  Copyright © 2019 clarknt. All rights reserved.
//

import UIKit
import WhirlyGlobe

enum MapType {
    case map
    case globe
}

class ViewController: UIViewController {

    @IBOutlet weak var containerView: UIView!
    
    private var mapSourcesButtonItem: UIBarButtonItem!
    
    private var globeMapViewController: MaplyBaseViewController?

    private var mapSources = [MapSource]()
    private var currentMapSource: MapSource?
    
    override func viewDidLoad() {
        super.viewDidLoad()

        setupToolbar()
        
        setupMap()
    }
    
    func setupToolbar() {
        mapSourcesButtonItem = UIBarButtonItem(image: UIImage(named: "mapsources"), style: .plain, target: self, action: #selector(mapSourcesButtonTapped))
        
        toolbarItems = [mapSourcesButtonItem]
        navigationController?.isToolbarHidden = false
    }
    
    func setupMap() {
        globeMapViewController = WhirlyGlobeViewController() // globe
        //globeMapViewController = MaplyViewController() // map
        
        containerView.addSubview(globeMapViewController!.view)
        globeMapViewController!.view.frame = containerView.bounds
        
        if let globeViewController = globeMapViewController as? WhirlyGlobeViewController {
            globeViewController.keepNorthUp = true
        }
        
        addChild(globeMapViewController!)
        
        addLocalMaps()
        addRemoteMaps()
        
        if mapSources.count > 0 {
            let defaultIndex = 0
            currentMapSource = mapSources[defaultIndex]
            addBaseLayer(source: defaultIndex)
            animateToDefaultPosition()
        }

    }
    
    func addLocalMaps() {
        guard let path = Bundle.main.resourcePath else { return }

        let fm = FileManager.default
        guard let files = try? fm.contentsOfDirectory(atPath: path) else { return }
        
        // see README.md for credits
        for file in files {
            if file.hasSuffix(LocalMapSource.suffix) {
                let filename = String(file.dropLast(LocalMapSource.suffix.count))
                let name = filename.replacingOccurrences(of: "-", with: " ").capitalized
                mapSources.append(LocalMapSource(name: name, filename: filename))
            }
        }
    }
    
    func addRemoteMaps() {
        // see README.md for credits
        mapSources.append(RemoteMapSource(name: "Terrain", url: "https://stamen-tiles.a.ssl.fastly.net/terrain/{z}/{x}/{y}", ext: "png", maxZoom: 16))

        mapSources.append(RemoteMapSource(name: "Toner", url: "https://stamen-tiles.a.ssl.fastly.net/toner/{z}/{x}/{y}", ext: "png", maxZoom: 16))

        mapSources.append(RemoteMapSource(name: "Watercolor", url: "https://stamen-tiles.a.ssl.fastly.net/watercolor/{z}/{x}/{y}", ext: "jpg", maxZoom: 16))
    }
    
    func addBaseLayer(source: Int) {
        let globeViewController = globeMapViewController as? WhirlyGlobeViewController

        // black background for a globe, white background for a map
        globeMapViewController!.clearColor = (globeViewController != nil) ? UIColor.black : UIColor.white
        
        // 30 fps
        globeMapViewController!.frameInterval = 2
        
        // prepare layer
        let layer: MaplyQuadImageTilesLayer?
        
        if mapSources[source].type == .local {
            layer = getLocalLayer(source: mapSources[source] as! LocalMapSource)
        }
        else {
            layer = getRemoteLayer(source: mapSources[source] as! RemoteMapSource, mapType: .globe)
        }
        
        globeMapViewController!.removeAllLayers()
        if let layer = layer {
            globeMapViewController!.add(layer)
        }
    }
    
    func animateToDefaultPosition() {
        let globeViewController = globeMapViewController as? WhirlyGlobeViewController
        let mapViewController = globeMapViewController as? MaplyViewController

        // start up over New York City
        let nycLong: Float = -73.935242
        let nycLat: Float = 40.730610
        
        if let globeViewController = globeViewController {
            globeViewController.height = 1.2
            globeViewController.animate(toPosition: MaplyCoordinateMakeWithDegrees(nycLong, nycLat), time: 1.0)
        }
        else if let mapViewController = mapViewController {
            mapViewController.height = 1.0
            mapViewController.animate(toPosition: MaplyCoordinateMakeWithDegrees(nycLong, nycLat), time: 1.0)
        }
    }
    
    func getLocalLayer(source: LocalMapSource) -> MaplyQuadImageTilesLayer? {
        guard let tileSource = MaplyMBTileSource(mbTiles: source.filename) else {
            print("Could not load local tile set")
            return nil
        }
        
        return MaplyQuadImageTilesLayer(tileSource: tileSource)!
    }
    
    func getRemoteLayer(source: RemoteMapSource, mapType: MapType)  -> MaplyQuadImageTilesLayer? {
        guard let tileSource = MaplyRemoteTileSource(baseURL: source.url, ext: source.ext, minZoom: 0, maxZoom: source.maxZoom) else {
                print("Could not create remote tile source")
                return nil
        }

        // remote tile set: setup a cache directory
        let baseCacheDir = NSSearchPathForDirectoriesInDomains(.cachesDirectory, .userDomainMask, true)[0]
        let tilesCacheDir = "\(baseCacheDir)/\(source.name.replacingOccurrences(of: " ", with: ""))/"
        tileSource.cacheDir = tilesCacheDir
        
        let layer = MaplyQuadImageTilesLayer(tileSource: tileSource)!
        layer.handleEdges = (mapType == .globe)
        layer.coverPoles = (mapType == .globe)
        layer.requireElev = false
        layer.waitLoad = false
        layer.drawPriority = 0
        layer.singleLevelLoading = false
        
        return layer
    }
    
    @objc func mapSourcesButtonTapped(_ sender: Any) {
        let ac = UIAlertController(title: "Map source", message: nil, preferredStyle: .actionSheet)
        ac.popoverPresentationController?.barButtonItem = mapSourcesButtonItem
        for source in mapSources {
            let prefix = (source.name == currentMapSource?.name) ? "✓ " : ""
            let title = prefix + source.name
            let mapTypeAction = UIAlertAction(title: title, style: .default) { [weak self] _ in
                let index = self?.mapSources.firstIndex(where: { $0.name == source.name }) ?? 0
                self?.currentMapSource = source
                self?.addBaseLayer(source: index)
            }
            ac.addAction(mapTypeAction)
        }
        ac.addAction(UIAlertAction(title: "Cancel", style: .cancel))
        present(ac, animated: true)
    }
}


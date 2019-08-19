//
//  ViewController.swift
//  WhirlyGlobe-Maply-Sample
//
//  Created by clarknt on 2019-08-16.
//  Copyright © 2019 clarknt. All rights reserved.
//

import UIKit
import WhirlyGlobe

enum GlobeMapType {
    case map
    case globe
}

class MainViewController: UIViewController {

    @IBOutlet weak var containerView: UIView!
    
    private var tilesButtonItem: UIBarButtonItem!
    private var globeMapButtonItem: UIBarButtonItem!
    private var globeImage = UIImage(named: "globe")
    private var mapImage = UIImage(named: "map")

    private var globeMapViewController: MaplyBaseViewController?

    private var tilesSources = [TilesSource]()
    private var currentTileSource: TilesSource?
    
    private var globeMapType: GlobeMapType = .globe
    
    override func viewDidLoad() {
        super.viewDidLoad()

        title = "WhirlyGlobe-Maply - Sample"
        
        setupToolbar()
        
        populateLocalTiles()
        populateRemoteTiles()
        
        setupView()
        addDefaultBaseLayer()
  
        animateToDefaultPosition()
    }
    
    func setupToolbar() {
        globeMapButtonItem = UIBarButtonItem(image: mapImage, style: .plain, target: self, action: #selector(globeMapButtonTapped))

        tilesButtonItem = UIBarButtonItem(image: UIImage(named: "tiles"), style: .plain, target: self, action: #selector(tilesButtonTapped))
        
        toolbarItems = [globeMapButtonItem, tilesButtonItem]
        navigationController?.isToolbarHidden = false
    }
    
    func setupView() {
        globeMapViewController?.removeFromParent()
        
        if globeMapType == .globe {
            globeMapViewController = WhirlyGlobeViewController()
            // not always working if done immediately
            DispatchQueue.main.async { [weak self] in
                self?.getGlobe()?.keepNorthUp = true
            }
        }
        else {
            globeMapViewController = MaplyViewController(mapType: .typeFlat)
            getMap()?.rotateGesture = false
        }
        
        containerView.addSubview(globeMapViewController!.view)
        globeMapViewController!.view.frame = containerView.bounds

        getGlobe()?.height = 1.2
        getMap()?.height = .pi / 2

        addChild(globeMapViewController!)
    }
    
    func addDefaultBaseLayer() {
        addBaseLayer(name: "Geography Class")
    }
    
    func addBaseLayer(name: String) {
        if tilesSources.count > 0 {
            let defaultIndex = tilesSources.firstIndex(where: { $0.name == name }) ?? 0
            
            currentTileSource = tilesSources[defaultIndex]
            setBaseLayer(source: defaultIndex)
        }
    }
    
    func populateLocalTiles() {
        guard let path = Bundle.main.resourcePath else { return }

        let fm = FileManager.default
        guard let files = try? fm.contentsOfDirectory(atPath: path) else { return }
        
        // see README.md for credits
        for file in files {
            if file.hasSuffix(LocalTilesSource.suffix) {
                let filename = String(file.dropLast(LocalTilesSource.suffix.count))
                let name = filename.replacingOccurrences(of: "-", with: " ").capitalized
                tilesSources.append(LocalTilesSource(name: name, filename: filename))
            }
        }
    }
    
    func populateRemoteTiles() {
        // see README.md for credits
        tilesSources.append(RemoteTilesSource(name: "Terrain", url: "https://stamen-tiles.a.ssl.fastly.net/terrain/{z}/{x}/{y}", ext: "png", maxZoom: 16))

        tilesSources.append(RemoteTilesSource(name: "Toner", url: "https://stamen-tiles.a.ssl.fastly.net/toner/{z}/{x}/{y}", ext: "png", maxZoom: 16))

        tilesSources.append(RemoteTilesSource(name: "Watercolor", url: "https://stamen-tiles.a.ssl.fastly.net/watercolor/{z}/{x}/{y}", ext: "jpg", maxZoom: 16))
    }
    
    func setBaseLayer(source: Int) {
        // black background for a globe, white background for a map
        globeMapViewController!.clearColor = globeMapType == .globe ? UIColor.black : UIColor.white
        
        // 30 fps
        globeMapViewController!.frameInterval = 2
        
        // prepare layer
        let layer: MaplyQuadImageTilesLayer?
        
        if tilesSources[source].type == .local {
            layer = getLocalLayer(source: tilesSources[source] as! LocalTilesSource)
        }
        else {
            layer = getRemoteLayer(source: tilesSources[source] as! RemoteTilesSource, globeMapType: .globe)
        }
        
        globeMapViewController!.removeAllLayers()
        if let layer = layer {
            globeMapViewController!.add(layer)
        }
    }
    
    func animateToDefaultPosition() {
        // start up over New York City
        let nycLong: Float = -73.935242
        let nycLat: Float = 40.730610
        
        animateToPosition(position: MaplyCoordinateMakeWithDegrees(nycLong, nycLat), time: 1.0)
    }
    
    func animateToPosition(position: MaplyCoordinate, time: TimeInterval) {
        if globeMapType == .globe {
            // wait for keepNorthUp to apply
            DispatchQueue.main.async { [weak self] in
                self?.getGlobe()?.animate(toPosition: position, time: time)
            }
        }
        else {
            getMap()?.animate(toPosition: position, time: time)
        }
    }
    
    func setPosition(position: MaplyCoordinate) {
        // preferring animate to setPosition result
        animateToPosition(position: position, time: 0.01)
    }
    
    func getLocalLayer(source: LocalTilesSource) -> MaplyQuadImageTilesLayer? {
        guard let tileSource = MaplyMBTileSource(mbTiles: source.filename) else {
            print("Could not load local tile set")
            return nil
        }
        
        return MaplyQuadImageTilesLayer(tileSource: tileSource)!
    }
    
    func getRemoteLayer(source: RemoteTilesSource, globeMapType: GlobeMapType)  -> MaplyQuadImageTilesLayer? {
        guard let tileSource = MaplyRemoteTileSource(baseURL: source.url, ext: source.ext, minZoom: 0, maxZoom: source.maxZoom) else {
                print("Could not create remote tile source")
                return nil
        }

        // remote tile set: setup a cache directory
        let baseCacheDir = NSSearchPathForDirectoriesInDomains(.cachesDirectory, .userDomainMask, true)[0]
        let tilesCacheDir = "\(baseCacheDir)/\(source.name.replacingOccurrences(of: " ", with: ""))/"
        tileSource.cacheDir = tilesCacheDir
        
        let layer = MaplyQuadImageTilesLayer(tileSource: tileSource)!
        layer.handleEdges = (globeMapType == .globe)
        layer.coverPoles = (globeMapType == .globe)
        layer.requireElev = false
        layer.waitLoad = false
        layer.drawPriority = 0
        layer.singleLevelLoading = false
        
        return layer
    }
    
    func getGlobe() -> WhirlyGlobeViewController? {
        return globeMapViewController as? WhirlyGlobeViewController
    }
    
    func getMap() -> MaplyViewController? {
        return globeMapViewController as? MaplyViewController
    }
    
    @objc func globeMapButtonTapped(_ sender: Any) {
        let position: MaplyCoordinate
        
        if globeMapType == .globe {
            globeMapType = .map
            globeMapButtonItem.image = globeImage
            position = getGlobe()!.getPosition()
        }
        else {
            globeMapType = .globe
            globeMapButtonItem.image = mapImage
            position = getMap()!.getPosition()
        }

        setupView()
        setPosition(position: position)

        if let currentTileName = currentTileSource?.name {
            addBaseLayer(name: currentTileName)
        }
        else {
            addDefaultBaseLayer()
        }
    }

    @objc func tilesButtonTapped(_ sender: Any) {
        let ac = UIAlertController(title: "Tiles source", message: nil, preferredStyle: .actionSheet)
        ac.popoverPresentationController?.barButtonItem = tilesButtonItem
        for source in tilesSources {
            let prefix = (source.name == currentTileSource?.name) ? "✓ " : ""
            let title = prefix + source.name
            let mapTypeAction = UIAlertAction(title: title, style: .default) { [weak self] _ in
                let index = self?.tilesSources.firstIndex(where: { $0.name == source.name }) ?? 0
                self?.currentTileSource = source
                self?.setBaseLayer(source: index)
            }
            ac.addAction(mapTypeAction)
        }
        ac.addAction(UIAlertAction(title: "Cancel", style: .cancel))
        present(ac, animated: true)
    }
}


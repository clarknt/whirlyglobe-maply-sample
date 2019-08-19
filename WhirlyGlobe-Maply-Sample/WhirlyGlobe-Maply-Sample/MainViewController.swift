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

    
    
    // MARK:- Properties: GUI
    
    @IBOutlet weak var containerView: UIView!
    
    private var tilesButtonItem: UIBarButtonItem!
    
    private var globeMapButtonItem: UIBarButtonItem!
    private var globeImage = UIImage(named: "globe")
    private var mapImage = UIImage(named: "map")

    private var layersButtonItem: UIBarButtonItem!

    private var globeMapViewController: MaplyBaseViewController?

    
    
    // MARK:- Properties: Data

    private var tilesSources = [TilesSource]()
    private var currentTileSource: TilesSource?
    
    private var layersSources = [LayerSource]()
    private var currentLayers = [String: MaplyViewControllerLayer]()
    
    private var currentGlobeMapType: GlobeMapType = .globe
    
    
    
    // MARK:- GUI
    
    override func viewDidLoad() {
        super.viewDidLoad()

        title = "WhirlyGlobe-Maply - Sample"
        
        setupToolbar()
        
        populateLocalTiles()
        populateRemoteTiles()
        populateLayers()
        
        setupView()
        setDefaultBaseLayer()
  
        animateToDefaultPosition()
    }
    
    func setupToolbar() {
        globeMapButtonItem = UIBarButtonItem(image: mapImage, style: .plain, target: self, action: #selector(globeMapButtonTapped))

        tilesButtonItem = UIBarButtonItem(image: UIImage(named: "tiles"), style: .plain, target: self, action: #selector(tilesButtonTapped))
        
        layersButtonItem = UIBarButtonItem(image: UIImage(named: "layers"), style: .plain, target: self, action: #selector(layersButtonTapped))
        
        toolbarItems = [globeMapButtonItem, tilesButtonItem, layersButtonItem]
        navigationController?.isToolbarHidden = false
    }
    
    func setupView() {
        globeMapViewController?.removeFromParent()
        
        if currentGlobeMapType == .globe {
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
    
    
    
    // MARK:- Data initialization
    
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
    
    func populateLayers() {
        // see README.md for credits
        
        let date = getNasaFormattedDate()

        layersSources.append(LayerSource(name: "Land surface temperature", url: "https://gibs.earthdata.nasa.gov/wmts/epsg3857/best/MODIS_Terra_Land_Surface_Temp_Day/default/\(date)/GoogleMapsCompatible_Level7/{z}/{y}/{x}", ext: "png", maxZoom: 7))

        layersSources.append(LayerSource(name: "Coastlines", url: "https://gibs.earthdata.nasa.gov/wmts/epsg3857/best/Coastlines/default/\(date)/GoogleMapsCompatible_Level9/{z}/{y}/{x}", ext: "png", maxZoom: 9))
        
        layersSources.append(LayerSource(name: "Coastlines / Borders / Roads", url: "https://gibs.earthdata.nasa.gov/wmts/epsg3857/best/Reference_Features/default/\(date)/GoogleMapsCompatible_Level9/{z}/{y}/{x}", ext: "png", maxZoom: 9))
    }
    
    
    
    // MARK:- Layers
    
    func setDefaultBaseLayer() {
        if let defaultSource = tilesSources.first(where: { $0.name == "Geography Class" }) {
            setBaseLayer(source: defaultSource)
        }
        else if tilesSources.count > 0 {
            setBaseLayer(source: tilesSources[0])
        }
    }
    
    func setBaseLayer(source: TilesSource) {
        // black background for a globe, white background for a map
        globeMapViewController!.clearColor = currentGlobeMapType == .globe ? UIColor.black : UIColor.white
        
        // 30 fps
        globeMapViewController!.frameInterval = 2
        
        // prepare layer
        let layer: MaplyQuadImageTilesLayer?
        
        if source.type == .local {
            layer = getLocalLayer(source: source as! LocalTilesSource)
        }
        else {
            layer = getRemoteLayer(source: source as! RemoteTilesSource, globeMapType: .globe)
        }
        
        globeMapViewController!.removeAllLayers()

        currentTileSource = source
        currentLayers.removeAll(keepingCapacity: true)

        if let layer = layer {
            globeMapViewController!.add(layer)
        }
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
    
    func toggleOverlayLayer(source: LayerSource) {
        let currentLayer = currentLayers[source.name]

        if currentLayer != nil {
            removeOverlayLayer(source: source)
            return
        }
        
        addOverlayLayer(source: source)
    }
    
    func removeOverlayLayer(source: LayerSource) {
        let currentLayer = currentLayers[source.name]
        
        // remove layer
        if currentLayer != nil {
            currentLayers[source.name] = nil
            globeMapViewController?.remove(currentLayer!)
        }
    }
    
    func addOverlayLayer(source: LayerSource) {
        // For network paging layers, where we'll store temp files
        let cacheDir = NSSearchPathForDirectoriesInDomains(.cachesDirectory, .userDomainMask, true)[0]
        if let tileSource = MaplyRemoteTileSource(baseURL: source.url, ext: source.ext, minZoom: 1, maxZoom: source.maxZoom) {
            tileSource.cacheDir = "\(cacheDir)/\(source.name.replacingOccurrences(of: " ", with: ""))/"
            //tileSource.tileInfo.cachedFileLifetime = 60*60*24 // invalidate data after 24 hours
            
            if let layer = MaplyQuadImageTilesLayer(tileSource: tileSource) {
                layer.coverPoles = false
                layer.handleEdges = false
                // half opacity
                layer.fade = 0.5
                currentLayers[source.name] = layer
                globeMapViewController?.add(layer)
            }
        }
    }
    
    
    
    // MARK:- Animation
    
    func animateToDefaultPosition() {
        // start up over New York City
        let nycLong: Float = -73.935242
        let nycLat: Float = 40.730610
        
        animateToPosition(position: MaplyCoordinateMakeWithDegrees(nycLong, nycLat), time: 1.0)
    }
    
    func animateToPosition(position: MaplyCoordinate, time: TimeInterval) {
        if currentGlobeMapType == .globe {
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
    

    
    // MARK:- Utils
    
    func getNasaFormattedDate() -> String {
        let date = Date()
        let format = DateFormatter()
        format.dateFormat = "yyyy"
        let year = format.string(from: date)
        format.dateFormat = "MM"
        let month = format.string(from: date)
        format.dateFormat = "dd"
        let day = format.string(from: date)
        
        return "\(year)-\(month)-\(day)"
    }
    
    func getGlobe() -> WhirlyGlobeViewController? {
        return globeMapViewController as? WhirlyGlobeViewController
    }
    
    func getMap() -> MaplyViewController? {
        return globeMapViewController as? MaplyViewController
    }
    
    
    
    // MARK:- Buttons tapped
    
    @objc func globeMapButtonTapped(_ sender: Any) {
        let position: MaplyCoordinate
        
        if currentGlobeMapType == .globe {
            currentGlobeMapType = .map
            globeMapButtonItem.image = globeImage
            position = getGlobe()!.getPosition()
        }
        else {
            currentGlobeMapType = .globe
            globeMapButtonItem.image = mapImage
            position = getMap()!.getPosition()
        }

        setupView()
        setPosition(position: position)

        if let currentTileSource = currentTileSource {
            setBaseLayer(source: currentTileSource)
        }
        else {
            setDefaultBaseLayer()
        }
    }

    @objc func tilesButtonTapped(_ sender: Any) {
        let ac = UIAlertController(title: "Tiles source", message: nil, preferredStyle: .actionSheet)
        ac.popoverPresentationController?.barButtonItem = tilesButtonItem
        for source in tilesSources {
            let prefix = (source.name == currentTileSource?.name) ? "✓ " : ""
            let title = prefix + source.name
            let mapTypeAction = UIAlertAction(title: title, style: .default) { [weak self] _ in
                guard source.name != self?.currentTileSource?.name else { return }

                self?.currentTileSource = source
                self?.setBaseLayer(source: source)
            }
            ac.addAction(mapTypeAction)
        }
        ac.addAction(UIAlertAction(title: "Cancel", style: .cancel))
        present(ac, animated: true)
    }
    
    @objc func layersButtonTapped(_ sender: Any) {
        let ac = UIAlertController(title: "Layers", message: nil, preferredStyle: .actionSheet)
        ac.popoverPresentationController?.barButtonItem = layersButtonItem
        for source in layersSources {
            let prefix = (currentLayers[source.name] != nil) ? "✓ " : ""
            let title = prefix + source.name
            let mapTypeAction = UIAlertAction(title: title, style: .default) { [weak self] _ in
                self?.toggleOverlayLayer(source: source)
            }
            ac.addAction(mapTypeAction)
        }
        ac.addAction(UIAlertAction(title: "Cancel", style: .cancel))
        present(ac, animated: true)
    }
}


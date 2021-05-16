/// <reference path="../../../typings/threejs/three.d.ts" />

module multipoly.engine {
  export class BoardController {

    /**
     * The DOM Element in which the drawing will happen.
     * @type HTMLDivElement
     */
    containerEl = null;

    /** @type String */
    assetsUrl = '';

    /** @type THREE.WebGLRenderer */
    renderer;

    /** @type THREE.Scene */
    scene;

    /** @type THREE.PerspectiveCamera */
    camera;

    /** @type THREE.OrbitControls */
    cameraController;

    /** @type Object */
    lights = {};

    /** @type Object */
    materials = {};

    /** @type THREE.Geometry */
    pieceGeometry = null;

    /** @type THREE.Geometry */
    houseGeometry = null;

    /** @type THREE.Geometry */
    diceGeometry = null;

    /** @type THREE.Geometry */
    dices = [];

    /** @type THREE.Mesh */
    boardModel;

    /** @type THREE.Mesh */
    groundModel;

    /**
     * The board square size.
     * @type Number
     * @constant
     */
    squareSize = 10;

    /** @type Object */
    selectedPiece = null;

    /** @type Object */
    callbacks = {};


    /**********************************************************************************************/
    /* Public methods *****************************************************************************/

    constructor(options) {
      if (options.callbacks != null) {
        this.callbacks = options.callbacks;
      }
      if (options.containerEl != null) {
        this.containerEl = options.containerEl;
      }

      if (options.assetsUrl != null) {
        this.assetsUrl = options.assetsUrl;
      }
    }

    /**
     * Draws the board.
     */
    drawBoard(callback) {
      this.initEngine();
      this.initLights();
      this.initMaterials();

      this.initObjects(() => {
        this.onAnimationFrame();

        callback();
      });

      this.initListeners();
    }


    /**
     * Adds a piece to the board.
     * @param {Object} piece The piece properties.
     */
    createPiece(piece) {
      var pieceObjGroup = new THREE.Object3D();
      var pieceMesh = new THREE.Mesh(this.pieceGeometry);
      pieceMesh.material = new THREE.MeshPhongMaterial({
        color: piece.color,
        shininess: 20
      });

      pieceMesh.scale.set(1.5, 1.5, 1.5);

      var position = this.boardToWorld(piece.pos);
      pieceMesh.position.set(position.x, 5, position.z);

      var shape = new THREE.TextGeometry(piece.name, {font: 'helvetiker'});
      var wrapper = new THREE.MeshNormalMaterial({color: 0x00ff00});
      var words = new THREE.Mesh(shape, wrapper);
      words.scale.set(0.01, 0.01, 0.01);
      words.position.set(position.x, 10, position.z);


      pieceObjGroup.add(pieceMesh);
      pieceObjGroup.add(words);

      return pieceObjGroup;
    }


    createDice() {
      var diceMesh = new THREE.Mesh(this.diceGeometry);
      diceMesh.scale.set(0.01, 0.01, 0.01);

      diceMesh.material = new THREE.MeshLambertMaterial({
        color: 0xFFFFFF,
        shininess: 0
      });

      return diceMesh;
    }

    /**
     * Adds a piece to the board.
     * @param {Object} piece The piece properties.
     */
    createHouse(house) {
      var pieceMesh = new THREE.Mesh(this.houseGeometry);
      pieceMesh.scale.set(0.01, 0.01, 0.01);

      var position = this.boardToWorld(house.pos);
      pieceMesh.position.set(position.x, 0, position.z);

      pieceMesh.material = new THREE.MeshPhongMaterial({
        color: house.color,
        shininess: 20
      });

      return pieceMesh;
    }


    setDice(dice) {
      var position = this.boardToWorld(dice.pos);
      this.dices[dice.id].scale.set(0.1, 0.1, 0.1);
      this.dices[dice.id].position.set(position.x, 5, position.z);
      this.dices[dice.id].rotation.set(3.1, 0, 1.6);
      this.scene.add(this.dices[dice.id]);
      this.setDiceToShow(this.dices[dice.id],dice.number);
    }

    removePieces() {
      for (var i = 0; i < this.scene.children.length; i++) {
        if (this.scene.children[i].name == "pieces") {
          this.scene.remove(this.scene.children[i]);
        }
      }
    }


    renderTable(board) {
      this.removePieces();
      var pieceObjGroup = new THREE.Object3D();
      pieceObjGroup.name = "pieces";

      for (var i = 0; i < board.length; i++) {
        for (var j = 0; j < board[i].length; j++) {
          if (board[i][j] != null) {

            if (board[i][j].type == "piece") {
              pieceObjGroup.add(this.createPiece(board[i][j]));
            }

            if (board[i][j].type == "house") {
              pieceObjGroup.add(this.createHouse(board[i][j]));
            }
          }
        }
      }
      this.scene.add(pieceObjGroup);
    }

    setDiceToShow(dice,number) {
      switch (number) {
        case 1:
          dice.rotation.set(3.1, 0, 1.6);
          break;
        case 2:
          dice.rotation.set(1.6, 1.6, 0);
          break;
        case 3:
          dice.rotation.set(3.2, 0, 0);
          break;
        case 4:
          dice.rotation.set(0, 0, 0);
          break;
        case 5:
          dice.rotation.set(1.5, 0, 0);
          break;
        case 6:
          dice.rotation.set(4.7, 0, 0);
          break;
      }
    }


    /**********************************************************************************************/
    /* Private methods ****************************************************************************/

    /**
     * Initialize some basic 3D engine elements.
     */

    initEngine() {
      var viewWidth = this.containerEl.offsetWidth;
      var viewHeight = this.containerEl.offsetHeight;

      // instantiate the WebGL Renderer
      this.renderer = new THREE.WebGLRenderer({
        antialias: true,
        alpha: true
      });
      this.renderer.setSize(viewWidth, viewHeight);
      this.renderer.setClearColor(0xffffff, 1);

      // create the scene
      this.scene = new THREE.Scene();

      // create camera
      this.camera = new THREE.PerspectiveCamera(35, viewWidth / viewHeight, 1, 1000);
      this.camera.position.set(this.squareSize * 4, 120, 150);
      this.cameraController = new THREE.TrackballControls(this.camera, this.containerEl);
      this.cameraController.center = new THREE.Vector3(this.squareSize * 4, 0, this.squareSize * 4);
      //
      this.scene.add(this.camera);

      this.containerEl.appendChild(this.renderer.domElement);
    }

    addLights() {
      this.scene.add(this.lights.topLight);
      this.scene.add(this.lights.whiteSideLight);
      this.scene.add(this.lights.blackSideLight);
      this.scene.add(this.lights.movingLight);
    }

    /**
     * Initialize the lights.
     */

    initLights() {
      // top light
      this.lights.topLight = new THREE.PointLight();
      this.lights.topLight.position.set(this.squareSize * 4, 150, this.squareSize * 4);
      this.lights.topLight.intensity = 0.4;

      // white's side light
      this.lights.whiteSideLight = new THREE.SpotLight();
      this.lights.whiteSideLight.position.set(this.squareSize * 4, 100, this.squareSize * 4 + 200);
      this.lights.whiteSideLight.intensity = 0.4;
      this.lights.whiteSideLight.shadowCameraFov = 55;

      // black's side light
      this.lights.blackSideLight = new THREE.SpotLight();
      this.lights.blackSideLight.position.set(this.squareSize * 4, 100, this.squareSize * 4 - 200);
      this.lights.blackSideLight.intensity = 0.4;
      this.lights.blackSideLight.shadowCameraFov = 55;

      // light that will follow the camera position
      this.lights.movingLight = new THREE.PointLight(0xf9edc9);
      this.lights.movingLight.position.set(0, 10, 0);
      this.lights.movingLight.intensity = 0.5;
      this.lights.movingLight.distance = 500;

      // add the lights in the scene
      this.addLights();
    }

    /**
     * Initialize the materials.
     */

    initMaterials() {
      // board material
      this.materials.boardMaterial = new THREE.MeshLambertMaterial({
        map: THREE.ImageUtils.loadTexture(this.assetsUrl + 'monopoly.jpg')
      });
	  this.materials.boardMaterial.map.anisotropy = 4;
	  this.materials.boardMaterial.magFilter = THREE.LinearFilter;
	  this.materials.boardMaterial.minFilter = THREE.LinearMipMapLinearFilter;

      // ground material
      this.materials.groundMaterial = new THREE.MeshBasicMaterial({
        transparent: true,
        map: THREE.ImageUtils.loadTexture(this.assetsUrl + 'ground.png')
      });

    }


    renderBoard() {
      this.scene.add(this.boardModel);
    }


    renderGround() {
      this.scene.add(this.groundModel);
    }

    /**
     * Initialize the objects.
     * @param {Object} callback Function to call when the objects have been loaded.
     */
    initObjects(callback) {
      var loader = new THREE.JSONLoader();
      var oloader = new THREE.ObjectLoader();
      var totalObjectsToLoad = 4; // board + the piece
      var loadedObjects = 0; // count the loaded pieces


      // load board
      loader.load(this.assetsUrl + 'board.js', (geom) => {
        this.boardModel = new THREE.Mesh(geom, this.materials.boardMaterial);
        this.boardModel.position.set(-44, -0.02, -44);

        this.renderBoard();

        loadedObjects++;

        if (loadedObjects === totalObjectsToLoad && callback) {
          callback();
        }
      });

      // load piece
      loader.load(this.assetsUrl + 'piece.js', (geometry) => {
        this.pieceGeometry = geometry;

        loadedObjects++;

        if (loadedObjects === totalObjectsToLoad && callback) {
          callback();
        }
      });

      loader.load(this.assetsUrl + 'house.js', (geometry) => {
        this.houseGeometry = geometry;

        loadedObjects++;

        if (loadedObjects === totalObjectsToLoad && callback) {
          callback();
        }
      });

      loader.load(this.assetsUrl + 'dice.js', (geometry) => {
        this.diceGeometry = geometry;
        //add dices
        this.dices[0] = this.createDice();
        this.dices[1] = this.createDice();

        loadedObjects++;

        if (loadedObjects === totalObjectsToLoad && callback) {
          callback();
        }
      });


      // add ground
      //this.groundModel = new THREE.Mesh(new THREE.PlaneGeometry(100, 100, 1, 1), this.materials.groundMaterial);
      //this.groundModel.position.set(-44, -1.52, -44);
      //this.groundModel.rotation.x = -90 * Math.PI / 180;

      //this.renderGround();
    }

    /**
     * Initialize listeners.
     */
    initListeners() {
      var domElement = this.renderer.domElement;
    }

    /**
     * The render loop.
     */
    onAnimationFrame() {
      requestAnimationFrame(this.onAnimationFrame.bind(this));

      this.cameraController.update();

      // update moving light position
      this.lights.movingLight.position.x = this.camera.position.x;
      this.lights.movingLight.position.z = this.camera.position.z;

      this.renderer.render(this.scene, this.camera);
    }


    /**
     * Converts the board position to 3D world position.
     * @param {Array} pos The board position.
     * @returns {THREE.Vector3}
     */
    boardToWorld(pos) {
      //needs to be recalculated
      var x = ((1 + pos[1]) * 3.3 - 3.3 / 2) - 6 -44;
      var z = ( (1 + pos[0]) * 3.3 - 3.3 / 2) - 6 -44;
      //var x = pos[1] * 3.5  - 11;
      //var z = pos[0] * 3.5 - 11;

      return new THREE.Vector3(x, 0, z);
    }
  }
}


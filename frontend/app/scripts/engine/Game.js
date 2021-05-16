/// <reference path="./BoardController.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../models/GameState.ts" />
var multipoly;
(function (multipoly) {
    var engine;
    (function (engine) {
        var Game = (function () {
            function Game(options) {
                this.options = {};
                /**********************************************************************************************/
                /* Private properties *************************************************************************/
                /** @type multipoly.engine.BoardController */
                this.boardController = null;
                this.browcount = 13;
                this.bcolcount = 13;
                this.maxPlayerOnOnePlace = 3;
                /**
                 * The board representation.
                 * @type Array
                 */
                this.board = [];
                if (options != null) {
                    this.options = options;
                    this.roomService = options.roomService;
                    this.roomService.setStateChangedCallback(this.onGameStateChanged.bind(this));
                }
                this.init();
            }
            Game.prototype.onGameStateChanged = function (state) {
                console.log(state);
                this.populateBoard();
            };
            /**********************************************************************************************/
            /* Private methods ****************************************************************************/
            Game.prototype.initBoard = function () {
                this.board = [];
                for (var i = 0; i < this.browcount * this.maxPlayerOnOnePlace; i++) {
                    this.board[i] = [];
                    for (var j = 0; j < this.browcount * this.maxPlayerOnOnePlace; j++) {
                        this.board[i][j] = null;
                    }
                }
            };
            /**
             * Initializer.
             */
            Game.prototype.init = function () {
                this.initBoard();
                this.boardController = new multipoly.engine.BoardController({
                    containerEl: this.options.containerEl,
                    assetsUrl: this.options.assetsUrl
                });
                this.boardController.drawBoard(this.onBoardReady.bind(this));
            };
            Game.prototype.getFieldSize = function (index) {
                if (index == 0 || index == 10 || index == 20 || index == 30) {
                    return [4, 4];
                }
                else {
                    return [2, 2];
                }
            };
            /**
             * Returns the map coordination when the given field starts
             * @param index can be between 0 - 39
             */
            Game.prototype.getFirstCoordinateOfFieldIndex = function (index) {
                var pos = [26, 26];
                for (var i = 1; i <= index; i++) {
                    var offset = this.getFieldSize(i - 1);
                    if (0 <= i && i <= 10) {
                        pos[1] -= offset[1];
                    }
                    if (11 <= i && i <= 20) {
                        pos[0] -= offset[0];
                    }
                    if (i == 21) {
                        pos[1] += offset[1] / 2;
                    }
                    if (21 < i && i <= 30) {
                        pos[1] += offset[1];
                    }
                    if (31 < i && i <= 39) {
                        pos[0] += offset[0];
                    }
                    if (i == 31) {
                        pos[0] += offset[0] / 2;
                        pos[1] += offset[1] / 2;
                    }
                }
                return pos;
            };
            /**
             * Returns all the possible coordinates of a field
             * @param index
             * @returns {Array}
             */
            Game.prototype.getCoordinatesOfField = function (index) {
                var sizeOfField = this.getFieldSize(index);
                var increase = false;
                var possibilities = [];
                if (0 <= index && index <= 10) {
                    increase = false;
                }
                /* jobb oldal*/
                if (11 <= index && index <= 20) {
                    increase = false;
                }
                /* 0 - 10 also sor */
                if (21 <= index && index <= 30) {
                    increase = false;
                }
                if (31 <= index && index <= 39) {
                    increase = false;
                }
                for (var i = 0; i <= Math.min(3, sizeOfField[0]); i++) {
                    for (var j = 0; j <= Math.min(3, sizeOfField[1]); j++) {
                        var base = this.getFirstCoordinateOfFieldIndex(index);
                        if (increase) {
                            base[0] += j;
                            base[1] += i;
                        }
                        else {
                            base[0] -= j;
                            base[1] -= i;
                        }
                        possibilities.push(base);
                    }
                }
                return possibilities;
            };
            Game.prototype.getNextFreeFieldPosition = function (field) {
                var positions = this.getCoordinatesOfField(field);
                var pos = positions[0];
                var i = 0;
                while (this.board[pos[0]][pos[1]] != null && i < positions.length) {
                    pos = positions[i];
                    i++;
                }
                return pos;
            };
            Game.prototype.pullToTheCenter = function (pos, field) {
                if (0 <= field && field <= 10) {
                    pos[0] -= 2 * this.getFieldSize(field)[0];
                }
                /* bal oldal*/
                if (11 <= field && field <= 20) {
                    pos[1] += this.getFieldSize(field)[1] / 2;
                }
                if (21 <= field && field <= 30) {
                    pos[0] += this.getFieldSize(field)[0];
                }
                if (31 <= field && field <= 39) {
                    pos[1] -= 2 * this.getFieldSize(field)[1];
                }
                return pos;
            };
            Game.prototype.getNextFreeHousePosition = function (field) {
                var positions = this.getCoordinatesOfField(field);
                var pos = this.pullToTheCenter(positions[0], field);
                var i = 0;
                while (this.board[pos[0]][pos[1]] != null && i < positions.length) {
                    pos = this.pullToTheCenter(positions[i], field);
                    i++;
                }
                return pos;
            };
            Game.prototype.populateBoard = function () {
                this.initBoard();
                var state = this.roomService.getCurrentGameState();
                var num0 = state.lastroll.length > 0 ? state.lastroll[0] : 1;
                var num1 = state.lastroll.length > 0 ? state.lastroll[1] : 1;
                this.boardController.setDice({ id: 0, pos: [7, 7], number: num0 });
                this.boardController.setDice({ id: 1, pos: [17, 17], number: num1 });
                for (var i = 0; i < state.players.length; i++) {
                    var player = state.players[i];
                    var pos = this.getNextFreeFieldPosition(player.pos);
                    this.board[pos[0]][pos[1]] = {
                        pos: pos,
                        type: "piece",
                        name: player.name,
                        color: this.roomService.getColorForPlayer(i)
                    };
                }
                if (state.properties != null) {
                    for (var i = 0; i < state.properties.length; i++) {
                        var property = state.properties[i];
                        if (property.owner != null) {
                            var pos = this.getNextFreeHousePosition(property.pos);
                            this.board[pos[0]][pos[1]] = {
                                pos: pos,
                                type: "house",
                                color: this.roomService.getColorForPlayer(property.owner)
                            };
                        }
                    }
                }
                this.boardController.renderTable(this.board);
            };
            /**
             * On board ready.
             */
            Game.prototype.onBoardReady = function () {
                // setup the board pieces
                this.populateBoard();
                /*
                for (var i = 0; i < 40; i++) {
                  for (var j = 0; j < 6; j++) {
                    var pos = this.getNextFreeFieldPosition(i);
                    this.board[pos[0]][pos[1]] = {
                      name: i + "." + j,
                      pos: pos,
                      type: "piece",
                      color: this.roomService.getColorForPlayer(i)
                    };
                  }
                }
                this.boardController.renderTable(this.board);
                */
                /*
                 this.boardController.setDice({pos: [7, 7], number: 2});
                 this.board[0][0] = {pos: [0, 0], type: "piece"};
                 this.board[1][0] = {pos: [1, 0], type: "piece"};
                 this.board[0][1] = {pos: [0, 1], type: "piece"};
                 this.board[1][1] = {pos: [1, 1], type: "piece"};
                 this.board[2][1] = {pos: [2, 1], type: "piece"};
                 this.board[2][0] = {pos: [2, 0], type: "piece"};
                 this.board[0][4] = {pos: [0, 4], type: "piece"};
                 this.board[1][4] = {pos: [1, 4], type: "piece"};
                 this.board[0][3] = {pos: [0, 3], type: "piece"};
                 this.board[1][3] = {pos: [1, 3], type: "piece"};
                 this.board[2][3] = {pos: [2, 3], type: "piece"};
                 this.board[2][4] = {pos: [2, 4], type: "piece"};
                 this.board[0][25] = {pos: [0, 25], type: "piece"};
                 this.board[1][25] = {pos: [1, 25], type: "piece"};
                 this.board[25][0] = {pos: [25, 0], type: "piece"};
                 this.board[25][25] = {pos: [25, 25], type: "piece"};
          
                 this.board[25][25] = {pos: [23, 23], type: "house"};
                 this.boardController.renderTable(this.board);
                 */
            };
            return Game;
        })();
        engine.Game = Game;
    })(engine = multipoly.engine || (multipoly.engine = {}));
})(multipoly || (multipoly = {}));
//# sourceMappingURL=Game.js.map
/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../models/Room.ts" />
/// <reference path="../models/GameState.ts" />
/// <reference path="../services/WebSocketService.ts" />
var multipoly;
(function (multipoly) {
    var services;
    (function (services) {
        var RoomService = (function () {
            function RoomService($q, apiUrl, $http, WebSocketService, $rootScope) {
                this.$q = $q;
                this.apiUrl = apiUrl;
                this.$http = $http;
                this.WebSocketService = WebSocketService;
                this.$rootScope = $rootScope;
                this.currentGameState = null;
                this.stateChangedCallback = null;
                this._rooms = new Array();
                this.$rootScope.$on("room", this.onRoomInfoReceived.bind(this));
                this.$rootScope.$on("roomremove", this.onRoomRemoved.bind(this));
                this.$rootScope.$on("login", this.onLoggedIn.bind(this));
                this.$rootScope.$on("status", this.onStatusChanged.bind(this));
                this.$rootScope.$on("startgame", this.onGameStarted.bind(this));
                this.$rootScope.$on("bid", this.onBidRequestReceived.bind(this));
            }
            RoomService.prototype.onBidRequestReceived = function (event, response) {
                if (response.error == null) {
                    if (this.currentGameState.bid == null) {
                        this.currentGameState.bid = new multipoly.models.Bid();
                    }
                    this.currentGameState.bid.parse(response);
                }
            };
            RoomService.prototype.onGameStarted = function (event, response) {
                this.getRoom(response.mapid).started = true;
            };
            RoomService.prototype.setStateChangedCallback = function (fn) {
                this.stateChangedCallback = fn;
            };
            RoomService.prototype.notifyStateChanged = function () {
                if (this.stateChangedCallback != null) {
                    this.stateChangedCallback(this.currentGameState);
                }
            };
            RoomService.prototype.onStatusChanged = function (event, response) {
                if (this.currentGameState == null) {
                    this.currentGameState = new multipoly.models.GameState();
                }
                this.currentGameState.parse(response);
                this.notifyStateChanged();
            };
            RoomService.prototype.onRoomRemoved = function (event, response) {
                for (var i = 0; i < this._rooms.length; i++) {
                    if (this._rooms[i].id == response.id) {
                        delete this._rooms[i];
                        this._rooms.length--;
                    }
                }
            };
            RoomService.prototype.onLoggedIn = function (event, response) {
                this._rooms = response.maps;
            };
            RoomService.prototype.setCurrentGameId = function (_id) {
                if (this.currentGameState == null) {
                    this.currentGameState = new multipoly.models.GameState();
                }
                this.currentGameState.id = _id;
            };
            RoomService.prototype.roomExistsWithId = function (id) {
                return (this.getRoom(id) != null);
            };
            RoomService.prototype.isMemberOf = function (room, user) {
                for (var i = 0; i < room.players.length; i++) {
                    if (room.players[i].name == user.name) {
                        return true;
                    }
                }
                return false;
            };
            RoomService.prototype.updateRoom = function (room) {
                var changed = false;
                if (this.roomExistsWithId(room.id)) {
                    var i = 0;
                    while (!changed && i < this._rooms.length) {
                        if (this._rooms[i].id == room.id) {
                            if (room.name != null) {
                                this._rooms[i].name = room.name;
                            }
                            if (room.players != null) {
                                this._rooms[i].players = room.players;
                            }
                            if (room.map != null) {
                                this._rooms[i].map = room.map;
                            }
                            changed = true;
                        }
                        i++;
                    }
                }
                else {
                    this._rooms.push(room);
                    changed = true;
                }
                return changed;
            };
            RoomService.prototype.onRoomInfoReceived = function (event, _room) {
                var room = new multipoly.models.Room();
                room.id = _room.id;
                room.name = _room.name;
                room.players = _room.players;
                this.updateRoom(room);
            };
            RoomService.prototype.setRooms = function (rooms) {
                this._rooms = rooms;
            };
            RoomService.prototype.getRooms = function () {
                return this._rooms;
            };
            RoomService.prototype.getRoom = function (id) {
                var room = null;
                var i = 0;
                while (room == null && i < this._rooms.length) {
                    if (this._rooms[i].id == id) {
                        room = this._rooms[i];
                    }
                    i++;
                }
                return room;
            };
            RoomService.prototype.create = function (room) {
                this.WebSocketService.fire({ cmd: 'createroom', name: room.name });
            };
            RoomService.prototype.join = function (room) {
                this.WebSocketService.fire({ cmd: 'join', id: room.id });
            };
            RoomService.prototype.leave = function (room) {
                this.WebSocketService.fire({ cmd: 'leave', id: room.id });
            };
            RoomService.prototype.start = function (room) {
                this.WebSocketService.fire({ cmd: 'startgame', id: room.id });
            };
            RoomService.prototype.getCurrentGameState = function () {
                return this.currentGameState;
            };
            RoomService.prototype.roll = function () {
                this.WebSocketService.fire({ cmd: 'roll' });
            };
            RoomService.prototype.endTurn = function () {
                this.WebSocketService.fire({ cmd: 'end' });
            };
            RoomService.prototype.buyField = function () {
                this.WebSocketService.fire({ cmd: 'buyfield' });
            };
            RoomService.prototype.sellField = function (id) {
                this.WebSocketService.fire({ cmd: 'sellfield', field: id });
            };
            RoomService.prototype.placeBid = function (bid) {
                this.WebSocketService.fire({ cmd: 'bid', value: bid });
            };
            RoomService.prototype.leaveJailWithCard = function () {
                this.WebSocketService.fire({ cmd: 'leavejailwithcard' });
            };
            RoomService.prototype.leaveJailWithPaying = function () {
                this.WebSocketService.fire({ cmd: 'leavejailwithpaying' });
            };
            RoomService.prototype.continue = function (token) {
                this.WebSocketService.fire({ cmd: 'continue', token: token });
            };
            RoomService.prototype.getColorForPlayer = function (index) {
                var colors = [
                    0x3366FF,
                    0x339933,
                    0xCC0033,
                    0xFF33FF,
                    0xFFCC00,
                    0xFFFF66,
                    0x000000,
                    0xFFFFFF
                ];
                return colors[index % colors.length];
            };
            RoomService.$inject = ["$q", "apiUrl", "$http", "WebSocketService", "$rootScope"];
            return RoomService;
        })();
        services.RoomService = RoomService;
    })(services = multipoly.services || (multipoly.services = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.services").service("RoomService", multipoly.services.RoomService);
//# sourceMappingURL=RoomService.js.map
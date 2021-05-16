/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="./Player.ts" />
var multipoly;
(function (multipoly) {
    var models;
    (function (models) {
        var Room = (function () {
            function Room() {
                this._players = new Array();
                this._started = false;
                this._nextPlayer = null;
            }
            Object.defineProperty(Room.prototype, "id", {
                get: function () {
                    return this._id;
                },
                set: function (value) {
                    this._id = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Room.prototype, "name", {
                get: function () {
                    return this._name;
                },
                set: function (value) {
                    this._name = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Room.prototype, "started", {
                get: function () {
                    return this._started;
                },
                set: function (value) {
                    this._started = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Room.prototype, "players", {
                get: function () {
                    return this._players;
                },
                set: function (value) {
                    this._players = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Room.prototype, "full", {
                get: function () {
                    return this._players != null && this._players.length == 8;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Room.prototype, "map", {
                get: function () {
                    return this._map;
                },
                set: function (value) {
                    this._map = value;
                },
                enumerable: true,
                configurable: true
            });
            Room.prototype.addPlayerByName = function (name) {
                var player = new multipoly.models.Player();
                player.name = name;
                this._players.push(player);
            };
            Object.defineProperty(Room.prototype, "nextPlayer", {
                get: function () {
                    return this._nextPlayer;
                },
                set: function (_player) {
                    this._nextPlayer = _player;
                },
                enumerable: true,
                configurable: true
            });
            Room.prototype.setNextPlayerByIndex = function (index) {
                if (this._players[index] != null) {
                    this._nextPlayer = this._players[index];
                }
            };
            return Room;
        })();
        models.Room = Room;
    })(models = multipoly.models || (multipoly.models = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.models").factory("Room", function () { return multipoly.models.Room; });
//# sourceMappingURL=Room.js.map
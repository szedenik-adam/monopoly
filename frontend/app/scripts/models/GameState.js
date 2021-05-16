/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="./Player.ts" />
/// <reference path="./Bid.ts" />
var multipoly;
(function (multipoly) {
    var models;
    (function (models) {
        var GameState = (function () {
            function GameState() {
                this.bid = new multipoly.models.Bid();
            }
            Object.defineProperty(GameState.prototype, "round", {
                get: function () {
                    return this._round;
                },
                set: function (value) {
                    this._round = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(GameState.prototype, "bid", {
                get: function () {
                    return this._bid;
                },
                set: function (value) {
                    this._bid = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(GameState.prototype, "id", {
                get: function () {
                    return this._id;
                },
                set: function (value) {
                    this._id = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(GameState.prototype, "lastroll", {
                get: function () {
                    return this._lastroll;
                },
                set: function (value) {
                    this._lastroll = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(GameState.prototype, "nextplayer", {
                get: function () {
                    return this._nextplayer;
                },
                set: function (value) {
                    this._nextplayer = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(GameState.prototype, "players", {
                get: function () {
                    return this._players;
                },
                set: function (value) {
                    this._players = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(GameState.prototype, "actions", {
                get: function () {
                    return this._actions;
                },
                set: function (value) {
                    this._actions = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(GameState.prototype, "properties", {
                get: function () {
                    return this._properties;
                },
                set: function (value) {
                    this._properties = value;
                },
                enumerable: true,
                configurable: true
            });
            GameState.prototype.parse = function (object) {
                this.players = object.players;
                this.nextplayer = object.nextplayer;
                this.actions = object.actions;
                this.lastroll = object.lastroll;
                this.properties = object.properties;
                this.round = object.round;
            };
            return GameState;
        })();
        models.GameState = GameState;
    })(models = multipoly.models || (multipoly.models = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.models").factory("Player", function () { return multipoly.models.Player; });
//# sourceMappingURL=GameState.js.map
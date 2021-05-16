/// <reference path="../../../typings/angularjs/angular.d.ts" />
var multipoly;
(function (multipoly) {
    var models;
    (function (models) {
        var Player = (function () {
            function Player(name) {
                if (name === void 0) { name = null; }
                this._pos = 0;
                this._money = 0;
                this._name = name;
            }
            Object.defineProperty(Player.prototype, "jailcard", {
                get: function () {
                    return this._jailcard;
                },
                set: function (value) {
                    this._jailcard = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Player.prototype, "card", {
                get: function () {
                    return this._card;
                },
                set: function (value) {
                    if (value != null) {
                        this._card = { key: value.key, value: value.value };
                    }
                    else {
                        this._card = null;
                    }
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Player.prototype, "name", {
                get: function () {
                    return this._name;
                },
                set: function (value) {
                    this._name = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Player.prototype, "pos", {
                get: function () {
                    return this._pos;
                },
                set: function (value) {
                    this._pos = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Player.prototype, "money", {
                get: function () {
                    return this._money;
                },
                set: function (value) {
                    this._money = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Player.prototype, "session", {
                get: function () {
                    return this._session;
                },
                set: function (value) {
                    this._session = value;
                },
                enumerable: true,
                configurable: true
            });
            return Player;
        })();
        models.Player = Player;
    })(models = multipoly.models || (multipoly.models = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.models").factory("Player", function () { return multipoly.models.Player; });
//# sourceMappingURL=Player.js.map
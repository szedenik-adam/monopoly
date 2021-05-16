/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="./Player.ts" />
var multipoly;
(function (multipoly) {
    var models;
    (function (models) {
        var Bid = (function () {
            function Bid() {
            }
            Object.defineProperty(Bid.prototype, "timeleftms", {
                get: function () {
                    return this._timeleftms;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Bid.prototype, "highestbid", {
                get: function () {
                    return this._highestbid;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(Bid.prototype, "highestbidder", {
                get: function () {
                    return this._highestbidder;
                },
                enumerable: true,
                configurable: true
            });
            Bid.prototype.parse = function (object) {
                this._timeleftms = object.timeleftms;
                this._highestbid = object.highestbid;
                this._highestbidder = object.highestbidder;
            };
            return Bid;
        })();
        models.Bid = Bid;
    })(models = multipoly.models || (multipoly.models = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.models").factory("Bid", function () { return multipoly.models.Bid; });
//# sourceMappingURL=Bid.js.map
/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../models/Room.ts" />
/// <reference path="../models/GameState.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var SellModalController = (function () {
            function SellModalController(RoomService, UserService, $modalInstance) {
                this.RoomService = RoomService;
                this.UserService = UserService;
                this.$modalInstance = $modalInstance;
                this.gameState = null;
                this.selectedProperty = null;
                this.gameState = this.RoomService.getCurrentGameState();
            }
            SellModalController.prototype.myPlayerIndex = function () {
                for (var i = 0; i < this.gameState.players.length; i++) {
                    if (this.gameState.players[i].name == this.UserService.getCurrentUser().name) {
                        return i;
                    }
                }
                return -1;
            };
            SellModalController.prototype.isMine = function (prop) {
                var me = this.myPlayerIndex();
                if (me == -1)
                    return false;
                return prop.owner == me;
            };
            SellModalController.prototype.getMyProperties = function () {
                var _this = this;
                return this.gameState.properties.filter(function (prop) { return _this.isMine(prop); });
            };
            SellModalController.prototype.ok = function () {
                this.$modalInstance.close(this.selectedProperty);
            };
            SellModalController.prototype.cancel = function () {
                this.$modalInstance.dismiss('cancel');
            };
            SellModalController.$inject = ["RoomService", "UserService", "$modalInstance"];
            return SellModalController;
        })();
        controllers.SellModalController = SellModalController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('SellModalController', multipoly.controllers.SellModalController);
//# sourceMappingURL=sellModal.js.map
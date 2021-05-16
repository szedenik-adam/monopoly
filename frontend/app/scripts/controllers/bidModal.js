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
        var BidModalController = (function () {
            function BidModalController(RoomService, UserService, $modalInstance, $scope, $interval) {
                var _this = this;
                this.RoomService = RoomService;
                this.UserService = UserService;
                this.$modalInstance = $modalInstance;
                this.$scope = $scope;
                this.$interval = $interval;
                this.gameState = null;
                this.remainingTime = 0;
                this.timer = null;
                this.gameState = this.RoomService.getCurrentGameState();
                $scope.$watch("gameState.bid.timeleftMs", this.onRemainingTimeExpanded.bind(this));
                this.remainingTime = Math.round(this.gameState.bid.timeleftms / 1000, 0);
                this.timer = $interval(function () {
                    _this.remainingTime -= 1;
                    if (_this.remainingTime <= 0) {
                        _this.cancel();
                        _this.$modalInstance.close();
                    }
                }, 1000);
            }
            BidModalController.prototype.onRemainingTimeExpanded = function (event) {
                this.remainingTime = Math.round(this.gameState.bid.timeleftms / 1000, 0);
            };
            BidModalController.prototype.getBidWinner = function () {
                if (this.gameState.bid.highestbidder > -1) {
                    return this.gameState.players[this.gameState.bid.highestbidder].name;
                }
                else {
                    return "starting price";
                }
            };
            BidModalController.prototype.placebid = function (bid) {
                this.RoomService.placeBid(this.gameState.bid.highestbid + bid);
            };
            BidModalController.prototype.ok = function () {
                this.placebid(this.bid - this.gameState.bid.highestbid);
            };
            BidModalController.prototype.cancel = function () {
                this.$modalInstance.dismiss('cancel');
            };
            BidModalController.$inject = ["RoomService", "UserService", "$modalInstance", "$scope", "$interval"];
            return BidModalController;
        })();
        controllers.BidModalController = BidModalController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('BidModalController', multipoly.controllers.BidModalController);
//# sourceMappingURL=bidModal.js.map
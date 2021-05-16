/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../filters/moneyFilter.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var TableController = (function () {
            function TableController(RoomService, UserService, $stateParams, $modal, toastr, $rootScope, $filter) {
                this.RoomService = RoomService;
                this.UserService = UserService;
                this.$stateParams = $stateParams;
                this.$modal = $modal;
                this.toastr = toastr;
                this.$rootScope = $rootScope;
                this.$filter = $filter;
                this.gameState = this.RoomService.getCurrentGameState();
                this.bidModalOpened = false;
                this.activate();
            }
            TableController.prototype.onSellField = function (event, response) {
                if (response.error != null) {
                    this.toastr.error(response.error);
                }
            };
            TableController.prototype.activate = function () {
                this.$rootScope.$on("sellfield", this.onSellField.bind(this));
                this.$rootScope.$on("bid", this.onBidRequest.bind(this));
            };
            TableController.prototype.myTurn = function () {
                return this.gameState._players[this.gameState._nextplayer].name == this.UserService.currentUser.name;
            };
            TableController.prototype.canRoll = function () {
                return (this.gameState.actions.indexOf("roll") != -1) && this.myTurn();
            };
            TableController.prototype.canSell = function () {
                return this.myTurn();
            };
            TableController.prototype.canBuy = function () {
                return (this.gameState.actions.indexOf("buyfield") != -1) && this.myTurn();
            };
            TableController.prototype.canEnd = function () {
                return (this.gameState.actions.indexOf("end") != -1) && this.myTurn();
            };
            TableController.prototype.roll = function () {
                this.RoomService.roll();
            };
            TableController.prototype.getPropertyByPos = function (pos) {
                for (var i = 0; i < this.gameState.properties.length; i++) {
                    if (this.gameState.properties[i].pos == pos) {
                        return this.gameState.properties[i];
                    }
                }
                return null;
            };
            TableController.prototype.getPropertyAtMyPos = function () {
                var me = this.gameState.players[this.getMyIndex()];
                var prop = this.getPropertyByPos(me.pos);
                return prop;
            };
            TableController.prototype.buyField = function () {
                this.RoomService.buyField();
            };
            TableController.prototype.endTurn = function () {
                this.RoomService.endTurn();
            };
            TableController.prototype.leaveJail = function (type) {
                if (type == "pay") {
                    this.RoomService.leaveJailWithPaying();
                }
                else {
                    this.RoomService.leaveJailWithCard();
                }
            };
            TableController.prototype.getIndexOfPlayer = function (name) {
                for (var i = 0; i < this.gameState._players.length; i++) {
                    if (this.gameState._players[i].name == name) {
                        return i;
                    }
                }
                return -1;
            };
            TableController.prototype.getMyIndex = function () {
                return this.getIndexOfPlayer(this.UserService.currentUser.name);
            };
            TableController.prototype.hasJailCard = function () {
                if (this.getMyIndex() > 0) {
                    return this.gameState._players[this.getMyIndex()].jailcard > 0;
                }
                else {
                    return 0;
                }
            };
            TableController.prototype.getLastCard = function () {
                var card = this.gameState._players[this.getMyIndex()].card;
                if (card & (1 << 30)) {
                    var added_money = card & ~(1 << 30);
                    return "Received money " + this.$filter("money")(added_money);
                }
                else if (card == 1) {
                    return "You are jailed!";
                }
                else if (card == 2) {
                    return "You received an 'Escape from jail' card";
                }
                else if (card & (1 << 30)) {
                    var lost_money = card & ~(1 << 31);
                    return "You lost " + this.$filter("money")(lost_money);
                }
                else {
                    return null;
                }
            };
            TableController.prototype.isInJail = function () {
                return this.gameState._players[this.getMyIndex()].jailed;
            };
            TableController.prototype.onBidRequest = function () {
                var _this = this;
                if (!this.bidModalOpened) {
                    this.bidModalOpened = true;
                    var modalInstance = this.$modal.open({
                        templateUrl: 'views/bidModal.html',
                        controller: 'BidModalController',
                        controllerAs: 'vm'
                    });
                    modalInstance.result.then(function (bid) {
                        _this.bidModalOpened = false;
                    }, function () {
                        _this.bidModalOpened = false;
                    });
                }
            };
            TableController.prototype.sell = function () {
                var _this = this;
                var modalInstance = this.$modal.open({
                    templateUrl: 'views/sellModal.html',
                    controller: 'SellModalController',
                    controllerAs: 'vm'
                });
                modalInstance.result.then(function (selectedItem) {
                    _this.RoomService.sellField(selectedItem.pos);
                });
            };
            TableController.$inject = ["RoomService", "UserService", "$stateParams", "$modal", "toastr", "$rootScope", "$filter"];
            return TableController;
        })();
        controllers.TableController = TableController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('TableController', multipoly.controllers.TableController);
//# sourceMappingURL=table.js.map
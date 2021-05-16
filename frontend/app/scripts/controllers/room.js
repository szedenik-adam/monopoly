/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../models/Room.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var RoomController = (function () {
            function RoomController(RoomService, $stateParams, $state, toastr, UserService, $scope) {
                this.RoomService = RoomService;
                this.$stateParams = $stateParams;
                this.$state = $state;
                this.toastr = toastr;
                this.UserService = UserService;
                this.$scope = $scope;
                this.room = null;
                this.activate();
            }
            RoomController.prototype.activate = function () {
                this.room = this.RoomService.getRoom(this.$stateParams.id);
                this.$scope.$on("startgame", this.onGameStarted.bind(this));
            };
            RoomController.prototype.onGameStarted = function (event, response) {
                this.showTable();
            };
            RoomController.prototype.isOwnerOfRoom = function () {
                return this.room.players[0].name == this.UserService.currentUser.name;
            };
            RoomController.prototype.leave = function () {
                this.RoomService.leave(this.room);
                this.$state.go("rooms");
            };
            RoomController.prototype.start = function () {
                this.RoomService.start(this.room);
                this.$state.go("table", { id: this.room.id });
            };
            RoomController.prototype.showTable = function () {
                this.RoomService.setCurrentGameId(this.room.id);
                this.$state.go("table", { id: this.room.id });
            };
            RoomController.$inject = ["RoomService", "$stateParams", "$state", "toastr", "UserService", "$scope"];
            return RoomController;
        })();
        controllers.RoomController = RoomController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('RoomController', multipoly.controllers.RoomController);
//# sourceMappingURL=room.js.map
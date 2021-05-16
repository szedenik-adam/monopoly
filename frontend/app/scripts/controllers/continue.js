/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var ContinueController = (function () {
            function ContinueController(RoomService, UserService, toastr, $stateParams, $scope, $state) {
                this.RoomService = RoomService;
                this.UserService = UserService;
                this.toastr = toastr;
                this.$stateParams = $stateParams;
                this.$scope = $scope;
                this.$state = $state;
                this.$inject = ["RoomService", "UserService", "toastr", "$stateParams", "$scope", "$state"];
                this.$scope.$on("continue", this.onContinueReceived.bind(this));
                this.RoomService.continue($stateParams.token);
            }
            ContinueController.prototype.onContinueReceived = function (event, response) {
                if (response.error != null) {
                    this.toastr.error(response.error);
                }
                else {
                    this.UserService.currentUser = new multipoly.models.User();
                    this.UserService.currentUser.name = response.username;
                    this.RoomService.join({ id: response.map });
                    this.$state.go("table", { id: response.map });
                }
            };
            return ContinueController;
        })();
        controllers.ContinueController = ContinueController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.controllers").controller("ContinueController", multipoly.controllers.ContinueController);
//# sourceMappingURL=continue.js.map
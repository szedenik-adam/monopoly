/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../models/User.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var MainController = (function () {
            function MainController(UserService, $rootScope, toastr) {
                this.UserService = UserService;
                this.$rootScope = $rootScope;
                this.toastr = toastr;
                $rootScope.$on("createroom", this.handleWebSocketError.bind(this));
                $rootScope.$on("join", this.handleWebSocketError.bind(this));
                $rootScope.$on("leave", this.handleWebSocketError.bind(this));
            }
            Object.defineProperty(MainController.prototype, "currentUser", {
                get: function () {
                    return this.UserService.currentUser;
                },
                enumerable: true,
                configurable: true
            });
            MainController.prototype.handleWebSocketError = function (event, data) {
                this.toastr.error(data.error);
            };
            MainController.$inject = ["UserService", "$rootScope", "toastr"];
            return MainController;
        })();
        controllers.MainController = MainController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('MainController', multipoly.controllers.MainController);
//# sourceMappingURL=main.js.map
/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/UserService.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var LogoutController = (function () {
            function LogoutController(UserService, $state) {
                this.UserService = UserService;
                this.$state = $state;
                UserService.logout();
                $state.go("home");
            }
            LogoutController.$inject = ["UserService", "$state"];
            return LogoutController;
        })();
        controllers.LogoutController = LogoutController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('LogoutController', multipoly.controllers.LogoutController);
//# sourceMappingURL=logout.js.map
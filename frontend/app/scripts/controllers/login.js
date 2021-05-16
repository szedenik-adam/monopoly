/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/UserService.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var LoginController = (function () {
            function LoginController(UserService, toastr, $state) {
                this.UserService = UserService;
                this.toastr = toastr;
                this.$state = $state;
            }
            LoginController.prototype.onLoginSuccessful = function (response) {
                this.toastr.success("Login successful!");
                this.$state.go("rooms");
            };
            LoginController.prototype.onLoginError = function (response) {
                this.toastr.error(response.data.error);
            };
            LoginController.prototype.login = function () {
                this.UserService.login(this.loginForm.name, this.loginForm.password).then(this.onLoginSuccessful.bind(this), this.onLoginError.bind(this));
            };
            LoginController.$inject = ["UserService", "toastr", "$state"];
            return LoginController;
        })();
        controllers.LoginController = LoginController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('LoginController', multipoly.controllers.LoginController);
//# sourceMappingURL=login.js.map
/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../models/User.ts" />
/// <reference path="../services/UserService.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var RegisterController = (function () {
            function RegisterController(User, UserService, toastr, $state) {
                this.User = User;
                this.UserService = UserService;
                this.toastr = toastr;
                this.$state = $state;
                this.newUser = null;
                this.newUser = new this.User();
            }
            RegisterController.prototype.onRegisterSuccess = function (response) {
                this.toastr.success("Registration was successful! Please log in!");
                this.$state.go("login");
            };
            RegisterController.prototype.onRegisterFailed = function (response) {
                this.toastr.error(response.data.error);
            };
            RegisterController.prototype.save = function () {
                this.UserService.register(this.newUser).then(this.onRegisterSuccess.bind(this), this.onRegisterFailed.bind(this));
            };
            RegisterController.$inject = ["User", "UserService", "toastr", "$state"];
            return RegisterController;
        })();
        controllers.RegisterController = RegisterController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('RegisterController', multipoly.controllers.RegisterController);
//# sourceMappingURL=register.js.map
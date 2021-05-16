/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/UserService.ts" />
module  multipoly.controllers {
  export class LoginController {
    static $inject = ["UserService","toastr","$state"];

    loginForm : {
      name:  string;
      password: string;
    };

    constructor (private UserService: multipoly.services.UserService,
                 private toastr: Toastr,
                 private $state: angular.ui.IStateService) {

    }

    onLoginSuccessful(response) {
      this.toastr.success("Login successful!");
      this.$state.go("rooms");
    }

    onLoginError(response) {
      this.toastr.error(response.data.error);
    }

    public login() {
      this.UserService.login(this.loginForm.name, this.loginForm.password).then(this.onLoginSuccessful.bind(this), this.onLoginError.bind(this))
    }
  }
}

angular.module('multipoly.controllers')
  .controller('LoginController', multipoly.controllers.LoginController);

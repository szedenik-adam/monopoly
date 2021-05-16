/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../models/User.ts" />
/// <reference path="../services/UserService.ts" />
module  multipoly.controllers {
  export class RegisterController {

    static $inject = ["User", "UserService","toastr","$state"];

    newUser:multipoly.models.User = null;

    constructor(private User:multipoly.models.User,
                private UserService:multipoly.services.UserService,
                private toastr: Toastr,
                private $state: angular.ui.IStateService) {
      this.newUser = new this.User();
    }

    private onRegisterSuccess(response) {
      this.toastr.success("Registration was successful! Please log in!");
      this.$state.go("login");
    }

    private onRegisterFailed(response) {
      this.toastr.error(response.data.error);
    }

    save() {
      this.UserService.register(this.newUser).then(this.onRegisterSuccess.bind(this), this.onRegisterFailed.bind(this));
    }

  }
}

angular.module('multipoly.controllers')
  .controller('RegisterController', multipoly.controllers.RegisterController);

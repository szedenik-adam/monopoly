/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/UserService.ts" />
module  multipoly.controllers {
  export class LogoutController {
    static $inject = ["UserService","$state"];

    constructor (private UserService: multipoly.services.UserService,
                  private $state : angular.ui.IStateService) {
      UserService.logout();
      $state.go("home");
    }

  }
}

angular.module('multipoly.controllers')
  .controller('LogoutController', multipoly.controllers.LogoutController);

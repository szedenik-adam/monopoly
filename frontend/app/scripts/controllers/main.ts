/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../models/User.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />


module  multipoly.controllers {
  export class MainController {
    static $inject = ["UserService", "$rootScope", "toastr"];

    get currentUser():multipoly.models.User {
      return this.UserService.currentUser;
    }

    handleWebSocketError(event, data) {
      this.toastr.error(data.error);
    }

    constructor(private UserService:multipoly.services.UserService,
                private $rootScope:angular.IRootScopeService,
                private toastr:Toastr) {
      $rootScope.$on("createroom", this.handleWebSocketError.bind(this));
      $rootScope.$on("join", this.handleWebSocketError.bind(this));
      $rootScope.$on("leave", this.handleWebSocketError.bind(this));

    }


  }
}
angular.module('multipoly.controllers')
  .controller('MainController', multipoly.controllers.MainController);


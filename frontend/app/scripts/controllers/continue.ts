/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />

module  multipoly.controllers {
  export class ContinueController {

    $inject = ["RoomService", "UserService", "toastr", "$stateParams", "$scope","$state"]

    constructor(private RoomService:multipoly.services.RoomService,
                private UserService:multipoly.services.UserService,
                private toastr,
                private $stateParams:angular.ui.IStateParamsService,
                private $scope:angular.IScope,
                private $state: angular.ui.IStateService) {

      this.$scope.$on("continue", this.onContinueReceived.bind(this));
      this.RoomService.continue($stateParams.token);
    }

    onContinueReceived(event, response) {
      if (response.error != null) {
        this.toastr.error(response.error);
      } else {
        this.UserService.currentUser = new multipoly.models.User();
        this.UserService.currentUser.name = response.username;
        this.RoomService.join({id: response.map});
        this.$state.go("table",{id: response.map});
      }
    }
  }
}

angular.module("multipoly.controllers")
  .controller("ContinueController", multipoly.controllers.ContinueController);

/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../models/Room.ts" />

module  multipoly.controllers {
  export class RoomController {

    static $inject = ["RoomService", "$stateParams", "$state", "toastr", "UserService","$scope"];
    room:multipoly.models.Room = null;

    constructor(private RoomService:multipoly.services.RoomService,
                private $stateParams:angular.ui.IStateParamsService,
                private $state:angular.ui.IStateService,
                private toastr:Toastr,
                private UserService:multipoly.services.UserService,
                private $scope: angular.IScope) {
      this.activate();
    }

    activate() {
      this.room = this.RoomService.getRoom(this.$stateParams.id);
      this.$scope.$on("startgame", this.onGameStarted.bind(this));
    }

    onGameStarted(event, response) {
      this.showTable();
    }

    isOwnerOfRoom() {
      return this.room.players[0].name == this.UserService.currentUser.name;
    }

    leave() {
      this.RoomService.leave(this.room);
      this.$state.go("rooms");
    }

    start() {
      this.RoomService.start(this.room);
      this.$state.go("table", {id: this.room.id});
    }

    showTable() {
      this.RoomService.setCurrentGameId(this.room.id);
      this.$state.go("table", {id: this.room.id});
    }
  }
}

angular.module('multipoly.controllers')
  .controller('RoomController', multipoly.controllers.RoomController);

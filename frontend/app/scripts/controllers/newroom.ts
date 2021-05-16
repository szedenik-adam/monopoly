/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../models/Room.ts" />
/// <reference path="../services/RoomService.ts" />
module  multipoly.controllers {
  export class NewRoomController {

    $inject = ["RoomService", "toastr", "$state"]

    room:multipoly.models.Room;

    constructor(private RoomService:multipoly.services.RoomService, private toastr:Toastr, private $state:angular.ui.IStateService) {
    }

    create() {
      this.RoomService.create(this.room);
      this.$state.go("rooms");
    }

  }
}

angular.module('multipoly.controllers')
  .controller('NewRoomController', multipoly.controllers.NewRoomController);

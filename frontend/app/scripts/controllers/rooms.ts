/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../models/Room.ts" />

module  multipoly.controllers {
  export class RoomsController {

    static $inject = ["RoomService", "UserService", "$state"];

    constructor(private RoomService:multipoly.services.RoomService,
                private UserService:multipoly.services.UserService,
                private $state:angular.ui.IStateService) {
    }

    get rooms():Array<multipoly.models.Room> {
      return this.RoomService.getRooms();
    }

    isMemberOf(room:multipoly.models.Room) {
      return this.RoomService.isMemberOf(room, this.UserService.currentUser);
    }

    enter(room:multipoly.models.Room) {
      this.RoomService.join(room);
      this.$state.go("room", {id: room.id})
    }

    join(room:multipoly.models.Room) {
      this.RoomService.join(room);
      this.$state.go("room", {id: room.id})
    }


  }
}
angular.module('multipoly.controllers')
  .controller('RoomsController', multipoly.controllers.RoomsController);


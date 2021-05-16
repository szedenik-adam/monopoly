/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../models/Room.ts" />
/// <reference path="../models/GameState.ts" />

module  multipoly.controllers {
  export class SellModalController {

    static $inject = ["RoomService", "UserService","$modalInstance"];

    gameState:multipoly.models.GameState = null;

    selectedProperty: any = null;

    constructor(private RoomService:multipoly.services.RoomService,
                private UserService:multipoly.services.UserService,
                private $modalInstance) {
      this.gameState = this.RoomService.getCurrentGameState();
    }

    myPlayerIndex() {
      for (var i = 0; i < this.gameState.players.length; i++) {
        if (this.gameState.players[i].name == this.UserService.getCurrentUser().name) {
          return i;
        }
      }
      return -1;
    }

    isMine(prop) {
      var me = this.myPlayerIndex();
      if (me == -1) return false;
      return prop.owner == me;
    }

    getMyProperties():Array<any> {
      return this.gameState.properties.filter((prop) => this.isMine(prop));
    }

    ok() {
      this.$modalInstance.close(this.selectedProperty);

    }

    cancel() {
      this.$modalInstance.dismiss('cancel');
    }

  }
}
angular.module('multipoly.controllers')
  .controller('SellModalController', multipoly.controllers.SellModalController);


/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../models/Room.ts" />
/// <reference path="../models/GameState.ts" />

module  multipoly.controllers {
  export class BidModalController {

    static $inject = ["RoomService", "UserService", "$modalInstance", "$scope", "$interval"];

    gameState:multipoly.models.GameState = null;
    bid:number;
    remainingTime:number = 0;
    timer = null;

    constructor(private RoomService:multipoly.services.RoomService,
                private UserService:multipoly.services.UserService,
                private $modalInstance,
                private $scope:angular.IScope,
                private $interval:angular.IIntervalService) {
      this.gameState = this.RoomService.getCurrentGameState();
      $scope.$watch("gameState.bid.timeleftMs", this.onRemainingTimeExpanded.bind(this));
      this.remainingTime = Math.round(this.gameState.bid.timeleftms / 1000, 0);
      this.timer = $interval(() => {
        this.remainingTime -= 1;
        if (this.remainingTime <= 0) {
          this.cancel();
          this.$modalInstance.close();
        }
      }, 1000);
    }

    onRemainingTimeExpanded(event) {
      this.remainingTime = Math.round(this.gameState.bid.timeleftms / 1000, 0);
    }


    getBidWinner() {
      if (this.gameState.bid.highestbidder > -1) {
        return this.gameState.players[this.gameState.bid.highestbidder].name;
      } else {
        return "starting price";
      }
    }

    placebid(bid) {
      this.RoomService.placeBid(this.gameState.bid.highestbid + bid);
    }

    ok() {
      this.placebid(this.bid - this.gameState.bid.highestbid);
    }

    cancel() {
      this.$modalInstance.dismiss('cancel');
    }

  }
}
angular.module('multipoly.controllers')
  .controller('BidModalController', multipoly.controllers.BidModalController);


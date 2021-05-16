/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../filters/moneyFilter.ts" />



module multipoly.controllers {
  export class TableController {
    static $inject = ["RoomService", "UserService", "$stateParams", "$modal", "toastr", "$rootScope","$filter"];

    gameState:multipoly.models.GameState = this.RoomService.getCurrentGameState();
    bidModalOpened:boolean = false;

    constructor(private RoomService:multipoly.services.RoomService,
                private UserService:multipoly.services.UserService,
                private $stateParams:angular.ui.IStateParamsService,
                private $modal,
                private toastr:Toastr,
                private $rootScope:angular.IRootScopeService,
                private $filter: angular.IFilterProvider) {
      this.activate();
    }

    onSellField(event, response) {
      if (response.error != null) {
        this.toastr.error(response.error);
      }
    }

    activate() {
      this.$rootScope.$on("sellfield", this.onSellField.bind(this));
      this.$rootScope.$on("bid", this.onBidRequest.bind(this));
    }

    myTurn() {
      return this.gameState._players[this.gameState._nextplayer].name == this.UserService.currentUser.name;
    }

    canRoll() {
      return (this.gameState.actions.indexOf("roll") != -1) && this.myTurn();
    }

    canSell() {
      return this.myTurn();
    }

    canBuy() {
      return (this.gameState.actions.indexOf("buyfield") != -1) && this.myTurn();
    }

    canEnd() {
      return (this.gameState.actions.indexOf("end") != -1) && this.myTurn();
    }

    roll() {
      this.RoomService.roll();
    }

    getPropertyByPos(pos:number) {
      for (var i = 0; i < this.gameState.properties.length; i++) {
        if (this.gameState.properties[i].pos == pos) {
          return this.gameState.properties[i];
        }
      }
      return null;
    }

    getPropertyAtMyPos() {
      var me = this.gameState.players[this.getMyIndex()];
      var prop = this.getPropertyByPos(me.pos);
      return prop;
    }

    buyField() {
      this.RoomService.buyField();
    }

    endTurn() {
      this.RoomService.endTurn();
    }

    leaveJail(type) {
      if (type == "pay") {
        this.RoomService.leaveJailWithPaying();
      } else {
        this.RoomService.leaveJailWithCard();
      }
    }

    getIndexOfPlayer(name:string) {
      for (var i = 0; i < this.gameState._players.length; i++) {
        if (this.gameState._players[i].name == name) {
          return i;
        }
      }
      return -1;
    }

    getMyIndex() {
      return this.getIndexOfPlayer(this.UserService.currentUser.name);
    }

    hasJailCard() {
      if (this.getMyIndex() > 0) {
        return this.gameState._players[this.getMyIndex()].jailcard > 0;
      } else {
        return 0;
      }
    }

    getLastCard() {
      var card = this.gameState._players[this.getMyIndex()].card;
      if (card & (1 << 30)) { // Add money happened
        var added_money = card & ~(1 << 30);
        return "Received money " + this.$filter("money")(added_money);
      } else if (card == 1) {
        return "You are jailed!";
      } else if (card == 2) {
        return "You received an 'Escape from jail' card";
      } else if (card & (1 << 30)) { //Remove money happened
        var lost_money = card & ~(1 << 31);
        return "You lost " + this.$filter("money")(lost_money);
      } else {
        return null;
      }
    }

    isInJail() {
      return this.gameState._players[this.getMyIndex()].jailed;
    }


    onBidRequest() {
      if (!this.bidModalOpened) {
        this.bidModalOpened = true;
        var modalInstance = this.$modal.open({
          templateUrl: 'views/bidModal.html',
          controller: 'BidModalController',
          controllerAs: 'vm'
        });

        modalInstance.result.then((bid) => {
          this.bidModalOpened = false;
        }, () => {
          this.bidModalOpened = false;
        });
      }
    }

    sell() {
      var modalInstance = this.$modal.open({
        templateUrl: 'views/sellModal.html',
        controller: 'SellModalController',
        controllerAs: 'vm'
      });

      modalInstance.result.then((selectedItem) => {
        this.RoomService.sellField(selectedItem.pos);
      });

    }
  }
}
angular.module('multipoly.controllers')
  .controller('TableController', multipoly.controllers.TableController);


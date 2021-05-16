/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="./Player.ts" />
/// <reference path="./Bid.ts" />


module multipoly.models {
  export class GameState {
    _id:number;
    _nextplayer:number;
    _players:Array<multipoly.models.Player>;
    _actions:Array<string>;
    _lastroll:Array<number>;
    _properties:Array<any>;
    _bid:multipoly.models.Bid;

    _round:number;

    get round():number {
      return this._round;
    }

    set round(value:number) {
      this._round = value;
    }


    get bid():multipoly.models.Bid {
      return this._bid;
    }

    set bid(value:multipoly.models.Bid) {
      this._bid = value;
    }

    get id():number {
      return this._id;
    }

    set id(value:number) {
      this._id = value;
    }

    get lastroll():Array<number> {
      return this._lastroll;
    }

    set lastroll(value:Array<number>) {
      this._lastroll = value;
    }

    get nextplayer():number {
      return this._nextplayer;
    }

    set nextplayer(value:number) {
      this._nextplayer = value;
    }

    get players():Array<multipoly.models.Player> {
      return this._players;
    }

    set players(value:Array<multipoly.models.Player>) {
      this._players = value;
    }

    get actions():Array<string> {
      return this._actions;
    }

    set actions(value:Array<string>) {
      this._actions = value;
    }

    get properties():Array<any> {
      return this._properties;
    }

    set properties(value:Array<any>) {
      this._properties = value;
    }

    parse(object:any) {
      this.players = object.players;
      this.nextplayer = object.nextplayer;
      this.actions = object.actions;
      this.lastroll = object.lastroll;
      this.properties = object.properties;
      this.round = object.round;
    }


    constructor() {
      this.bid = new multipoly.models.Bid();
    }


  }
}

angular.module("multipoly.models").factory("Player", () => multipoly.models.Player);

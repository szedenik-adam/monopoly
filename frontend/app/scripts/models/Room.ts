/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="./Player.ts" />
module multipoly.models {
  export class Room {

    _id:number;
    _name:string;
    _players:Array<multipoly.models.Player>;
    _started:boolean;
    _map:any;
    _nextPlayer: multipoly.models.Player;

    constructor() {
      this._players = new Array<multipoly.models.Player>();
      this._started = false;
      this._nextPlayer = null;
    }

    get id():number {
      return this._id;
    }

    set id(value:number) {
      this._id = value;
    }

    set name(value:string) {
      this._name = value;
    }

    set started(value:boolean) {
      this._started = value;
    }

    set players(value:Array<multipoly.models.Player>) {
      this._players = value;
    }

    get full():boolean {
      return this._players != null && this._players.length == 8;
    }

    get players():Array<multipoly.models.Player> {
      return this._players;
    }

    get started():boolean {
      return this._started;
    }

    get name():string {
      return this._name;
    }

    get map():any {
      return this._map;
    }

    set map(value:any) {
      this._map = value;
    }

    addPlayerByName(name:string) {
      var player:multipoly.models.Player = new multipoly.models.Player();
      player.name = name;
      this._players.push(player);
    }

    set nextPlayer(_player: multipoly.models.Player) {
      this._nextPlayer = _player;
    }

    get nextPlayer() {
      return this._nextPlayer;
    }

    setNextPlayerByIndex(index: number): void {
      if (this._players[index] != null) {
        this._nextPlayer = this._players[index];
      }
    }
  }
}

angular.module("multipoly.models").factory("Room", () => multipoly.models.Room);

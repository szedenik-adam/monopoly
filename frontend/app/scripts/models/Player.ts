/// <reference path="../../../typings/angularjs/angular.d.ts" />
module multipoly.models {
  export class Player {
    _name: string;
    _pos: number;
    _money: number;
    _session: string;
    _card:any;
    _jailcard:number;

    constructor(name: string = null) {
      this._pos = 0;
      this._money = 0;
      this._name = name;
    }

    get jailcard():number {
      return this._jailcard;
    }

    set jailcard(value:number) {
      this._jailcard = value;
    }

    get card():any {
      return this._card;
    }

    set card(value :any) {
      if (value != null) {
        this._card = {key: value.key, value: value.value};
      } else {
        this._card = null;
      }
    }

    get name() : string {
      return this._name;
    }

    get pos() : number {
      return this._pos;
    }

    get money() : number {
      return this._money;
    }

    get session() : string {
      return this._session;
    }

    set name(value: string) {
      this._name = value;
    }

    set pos(value: number) {
      this._pos = value;
    }

    set money(value: number) {
      this._money = value;
    }

    set session(value: string) {
      this._session = value;
    }
  }
}

angular.module("multipoly.models").factory("Player", () => multipoly.models.Player);

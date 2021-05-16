/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="./Player.ts" />
module multipoly.models {
  export class Bid {

    private _timeleftms:number;
    private _highestbid:number;
    private _highestbidder:number;

    public get timeleftms():number {
      return this._timeleftms;
    }

    public get highestbid():number {
      return this._highestbid;
    }

    public get highestbidder():number {
      return this._highestbidder;
    }

    parse(object:any) {
      this._timeleftms = object.timeleftms;
      this._highestbid = object.highestbid;
      this._highestbidder = object.highestbidder;
    }

  }

}

angular.module("multipoly.models").factory("Bid", () => multipoly.models.Bid);

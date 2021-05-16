/// <reference path="../../../typings/angularjs/angular.d.ts" />
module multipoly.models {
  export class User {

    private _name:string;
    private _email:string;
    private _password:string;
    private _session: string;

    constructor() {
    }

    public get name():string {
      return this._name;
    }

    public get email():string {
      return this._email;
    }

    public get password():string {
      return this._password;
    }

    public get session():string {
      return this._session;
    }

    public set name(value) {
      this._name = value;
    }

    public set email(value) {
      this._email = value;
    }

    public set password(value) {
      this._password = value;
    }

    public set session(value) {
      this._session = value;
    }
  }
}

angular.module("multipoly.models").factory("User", () => multipoly.models.User);

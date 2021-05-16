/// <reference path="../../../typings/angularjs/angular.d.ts" />

module  multipoly.controllers {
  export class HomeController {

    constructor() {
    }
  }
}

angular.module("multipoly.controllers")
  .controller("HomeController", multipoly.controllers.HomeController);

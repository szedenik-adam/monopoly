/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/threejs/three.d.ts" />
/// <reference path="../engine/Game.ts" />
/// <reference path="../services/RoomService.ts" />

module multipoly.directives {
  export class GameBoard implements ng.IDirective {

    public scope:any = {
      roomId: "="
    };
    public restrict:string = "AE";

    constructor(private roomService:multipoly.services.RoomService) {

    }

    public link($scope:ng.IScope, container:JQuery, attributes:ng.IAttributes) {
      var game = new multipoly.engine.Game({
        // The DOM element in which the drawing will happen.
        containerEl: container[0],
        roomService: $scope.$parent.vm.RoomService,
        // The base URL from where the BoardController will load its data.
        assetsUrl: '3d_assets/'
      });
    }
  }
}

angular.module("multipoly.directives")
  .directive("gameBoard", ["RoomService", () => new multipoly.directives.GameBoard(multipoly.services.RoomService)]);

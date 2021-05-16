/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/threejs/three.d.ts" />
/// <reference path="../engine/Game.ts" />
/// <reference path="../services/RoomService.ts" />
var multipoly;
(function (multipoly) {
    var directives;
    (function (directives) {
        var GameBoard = (function () {
            function GameBoard(roomService) {
                this.roomService = roomService;
                this.scope = {
                    roomId: "="
                };
                this.restrict = "AE";
            }
            GameBoard.prototype.link = function ($scope, container, attributes) {
                var game = new multipoly.engine.Game({
                    // The DOM element in which the drawing will happen.
                    containerEl: container[0],
                    roomService: $scope.$parent.vm.RoomService,
                    // The base URL from where the BoardController will load its data.
                    assetsUrl: '3d_assets/'
                });
            };
            return GameBoard;
        })();
        directives.GameBoard = GameBoard;
    })(directives = multipoly.directives || (multipoly.directives = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.directives").directive("gameBoard", ["RoomService", function () { return new multipoly.directives.GameBoard(multipoly.services.RoomService); }]);
//# sourceMappingURL=board.js.map
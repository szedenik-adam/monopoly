/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../services/RoomService.ts" />
/// <reference path="../services/UserService.ts" />
/// <reference path="../models/Room.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var RoomsController = (function () {
            function RoomsController(RoomService, UserService, $state) {
                this.RoomService = RoomService;
                this.UserService = UserService;
                this.$state = $state;
            }
            Object.defineProperty(RoomsController.prototype, "rooms", {
                get: function () {
                    return this.RoomService.getRooms();
                },
                enumerable: true,
                configurable: true
            });
            RoomsController.prototype.isMemberOf = function (room) {
                return this.RoomService.isMemberOf(room, this.UserService.currentUser);
            };
            RoomsController.prototype.enter = function (room) {
                this.RoomService.join(room);
                this.$state.go("room", { id: room.id });
            };
            RoomsController.prototype.join = function (room) {
                this.RoomService.join(room);
                this.$state.go("room", { id: room.id });
            };
            RoomsController.$inject = ["RoomService", "UserService", "$state"];
            return RoomsController;
        })();
        controllers.RoomsController = RoomsController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('RoomsController', multipoly.controllers.RoomsController);
//# sourceMappingURL=rooms.js.map
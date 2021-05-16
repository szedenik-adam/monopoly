/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../../../typings/toastr/toastr.d.ts" />
/// <reference path="../../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="../models/Room.ts" />
/// <reference path="../services/RoomService.ts" />
var multipoly;
(function (multipoly) {
    var controllers;
    (function (controllers) {
        var NewRoomController = (function () {
            function NewRoomController(RoomService, toastr, $state) {
                this.RoomService = RoomService;
                this.toastr = toastr;
                this.$state = $state;
                this.$inject = ["RoomService", "toastr", "$state"];
            }
            NewRoomController.prototype.create = function () {
                this.RoomService.create(this.room);
                this.$state.go("rooms");
            };
            return NewRoomController;
        })();
        controllers.NewRoomController = NewRoomController;
    })(controllers = multipoly.controllers || (multipoly.controllers = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly.controllers').controller('NewRoomController', multipoly.controllers.NewRoomController);
//# sourceMappingURL=newroom.js.map
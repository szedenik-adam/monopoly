/// <reference path="../../typings/angularjs/angular.d.ts" />
/// <reference path="../../typings/angular-ui-router/angular-ui-router.d.ts" />
/// <reference path="./controllers/home.ts" />
/// <reference path="./controllers/main.ts" />
/// <reference path="./controllers/room.ts" />
/// <reference path="./controllers/rooms.ts" />
/// <reference path="./controllers/table.ts" />
/// <reference path="./controllers/logout.ts" />
/// <reference path="./controllers/register.ts" />
/// <reference path="./controllers/login.ts" />
/// <reference path="./controllers/continue.ts" />
/// <reference path="./services/UserService.ts" />
'use strict';
var multipoly;
(function (multipoly) {
    var config;
    (function (config) {
        function uiRouter($stateProvider, $urlRouterProvider) {
            $urlRouterProvider.otherwise("/home");
            $stateProvider.state('home', {
                url: "/home",
                templateUrl: "views/home.html",
                controller: multipoly.controllers.HomeController,
                controllerAs: "vm"
            }).state('continue', {
                url: "/continue?token",
                templateUrl: "views/continue.html",
                controller: multipoly.controllers.ContinueController,
                controllerAs: "vm"
            }).state('rooms', {
                url: "/rooms",
                templateUrl: "views/rooms.html",
                controller: multipoly.controllers.RoomsController,
                controllerAs: "vm"
            }).state('newroom', {
                url: "/rooms/new",
                templateUrl: "views/newroom.html",
                controller: multipoly.controllers.NewRoomController,
                controllerAs: "vm"
            }).state('room', {
                url: "/room/:id",
                templateUrl: "views/room.html",
                controller: multipoly.controllers.RoomController,
                controllerAs: "vm"
            }).state('login', {
                url: "/login",
                templateUrl: "views/login.html",
                controller: multipoly.controllers.LoginController,
                controllerAs: "vm"
            }).state('register', {
                url: "/register",
                templateUrl: "views/register.html",
                controller: multipoly.controllers.RegisterController,
                controllerAs: "vm"
            }).state('logout', {
                url: "/logout",
                controller: multipoly.controllers.LogoutController,
                controllerAs: "vm"
            }).state('table', {
                url: "/table/:id",
                templateUrl: "views/table.html",
                controller: multipoly.controllers.TableController,
                controllerAs: "vm"
            });
        }
        config.uiRouter = uiRouter;
        function init($rootScope, UserService, $state) {
            function isOnlyForRegisteredUsers(state) {
                return (state.name == "rooms" || state.name == "room" || state.name == "newroom" || state.name == "table");
            }
            function onStateChangeStart(event, toState, toParams, fromState, fromParams) {
                if (isOnlyForRegisteredUsers(toState) && UserService.getCurrentUser() == null) {
                    event.preventDefault();
                    $state.go("login");
                    return false;
                }
            }
            $rootScope.$on("$stateChangeStart", onStateChangeStart.bind(this));
        }
        config.init = init;
    })(config = multipoly.config || (multipoly.config = {}));
})(multipoly || (multipoly = {}));
angular.module('multipoly', ['multipoly.controllers', 'multipoly.services', 'multipoly.models', 'multipoly.directives', 'multipoly.filters', 'ngAnimate', 'ui.router', 'toastr', 'ngWebSocket', 'ui.bootstrap']).constant('version', 'v0.1.0').constant("apiUrl", "http://" + location.host).constant("socketUrl", "ws://" + location.hostname + ":81").config(multipoly.config.uiRouter, ["$stateProvider", "$urlRouterProvider"]).run(multipoly.config.init, ["$rootScope,UserService,$state"]);
//# sourceMappingURL=app.js.map
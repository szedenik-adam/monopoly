/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../models/User.ts" />
/// <reference path="../services/WebSocketService.ts" />
/// <reference path="../services/RoomService.ts" />
var multipoly;
(function (multipoly) {
    var services;
    (function (services) {
        var UserService = (function () {
            function UserService($q, User, apiUrl, $http, WebSocketService, RoomService) {
                this.$q = $q;
                this.User = User;
                this.apiUrl = apiUrl;
                this.$http = $http;
                this.WebSocketService = WebSocketService;
                this.RoomService = RoomService;
                this.users = new Map();
            }
            UserService.prototype.getCurrentUser = function () {
                return this.currentUser;
            };
            UserService.prototype.register = function (user) {
                return this.$http.post(this.apiUrl + "/register", {
                    name: user.name,
                    pass: user.password,
                    email: user.email
                });
            };
            UserService.prototype.login = function (name, password) {
                var _this = this;
                return this.$http.post(this.apiUrl + "/login", {
                    name: name,
                    pass: password,
                }).then(function (response) {
                    _this.currentUser = new multipoly.models.User();
                    _this.currentUser.session = response.data["session"];
                    _this.currentUser.name = name;
                    _this.currentUser.password = password;
                    return _this.WebSocketService.fire({ cmd: "login", session: _this.currentUser.session });
                });
            };
            UserService.prototype.logout = function () {
                this.WebSocketService.close();
                this.currentUser = null;
            };
            UserService.$inject = ["$q", "User", "apiUrl", "$http", "WebSocketService", "RoomService"];
            return UserService;
        })();
        services.UserService = UserService;
    })(services = multipoly.services || (multipoly.services = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.services").service("UserService", multipoly.services.UserService);
//# sourceMappingURL=UserService.js.map
/// <reference path="../../../typings/angularjs/angular.d.ts" />
var multipoly;
(function (multipoly) {
    var models;
    (function (models) {
        var User = (function () {
            function User() {
            }
            Object.defineProperty(User.prototype, "name", {
                get: function () {
                    return this._name;
                },
                set: function (value) {
                    this._name = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(User.prototype, "email", {
                get: function () {
                    return this._email;
                },
                set: function (value) {
                    this._email = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(User.prototype, "password", {
                get: function () {
                    return this._password;
                },
                set: function (value) {
                    this._password = value;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(User.prototype, "session", {
                get: function () {
                    return this._session;
                },
                set: function (value) {
                    this._session = value;
                },
                enumerable: true,
                configurable: true
            });
            return User;
        })();
        models.User = User;
    })(models = multipoly.models || (multipoly.models = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.models").factory("User", function () { return multipoly.models.User; });
//# sourceMappingURL=User.js.map
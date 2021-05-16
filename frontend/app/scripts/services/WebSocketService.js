/// <reference path="../../../typings/angularjs/angular.d.ts" />
var multipoly;
(function (multipoly) {
    var services;
    (function (services) {
        var WebSocketService = (function () {
            function WebSocketService(socketUrl, $websocket, $q, $rootScope) {
                this.socketUrl = socketUrl;
                this.$websocket = $websocket;
                this.$q = $q;
                this.$rootScope = $rootScope;
                this.callbacks = {};
                this.init();
            }
            WebSocketService.prototype.init = function () {
                this.websocket = this.$websocket(this.socketUrl);
                this.websocket.onMessage(this.onMessageReceived.bind(this));
            };
            WebSocketService.prototype.fire = function (data) {
                this.websocket.send(JSON.stringify(data));
            };
            WebSocketService.prototype.onMessageReceived = function (message) {
                console.log(message);
                var data = JSON.parse(message.data);
                this.$rootScope.$broadcast(data.type, data);
            };
            WebSocketService.prototype.close = function () {
                this.websocket.close(true);
                this.init();
            };
            WebSocketService.$inject = ["socketUrl", "$websocket", "$q", "$rootScope"];
            return WebSocketService;
        })();
        services.WebSocketService = WebSocketService;
    })(services = multipoly.services || (multipoly.services = {}));
})(multipoly || (multipoly = {}));
angular.module("multipoly.services").service("WebSocketService", multipoly.services.WebSocketService);
//# sourceMappingURL=WebSocketService.js.map
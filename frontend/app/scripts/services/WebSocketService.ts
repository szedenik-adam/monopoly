/// <reference path="../../../typings/angularjs/angular.d.ts" />

module multipoly.services {
  export class WebSocketService {

    static $inject = ["socketUrl", "$websocket", "$q", "$rootScope"];
    callbacks:any = {};
    websocket:any;

    constructor(private socketUrl:string, private $websocket:any, private $q:angular.IQService, private $rootScope:angular.IRootScopeService) {
      this.init();
    }

    init() {
      this.websocket = this.$websocket(this.socketUrl);
      this.websocket.onMessage(this.onMessageReceived.bind(this));
    }

    fire(data) {
      this.websocket.send(JSON.stringify(data));
    }

    onMessageReceived(message) {
      console.log(message);
      var data = JSON.parse(message.data);
      this.$rootScope.$broadcast(data.type, data);
    }

    close() {
      this.websocket.close(true);
      this.init();
    }
  }
}

angular.module("multipoly.services").service("WebSocketService", multipoly.services.WebSocketService);

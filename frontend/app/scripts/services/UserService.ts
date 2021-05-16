/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../models/User.ts" />
/// <reference path="../services/WebSocketService.ts" />
/// <reference path="../services/RoomService.ts" />
module multipoly.services {
  export class UserService {

    static $inject = ["$q", "User", "apiUrl", "$http", "WebSocketService", "RoomService"];

    users:Map<String,multipoly.models.User>;
    currentUser:multipoly.models.User;

    constructor(private $q:angular.IQService,
                private User:multipoly.models.User,
                private apiUrl:string,
                private $http:angular.IHttpService,
                private WebSocketService:multipoly.services.WebSocketService,
                private RoomService:multipoly.services.RoomService) {
      this.users = new Map<String,multipoly.models.User>();
    }

    getCurrentUser():multipoly.models.User {
      return this.currentUser;
    }

    register(user):angular.IHttpPromise<any> {
      return this.$http.post(this.apiUrl + "/register", {
        name: user.name,
        pass: user.password,
        email: user.email
      });
    }

    login(name, password):angular.IPromise<void> {
      return this.$http.post(this.apiUrl + "/login", {
        name: name,
        pass: password,
      }).then(
        (response) => {
          this.currentUser = new multipoly.models.User();
          this.currentUser.session = response.data["session"];
          this.currentUser.name = name;
          this.currentUser.password = password;

          return this.WebSocketService.fire({cmd: "login", session: this.currentUser.session});
        }
      );
    }

    logout() {
      this.WebSocketService.close();
      this.currentUser = null;
    }
  }
}

angular.module("multipoly.services").service("UserService", multipoly.services.UserService);

/// <reference path="../../../typings/angularjs/angular.d.ts" />
/// <reference path="../models/Room.ts" />
/// <reference path="../models/GameState.ts" />
/// <reference path="../services/WebSocketService.ts" />

module multipoly.services {
  export class RoomService {

    static $inject = ["$q", "apiUrl", "$http", "WebSocketService", "$rootScope"];

    socket:any;

    _rooms:Array<multipoly.models.Room>;
    currentGameState:multipoly.models.GameState = null;
    stateChangedCallback:any = null;

    constructor(private $q:angular.IQService,
                private apiUrl:string,
                private $http:angular.IHttpService,
                private WebSocketService:multipoly.services.WebSocketService,
                private $rootScope:angular.IRootScopeService) {

      this._rooms = new Array<multipoly.models.Room>();
      this.$rootScope.$on("room", this.onRoomInfoReceived.bind(this));
      this.$rootScope.$on("roomremove", this.onRoomRemoved.bind(this));
      this.$rootScope.$on("login", this.onLoggedIn.bind(this));
      this.$rootScope.$on("status", this.onStatusChanged.bind(this));
      this.$rootScope.$on("startgame", this.onGameStarted.bind(this));
      this.$rootScope.$on("bid", this.onBidRequestReceived.bind(this));

    }

    onBidRequestReceived(event, response) {
      if (response.error == null) {
        if (this.currentGameState.bid == null) {
          this.currentGameState.bid = new multipoly.models.Bid();
        }
        this.currentGameState.bid.parse(response);
      }
    }

    onGameStarted(event, response) {
      this.getRoom(response.mapid).started = true;
    }

    setStateChangedCallback(fn:any) {
      this.stateChangedCallback = fn;
    }

    notifyStateChanged() {
      if (this.stateChangedCallback != null) {
        this.stateChangedCallback(this.currentGameState);
      }
    }

    onStatusChanged(event, response) {
      if (this.currentGameState == null) {
        this.currentGameState = new multipoly.models.GameState();
      }
      this.currentGameState.parse(response);
      this.notifyStateChanged();
    }

    onRoomRemoved(event, response) {
      for (var i = 0; i < this._rooms.length; i++) {
        if (this._rooms[i].id == response.id) {
          delete this._rooms[i];
          this._rooms.length--;
        }
      }
    }

    onLoggedIn(event, response) {
      this._rooms = <Array<multipoly.models.Room>>response.maps;
    }

    setCurrentGameId(_id:number) {
      if (this.currentGameState == null) {
        this.currentGameState = new multipoly.models.GameState();
      }
      this.currentGameState.id = _id;
    }

    roomExistsWithId(id) {
      return (this.getRoom(id) != null);
    }

    isMemberOf(room, user) {
      for (var i = 0; i < room.players.length; i++) {
        if (room.players[i].name == user.name) {
          return true;
        }
      }
      return false;
    }

    updateRoom(room:multipoly.models.Room):boolean {
      var changed = false;
      if (this.roomExistsWithId(room.id)) {
        var i = 0;
        while (!changed && i < this._rooms.length) {
          if (this._rooms[i].id == room.id) {
            if (room.name != null) {
              this._rooms[i].name = room.name;
            }
            if (room.players != null) {
              this._rooms[i].players = room.players;
            }
            if (room.map != null) {
              this._rooms[i].map = room.map;
            }
            changed = true;
          }
          i++;
        }
      } else {
        this._rooms.push(room);
        changed = true;
      }
      return changed;
    }

    onRoomInfoReceived(event, _room) {
      var room:multipoly.models.Room = new multipoly.models.Room();
      room.id = _room.id;
      room.name = _room.name;
      room.players = _room.players;
      this.updateRoom(room);
    }

    setRooms(rooms:Array<multipoly.models.Room>) {
      this._rooms = rooms;
    }

    public getRooms():Array<multipoly.models.Room> {
      return this._rooms;
    }

    public getRoom(id:number):multipoly.models.Room {
      var room = null;
      var i = 0;
      while (room == null && i < this._rooms.length) {
        if (this._rooms[i].id == id) {
          room = this._rooms[i];
        }
        i++;
      }
      return room;
    }

    public create(room:multipoly.models.Room):angular.IPromise<any> {
      this.WebSocketService.fire({cmd: 'createroom', name: room.name});
    }

    public join(room:multipoly.models.Room):angular.IPromise<any> {
      this.WebSocketService.fire({cmd: 'join', id: room.id});
    }

    public leave(room:multipoly.models.Room):angular.IPromise<any> {
      this.WebSocketService.fire({cmd: 'leave', id: room.id});
    }

    public start(room:multipoly.models.Room):angular.IPromise<any> {
      this.WebSocketService.fire({cmd: 'startgame', id: room.id});
    }

    public getCurrentGameState():multipoly.models.GameState {
      return this.currentGameState;
    }

    public roll() {
      this.WebSocketService.fire({cmd: 'roll'});
    }

    public endTurn() {
      this.WebSocketService.fire({cmd: 'end'});
    }

    public buyField() {
      this.WebSocketService.fire({cmd: 'buyfield'});
    }

    public sellField(id: number) {
      this.WebSocketService.fire({cmd: 'sellfield', field: id});
    }

    public placeBid(bid: number) {
      this.WebSocketService.fire({cmd: 'bid', value: bid});
    }

    public leaveJailWithCard() {
      this.WebSocketService.fire({cmd: 'leavejailwithcard'});
    }

    public leaveJailWithPaying() {
      this.WebSocketService.fire({cmd: 'leavejailwithpaying'});
    }

    public continue(token) {
      this.WebSocketService.fire({cmd: 'continue', token: token});
    }

    public getColorForPlayer(index:number) {
      var colors = [
        /*blue*/0x3366FF,
        /*green*/ 0x339933,
        /*red*/ 0xCC0033,
        /*purple*/ 0xFF33FF,
        /*orange*/ 0xFFCC00,
        /*yellow*/ 0xFFFF66,
        /*black*/ 0x000000,
        /*white*/ 0xFFFFFF
      ];
      return colors[index % colors.length];
    }

  }
}

angular.module("multipoly.services").service("RoomService", multipoly.services.RoomService);

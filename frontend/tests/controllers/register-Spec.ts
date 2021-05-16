/// <reference path="../../typings/jasmine/jasmine.d.ts" />
/// <reference path="../../typings/angularjs/angular.d.ts" />
/// <reference path="../../typings/angularjs/angular-mocks.d.ts" />
/// <reference path="../../app/scripts/controllers/register.ts" />

describe("register controller", function () {

  var controller:multipoly.controllers.RegisterController;
  var scope:angular.IScope;

  beforeEach(angular.mock.module("multipoly"));
  beforeEach(inject(($rootScope:angular.IRootScopeService) => {
    scope = $rootScope.$new();
  }));

  beforeEach(inject(($controller:angular.IControllerService) => {
    controller = $controller('RegisterController', {
      $scope: scope
    });
  }));




  it('s okay', () => {

  });


});

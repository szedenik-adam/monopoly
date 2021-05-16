/// <reference path="../../typings/jasmine/jasmine.d.ts" />
/// <reference path="../../typings/angularjs/angular.d.ts" />
/// <reference path="../../typings/angularjs/angular-mocks.d.ts" />
/// <reference path="../../app/scripts/controllers/register.ts" />
describe("register controller", function () {
    var controller;
    var scope;
    beforeEach(angular.mock.module("multipoly"));
    beforeEach(inject(function ($rootScope) {
        scope = $rootScope.$new();
    }));
    beforeEach(inject(function ($controller) {
        controller = $controller('RegisterController', {
            $scope: scope
        });
    }));
    it('s okay', function () {
    });
});
//# sourceMappingURL=register-Spec.js.map
/// <reference path="../../../typings/angularjs/angular.d.ts" />

module monopoly.filters {
  export function moneyFilter() {
    return function (input) {
      if (input == null) return 0;
      return input.toFixed(2).replace(/./g, function (c, i, a) {
        return i && c !== "." && ((a.length - i) % 3 === 0) ? ',' + c : c;
      }) + " Ft";
    }
  }
}
angular.module("multipoly.filters").filter("money", monopoly.filters.moneyFilter);

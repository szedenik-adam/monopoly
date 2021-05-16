/// <reference path="../../../typings/angularjs/angular.d.ts" />
var monopoly;
(function (monopoly) {
    var filters;
    (function (filters) {
        function moneyFilter() {
            return function (input) {
                if (input == null)
                    return 0;
                return input.toFixed(2).replace(/./g, function (c, i, a) {
                    return i && c !== "." && ((a.length - i) % 3 === 0) ? ',' + c : c;
                }) + " Ft";
            };
        }
        filters.moneyFilter = moneyFilter;
    })(filters = monopoly.filters || (monopoly.filters = {}));
})(monopoly || (monopoly = {}));
angular.module("multipoly.filters").filter("money", monopoly.filters.moneyFilter);
//# sourceMappingURL=moneyFilter.js.map
var app = angular.module('app');

app.directive("scroll", function($timeout) {
    return {
        restrict: "A",
        link: function(scope, element, attr) {
            scope.$watchCollection(attr.scroll, function(newVal) {
                $timeout(function() {
                    element[0].scrollTop = element[0].scrollHeight;
                });
            });
        }
    }
});

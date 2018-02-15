var app = angular.module('app');

app.directive("reloadInfo", function() {
    return {
        restrict : "E",
        transclude: true,
        templateUrl : "templates/reload_info.html"
    };
});

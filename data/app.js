/*******************************************************************************
 * AngularJS Application                                                       *
 ******************************************************************************/

var app = angular.module('app', ['ngRoute', 'ngMd5', 'ngSanitize']);

/* Alle Pagelets bei Aufruf der Hauptseite cachen */
app.run(function ($templateCache, $route, $http) {
    var url;

    for (var i in $route.routes)
    {
        if (url = $route.routes[i].templateUrl)
        {
            $http.get(url, {cache: $templateCache});
        }
    }

    /*
     * Erweiterung der Number-Klasse um Funktion pad().
     * Funktion fügt Zahlen führende Nullen hinzu. Über
     * den optionalen Parameter size kann die gewünschte
     * Gesamtlänge angegeben werden (Standard: 2).
     * Das zurückgegebene Objekt ist ein String.
     */
    Number.prototype.pad = function(size) {
        var s = String(this);
        while (s.length < (size || 2)) {s = "0" + s;}
        return s;
    }
})


/*******************************************************************************
 * Routen                                                                      *
 ******************************************************************************/

app.config(function($routeProvider, $locationProvider) {
    $routeProvider

        .when('/', {
            templateUrl :   'views/home.html',
            controller  :   'homeCtrl',
            activeTab   :   'home'
        })

        .when('/diagnostic/info', {
            templateUrl :   'views/diagnostic/info.html',
            controller  :   'diagnosticInformationCtrl',
            activeTab   :   'diagnostic'
        })

        .otherwise({ redirectTo: '/' });
})


/*******************************************************************************
 * Hauptcontroller SPA                                                         *
 ******************************************************************************/

app.controller('appCtrl', ['$scope', '$route', 'Page', function($scope, $route, Page) {
    $scope.page = Page;
    $scope.route = $route;
}]);


app.controller('navBarCtrl', ['$scope', '$http', '$timeout', '$location', 'Page', function($scope, $http, $timeout, $location, Page) {
    $scope.system_label = "";

    $scope.isActive = function (viewLocation) {
        return viewLocation === $location.path();
    };
}]);

var app = angular.module('app');

app.controller('homeCtrl', function($scope, $http, $timeout, Page) {
    Page.setSubTitle("");

    $scope.show_picture = false;
    $scope.show_oem = false;

    $timeout(function () {
        $scope.show_picture = true;
        angular.element("#picture").addClass("fadeInLeft");
    }, 400);

    $timeout(function () {
        $scope.show_oem = true;
        angular.element("#oem").addClass("fadeInUp");
    }, 800);

    $timeout(function () {
        angular.element("#home").addClass("pulse");
    }, 1200);
});

var app = angular.module('app');

app.controller('setupVehicleCtrl', function($scope, $timeout, $http, Page, DataAutoReload) {
    Page.setSubTitle("Setup Vehicle");

    $scope.ardata = {};

    function getVehicle() {
        $http.get("vehicle.json")
            .success(function(response) {
                $scope.ardata = response;
            })
    }

    $scope.saveClicked = function () {

        $http.get("setVehicle.json?" +
            "manufacturer=" + $scope.ardata.manufacturer +
            "&build_series=" + $scope.ardata.build_series +
            "&nameplate=" + $scope.ardata.nameplate +
            "&model=" + $scope.ardata.model +
            "&trim_level=" + $scope.ardata.trim_level +
            "&year_of_manufacture=" + $scope.ardata.year_of_manufacture +
            "&engine=" + $scope.ardata.engine +
            "&transmission=" + $scope.ardata.transmission +
            "&kba=" + $scope.ardata.kba
        );

    }

    /* Automatische Aktualisierung der Daten einrichten */
    //$scope.ardata = { response : {}, last_update : ""};
    //data_loop = DataAutoReload.setup("info.json", 1250, $scope.ardata);

    //$scope.$on('$destroy', function() {
    //    DataAutoReload.cancel(data_loop)
    //});

    getVehicle();

    /* Informationen in Modal laden */
    $scope.modal_info;

    $scope.updateModal = function (test_case) {
        $scope.modal_info = test_case;
    }
    /* Ladeanimation nach beenden deaktivieren, um Modal nicht zu blockieren */
    $scope.loading_animation = true;

    $timeout(function () {
        $scope.loading_animation = false;
    }, 1500);
});

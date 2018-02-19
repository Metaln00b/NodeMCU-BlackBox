var app = angular.module('app');

app.controller('diagnosticDataCtrl', function($scope, $timeout, $http, Page, DataAutoReload) {
    Page.setSubTitle("Live Data");

    $scope.ardata_vehicle = {};

    function getVehicle() {
        $http.get("vehicle.json")
            .success(function(response) {
                $scope.ardata_vehicle = response;
            })
    }

    function getSettings() {
        $http.get("settings.json")
            .success(function(response) {
                $scope.ardata_settings = response;
                if ($scope.ardata_settings.onoff === 'true') {
                    $scope.onoff = true;
                } else {
                    $scope.onoff = false;
                }
            })
    }

    function FORMULA(raw, i_min, i_max, o_min, o_max) {
        return ((raw - i_min) * (o_max - o_min) / (i_max - i_min) + o_min);
    }

    /* Automatische Aktualisierung der Daten einrichten */
    $scope.ardata = { response : {}, last_update : ""};
    data_loop = DataAutoReload.setup("data.json", 500, $scope.ardata);

    $scope.getRPM_percentage = function () {
        return (100 * (parseInt($scope.ardata.response.rpm) / parseInt($scope.ardata.response.rpm_max))).toFixed(2);
    }

    $scope.getEngTemp_percentage = function () {
        return (FORMULA(parseInt($scope.ardata.response.engtemp_degC), parseInt($scope.ardata.response.engtemp_degC_min), parseInt($scope.ardata.response.engtemp_degC_max), 0, 100)).toFixed(2);
    }

    $scope.getVolume_percentage = function () {
        return (100 * (parseInt($scope.ardata.response.volume_l) / parseInt($scope.ardata.response.volume_l_max))).toFixed(2);
    }

    $scope.getCustomValue_percentage = function () {
        return (FORMULA(parseInt($scope.ardata.response.custom_value), parseInt($scope.ardata_settings.outputmin), parseInt($scope.ardata_settings.outputmax), 0, 100)).toFixed(2);
    }

    $scope.$on('$destroy', function() {
        DataAutoReload.cancel(data_loop)
    });

    getVehicle();
    getSettings();

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

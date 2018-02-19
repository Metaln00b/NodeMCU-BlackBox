var app = angular.module('app');

app.controller('displaySettingsCtrl', function($scope, $timeout, $http, Page, DataAutoReload) {
    Page.setSubTitle("Display Settings");

    $scope.onoff = false;
    $scope.ardata = {};
    //$scope.ardata_old = {};

    function getSettings() {
        $http.get("settings.json")
            .success(function(response) {
                $scope.ardata = response;
                //$scope.ardata_old = angular.copy($scope.ardata);
                if ($scope.ardata.onoff === 'true') {
                    $scope.onoff = true;
                    angular.element(".btnON").addClass("active");
                    angular.element(".btnOFF").removeClass("active");
                } else {
                    angular.element(".btnOFF").addClass("active");
                    angular.element(".btnON").removeClass("active");
                }
            })
    }
/*
    function getInfo() {
        $http.get("info.json")
            .success(function(response) {
                $scope.ardata = response.value;
            })
    }
*/

    $scope.activate = function () {
        angular.element(".btnON").addClass("active");
        angular.element(".btnOFF").removeClass("active");
        $scope.onoff = true;
    }

    $scope.deactivate = function () {
        angular.element(".btnOFF").addClass("active");
        angular.element(".btnON").removeClass("active");
        $scope.onoff = false;
    }

    $scope.saveClicked = function () {

        if ($scope.onoff === true) {
            $scope.ardata.onoff = 'true';
        }
        else
        {
            $scope.ardata.onoff = 'false';
        }

        $http.get("setSettings.json?" +
            "onoff=" + $scope.ardata.onoff +
            "&name=" + $scope.ardata.name +
            "&id=" + $scope.ardata.id +
            "&byte_n=" + $scope.ardata.byte_n +
            "&inputmin=" + $scope.ardata.inputmin +
            "&inputmax=" + $scope.ardata.inputmax +
            "&outputmin=" + $scope.ardata.outputmin +
            "&outputmax=" + $scope.ardata.outputmax +
            "&unit=" + $scope.ardata.unit
        );
    }

    /* Automatische Aktualisierung der Daten einrichten */
    //$scope.ardata = { response : {}, last_update : ""};
    //data_loop = DataAutoReload.setup("info.json", 1250, $scope.ardata);

    //$scope.$on('$destroy', function() {
    //    DataAutoReload.cancel(data_loop)
    //});

    getSettings();
//    getInfo();

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

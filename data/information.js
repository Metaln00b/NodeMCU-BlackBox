var app = angular.module('app');

app.controller('diagnosticInformationCtrl', function($scope, $timeout, $http, Page, DataAutoReload) {
    Page.setSubTitle("Information");

    function getInfo() {
        $http.get("info.json")
            .success(function(response) {
                $scope.ardata = response;
            })
    }

    $scope.saveClicked = function () {

        $http.get("setinfo.json?" +
            "id=" + $scope.ardata.id +
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

    getInfo();

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

var app = angular.module('app');

app.factory('DataAutoReload', function($interval, $http) {

    return {
        setup: function(url, time_ms, data, success, error) {
            function getData(success, error) {
                var http_promise = $http.get(url);
                http_promise.success(function (response) {
                    data.response = response;

                    date = new Date();
                    data.last_update = date.toLocaleString();

                    if (typeof success !== 'undefined') {
                        success(response);
                    }
                });

                if (typeof error !== 'undefined') {
                    http_promise.error(error);
                }
            }

            getData(success, error);

            var interval_promise = $interval(function() {
                getData(success, error);
            }, time_ms);

            return interval_promise
         },

        start: function(promise) { },
        stop: function(promise) { },

        cancel: function(promise) {
            $interval.cancel(data_loop);
        },
    };
});

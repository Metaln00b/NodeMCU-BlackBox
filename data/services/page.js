var app = angular.module('app');

app.factory('Page', function() {
    var baseTitle = "";
    var subTitle = "";

    return {
        getTitle: function() {
            if (subTitle.length > 0) {
                return baseTitle + " ("+ subTitle +")";
            } else {
                return baseTitle;
            }
         },

        getBaseTitle: function() { return baseTitle; },
        getSubTitle: function() { return subTitle; },

        setBaseTitle: function(newBaseTitle) { baseTitle = newBaseTitle },
        setSubTitle: function(newSubTitle) { subTitle = newSubTitle }
    };
});

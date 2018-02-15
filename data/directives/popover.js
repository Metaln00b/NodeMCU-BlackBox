var app = angular.module('app');

app.directive('popover', function($compile){
    return {
        restrict: 'A',
        link: function(scope, element, attributes) {
            var temp = angular.element("#" + attributes.popoverTemplateId);
            var contents = $compile(temp)(scope);

            $(element).popover({
                container: 'body',
                html: true,
                content: contents,
                placement: attributes.popoverPlacement
            });
        }
    };
});

(function () {
    return function (pageID) {
        var __ag__js_handlers__ = [];
        var __ag__native_handlers__ = [];
        var rootNode = AG.getPageRootNodeById(pageID);


        function registerJSHandler(handlerName, func) {
            __ag__js_handlers__[handlerName] = func;
        }

        function callNativeHandler(handlerName, params) {
            var handler = __ag__native_handlers__[handlerName];
            handler && handler(params);
        }

        return {
            evaluate: function (script) {
                return eval(script);
            },
            callJSHandler: function (handlerName, params) {
                var handler = __ag__js_handlers__[handlerName];
                handler && handler(params);
            },
            registerNativeHandler: function (handlerName, func) {
                __ag__native_handlers__[handlerName] = func;
            }
        };
    }
})();
var global = this;
global.window = global;
var window = global;

if (!Object.assign) {
    Object.assign = function (target) {
        for (var i = 1; i < arguments.length; i++) {
            var source = arguments[i];
            for (var key in source) {
                if (Object.prototype.hasOwnProperty.call(source, key)) {
                    target[key] = source[key];
                }
            }
        }
        return target;
    };
}

var AG = {
    getPageRootNodeById: function () {
    },
    createCanvasNode: function () {
    },
    getOptionsFromBundle: function () {
    },
    getCanvasNodeById: function () {
    },
    registerChart: function (chart) {

    },
    initGestures: function () {

    },
    log: function () {

    },
    documentElement: {},
};

var document = {
    createElement: function (elementType) {
        if (elementType === 'canvas') {
            return AG.createCanvasNode();
        }
    },
    getElementById: function (id) {
        if (typeof id === 'number' || typeof id === 'string') {
            return AG.getPageRootNodeById(id);
        }
    },
};

var navigator = undefined;
var location = undefined;

function _contextCreateImageData(width, height) {
    return {data: new Array(4 * width * height), width: width, height: height};
}
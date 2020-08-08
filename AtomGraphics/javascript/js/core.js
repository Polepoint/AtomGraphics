this.window = this;

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
    getCanvasNodeById: function () {
    },
    log: function () {

    },

    createImage: function () {

    },

    documentElement: {},
};

// var console = {
//
//     log: function (msg) {
//         AG.log(msg);
//     }
// };

var navigator = undefined;
var location = undefined;

function Image() {
    return AG.createImage();
}
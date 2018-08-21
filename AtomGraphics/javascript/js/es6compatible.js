if (!Array.prototype.includes) {
    Array.prototype.includes = function() {
        Object.defineProperty(Array.prototype, 'includes', {
            value: function(searchElement, fromIndex) {
                if (this == null) {
                    throw new TypeError('"this" is null or not defined');
                }
                var o = Object(this);
                var len = o.length >>> 0;
                if (len === 0) {
                    return false;
                }
                var n = fromIndex | 0;
                var k = Math.max(n >= 0 ? n : len - Math.abs(n), 0);
                while (k < len) {
                    if (o[k] === searchElement) {
                        return true;
                    }
                    k++;
                }
                return false;
            },
        });
    };
}

if (typeof Symbol === 'undefined') {
    var _createClass = function() {
        function defineProperties(target, props) {
            for (var i = 0; i < props.length; i++) {
                var descriptor = props[i];
                descriptor.enumerable = descriptor.enumerable || false;
                descriptor.configurable = true;
                if ('value' in descriptor) descriptor.writable = true;
                Object.defineProperty(target, descriptor.key, descriptor);
            }
        }

        return function(Constructor, protoProps, staticProps) {
            if (protoProps) defineProperties(Constructor.prototype, protoProps);
            if (staticProps) defineProperties(Constructor, staticProps);
            return Constructor;
        };
    }();

    function _classCallCheck(instance, Constructor) {
        if (!(instance instanceof Constructor)) {
            throw new TypeError('Cannot call a class as a function');
        }
    }


    var Symbol = function() {
        function Symbol(key) {
            _classCallCheck(this, Symbol);

            this.key = key;
        }

        _createClass(Symbol, [{
            key: 'toString',
            value: function toString() {
                return '' + this.key;
            },
        }], [{
            key: 'for',
            value: function _for(key) {
                var sharedSymbol = Symbol.sharedSymbol;
                if (sharedSymbol[key] === undefined) {
                    var obj = new Symbol(key);
                    sharedSymbol[key] = obj;
                    return obj;
                }
                return sharedSymbol[key];
            },
        }, {
            key: 'keyFor',
            value: function keyFor(sb) {
                if (sb === undefined) {
                    return '';
                }
                return sb.key;
            },
        }]);

        return Symbol;
    }();

    Symbol.sharedSymbol = {};

    window.Symbol = Symbol;
}
(function (global, factory) {
	typeof exports === 'object' && typeof module !== 'undefined' ? module.exports = factory() :
	typeof define === 'function' && define.amd ? define(factory) :
	(global.VanMobileExp = factory());
}(this, (function () { 'use strict';

var keys = Object.keys || function(obj){

    var keys = [];

    forOwn(obj, function(value, key){
        keys.push(key);
    });

    return keys;
};

/**
 * Created by eason on 2017/7/28.
 */

var freeze = Object.freeze;

// assume there are only two types of global
var global$1 = typeof window === 'object' ? window : global$1;

var DEV = global$1.__VANDEV__;

var isObject = "[object Object]" === Object.prototype.toString.call(null) ?
    function(t) {
        return null !== t && void 0 !== t && "[object Object]" === Object.prototype.toString.call(t) && void 0 === t.ownerDocument
    } : function(t) {
        return "[object Object]" === Object.prototype.toString.call(t)
    };

var forOwn = function (obj, cb) {
    for (var n in obj){
        if (obj.hasOwnProperty(n)) {
            if (cb(obj[n], n) === false) {
                return
            }
        }
    }
};

var forEach = function (arr, cb) {
    for (var i = 0, len = arr.length; i < len; i++) {
        if (cb(arr[i], i) === false) {
            return
        }
    }
};

var map = function (arr, f) {
    var newArr = [];
    for (var i = 0, len = arr.length; i < len; i++) {
        newArr.push(f(arr[i], i, arr));
    }
    return newArr
};

/**
 * test if superset contains subset
 * @param {Object} superset
 * @param {Object} subset
 * @returns {boolean}
 */
function isMatch (superset, subset) {
    for (var k in subset) {
        if (subset.hasOwnProperty(k)
            && superset.hasOwnProperty(k)
            && superset[k] !== subset[k]) {
            return false
        }
    }
    return true
}

// https://gist.github.com/developit/6e518a2ea4f2da03b0a8e4a2f81bc362
var shallowEqual = function (a, b) {
    for (var key in a) { if (a[key]!==b[key]) { return false; } }
    for (var key$1 in b) { if (!(key$1 in a)) { return false; } }
    return true;
};

var wontBind = {
    constructor: true,
    render: true,
    componentWillMount: true,
    componentDidMount: true,
    componentWillReceiveProps: true,
    shouldComponentUpdate: true,
    componentWillUpdate: true,
    componentDidUpdate: true,
    componentWillUnmount: true,
    forceUpdate: true,
    componentDidCatch: true,
};
// todo, test in IE8
var autoBind = Object.getOwnPropertyNames ?
    function (self) {
        var proto = self.constructor.prototype;
        map(Object.getOwnPropertyNames(proto), function (k) {
            var v = self[k];
            if (typeof v === 'function' && !wontBind[k]) {
                self[k] = v.bind(self);
            }
        });
    }
    :
    function (self) {
        var proto = self.constructor.prototype;
        forOwn(proto, function (v, k) {
            if (typeof v === 'function' && !wontBind[k]) {
                self[k] = v.bind(self);
            }
        });
    };

var frozenSymbol = Symbol.for('frozenSymbol');
function freezeAll (obj) {
    if (!obj
        || typeof obj !== 'object'
        || Object.isFrozen(obj)
        || obj[frozenSymbol]
    ) {
        return
    }

    // obj but not plain obj, e.g, fn, instance
    if (!Array.isArray(obj)) {
        var proto = obj;
        while (Object.getPrototypeOf(proto) !== null) {
            proto = Object.getPrototypeOf(proto);
        }
        if (Object.getPrototypeOf(obj) !== proto) {
            obj[frozenSymbol] = true;
            return
        }
    }

    for (var k in obj) {
        if (obj.hasOwnProperty(k)) {
            freezeAll(obj[k]);
        }
    }
    freeze(obj);
    return obj
}

/**
 * Created by Jeffrey on 2018/1/17.
 */

var assign = Object.assign || function(target){
    var arguments$1 = arguments;


    for (var j = 1, len = arguments.length; j < len; j++) {
        forOwn(arguments$1[j], function(value, key){
            target[key] = value;
        });
    }

    return target;
};

/**
 * Created by Jeffrey on 2018/1/17.
 */

var create = Object.create || function (proto) {
    function F () {}
    F.prototype = proto;
    return new F;
};

/** Virtual DOM Node */
function VNode() {}

/** Global options
 *	@public
 *	@namespace options {Object}
 */
var options = {

	/** If `true`, `prop` changes trigger synchronous component updates.
	 *	@name syncComponentUpdates
	 *	@type Boolean
	 *	@default true
	 */
	//syncComponentUpdates: true,

	/** Processes all created VNodes.
	 *	@param {VNode} vnode	A newly-created VNode to normalize/process
	 */
	//vnode(vnode) { }

	/** Hook invoked after a component is mounted. */
	// afterMount(component) { }

	/** Hook invoked after the DOM is updated with a component's latest render. */
	// afterUpdate(component) { }

	/** Hook invoked immediately before a component is unmounted. */
	// beforeUnmount(component) { }
};

var stack = [];

var EMPTY_CHILDREN = [];

/**
 * JSX/hyperscript reviver.
 * @see http://jasonformat.com/wtf-is-jsx
 * Benchmarks: https://esbench.com/bench/57ee8f8e330ab09900a1a1a0
 *
 * Note: this is exported as both `h()` and `createElement()` for compatibility reasons.
 *
 * Creates a VNode (virtual DOM element). A tree of VNodes can be used as a lightweight representation
 * of the structure of a DOM tree. This structure can be realized by recursively comparing it against
 * the current _actual_ DOM structure, and applying only the differences.
 *
 * `h()`/`createElement()` accepts an element name, a list of attributes/props,
 * and optionally children to append to the element.
 *
 * @example The following DOM tree
 *
 * `<div id="foo" name="bar">Hello!</div>`
 *
 * can be constructed using this function as:
 *
 * `h('div', { id: 'foo', name : 'bar' }, 'Hello!');`
 *
 * @param {string} nodeName	An element name. Ex: `div`, `a`, `span`, etc.
 * @param {Object} attributes	Any attributes/props to set on the created element.
 * @param rest			Additional arguments are taken to be children to append. Can be infinitely nested Arrays.
 *
 * @public
 */
function h(nodeName, attributes) {
	var arguments$1 = arguments;

	var children=EMPTY_CHILDREN, lastSimple, child, simple, i;
	for (i=arguments.length; i-- > 2; ) {
		stack.push(arguments$1[i]);
	}
	if (attributes && attributes.children!=null) {
		if (!stack.length) { stack.push(attributes.children); }
		delete attributes.children;
	}
	while (stack.length) {
		if ((child = stack.pop()) && child.pop!==undefined) {
			for (i=child.length; i--; ) { stack.push(child[i]); }
		}
		else {
			if (typeof child==='boolean') { child = null; }

			if ((simple = typeof nodeName!=='function')) {
				if (child==null) { child = ''; }
				else if (typeof child==='number') { child = String(child); }
				else if (typeof child!=='string') { simple = false; }
			}

			if (simple && lastSimple) {
				children[children.length-1] += child;
			}
			else if (children===EMPTY_CHILDREN) {
				children = [child];
			}
			else {
				children.push(child);
			}

			lastSimple = simple;
		}
	}

	var p = new VNode();
	p.nodeName = nodeName;
	p.children = children;
	p.attributes = attributes==null ? undefined : attributes;
	p.key = attributes==null ? undefined : attributes.key;

	{
        // if a "vnode hook" is defined, pass every created VNode to it
        if (options.vnode !== undefined) { options.vnode(p); }
    }

	return p;
}

/**
 *  Copy all properties from `props` onto `obj`.
 *  @param {Object} obj		Object onto which properties should be copied.
 *  @param {Object} props	Object from which to copy properties.
 *  @returns obj
 *  @private
 */
function extend(obj, props) {
	for (var i in props) { obj[i] = props[i]; }
	return obj;
}

/**
 * Call a function asynchronously, as soon as possible. Makes
 * use of HTML Promise to schedule the callback if available,
 * otherwise falling back to `setTimeout` (mainly for IE<11).
 *
 * @param {Function} callback
 */
var Promise = typeof window !== 'undefined' && window.Promise;
var defer = typeof Promise=='function' ? Promise.resolve().then.bind(Promise.resolve()) : setTimeout;

// render modes

var NO_RENDER = 0;
var SYNC_RENDER = 1;
var FORCE_RENDER = 2;
var ASYNC_RENDER = 3;


var ATTR_KEY = '__vanattr_';
var RENDERED = 1;

var TransformNamesMap = {
    position: 1,
    scale: 1,
    origin: 1,
};
var AnimationNamesMap = {
    position: 1,
    scale: 1,
    rotation: 1,
    origin: 1,
    shape: 1,
    style: 1,
};
var TransformNames = keys(TransformNamesMap);
var AnimationNames = keys(AnimationNamesMap);

/** Managed queue of dirty components to be re-rendered */

var items = [];

function enqueueRender(component) {
	if (!component._dirty && (component._dirty = true) && items.push(component)==1) {
		(options.debounceRendering || defer)(rerender);
	}
}

function rerender() {
	var p, list = items;
	items = [];
	while ( (p = list.pop()) ) {
		if (p._dirty) { renderComponent(p); }
	}
}

function createCommonjsModule(fn, module) {
	return module = { exports: {} }, fn(module, module.exports), module.exports;
}

/**
 * Copyright (c) 2013-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * 
 */

function makeEmptyFunction(arg) {
  return function () {
    return arg;
  };
}

/**
 * This function accepts and discards inputs; it has no side effects. This is
 * primarily useful idiomatically for overridable function endpoints which
 * always need to be callable, since JS lacks a null-call idiom ala Cocoa.
 */
var emptyFunction = function emptyFunction() {};

emptyFunction.thatReturns = makeEmptyFunction;
emptyFunction.thatReturnsFalse = makeEmptyFunction(false);
emptyFunction.thatReturnsTrue = makeEmptyFunction(true);
emptyFunction.thatReturnsNull = makeEmptyFunction(null);
emptyFunction.thatReturnsThis = function () {
  return this;
};
emptyFunction.thatReturnsArgument = function (arg) {
  return arg;
};

var emptyFunction_1 = emptyFunction;

/**
 * Copyright (c) 2013-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 */

/**
 * Use invariant() to assert state which your program assumes to be true.
 *
 * Provide sprintf-style format (only %s is supported) and arguments
 * to provide information about what broke and what you were
 * expecting.
 *
 * The invariant message will be stripped in production, but the invariant
 * will remain to ensure logic does not differ in production.
 */

var validateFormat = function validateFormat(format) {};

{
  validateFormat = function validateFormat(format) {
    if (format === undefined) {
      throw new Error('invariant requires an error message argument');
    }
  };
}

function invariant(condition, format, a, b, c, d, e, f) {
  validateFormat(format);

  if (!condition) {
    var error;
    if (format === undefined) {
      error = new Error('Minified exception occurred; use the non-minified dev environment ' + 'for the full error message and additional helpful warnings.');
    } else {
      var args = [a, b, c, d, e, f];
      var argIndex = 0;
      error = new Error(format.replace(/%s/g, function () {
        return args[argIndex++];
      }));
      error.name = 'Invariant Violation';
    }

    error.framesToPop = 1; // we don't care about invariant's own frame
    throw error;
  }
}

var invariant_1 = invariant;

/**
 * Similar to invariant but only logs a warning if the condition is not met.
 * This can be used to log issues in development environments in critical
 * paths. Removing the logging code for production environments will keep the
 * same logic and follow the same code paths.
 */

var warning = emptyFunction_1;

{
  var printWarning = function printWarning(format) {
    var arguments$1 = arguments;

    for (var _len = arguments.length, args = Array(_len > 1 ? _len - 1 : 0), _key = 1; _key < _len; _key++) {
      args[_key - 1] = arguments$1[_key];
    }

    var argIndex = 0;
    var message = 'Warning: ' + format.replace(/%s/g, function () {
      return args[argIndex++];
    });
    if (typeof console !== 'undefined') {
      console.error(message);
    }
    try {
      // --- Welcome to debugging React ---
      // This error was thrown as a convenience so that you can use this stack
      // to find the callsite that caused this warning to fire.
      throw new Error(message);
    } catch (x) {}
  };

  warning = function warning(condition, format) {
    var arguments$1 = arguments;

    if (format === undefined) {
      throw new Error('`warning(condition, format, ...args)` requires a warning ' + 'message argument');
    }

    if (format.indexOf('Failed Composite propType: ') === 0) {
      return; // Ignore CompositeComponent proptype check.
    }

    if (!condition) {
      for (var _len2 = arguments.length, args = Array(_len2 > 2 ? _len2 - 2 : 0), _key2 = 2; _key2 < _len2; _key2++) {
        args[_key2 - 2] = arguments$1[_key2];
      }

      printWarning.apply(undefined, [format].concat(args));
    }
  };
}

var warning_1 = warning;

/*
object-assign
(c) Sindre Sorhus
@license MIT
*/
/* eslint-disable no-unused-vars */
var getOwnPropertySymbols = Object.getOwnPropertySymbols;
var hasOwnProperty = Object.prototype.hasOwnProperty;
var propIsEnumerable = Object.prototype.propertyIsEnumerable;

function toObject(val) {
	if (val === null || val === undefined) {
		throw new TypeError('Object.assign cannot be called with null or undefined');
	}

	return Object(val);
}

function shouldUseNative() {
	try {
		if (!Object.assign) {
			return false;
		}

		// Detect buggy property enumeration order in older V8 versions.

		// https://bugs.chromium.org/p/v8/issues/detail?id=4118
		var test1 = new String('abc');  // eslint-disable-line no-new-wrappers
		test1[5] = 'de';
		if (Object.getOwnPropertyNames(test1)[0] === '5') {
			return false;
		}

		// https://bugs.chromium.org/p/v8/issues/detail?id=3056
		var test2 = {};
		for (var i = 0; i < 10; i++) {
			test2['_' + String.fromCharCode(i)] = i;
		}
		var order2 = Object.getOwnPropertyNames(test2).map(function (n) {
			return test2[n];
		});
		if (order2.join('') !== '0123456789') {
			return false;
		}

		// https://bugs.chromium.org/p/v8/issues/detail?id=3056
		var test3 = {};
		'abcdefghijklmnopqrst'.split('').forEach(function (letter) {
			test3[letter] = letter;
		});
		if (Object.keys(Object.assign({}, test3)).join('') !==
				'abcdefghijklmnopqrst') {
			return false;
		}

		return true;
	} catch (err) {
		// We don't expect any of the above to throw, but better to be safe.
		return false;
	}
}

var objectAssign = shouldUseNative() ? Object.assign : function (target, source) {
	var arguments$1 = arguments;

	var from;
	var to = toObject(target);
	var symbols;

	for (var s = 1; s < arguments.length; s++) {
		from = Object(arguments$1[s]);

		for (var key in from) {
			if (hasOwnProperty.call(from, key)) {
				to[key] = from[key];
			}
		}

		if (getOwnPropertySymbols) {
			symbols = getOwnPropertySymbols(from);
			for (var i = 0; i < symbols.length; i++) {
				if (propIsEnumerable.call(from, symbols[i])) {
					to[symbols[i]] = from[symbols[i]];
				}
			}
		}
	}

	return to;
};

/**
 * Copyright (c) 2013-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

var ReactPropTypesSecret = 'SECRET_DO_NOT_PASS_THIS_OR_YOU_WILL_BE_FIRED';

var ReactPropTypesSecret_1 = ReactPropTypesSecret;

{
  var invariant$1 = invariant_1;
  var warning$1 = warning_1;
  var ReactPropTypesSecret$1 = ReactPropTypesSecret_1;
  var loggedTypeFailures = {};
}

/**
 * Assert that the values match with the type specs.
 * Error messages are memorized and will only be shown once.
 *
 * @param {object} typeSpecs Map of name to a ReactPropType
 * @param {object} values Runtime values that need to be type-checked
 * @param {string} location e.g. "prop", "context", "child context"
 * @param {string} componentName Name of the component for error messages.
 * @param {?Function} getStack Returns the component stack.
 * @private
 */
function checkPropTypes(typeSpecs, values, location, componentName, getStack) {
  {
    for (var typeSpecName in typeSpecs) {
      if (typeSpecs.hasOwnProperty(typeSpecName)) {
        var error;
        // Prop type validation may throw. In case they do, we don't want to
        // fail the render phase where it didn't fail before. So we log it.
        // After these have been cleaned up, we'll let them throw.
        try {
          // This is intentionally an invariant that gets caught. It's the same
          // behavior as without this statement except with a better message.
          invariant$1(typeof typeSpecs[typeSpecName] === 'function', '%s: %s type `%s` is invalid; it must be a function, usually from ' + 'the `prop-types` package, but received `%s`.', componentName || 'React class', location, typeSpecName, typeof typeSpecs[typeSpecName]);
          error = typeSpecs[typeSpecName](values, typeSpecName, componentName, location, null, ReactPropTypesSecret$1);
        } catch (ex) {
          error = ex;
        }
        warning$1(!error || error instanceof Error, '%s: type specification of %s `%s` is invalid; the type checker ' + 'function must return `null` or an `Error` but returned a %s. ' + 'You may have forgotten to pass an argument to the type checker ' + 'creator (arrayOf, instanceOf, objectOf, oneOf, oneOfType, and ' + 'shape all require an argument).', componentName || 'React class', location, typeSpecName, typeof error);
        if (error instanceof Error && !(error.message in loggedTypeFailures)) {
          // Only monitor this failure once because there tends to be a lot of the
          // same error.
          loggedTypeFailures[error.message] = true;

          var stack = getStack ? getStack() : '';

          warning$1(false, 'Failed %s type: %s%s', location, error.message, stack != null ? stack : '');
        }
      }
    }
  }
}

var checkPropTypes_1 = checkPropTypes;

var factoryWithTypeCheckers = function(isValidElement, throwOnDirectAccess) {
  /* global Symbol */
  var ITERATOR_SYMBOL = typeof Symbol === 'function' && Symbol.iterator;
  var FAUX_ITERATOR_SYMBOL = '@@iterator'; // Before Symbol spec.

  /**
   * Returns the iterator method function contained on the iterable object.
   *
   * Be sure to invoke the function with the iterable as context:
   *
   *     var iteratorFn = getIteratorFn(myIterable);
   *     if (iteratorFn) {
   *       var iterator = iteratorFn.call(myIterable);
   *       ...
   *     }
   *
   * @param {?object} maybeIterable
   * @return {?function}
   */
  function getIteratorFn(maybeIterable) {
    var iteratorFn = maybeIterable && (ITERATOR_SYMBOL && maybeIterable[ITERATOR_SYMBOL] || maybeIterable[FAUX_ITERATOR_SYMBOL]);
    if (typeof iteratorFn === 'function') {
      return iteratorFn;
    }
  }

  /**
   * Collection of methods that allow declaration and validation of props that are
   * supplied to React components. Example usage:
   *
   *   var Props = require('ReactPropTypes');
   *   var MyArticle = React.createClass({
   *     propTypes: {
   *       // An optional string prop named "description".
   *       description: Props.string,
   *
   *       // A required enum prop named "category".
   *       category: Props.oneOf(['News','Photos']).isRequired,
   *
   *       // A prop named "dialog" that requires an instance of Dialog.
   *       dialog: Props.instanceOf(Dialog).isRequired
   *     },
   *     render: function() { ... }
   *   });
   *
   * A more formal specification of how these methods are used:
   *
   *   type := array|bool|func|object|number|string|oneOf([...])|instanceOf(...)
   *   decl := ReactPropTypes.{type}(.isRequired)?
   *
   * Each and every declaration produces a function with the same signature. This
   * allows the creation of custom validation functions. For example:
   *
   *  var MyLink = React.createClass({
   *    propTypes: {
   *      // An optional string or URI prop named "href".
   *      href: function(props, propName, componentName) {
   *        var propValue = props[propName];
   *        if (propValue != null && typeof propValue !== 'string' &&
   *            !(propValue instanceof URI)) {
   *          return new Error(
   *            'Expected a string or an URI for ' + propName + ' in ' +
   *            componentName
   *          );
   *        }
   *      }
   *    },
   *    render: function() {...}
   *  });
   *
   * @internal
   */

  var ANONYMOUS = '<<anonymous>>';

  // Important!
  // Keep this list in sync with production version in `./factoryWithThrowingShims.js`.
  var ReactPropTypes = {
    array: createPrimitiveTypeChecker('array'),
    bool: createPrimitiveTypeChecker('boolean'),
    func: createPrimitiveTypeChecker('function'),
    number: createPrimitiveTypeChecker('number'),
    object: createPrimitiveTypeChecker('object'),
    string: createPrimitiveTypeChecker('string'),
    symbol: createPrimitiveTypeChecker('symbol'),

    any: createAnyTypeChecker(),
    arrayOf: createArrayOfTypeChecker,
    element: createElementTypeChecker(),
    instanceOf: createInstanceTypeChecker,
    node: createNodeChecker(),
    objectOf: createObjectOfTypeChecker,
    oneOf: createEnumTypeChecker,
    oneOfType: createUnionTypeChecker,
    shape: createShapeTypeChecker,
    exact: createStrictShapeTypeChecker,
  };

  /**
   * inlined Object.is polyfill to avoid requiring consumers ship their own
   * https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/is
   */
  /*eslint-disable no-self-compare*/
  function is(x, y) {
    // SameValue algorithm
    if (x === y) {
      // Steps 1-5, 7-10
      // Steps 6.b-6.e: +0 != -0
      return x !== 0 || 1 / x === 1 / y;
    } else {
      // Step 6.a: NaN == NaN
      return x !== x && y !== y;
    }
  }
  /*eslint-enable no-self-compare*/

  /**
   * We use an Error-like object for backward compatibility as people may call
   * PropTypes directly and inspect their output. However, we don't use real
   * Errors anymore. We don't inspect their stack anyway, and creating them
   * is prohibitively expensive if they are created too often, such as what
   * happens in oneOfType() for any type before the one that matched.
   */
  function PropTypeError(message) {
    this.message = message;
    this.stack = '';
  }
  // Make `instanceof Error` still work for returned errors.
  PropTypeError.prototype = Error.prototype;

  function createChainableTypeChecker(validate) {
    {
      var manualPropTypeCallCache = {};
      var manualPropTypeWarningCount = 0;
    }
    function checkType(isRequired, props, propName, componentName, location, propFullName, secret) {
      componentName = componentName || ANONYMOUS;
      propFullName = propFullName || propName;

      if (secret !== ReactPropTypesSecret_1) {
        if (throwOnDirectAccess) {
          // New behavior only for users of `prop-types` package
          invariant_1(
            false,
            'Calling PropTypes validators directly is not supported by the `prop-types` package. ' +
            'Use `PropTypes.checkPropTypes()` to call them. ' +
            'Read more at http://fb.me/use-check-prop-types'
          );
        } else if ("development" !== 'production' && typeof console !== 'undefined') {
          // Old behavior for people using React.PropTypes
          var cacheKey = componentName + ':' + propName;
          if (
            !manualPropTypeCallCache[cacheKey] &&
            // Avoid spamming the console because they are often not actionable except for lib authors
            manualPropTypeWarningCount < 3
          ) {
            warning_1(
              false,
              'You are manually calling a React.PropTypes validation ' +
              'function for the `%s` prop on `%s`. This is deprecated ' +
              'and will throw in the standalone `prop-types` package. ' +
              'You may be seeing this warning due to a third-party PropTypes ' +
              'library. See https://fb.me/react-warning-dont-call-proptypes ' + 'for details.',
              propFullName,
              componentName
            );
            manualPropTypeCallCache[cacheKey] = true;
            manualPropTypeWarningCount++;
          }
        }
      }
      if (props[propName] == null) {
        if (isRequired) {
          if (props[propName] === null) {
            return new PropTypeError('The ' + location + ' `' + propFullName + '` is marked as required ' + ('in `' + componentName + '`, but its value is `null`.'));
          }
          return new PropTypeError('The ' + location + ' `' + propFullName + '` is marked as required in ' + ('`' + componentName + '`, but its value is `undefined`.'));
        }
        return null;
      } else {
        return validate(props, propName, componentName, location, propFullName);
      }
    }

    var chainedCheckType = checkType.bind(null, false);
    chainedCheckType.isRequired = checkType.bind(null, true);

    return chainedCheckType;
  }

  function createPrimitiveTypeChecker(expectedType) {
    function validate(props, propName, componentName, location, propFullName, secret) {
      var propValue = props[propName];
      var propType = getPropType(propValue);
      if (propType !== expectedType) {
        // `propValue` being instance of, say, date/regexp, pass the 'object'
        // check, but we can offer a more precise error message here rather than
        // 'of type `object`'.
        var preciseType = getPreciseType(propValue);

        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of type ' + ('`' + preciseType + '` supplied to `' + componentName + '`, expected ') + ('`' + expectedType + '`.'));
      }
      return null;
    }
    return createChainableTypeChecker(validate);
  }

  function createAnyTypeChecker() {
    return createChainableTypeChecker(emptyFunction_1.thatReturnsNull);
  }

  function createArrayOfTypeChecker(typeChecker) {
    function validate(props, propName, componentName, location, propFullName) {
      if (typeof typeChecker !== 'function') {
        return new PropTypeError('Property `' + propFullName + '` of component `' + componentName + '` has invalid PropType notation inside arrayOf.');
      }
      var propValue = props[propName];
      if (!Array.isArray(propValue)) {
        var propType = getPropType(propValue);
        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of type ' + ('`' + propType + '` supplied to `' + componentName + '`, expected an array.'));
      }
      for (var i = 0; i < propValue.length; i++) {
        var error = typeChecker(propValue, i, componentName, location, propFullName + '[' + i + ']', ReactPropTypesSecret_1);
        if (error instanceof Error) {
          return error;
        }
      }
      return null;
    }
    return createChainableTypeChecker(validate);
  }

  function createElementTypeChecker() {
    function validate(props, propName, componentName, location, propFullName) {
      var propValue = props[propName];
      if (!isValidElement(propValue)) {
        var propType = getPropType(propValue);
        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of type ' + ('`' + propType + '` supplied to `' + componentName + '`, expected a single ReactElement.'));
      }
      return null;
    }
    return createChainableTypeChecker(validate);
  }

  function createInstanceTypeChecker(expectedClass) {
    function validate(props, propName, componentName, location, propFullName) {
      if (!(props[propName] instanceof expectedClass)) {
        var expectedClassName = expectedClass.name || ANONYMOUS;
        var actualClassName = getClassName(props[propName]);
        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of type ' + ('`' + actualClassName + '` supplied to `' + componentName + '`, expected ') + ('instance of `' + expectedClassName + '`.'));
      }
      return null;
    }
    return createChainableTypeChecker(validate);
  }

  function createEnumTypeChecker(expectedValues) {
    if (!Array.isArray(expectedValues)) {
      warning_1(false, 'Invalid argument supplied to oneOf, expected an instance of array.');
      return emptyFunction_1.thatReturnsNull;
    }

    function validate(props, propName, componentName, location, propFullName) {
      var propValue = props[propName];
      for (var i = 0; i < expectedValues.length; i++) {
        if (is(propValue, expectedValues[i])) {
          return null;
        }
      }

      var valuesString = JSON.stringify(expectedValues);
      return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of value `' + propValue + '` ' + ('supplied to `' + componentName + '`, expected one of ' + valuesString + '.'));
    }
    return createChainableTypeChecker(validate);
  }

  function createObjectOfTypeChecker(typeChecker) {
    function validate(props, propName, componentName, location, propFullName) {
      if (typeof typeChecker !== 'function') {
        return new PropTypeError('Property `' + propFullName + '` of component `' + componentName + '` has invalid PropType notation inside objectOf.');
      }
      var propValue = props[propName];
      var propType = getPropType(propValue);
      if (propType !== 'object') {
        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of type ' + ('`' + propType + '` supplied to `' + componentName + '`, expected an object.'));
      }
      for (var key in propValue) {
        if (propValue.hasOwnProperty(key)) {
          var error = typeChecker(propValue, key, componentName, location, propFullName + '.' + key, ReactPropTypesSecret_1);
          if (error instanceof Error) {
            return error;
          }
        }
      }
      return null;
    }
    return createChainableTypeChecker(validate);
  }

  function createUnionTypeChecker(arrayOfTypeCheckers) {
    if (!Array.isArray(arrayOfTypeCheckers)) {
      warning_1(false, 'Invalid argument supplied to oneOfType, expected an instance of array.');
      return emptyFunction_1.thatReturnsNull;
    }

    for (var i = 0; i < arrayOfTypeCheckers.length; i++) {
      var checker = arrayOfTypeCheckers[i];
      if (typeof checker !== 'function') {
        warning_1(
          false,
          'Invalid argument supplied to oneOfType. Expected an array of check functions, but ' +
          'received %s at index %s.',
          getPostfixForTypeWarning(checker),
          i
        );
        return emptyFunction_1.thatReturnsNull;
      }
    }

    function validate(props, propName, componentName, location, propFullName) {
      for (var i = 0; i < arrayOfTypeCheckers.length; i++) {
        var checker = arrayOfTypeCheckers[i];
        if (checker(props, propName, componentName, location, propFullName, ReactPropTypesSecret_1) == null) {
          return null;
        }
      }

      return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` supplied to ' + ('`' + componentName + '`.'));
    }
    return createChainableTypeChecker(validate);
  }

  function createNodeChecker() {
    function validate(props, propName, componentName, location, propFullName) {
      if (!isNode(props[propName])) {
        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` supplied to ' + ('`' + componentName + '`, expected a ReactNode.'));
      }
      return null;
    }
    return createChainableTypeChecker(validate);
  }

  function createShapeTypeChecker(shapeTypes) {
    function validate(props, propName, componentName, location, propFullName) {
      var propValue = props[propName];
      var propType = getPropType(propValue);
      if (propType !== 'object') {
        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of type `' + propType + '` ' + ('supplied to `' + componentName + '`, expected `object`.'));
      }
      for (var key in shapeTypes) {
        var checker = shapeTypes[key];
        if (!checker) {
          continue;
        }
        var error = checker(propValue, key, componentName, location, propFullName + '.' + key, ReactPropTypesSecret_1);
        if (error) {
          return error;
        }
      }
      return null;
    }
    return createChainableTypeChecker(validate);
  }

  function createStrictShapeTypeChecker(shapeTypes) {
    function validate(props, propName, componentName, location, propFullName) {
      var propValue = props[propName];
      var propType = getPropType(propValue);
      if (propType !== 'object') {
        return new PropTypeError('Invalid ' + location + ' `' + propFullName + '` of type `' + propType + '` ' + ('supplied to `' + componentName + '`, expected `object`.'));
      }
      // We need to check all keys in case some are required but missing from
      // props.
      var allKeys = objectAssign({}, props[propName], shapeTypes);
      for (var key in allKeys) {
        var checker = shapeTypes[key];
        if (!checker) {
          return new PropTypeError(
            'Invalid ' + location + ' `' + propFullName + '` key `' + key + '` supplied to `' + componentName + '`.' +
            '\nBad object: ' + JSON.stringify(props[propName], null, '  ') +
            '\nValid keys: ' +  JSON.stringify(Object.keys(shapeTypes), null, '  ')
          );
        }
        var error = checker(propValue, key, componentName, location, propFullName + '.' + key, ReactPropTypesSecret_1);
        if (error) {
          return error;
        }
      }
      return null;
    }

    return createChainableTypeChecker(validate);
  }

  function isNode(propValue) {
    switch (typeof propValue) {
      case 'number':
      case 'string':
      case 'undefined':
        return true;
      case 'boolean':
        return !propValue;
      case 'object':
        if (Array.isArray(propValue)) {
          return propValue.every(isNode);
        }
        if (propValue === null || isValidElement(propValue)) {
          return true;
        }

        var iteratorFn = getIteratorFn(propValue);
        if (iteratorFn) {
          var iterator = iteratorFn.call(propValue);
          var step;
          if (iteratorFn !== propValue.entries) {
            while (!(step = iterator.next()).done) {
              if (!isNode(step.value)) {
                return false;
              }
            }
          } else {
            // Iterator will provide entry [k,v] tuples rather than values.
            while (!(step = iterator.next()).done) {
              var entry = step.value;
              if (entry) {
                if (!isNode(entry[1])) {
                  return false;
                }
              }
            }
          }
        } else {
          return false;
        }

        return true;
      default:
        return false;
    }
  }

  function isSymbol(propType, propValue) {
    // Native Symbol.
    if (propType === 'symbol') {
      return true;
    }

    // 19.4.3.5 Symbol.prototype[@@toStringTag] === 'Symbol'
    if (propValue['@@toStringTag'] === 'Symbol') {
      return true;
    }

    // Fallback for non-spec compliant Symbols which are polyfilled.
    if (typeof Symbol === 'function' && propValue instanceof Symbol) {
      return true;
    }

    return false;
  }

  // Equivalent of `typeof` but with special handling for array and regexp.
  function getPropType(propValue) {
    var propType = typeof propValue;
    if (Array.isArray(propValue)) {
      return 'array';
    }
    if (propValue instanceof RegExp) {
      // Old webkits (at least until Android 4.0) return 'function' rather than
      // 'object' for typeof a RegExp. We'll normalize this here so that /bla/
      // passes PropTypes.object.
      return 'object';
    }
    if (isSymbol(propType, propValue)) {
      return 'symbol';
    }
    return propType;
  }

  // This handles more types than `getPropType`. Only used for error messages.
  // See `createPrimitiveTypeChecker`.
  function getPreciseType(propValue) {
    if (typeof propValue === 'undefined' || propValue === null) {
      return '' + propValue;
    }
    var propType = getPropType(propValue);
    if (propType === 'object') {
      if (propValue instanceof Date) {
        return 'date';
      } else if (propValue instanceof RegExp) {
        return 'regexp';
      }
    }
    return propType;
  }

  // Returns a string that is postfixed to a warning about an invalid type.
  // For example, "undefined" or "of type array"
  function getPostfixForTypeWarning(value) {
    var type = getPreciseType(value);
    switch (type) {
      case 'array':
      case 'object':
        return 'an ' + type;
      case 'boolean':
      case 'date':
      case 'regexp':
        return 'a ' + type;
      default:
        return type;
    }
  }

  // Returns class name of the object, if any.
  function getClassName(propValue) {
    if (!propValue.constructor || !propValue.constructor.name) {
      return ANONYMOUS;
    }
    return propValue.constructor.name;
  }

  ReactPropTypes.checkPropTypes = checkPropTypes_1;
  ReactPropTypes.PropTypes = ReactPropTypes;

  return ReactPropTypes;
};

var propTypes = createCommonjsModule(function (module) {
/**
 * Copyright (c) 2013-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

{
  var REACT_ELEMENT_TYPE = (typeof Symbol === 'function' &&
    Symbol.for &&
    Symbol.for('react.element')) ||
    0xeac7;

  var isValidElement = function(object) {
    return typeof object === 'object' &&
      object !== null &&
      object.$$typeof === REACT_ELEMENT_TYPE;
  };

  // By explicitly using `prop-types` you are opting into new development behavior.
  // http://fb.me/prop-types-in-prod
  var throwOnDirectAccess = true;
  module.exports = factoryWithTypeCheckers(isValidElement, throwOnDirectAccess);
}
});

/**
 * Check if two nodes are equivalent.
 *
 * @param {Node} node			DOM Node to compare
 * @param {VNode} vnode			Virtual DOM node to compare
 * @param {boolean} [hydrating=false]	If true, ignores component constructors when comparing.
 * @private
 */
function isSameNodeType(node, vnode, hydrating) {
	if (typeof vnode.nodeName==='string') {
		return isNamedNode(node, vnode.nodeName);
	}
	return hydrating || node._componentConstructor===vnode.nodeName;
}


/**
 * Check if an Element has a given nodeName, case-insensitively.
 *
 * @param {Element} node	A DOM Element to inspect the name of.
 * @param {String} nodeName	Unnormalized name to compare against.
 */
function isNamedNode(node, nodeName) {
    return node.type === nodeName;
}


/**
 * Reconstruct Component-style `props` from a VNode.
 * Ensures default/fallback values from `defaultProps`:
 * Own-properties of `defaultProps` not present in `vnode.attributes` are added.
 *
 * @param {VNode} vnode
 * @returns {Object} props
 */
function getNodeProps(vnode) {
	var props = extend({}, vnode.attributes);
	props.children = vnode.children;

	var defaultProps = vnode.nodeName.defaultProps;
	if (defaultProps!==undefined) {
		for (var i in defaultProps) {
			if (props[i]===undefined) {
				props[i] = defaultProps[i];
			}
		}
	}

    {
        var Comp = vnode.nodeName;
        if (Comp.propTypes) {
            propTypes.checkPropTypes(Comp.propTypes, props, 'prop', Comp.displayName || Comp.name);
        }
        forOwn(props, function (v) { return freezeAll(v); });
    }

	return props;
}

/**
 * @module zrender/core/util
 */

// 用于处理merge时无法遍历Date等对象的问题
var BUILTIN_OBJECT = {
    '[object Function]': 1,
    '[object RegExp]': 1,
    '[object Date]': 1,
    '[object Error]': 1,
    '[object CanvasGradient]': 1,
    '[object CanvasPattern]': 1,
    // For node-canvas
    '[object Image]': 1,
    '[object Canvas]': 1
};

var TYPED_ARRAY = {
    '[object Int8Array]': 1,
    '[object Uint8Array]': 1,
    '[object Uint8ClampedArray]': 1,
    '[object Int16Array]': 1,
    '[object Uint16Array]': 1,
    '[object Int32Array]': 1,
    '[object Uint32Array]': 1,
    '[object Float32Array]': 1,
    '[object Float64Array]': 1
};

var objToString = Object.prototype.toString;

var arrayProto$1 = Array.prototype;
var nativeForEach = arrayProto$1.forEach;
var nativeSlice = arrayProto$1.slice;
var nativeMap$1 = arrayProto$1.map;

// Avoid assign to an exported variable, for transforming to cjs.
var methods = {};

/**
 * Those data types can be cloned:
 *     Plain object, Array, TypedArray, number, string, null, undefined.
 * Those data types will be assgined using the orginal data:
 *     BUILTIN_OBJECT
 * Instance of user defined class will be cloned to a plain object, without
 * properties in prototype.
 * Other data types is not supported (not sure what will happen).
 *
 * Caution: do not support clone Date, for performance consideration.
 * (There might be a large number of date in `series.data`).
 * So date should not be modified in and out of echarts.
 *
 * @param {*} source
 * @return {*} new
 */
function clone$1(source) {
    if (source == null || typeof source != 'object') {
        return source;
    }

    var result = source;
    var typeStr = objToString.call(source);

    if (typeStr === '[object Array]') {
        result = [];
        for (var i = 0, len = source.length; i < len; i++) {
            result[i] = clone$1(source[i]);
        }
    }
    else if (TYPED_ARRAY[typeStr]) {
        var Ctor = source.constructor;
        if (source.constructor.from) {
            result = Ctor.from(source);
        }
        else {
            result = new Ctor(source.length);
            for (var i = 0, len = source.length; i < len; i++) {
                result[i] = clone$1(source[i]);
            }
        }
    }
    else if (!BUILTIN_OBJECT[typeStr] && !isPrimitive(source) && !isDom(source)) {
        result = {};
        for (var key in source) {
            if (source.hasOwnProperty(key)) {
                result[key] = clone$1(source[key]);
            }
        }
    }

    return result;
}

/**
 * @memberOf module:zrender/core/util
 * @param {*} target
 * @param {*} source
 * @param {boolean} [overwrite=false]
 */
function merge$1(target, source, overwrite) {
    // We should escapse that source is string
    // and enter for ... in ...
    if (!isObject$1(source) || !isObject$1(target)) {
        return overwrite ? clone$1(source) : target;
    }

    for (var key in source) {
        if (source.hasOwnProperty(key)) {
            var targetProp = target[key];
            var sourceProp = source[key];

            if (isObject$1(sourceProp)
                && isObject$1(targetProp)
                && !isArray$1(sourceProp)
                && !isArray$1(targetProp)
                && !isDom(sourceProp)
                && !isDom(targetProp)
                && !isBuiltInObject(sourceProp)
                && !isBuiltInObject(targetProp)
                && !isPrimitive(sourceProp)
                && !isPrimitive(targetProp)
            ) {
                // 如果需要递归覆盖，就递归调用merge
                merge$1(targetProp, sourceProp, overwrite);
            }
            else if (overwrite || !(key in target)) {
                // 否则只处理overwrite为true，或者在目标对象中没有此属性的情况
                // NOTE，在 target[key] 不存在的时候也是直接覆盖
                target[key] = clone$1(source[key], true);
            }
        }
    }

    return target;
}

/**
 * @param {*} target
 * @param {*} source
 * @memberOf module:zrender/core/util
 */
function extend$1(target, source) {
    for (var key in source) {
        if (source.hasOwnProperty(key)) {
            target[key] = source[key];
        }
    }
    return target;
}

/**
 * @param {*} target
 * @param {*} source
 * @param {boolean} [overlay=false]
 * @memberOf module:zrender/core/util
 */
function defaults(target, source, overlay) {
    for (var key in source) {
        if (source.hasOwnProperty(key)
            && (overlay ? source[key] != null : target[key] == null)
        ) {
            target[key] = source[key];
        }
    }
    return target;
}

var createCanvas = function () {
    return methods.createCanvas();
};

methods.createCanvas = function () {
    return document.createElement('canvas');
};

// FIXME
var _ctx;

function getContext() {
    if (!_ctx) {
        // Use util.createCanvas instead of createCanvas
        // because createCanvas may be overwritten in different environment
        _ctx = createCanvas().getContext('2d');
    }
    return _ctx;
}

/**
 * 查询数组中元素的index
 * @memberOf module:zrender/core/util
 */
function indexOf$1(array, value) {
    if (array) {
        if (array.indexOf) {
            return array.indexOf(value);
        }
        for (var i = 0, len = array.length; i < len; i++) {
            if (array[i] === value) {
                return i;
            }
        }
    }
    return -1;
}

/**
 * 构造类继承关系
 *
 * @memberOf module:zrender/core/util
 * @param {Function} clazz 源类
 * @param {Function} baseClazz 基类
 */
function inherits(clazz, baseClazz) {
    var clazzPrototype = clazz.prototype;
    function F() {}
    F.prototype = baseClazz.prototype;
    clazz.prototype = new F();

    for (var prop in clazzPrototype) {
        clazz.prototype[prop] = clazzPrototype[prop];
    }
    clazz.prototype.constructor = clazz;
    clazz.superClass = baseClazz;
}

/**
 * @memberOf module:zrender/core/util
 * @param {Object|Function} target
 * @param {Object|Function} sorce
 * @param {boolean} overlay
 */
function mixin(target, source, overlay) {
    target = 'prototype' in target ? target.prototype : target;
    source = 'prototype' in source ? source.prototype : source;

    defaults(target, source, overlay);
}

/**
 * Consider typed array.
 * @param {Array|TypedArray} data
 */
function isArrayLike(data) {
    if (! data) {
        return;
    }
    if (typeof data == 'string') {
        return false;
    }
    return typeof data.length == 'number';
}

/**
 * 数组或对象遍历
 * @memberOf module:zrender/core/util
 * @param {Object|Array} obj
 * @param {Function} cb
 * @param {*} [context]
 */
function each$1(obj, cb, context) {
    if (!(obj && cb)) {
        return;
    }
    if (obj.forEach && obj.forEach === nativeForEach) {
        obj.forEach(cb, context);
    }
    else if (obj.length === +obj.length) {
        for (var i = 0, len = obj.length; i < len; i++) {
            cb.call(context, obj[i], i, obj);
        }
    }
    else {
        for (var key in obj) {
            if (obj.hasOwnProperty(key)) {
                cb.call(context, obj[key], key, obj);
            }
        }
    }
}

/**
 * 数组映射
 * @memberOf module:zrender/core/util
 * @param {Array} obj
 * @param {Function} cb
 * @param {*} [context]
 * @return {Array}
 */
function map$1(obj, cb, context) {
    if (!(obj && cb)) {
        return;
    }
    if (obj.map && obj.map === nativeMap$1) {
        return obj.map(cb, context);
    }
    else {
        var result = [];
        for (var i = 0, len = obj.length; i < len; i++) {
            result.push(cb.call(context, obj[i], i, obj));
        }
        return result;
    }
}

/**
 * @memberOf module:zrender/core/util
 * @param {Function} func
 * @param {*} context
 * @return {Function}
 */
function bind(func, context) {
    var args = nativeSlice.call(arguments, 2);
    return function () {
        return func.apply(context, args.concat(nativeSlice.call(arguments)));
    };
}

/**
 * @memberOf module:zrender/core/util
 * @param {*} value
 * @return {boolean}
 */
function isArray$1(value) {
    return objToString.call(value) === '[object Array]';
}

/**
 * @memberOf module:zrender/core/util
 * @param {*} value
 * @return {boolean}
 */
function isFunction$1(value) {
    return typeof value === 'function';
}

/**
 * @memberOf module:zrender/core/util
 * @param {*} value
 * @return {boolean}
 */
function isString$1(value) {
    return objToString.call(value) === '[object String]';
}

/**
 * @memberOf module:zrender/core/util
 * @param {*} value
 * @return {boolean}
 */
function isObject$1(value) {
    // Avoid a V8 JIT bug in Chrome 19-20.
    // See https://code.google.com/p/v8/issues/detail?id=2291 for more details.
    var type = typeof value;
    return type === 'function' || (!!value && type == 'object');
}

/**
 * @memberOf module:zrender/core/util
 * @param {*} value
 * @return {boolean}
 */
function isBuiltInObject(value) {
    return !!BUILTIN_OBJECT[objToString.call(value)];
}

/**
 * @memberOf module:zrender/core/util
 * @param {*} value
 * @return {boolean}
 */
function isDom(value) {
    return typeof value === 'object'
        && typeof value.nodeType === 'number'
        && typeof value.ownerDocument === 'object';
}

function retrieve2(value0, value1) {
    return value0 != null
        ? value0
        : value1;
}

function retrieve3(value0, value1, value2) {
    return value0 != null
        ? value0
        : value1 != null
        ? value1
        : value2;
}

/**
 * Normalize css liked array configuration
 * e.g.
 *  3 => [3, 3, 3, 3]
 *  [4, 2] => [4, 2, 4, 2]
 *  [4, 3, 2] => [4, 3, 2, 3]
 * @param {number|Array.<number>} val
 * @return {Array.<number>}
 */
function normalizeCssArray(val) {
    if (typeof (val) === 'number') {
        return [val, val, val, val];
    }
    var len = val.length;
    if (len === 2) {
        // vertical | horizontal
        return [val[0], val[1], val[0], val[1]];
    }
    else if (len === 3) {
        // top | horizontal | bottom
        return [val[0], val[1], val[2], val[1]];
    }
    return val;
}

/**
 * @memberOf module:zrender/core/util
 * @param {string} str string to be trimed
 * @return {string} trimed string
 */
function trim(str) {
    if (typeof str.trim === 'function') {
        return str.trim();
    }
    else {
        return str.replace(/^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g, '');
    }
}

var primitiveKey = '__ec_primitive__';

function isPrimitive(obj) {
    return obj[primitiveKey];
}

var SHADOW_PROPS = {
    'shadowBlur': 1,
    'shadowOffsetX': 1,
    'shadowOffsetY': 1,
    'textShadowBlur': 1,
    'textShadowOffsetX': 1,
    'textShadowOffsetY': 1,
    'textBoxShadowBlur': 1,
    'textBoxShadowOffsetX': 1,
    'textBoxShadowOffsetY': 1
};

function fixShadow (ctx, propName, value) {
    if (SHADOW_PROPS.hasOwnProperty(propName)) {
        return value *= ctx.dpr;
    }
    return value;
}

var STYLE_COMMON_PROPS = [
    ['shadowBlur', 0], ['shadowOffsetX', 0], ['shadowOffsetY', 0], ['shadowColor', '#000'],
    ['lineCap', 'butt'], ['lineJoin', 'miter'], ['miterLimit', 10]
];

// var SHADOW_PROPS = STYLE_COMMON_PROPS.slice(0, 4);
// var LINE_PROPS = STYLE_COMMON_PROPS.slice(4);

var Style = function (opts, host) {
    this.extendFrom(opts, false);
    this.host = host;
};

function createLinearGradient(ctx, obj, rect) {
    var x = obj.x == null ? 0 : obj.x;
    var x2 = obj.x2 == null ? 1 : obj.x2;
    var y = obj.y == null ? 0 : obj.y;
    var y2 = obj.y2 == null ? 0 : obj.y2;

    if (!obj.global) {
        x = x * rect.width + rect.x;
        x2 = x2 * rect.width + rect.x;
        y = y * rect.height + rect.y;
        y2 = y2 * rect.height + rect.y;
    }

    var canvasGradient = ctx.createLinearGradient(x, y, x2, y2);

    return canvasGradient;
}

function createRadialGradient(ctx, obj, rect) {
    var width = rect.width;
    var height = rect.height;
    var min = Math.min(width, height);

    var x = obj.x == null ? 0.5 : obj.x;
    var y = obj.y == null ? 0.5 : obj.y;
    var r = obj.r == null ? 0.5 : obj.r;
    if (!obj.global) {
        x = x * width + rect.x;
        y = y * height + rect.y;
        r = r * min;
    }

    var canvasGradient = ctx.createRadialGradient(x, y, 0, x, y, r);

    return canvasGradient;
}


Style.prototype = {

    constructor: Style,

    /**
     * @type {module:zrender/graphic/Displayable}
     */
    host: null,

    /**
     * @type {string}
     */
    fill: '#000',

    /**
     * @type {string}
     */
    stroke: null,

    /**
     * @type {number}
     */
    opacity: 1,

    /**
     * @type {Array.<number>}
     */
    lineDash: null,

    /**
     * @type {number}
     */
    lineDashOffset: 0,

    /**
     * @type {number}
     */
    shadowBlur: 0,

    /**
     * @type {number}
     */
    shadowOffsetX: 0,

    /**
     * @type {number}
     */
    shadowOffsetY: 0,

    /**
     * @type {number}
     */
    lineWidth: 1,

    /**
     * If stroke ignore scale
     * @type {Boolean}
     */
    strokeNoScale: false,

    // Bounding rect text configuration
    // Not affected by element transform
    /**
     * @type {string}
     */
    text: null,

    /**
     * If `fontSize` or `fontFamily` exists, `font` will be reset by
     * `fontSize`, `fontStyle`, `fontWeight`, `fontFamily`.
     * So do not visit it directly in upper application (like echarts),
     * but use `contain/text#makeFont` instead.
     * @type {string}
     */
    font: null,

    /**
     * The same as font. Use font please.
     * @deprecated
     * @type {string}
     */
    textFont: null,

    /**
     * It helps merging respectively, rather than parsing an entire font string.
     * @type {string}
     */
    fontStyle: null,

    /**
     * It helps merging respectively, rather than parsing an entire font string.
     * @type {string}
     */
    fontWeight: null,

    /**
     * It helps merging respectively, rather than parsing an entire font string.
     * Should be 12 but not '12px'.
     * @type {number}
     */
    fontSize: null,

    /**
     * It helps merging respectively, rather than parsing an entire font string.
     * @type {string}
     */
    fontFamily: null,

    /**
     * Reserved for special functinality, like 'hr'.
     * @type {string}
     */
    textTag: null,

    /**
     * @type {string}
     */
    textFill: '#000',

    /**
     * @type {string}
     */
    textStroke: null,

    /**
     * @type {number}
     */
    textWidth: null,

    /**
     * Only for textBackground.
     * @type {number}
     */
    textHeight: null,

    /**
     * textStroke may be set as some color as a default
     * value in upper applicaion, where the default value
     * of textStrokeWidth should be 0 to make sure that
     * user can choose to do not use text stroke.
     * @type {number}
     */
    textStrokeWidth: 0,

    /**
     * @type {number}
     */
    textLineHeight: null,

    /**
     * 'inside', 'left', 'right', 'top', 'bottom'
     * [x, y]
     * Based on x, y of rect.
     * @type {string|Array.<number>}
     * @default 'inside'
     */
    textPosition: 'inside',

    /**
     * If not specified, use the boundingRect of a `displayable`.
     * @type {Object}
     */
    textRect: null,

    /**
     * [x, y]
     * @type {Array.<number>}
     */
    textOffset: null,

    /**
     * @type {string}
     */
    textAlign: null,

    /**
     * @type {string}
     */
    textVerticalAlign: null,

    /**
     * @type {number}
     */
    textDistance: 5,

    /**
     * @type {string}
     */
    textShadowColor: 'transparent',

    /**
     * @type {number}
     */
    textShadowBlur: 0,

    /**
     * @type {number}
     */
    textShadowOffsetX: 0,

    /**
     * @type {number}
     */
    textShadowOffsetY: 0,

    /**
     * @type {string}
     */
    textBoxShadowColor: 'transparent',

    /**
     * @type {number}
     */
    textBoxShadowBlur: 0,

    /**
     * @type {number}
     */
    textBoxShadowOffsetX: 0,

    /**
     * @type {number}
     */
    textBoxShadowOffsetY: 0,

    /**
     * Whether transform text.
     * Only useful in Path and Image element
     * @type {boolean}
     */
    transformText: false,

    /**
     * Text rotate around position of Path or Image
     * Only useful in Path and Image element and transformText is false.
     */
    textRotation: 0,

    /**
     * Text origin of text rotation, like [10, 40].
     * Based on x, y of rect.
     * Useful in label rotation of circular symbol.
     * By default, this origin is textPosition.
     * Can be 'center'.
     * @type {string|Array.<number>}
     */
    textOrigin: null,

    /**
     * @type {string}
     */
    textBackgroundColor: null,

    /**
     * @type {string}
     */
    textBorderColor: null,

    /**
     * @type {number}
     */
    textBorderWidth: 0,

    /**
     * @type {number}
     */
    textBorderRadius: 0,

    /**
     * Can be `2` or `[2, 4]` or `[2, 3, 4, 5]`
     * @type {number|Array.<number>}
     */
    textPadding: null,

    /**
     * Text styles for rich text.
     * @type {Object}
     */
    rich: null,

    /**
     * {outerWidth, outerHeight, ellipsis, placeholder}
     * @type {Object}
     */
    truncate: null,

    /**
     * https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/globalCompositeOperation
     * @type {string}
     */
    blend: null,

    /**
     * @param {CanvasRenderingContext2D} ctx
     */
    bind: function (ctx, el, prevEl) {
        var style = this;
        var prevStyle = prevEl && prevEl.style;
        var firstDraw = !prevStyle;

        for (var i = 0; i < STYLE_COMMON_PROPS.length; i++) {
            var prop = STYLE_COMMON_PROPS[i];
            var styleName = prop[0];

            if (firstDraw || style[styleName] !== prevStyle[styleName]) {
                // FIXME Invalid property value will cause style leak from previous element.
                ctx[styleName] =
                    fixShadow(ctx, styleName, style[styleName] || prop[1]);
            }
        }

        if ((firstDraw || style.fill !== prevStyle.fill)) {
            ctx.fillStyle = style.fill;
        }
        if ((firstDraw || style.stroke !== prevStyle.stroke)) {
            ctx.strokeStyle = style.stroke;
        }
        if ((firstDraw || style.opacity !== prevStyle.opacity)) {
            ctx.globalAlpha = style.opacity == null ? 1 : style.opacity;
        }

        if ((firstDraw || style.blend !== prevStyle.blend)) {
            ctx.globalCompositeOperation = style.blend || 'source-over';
        }
        if (this.hasStroke()) {
            var lineWidth = style.lineWidth;
            ctx.lineWidth = lineWidth / (
                (this.strokeNoScale && el && el.getLineScale) ? el.getLineScale() : 1
            );
        }
    },

    hasFill: function () {
        var fill = this.fill;
        return fill != null && fill !== 'none';
    },

    hasStroke: function () {
        var stroke = this.stroke;
        return stroke != null && stroke !== 'none' && this.lineWidth > 0;
    },

    /**
     * Extend from other style
     * @param {zrender/graphic/Style} otherStyle
     * @param {boolean} overwrite true: overwrirte any way.
     *                            false: overwrite only when !target.hasOwnProperty
     *                            others: overwrite when property is not null/undefined.
     */
    extendFrom: function (otherStyle, overwrite) {
        var this$1 = this;

        if (otherStyle) {
            for (var name in otherStyle) {
                if (otherStyle.hasOwnProperty(name)
                    && (overwrite === true
                        || (
                            overwrite === false
                                ? !this$1.hasOwnProperty(name)
                                : otherStyle[name] != null
                        )
                    )
                ) {
                    this$1[name] = otherStyle[name];
                }
            }
        }
    },

    /**
     * Batch setting style with a given object
     * @param {Object|string} obj
     * @param {*} [obj]
     */
    set: function (obj, value) {
        if (typeof obj === 'string') {
            this[obj] = value;
        }
        else {
            this.extendFrom(obj, true);
        }
    },

    /**
     * Clone
     * @return {zrender/graphic/Style} [description]
     */
    clone: function () {
        var newStyle = new this.constructor();
        newStyle.extendFrom(this, true);
        return newStyle;
    },

    getGradient: function (ctx, obj, rect) {
        var method = obj.type === 'radial' ? createRadialGradient : createLinearGradient;
        var canvasGradient = method(ctx, obj, rect);
        var colorStops = obj.colorStops;
        for (var i = 0; i < colorStops.length; i++) {
            canvasGradient.addColorStop(
                colorStops[i].offset, colorStops[i].color
            );
        }
        return canvasGradient;
    }

};

var styleProto = Style.prototype;
for (var i = 0; i < STYLE_COMMON_PROPS.length; i++) {
    var prop = STYLE_COMMON_PROPS[i];
    if (!(prop[0] in styleProto)) {
        styleProto[prop[0]] = prop[1];
    }
}

// Provide for others
Style.getGradient = styleProto.getGradient;

/**
 * zrender: 生成唯一id
 *
 * @author errorrik (errorrik@gmail.com)
 */

var idStart = 0x0907;

function guid () {
    return idStart++;
}

/**
 * 事件扩展
 * @module zrender/mixin/Eventful
 * @author Kener (@Kener-林峰, kener.linfeng@gmail.com)
 *         pissang (https://www.github.com/pissang)
 */

var arrySlice = Array.prototype.slice;

/**
 * 事件分发器
 * @alias module:zrender/mixin/Eventful
 * @constructor
 */
var Eventful = function () {
    this._$handlers = {};
};

Eventful.prototype = {

    constructor: Eventful,

    /**
     * 单次触发绑定，trigger后销毁
     *
     * @param {string} event 事件名
     * @param {Function} handler 响应函数
     * @param {Object} context
     */
    one: function (event, handler, context) {
        var this$1 = this;

        var _h = this._$handlers;

        if (!handler || !event) {
            return this;
        }

        if (!_h[event]) {
            _h[event] = [];
        }

        for (var i = 0; i < _h[event].length; i++) {
            if (_h[event][i].h === handler) {
                return this$1;
            }
        }

        _h[event].push({
            h: handler,
            one: true,
            ctx: context || this
        });

        return this;
    },

    /**
     * 绑定事件
     * @param {string} event 事件名
     * @param {Function} handler 事件处理函数
     * @param {Object} [context]
     */
    on: function (event, handler, context) {
        var this$1 = this;

        var _h = this._$handlers;

        if (!handler || !event) {
            return this;
        }

        if (!_h[event]) {
            _h[event] = [];
        }

        for (var i = 0; i < _h[event].length; i++) {
            if (_h[event][i].h === handler) {
                return this$1;
            }
        }

        _h[event].push({
            h: handler,
            one: false,
            ctx: context || this
        });

        return this;
    },

    /**
     * 是否绑定了事件
     * @param  {string}  event
     * @return {boolean}
     */
    isSilent: function (event) {
        var _h = this._$handlers;
        return _h[event] && _h[event].length;
    },

    /**
     * 解绑事件
     * @param {string} event 事件名
     * @param {Function} [handler] 事件处理函数
     */
    off: function (event, handler) {
        var _h = this._$handlers;

        if (!event) {
            this._$handlers = {};
            return this;
        }

        if (handler) {
            if (_h[event]) {
                var newList = [];
                for (var i = 0, l = _h[event].length; i < l; i++) {
                    if (_h[event][i]['h'] != handler) {
                        newList.push(_h[event][i]);
                    }
                }
                _h[event] = newList;
            }

            if (_h[event] && _h[event].length === 0) {
                delete _h[event];
            }
        }
        else {
            delete _h[event];
        }

        return this;
    },

    /**
     * 事件分发
     *
     * @param {string} type 事件类型
     */
    trigger: function (type) {
        if (this._$handlers[type]) {
            var args = arguments;
            var argLen = args.length;

            if (argLen > 3) {
                args = arrySlice.call(args, 1);
            }

            var _h = this._$handlers[type];
            var len = _h.length;
            for (var i = 0; i < len;) {
                // Optimize advise from backbone
                switch (argLen) {
                    case 1:
                        _h[i]['h'].call(_h[i]['ctx']);
                        break;
                    case 2:
                        _h[i]['h'].call(_h[i]['ctx'], args[1]);
                        break;
                    case 3:
                        _h[i]['h'].call(_h[i]['ctx'], args[1], args[2]);
                        break;
                    default:
                        // have more than 2 given arguments
                        _h[i]['h'].apply(_h[i]['ctx'], args);
                        break;
                }

                if (_h[i]['one']) {
                    _h.splice(i, 1);
                    len--;
                }
                else {
                    i++;
                }
            }
        }

        return this;
    },

    /**
     * 带有context的事件分发, 最后一个参数是事件回调的context
     * @param {string} type 事件类型
     */
    triggerWithContext: function (type) {
        if (this._$handlers[type]) {
            var args = arguments;
            var argLen = args.length;

            if (argLen > 4) {
                args = arrySlice.call(args, 1, args.length - 1);
            }
            var ctx = args[args.length - 1];

            var _h = this._$handlers[type];
            var len = _h.length;
            for (var i = 0; i < len;) {
                // Optimize advise from backbone
                switch (argLen) {
                    case 1:
                        _h[i]['h'].call(ctx);
                        break;
                    case 2:
                        _h[i]['h'].call(ctx, args[1]);
                        break;
                    case 3:
                        _h[i]['h'].call(ctx, args[1], args[2]);
                        break;
                    default:
                        // have more than 2 given arguments
                        _h[i]['h'].apply(ctx, args);
                        break;
                }

                if (_h[i]['one']) {
                    _h.splice(i, 1);
                    len--;
                }
                else {
                    i++;
                }
            }
        }

        return this;
    }
};

/**
 * 3x2矩阵操作类
 * @exports zrender/tool/matrix
 */

var ArrayCtor = typeof Float32Array === 'undefined'
    ? Array
   : Float32Array;

/**
 * 创建一个单位矩阵
 * @return {Float32Array|Array.<number>}
 */
function create$1() {
    var out = new ArrayCtor(6);
    identity(out);

    return out;
}

/**
 * 设置矩阵为单位矩阵
 * @param {Float32Array|Array.<number>} out
 */
function identity(out) {
    out[0] = 1;
    out[1] = 0;
    out[2] = 0;
    out[3] = 1;
    out[4] = 0;
    out[5] = 0;
    return out;
}

/**
 * 复制矩阵
 * @param {Float32Array|Array.<number>} out
 * @param {Float32Array|Array.<number>} m
 */
function copy(out, m) {
    out[0] = m[0];
    out[1] = m[1];
    out[2] = m[2];
    out[3] = m[3];
    out[4] = m[4];
    out[5] = m[5];
    return out;
}

/**
 * 矩阵相乘
 * @param {Float32Array|Array.<number>} out
 * @param {Float32Array|Array.<number>} m1
 * @param {Float32Array|Array.<number>} m2
 */
function mul(out, m1, m2) {
    // Consider matrix.mul(m, m2, m);
    // where out is the same as m2.
    // So use temp variable to escape error.
    var out0 = m1[0] * m2[0] + m1[2] * m2[1];
    var out1 = m1[1] * m2[0] + m1[3] * m2[1];
    var out2 = m1[0] * m2[2] + m1[2] * m2[3];
    var out3 = m1[1] * m2[2] + m1[3] * m2[3];
    var out4 = m1[0] * m2[4] + m1[2] * m2[5] + m1[4];
    var out5 = m1[1] * m2[4] + m1[3] * m2[5] + m1[5];
    out[0] = out0;
    out[1] = out1;
    out[2] = out2;
    out[3] = out3;
    out[4] = out4;
    out[5] = out5;
    return out;
}

/**
 * 平移变换
 * @param {Float32Array|Array.<number>} out
 * @param {Float32Array|Array.<number>} a
 * @param {Float32Array|Array.<number>} v
 */
function translate(out, a, v) {
    out[0] = a[0];
    out[1] = a[1];
    out[2] = a[2];
    out[3] = a[3];
    out[4] = a[4] + v[0];
    out[5] = a[5] + v[1];
    return out;
}

/**
 * 旋转变换
 * @param {Float32Array|Array.<number>} out
 * @param {Float32Array|Array.<number>} a
 * @param {number} rad
 */
function rotate(out, a, rad) {
    var aa = a[0];
    var ac = a[2];
    var atx = a[4];
    var ab = a[1];
    var ad = a[3];
    var aty = a[5];
    var st = Math.sin(rad);
    var ct = Math.cos(rad);

    out[0] = aa * ct + ab * st;
    out[1] = -aa * st + ab * ct;
    out[2] = ac * ct + ad * st;
    out[3] = -ac * st + ct * ad;
    out[4] = ct * atx + st * aty;
    out[5] = ct * aty - st * atx;
    return out;
}

/**
 * 缩放变换
 * @param {Float32Array|Array.<number>} out
 * @param {Float32Array|Array.<number>} a
 * @param {Float32Array|Array.<number>} v
 */
function scale(out, a, v) {
    var vx = v[0];
    var vy = v[1];
    out[0] = a[0] * vx;
    out[1] = a[1] * vy;
    out[2] = a[2] * vx;
    out[3] = a[3] * vy;
    out[4] = a[4] * vx;
    out[5] = a[5] * vy;
    return out;
}

/**
 * 求逆矩阵
 * @param {Float32Array|Array.<number>} out
 * @param {Float32Array|Array.<number>} a
 */
function invert(out, a) {

    var aa = a[0];
    var ac = a[2];
    var atx = a[4];
    var ab = a[1];
    var ad = a[3];
    var aty = a[5];

    var det = aa * ad - ab * ac;
    if (!det) {
        return null;
    }
    det = 1.0 / det;

    out[0] = ad * det;
    out[1] = -ab * det;
    out[2] = -ac * det;
    out[3] = aa * det;
    out[4] = (ac * aty - ad * atx) * det;
    out[5] = (ab * atx - aa * aty) * det;
    return out;
}

var ArrayCtor$1 = typeof Float32Array === 'undefined'
    ? Array
    : Float32Array;

/**
 * 创建一个向量
 * @param {number} [x=0]
 * @param {number} [y=0]
 * @return {Vector2}
 */
function create$2(x, y) {
    var out = new ArrayCtor$1(2);
    if (x == null) {
        x = 0;
    }
    if (y == null) {
        y = 0;
    }
    out[0] = x;
    out[1] = y;
    return out;
}

/**
 * 克隆一个向量
 * @param {Vector2} v
 * @return {Vector2}
 */
function clone$2(v) {
    var out = new ArrayCtor$1(2);
    out[0] = v[0];
    out[1] = v[1];
    return out;
}

/**
 * 向量相加
 * @param {Vector2} out
 * @param {Vector2} v1
 * @param {Vector2} v2
 */
function add(out, v1, v2) {
    out[0] = v1[0] + v2[0];
    out[1] = v1[1] + v2[1];
    return out;
}

/**
 * 向量相减
 * @param {Vector2} out
 * @param {Vector2} v1
 * @param {Vector2} v2
 */
function sub(out, v1, v2) {
    out[0] = v1[0] - v2[0];
    out[1] = v1[1] - v2[1];
    return out;
}

/**
 * 向量缩放
 * @param {Vector2} out
 * @param {Vector2} v
 * @param {number} s
 */
function scale$1(out, v, s) {
    out[0] = v[0] * s;
    out[1] = v[1] * s;
    return out;
}

/**
 * 计算向量间距离
 * @param {Vector2} v1
 * @param {Vector2} v2
 * @return {number}
 */
function distance(v1, v2) {
    return Math.sqrt(
        (v1[0] - v2[0]) * (v1[0] - v2[0])
        + (v1[1] - v2[1]) * (v1[1] - v2[1])
    );
}
var dist = distance;

/**
 * 向量距离平方
 * @param {Vector2} v1
 * @param {Vector2} v2
 * @return {number}
 */
function distanceSquare(v1, v2) {
    return (v1[0] - v2[0]) * (v1[0] - v2[0])
        + (v1[1] - v2[1]) * (v1[1] - v2[1]);
}
var distSquare = distanceSquare;

/**
 * 矩阵左乘向量
 * @param {Vector2} out
 * @param {Vector2} v
 * @param {Vector2} m
 */
function applyTransform(out, v, m) {
    var x = v[0];
    var y = v[1];
    out[0] = m[0] * x + m[2] * y + m[4];
    out[1] = m[1] * x + m[3] * y + m[5];
    return out;
}

/**
 * 求两个向量最小值
 * @param  {Vector2} out
 * @param  {Vector2} v1
 * @param  {Vector2} v2
 */
function min(out, v1, v2) {
    out[0] = Math.min(v1[0], v2[0]);
    out[1] = Math.min(v1[1], v2[1]);
    return out;
}

/**
 * 求两个向量最大值
 * @param  {Vector2} out
 * @param  {Vector2} v1
 * @param  {Vector2} v2
 */
function max(out, v1, v2) {
    out[0] = Math.max(v1[0], v2[0]);
    out[1] = Math.max(v1[1], v2[1]);
    return out;
}

/**
 * 提供变换扩展
 * @module zrender/mixin/Transformable
 * @author pissang (https://www.github.com/pissang)
 */

var mIdentity = identity;

var EPSILON = 5e-5;

function isNotAroundZero(val) {
    return val > EPSILON || val < -EPSILON;
}

/**
 * @alias module:zrender/mixin/Transformable
 * @constructor
 */
var Transformable = function (opts) {
    opts = opts || {};
    // If there are no given position, rotation, scale
    if (!opts.position) {
        /**
         * 平移
         * @type {Array.<number>}
         * @default [0, 0]
         */
        this.position = [0, 0];
    }
    if (opts.rotation == null) {
        /**
         * 旋转
         * @type {Array.<number>}
         * @default 0
         */
        this.rotation = 0;
    }
    if (!opts.scale) {
        /**
         * 缩放
         * @type {Array.<number>}
         * @default [1, 1]
         */
        this.scale = [1, 1];
    }
    /**
     * 旋转和缩放的原点
     * @type {Array.<number>}
     * @default null
     */
    this.origin = this.origin || null;
};

var transformableProto = Transformable.prototype;
transformableProto.transform = null;

/**
 * 判断是否需要有坐标变换
 * 如果有坐标变换, 则从position, rotation, scale以及父节点的transform计算出自身的transform矩阵
 */
transformableProto.needLocalTransform = function () {
    return isNotAroundZero(this.rotation)
        || isNotAroundZero(this.position[0])
        || isNotAroundZero(this.position[1])
        || isNotAroundZero(this.scale[0] - 1)
        || isNotAroundZero(this.scale[1] - 1);
};

transformableProto.updateTransform = function () {
    var parent = this.parent;
    var parentHasTransform = parent && parent.transform;
    var needLocalTransform = this.needLocalTransform();

    var m = this.transform;
    if (!(needLocalTransform || parentHasTransform)) {
        m && mIdentity(m);
        return;
    }

    m = m || create$1();

    if (needLocalTransform) {
        this.getLocalTransform(m);
    }
    else {
        mIdentity(m);
    }

    // 应用父节点变换
    if (parentHasTransform) {
        if (needLocalTransform) {
            mul(m, parent.transform, m);
        }
        else {
            copy(m, parent.transform);
        }
    }
    // 保存这个变换矩阵
    this.transform = m;

    this.invTransform = this.invTransform || create$1();
    invert(this.invTransform, m);
};

transformableProto.getLocalTransform = function (m) {
    return Transformable.getLocalTransform(this, m);
};

/**
 * 将自己的transform应用到context上
 * @param {CanvasRenderingContext2D} ctx
 */
transformableProto.setTransform = function (ctx) {
    var m = this.transform;
    var dpr = ctx.dpr || 1;
    if (m) {
        ctx.setTransform(dpr * m[0], dpr * m[1], dpr * m[2], dpr * m[3], dpr * m[4], dpr * m[5]);
    }
    else {
        ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    }
};

transformableProto.restoreTransform = function (ctx) {
    var dpr = ctx.dpr || 1;
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
};

var tmpTransform = [];

/**
 * 分解`transform`矩阵到`position`, `rotation`, `scale`
 */
transformableProto.decomposeTransform = function () {
    if (!this.transform) {
        return;
    }
    var parent = this.parent;
    var m = this.transform;
    if (parent && parent.transform) {
        // Get local transform and decompose them to position, scale, rotation
        mul(tmpTransform, parent.invTransform, m);
        m = tmpTransform;
    }
    var sx = m[0] * m[0] + m[1] * m[1];
    var sy = m[2] * m[2] + m[3] * m[3];
    var position = this.position;
    var scale$$1 = this.scale;
    if (isNotAroundZero(sx - 1)) {
        sx = Math.sqrt(sx);
    }
    if (isNotAroundZero(sy - 1)) {
        sy = Math.sqrt(sy);
    }
    if (m[0] < 0) {
        sx = -sx;
    }
    if (m[3] < 0) {
        sy = -sy;
    }
    position[0] = m[4];
    position[1] = m[5];
    scale$$1[0] = sx;
    scale$$1[1] = sy;
    this.rotation = Math.atan2(-m[1] / sy, m[0] / sx);
};

/**
 * Get global scale
 * @return {Array.<number>}
 */
transformableProto.getGlobalScale = function () {
    var m = this.transform;
    if (!m) {
        return [1, 1];
    }
    var sx = Math.sqrt(m[0] * m[0] + m[1] * m[1]);
    var sy = Math.sqrt(m[2] * m[2] + m[3] * m[3]);
    if (m[0] < 0) {
        sx = -sx;
    }
    if (m[3] < 0) {
        sy = -sy;
    }
    return [sx, sy];
};
/**
 * 变换坐标位置到 shape 的局部坐标空间
 * @method
 * @param {number} x
 * @param {number} y
 * @return {Array.<number>}
 */
transformableProto.transformCoordToLocal = function (x, y) {
    var v2 = [x, y];
    var invTransform = this.invTransform;
    if (invTransform) {
        applyTransform(v2, v2, invTransform);
    }
    return v2;
};

/**
 * 变换局部坐标位置到全局坐标空间
 * @method
 * @param {number} x
 * @param {number} y
 * @return {Array.<number>}
 */
transformableProto.transformCoordToGlobal = function (x, y) {
    var v2 = [x, y];
    var transform = this.transform;
    if (transform) {
        applyTransform(v2, v2, transform);
    }
    return v2;
};

/**
 * @static
 * @param {Object} target
 * @param {Array.<number>} target.origin
 * @param {number} target.rotation
 * @param {Array.<number>} target.position
 * @param {Array.<number>} [m]
 */
Transformable.getLocalTransform = function (target, m) {
    m = m || [];
    mIdentity(m);

    var origin = target.origin;
    var scale$$1 = target.scale || [1, 1];
    var rotation = target.rotation || 0;
    var position = target.position || [0, 0];

    if (origin) {
        // Translate to origin
        m[4] -= origin[0];
        m[5] -= origin[1];
    }
    scale(m, m, scale$$1);
    if (rotation) {
        rotate(m, m, rotation);
    }
    if (origin) {
        // Translate back from origin
        m[4] += origin[0];
        m[5] += origin[1];
    }

    m[4] += position[0];
    m[5] += position[1];

    return m;
};

/**
 * 缓动代码来自 https://github.com/sole/tween.js/blob/master/src/Tween.js
 * @see http://sole.github.io/tween.js/examples/03_graphs.html
 * @exports zrender/animation/easing
 */
var easing = {
    /**
    * @param {number} k
    * @return {number}
    */
    linear: function (k) {
        return k;
    },

    /**
    * @param {number} k
    * @return {number}
    */
    quadraticIn: function (k) {
        return k * k;
    },
    /**
    * @param {number} k
    * @return {number}
    */
    quadraticOut: function (k) {
        return k * (2 - k);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    quadraticInOut: function (k) {
        if ((k *= 2) < 1) {
            return 0.5 * k * k;
        }
        return -0.5 * (--k * (k - 2) - 1);
    },

    // 三次方的缓动（t^3）
    /**
    * @param {number} k
    * @return {number}
    */
    cubicIn: function (k) {
        return k * k * k;
    },
    /**
    * @param {number} k
    * @return {number}
    */
    cubicOut: function (k) {
        return --k * k * k + 1;
    },
    /**
    * @param {number} k
    * @return {number}
    */
    cubicInOut: function (k) {
        if ((k *= 2) < 1) {
            return 0.5 * k * k * k;
        }
        return 0.5 * ((k -= 2) * k * k + 2);
    },

    // 四次方的缓动（t^4）
    /**
    * @param {number} k
    * @return {number}
    */
    quarticIn: function (k) {
        return k * k * k * k;
    },
    /**
    * @param {number} k
    * @return {number}
    */
    quarticOut: function (k) {
        return 1 - (--k * k * k * k);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    quarticInOut: function (k) {
        if ((k *= 2) < 1) {
            return 0.5 * k * k * k * k;
        }
        return -0.5 * ((k -= 2) * k * k * k - 2);
    },

    // 五次方的缓动（t^5）
    /**
    * @param {number} k
    * @return {number}
    */
    quinticIn: function (k) {
        return k * k * k * k * k;
    },
    /**
    * @param {number} k
    * @return {number}
    */
    quinticOut: function (k) {
        return --k * k * k * k * k + 1;
    },
    /**
    * @param {number} k
    * @return {number}
    */
    quinticInOut: function (k) {
        if ((k *= 2) < 1) {
            return 0.5 * k * k * k * k * k;
        }
        return 0.5 * ((k -= 2) * k * k * k * k + 2);
    },

    // 正弦曲线的缓动（sin(t)）
    /**
    * @param {number} k
    * @return {number}
    */
    sinusoidalIn: function (k) {
        return 1 - Math.cos(k * Math.PI / 2);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    sinusoidalOut: function (k) {
        return Math.sin(k * Math.PI / 2);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    sinusoidalInOut: function (k) {
        return 0.5 * (1 - Math.cos(Math.PI * k));
    },

    // 指数曲线的缓动（2^t）
    /**
    * @param {number} k
    * @return {number}
    */
    exponentialIn: function (k) {
        return k === 0 ? 0 : Math.pow(1024, k - 1);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    exponentialOut: function (k) {
        return k === 1 ? 1 : 1 - Math.pow(2, -10 * k);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    exponentialInOut: function (k) {
        if (k === 0) {
            return 0;
        }
        if (k === 1) {
            return 1;
        }
        if ((k *= 2) < 1) {
            return 0.5 * Math.pow(1024, k - 1);
        }
        return 0.5 * (-Math.pow(2, -10 * (k - 1)) + 2);
    },

    // 圆形曲线的缓动（sqrt(1-t^2)）
    /**
    * @param {number} k
    * @return {number}
    */
    circularIn: function (k) {
        return 1 - Math.sqrt(1 - k * k);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    circularOut: function (k) {
        return Math.sqrt(1 - (--k * k));
    },
    /**
    * @param {number} k
    * @return {number}
    */
    circularInOut: function (k) {
        if ((k *= 2) < 1) {
            return -0.5 * (Math.sqrt(1 - k * k) - 1);
        }
        return 0.5 * (Math.sqrt(1 - (k -= 2) * k) + 1);
    },

    // 创建类似于弹簧在停止前来回振荡的动画
    /**
    * @param {number} k
    * @return {number}
    */
    elasticIn: function (k) {
        var s;
        var a = 0.1;
        var p = 0.4;
        if (k === 0) {
            return 0;
        }
        if (k === 1) {
            return 1;
        }
        if (!a || a < 1) {
            a = 1; s = p / 4;
        }
        else {
            s = p * Math.asin(1 / a) / (2 * Math.PI);
        }
        return -(a * Math.pow(2, 10 * (k -= 1)) *
                    Math.sin((k - s) * (2 * Math.PI) / p));
    },
    /**
    * @param {number} k
    * @return {number}
    */
    elasticOut: function (k) {
        var s;
        var a = 0.1;
        var p = 0.4;
        if (k === 0) {
            return 0;
        }
        if (k === 1) {
            return 1;
        }
        if (!a || a < 1) {
            a = 1; s = p / 4;
        }
        else {
            s = p * Math.asin(1 / a) / (2 * Math.PI);
        }
        return (a * Math.pow(2, -10 * k) *
                Math.sin((k - s) * (2 * Math.PI) / p) + 1);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    elasticInOut: function (k) {
        var s;
        var a = 0.1;
        var p = 0.4;
        if (k === 0) {
            return 0;
        }
        if (k === 1) {
            return 1;
        }
        if (!a || a < 1) {
            a = 1; s = p / 4;
        }
        else {
            s = p * Math.asin(1 / a) / (2 * Math.PI);
        }
        if ((k *= 2) < 1) {
            return -0.5 * (a * Math.pow(2, 10 * (k -= 1))
                * Math.sin((k - s) * (2 * Math.PI) / p));
        }
        return a * Math.pow(2, -10 * (k -= 1))
                * Math.sin((k - s) * (2 * Math.PI) / p) * 0.5 + 1;

    },

    // 在某一动画开始沿指示的路径进行动画处理前稍稍收回该动画的移动
    /**
    * @param {number} k
    * @return {number}
    */
    backIn: function (k) {
        var s = 1.70158;
        return k * k * ((s + 1) * k - s);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    backOut: function (k) {
        var s = 1.70158;
        return --k * k * ((s + 1) * k + s) + 1;
    },
    /**
    * @param {number} k
    * @return {number}
    */
    backInOut: function (k) {
        var s = 1.70158 * 1.525;
        if ((k *= 2) < 1) {
            return 0.5 * (k * k * ((s + 1) * k - s));
        }
        return 0.5 * ((k -= 2) * k * ((s + 1) * k + s) + 2);
    },

    // 创建弹跳效果
    /**
    * @param {number} k
    * @return {number}
    */
    bounceIn: function (k) {
        return 1 - easing.bounceOut(1 - k);
    },
    /**
    * @param {number} k
    * @return {number}
    */
    bounceOut: function (k) {
        if (k < (1 / 2.75)) {
            return 7.5625 * k * k;
        }
        else if (k < (2 / 2.75)) {
            return 7.5625 * (k -= (1.5 / 2.75)) * k + 0.75;
        }
        else if (k < (2.5 / 2.75)) {
            return 7.5625 * (k -= (2.25 / 2.75)) * k + 0.9375;
        }
        else {
            return 7.5625 * (k -= (2.625 / 2.75)) * k + 0.984375;
        }
    },
    /**
    * @param {number} k
    * @return {number}
    */
    bounceInOut: function (k) {
        if (k < 0.5) {
            return easing.bounceIn(k * 2) * 0.5;
        }
        return easing.bounceOut(k * 2 - 1) * 0.5 + 0.5;
    }
};

/**
 * 动画主控制器
 * @config target 动画对象，可以是数组，如果是数组的话会批量分发onframe等事件
 * @config life(1000) 动画时长
 * @config delay(0) 动画延迟时间
 * @config loop(true)
 * @config gap(0) 循环的间隔时间
 * @config onframe
 * @config easing(optional)
 * @config ondestroy(optional)
 * @config onrestart(optional)
 *
 * TODO pause
 */

function Clip(options) {

    this._target = options.target;

    // 生命周期
    this._life = options.life || 1000;
    // 延时
    this._delay = options.delay || 0;
    // 开始时间
    // this._startTime = new Date().getTime() + this._delay;// 单位毫秒
    this._initialized = false;

    // 是否循环
    this.loop = options.loop == null ? false : options.loop;

    this.gap = options.gap || 0;

    this.easing = options.easing || 'Linear';

    this.onframe = options.onframe;
    this.ondestroy = options.ondestroy;
    this.onrestart = options.onrestart;

    this._pausedTime = 0;
    this._paused = false;
}

Clip.prototype = {

    constructor: Clip,

    step: function (globalTime, deltaTime) {
        // Set startTime on first step, or _startTime may has milleseconds different between clips
        // PENDING
        if (!this._initialized) {
            this._startTime = globalTime + this._delay;
            this._initialized = true;
        }

        if (this._paused) {
            this._pausedTime += deltaTime;
            return;
        }

        var percent = (globalTime - this._startTime - this._pausedTime) / this._life;

        // 还没开始
        if (percent < 0) {
            return;
        }

        percent = Math.min(percent, 1);

        var easing$$1 = this.easing;
        var easingFunc = typeof easing$$1 == 'string' ? easing[easing$$1] : easing$$1;
        var schedule = typeof easingFunc === 'function'
            ? easingFunc(percent)
            : percent;

        this.fire('frame', schedule);

        // 结束
        if (percent == 1) {
            if (this.loop) {
                this.restart (globalTime);
                // 重新开始周期
                // 抛出而不是直接调用事件直到 stage.update 后再统一调用这些事件
                return 'restart';
            }

            // 动画完成将这个控制器标识为待删除
            // 在Animation.update中进行批量删除
            this._needsRemove = true;
            return 'destroy';
        }

        return null;
    },

    restart: function (globalTime) {
        var remainder = (globalTime - this._startTime - this._pausedTime) % this._life;
        this._startTime = globalTime - remainder + this.gap;
        this._pausedTime = 0;

        this._needsRemove = false;
    },

    fire: function (eventType, arg) {
        eventType = 'on' + eventType;
        if (this[eventType]) {
            this[eventType](this._target, arg);
        }
    },

    pause: function () {
        this._paused = true;
    },

    resume: function () {
        this._paused = false;
    }
};

// Simple LRU cache use doubly linked list
// @module zrender/core/LRU

/**
 * Simple double linked list. Compared with array, it has O(1) remove operation.
 * @constructor
 */
var LinkedList = function () {

    /**
     * @type {module:zrender/core/LRU~Entry}
     */
    this.head = null;

    /**
     * @type {module:zrender/core/LRU~Entry}
     */
    this.tail = null;

    this._len = 0;
};

var linkedListProto = LinkedList.prototype;
/**
 * Insert a new value at the tail
 * @param  {} val
 * @return {module:zrender/core/LRU~Entry}
 */
linkedListProto.insert = function (val) {
    var entry = new Entry(val);
    this.insertEntry(entry);
    return entry;
};

/**
 * Insert an entry at the tail
 * @param  {module:zrender/core/LRU~Entry} entry
 */
linkedListProto.insertEntry = function (entry) {
    if (!this.head) {
        this.head = this.tail = entry;
    }
    else {
        this.tail.next = entry;
        entry.prev = this.tail;
        entry.next = null;
        this.tail = entry;
    }
    this._len++;
};

/**
 * Remove entry.
 * @param  {module:zrender/core/LRU~Entry} entry
 */
linkedListProto.remove = function (entry) {
    var prev = entry.prev;
    var next = entry.next;
    if (prev) {
        prev.next = next;
    }
    else {
        // Is head
        this.head = next;
    }
    if (next) {
        next.prev = prev;
    }
    else {
        // Is tail
        this.tail = prev;
    }
    entry.next = entry.prev = null;
    this._len--;
};

/**
 * @return {number}
 */
linkedListProto.len = function () {
    return this._len;
};

/**
 * Clear list
 */
linkedListProto.clear = function () {
    this.head = this.tail = null;
    this._len = 0;
};

/**
 * @constructor
 * @param {} val
 */
var Entry = function (val) {
    /**
     * @type {}
     */
    this.value = val;

    /**
     * @type {module:zrender/core/LRU~Entry}
     */
    this.next;

    /**
     * @type {module:zrender/core/LRU~Entry}
     */
    this.prev;
};

/**
 * LRU Cache
 * @constructor
 * @alias module:zrender/core/LRU
 */
var LRU = function (maxSize) {

    this._list = new LinkedList();

    this._map = {};

    this._maxSize = maxSize || 10;

    this._lastRemovedEntry = null;
};

var LRUProto = LRU.prototype;

/**
 * @param  {string} key
 * @param  {} value
 * @return {} Removed value
 */
LRUProto.put = function (key, value) {
    var list = this._list;
    var map = this._map;
    var removed = null;
    if (map[key] == null) {
        var len = list.len();
        // Reuse last removed entry
        var entry = this._lastRemovedEntry;

        if (len >= this._maxSize && len > 0) {
            // Remove the least recently used
            var leastUsedEntry = list.head;
            list.remove(leastUsedEntry);
            delete map[leastUsedEntry.key];

            removed = leastUsedEntry.value;
            this._lastRemovedEntry = leastUsedEntry;
        }

        if (entry) {
            entry.value = value;
        }
        else {
            entry = new Entry(value);
        }
        entry.key = key;
        list.insertEntry(entry);
        map[key] = entry;
    }

    return removed;
};

/**
 * @param  {string} key
 * @return {}
 */
LRUProto.get = function (key) {
    var entry = this._map[key];
    var list = this._list;
    if (entry != null) {
        // Put the latest used entry in the tail
        if (entry !== list.tail) {
            list.remove(entry);
            list.insertEntry(entry);
        }

        return entry.value;
    }
};

/**
 * Clear the cache
 */
LRUProto.clear = function () {
    this._list.clear();
    this._map = {};
};

var kCSSColorTable = {
    'transparent': [0,0,0,0], 'aliceblue': [240,248,255,1],
    'antiquewhite': [250,235,215,1], 'aqua': [0,255,255,1],
    'aquamarine': [127,255,212,1], 'azure': [240,255,255,1],
    'beige': [245,245,220,1], 'bisque': [255,228,196,1],
    'black': [0,0,0,1], 'blanchedalmond': [255,235,205,1],
    'blue': [0,0,255,1], 'blueviolet': [138,43,226,1],
    'brown': [165,42,42,1], 'burlywood': [222,184,135,1],
    'cadetblue': [95,158,160,1], 'chartreuse': [127,255,0,1],
    'chocolate': [210,105,30,1], 'coral': [255,127,80,1],
    'cornflowerblue': [100,149,237,1], 'cornsilk': [255,248,220,1],
    'crimson': [220,20,60,1], 'cyan': [0,255,255,1],
    'darkblue': [0,0,139,1], 'darkcyan': [0,139,139,1],
    'darkgoldenrod': [184,134,11,1], 'darkgray': [169,169,169,1],
    'darkgreen': [0,100,0,1], 'darkgrey': [169,169,169,1],
    'darkkhaki': [189,183,107,1], 'darkmagenta': [139,0,139,1],
    'darkolivegreen': [85,107,47,1], 'darkorange': [255,140,0,1],
    'darkorchid': [153,50,204,1], 'darkred': [139,0,0,1],
    'darksalmon': [233,150,122,1], 'darkseagreen': [143,188,143,1],
    'darkslateblue': [72,61,139,1], 'darkslategray': [47,79,79,1],
    'darkslategrey': [47,79,79,1], 'darkturquoise': [0,206,209,1],
    'darkviolet': [148,0,211,1], 'deeppink': [255,20,147,1],
    'deepskyblue': [0,191,255,1], 'dimgray': [105,105,105,1],
    'dimgrey': [105,105,105,1], 'dodgerblue': [30,144,255,1],
    'firebrick': [178,34,34,1], 'floralwhite': [255,250,240,1],
    'forestgreen': [34,139,34,1], 'fuchsia': [255,0,255,1],
    'gainsboro': [220,220,220,1], 'ghostwhite': [248,248,255,1],
    'gold': [255,215,0,1], 'goldenrod': [218,165,32,1],
    'gray': [128,128,128,1], 'green': [0,128,0,1],
    'greenyellow': [173,255,47,1], 'grey': [128,128,128,1],
    'honeydew': [240,255,240,1], 'hotpink': [255,105,180,1],
    'indianred': [205,92,92,1], 'indigo': [75,0,130,1],
    'ivory': [255,255,240,1], 'khaki': [240,230,140,1],
    'lavender': [230,230,250,1], 'lavenderblush': [255,240,245,1],
    'lawngreen': [124,252,0,1], 'lemonchiffon': [255,250,205,1],
    'lightblue': [173,216,230,1], 'lightcoral': [240,128,128,1],
    'lightcyan': [224,255,255,1], 'lightgoldenrodyellow': [250,250,210,1],
    'lightgray': [211,211,211,1], 'lightgreen': [144,238,144,1],
    'lightgrey': [211,211,211,1], 'lightpink': [255,182,193,1],
    'lightsalmon': [255,160,122,1], 'lightseagreen': [32,178,170,1],
    'lightskyblue': [135,206,250,1], 'lightslategray': [119,136,153,1],
    'lightslategrey': [119,136,153,1], 'lightsteelblue': [176,196,222,1],
    'lightyellow': [255,255,224,1], 'lime': [0,255,0,1],
    'limegreen': [50,205,50,1], 'linen': [250,240,230,1],
    'magenta': [255,0,255,1], 'maroon': [128,0,0,1],
    'mediumaquamarine': [102,205,170,1], 'mediumblue': [0,0,205,1],
    'mediumorchid': [186,85,211,1], 'mediumpurple': [147,112,219,1],
    'mediumseagreen': [60,179,113,1], 'mediumslateblue': [123,104,238,1],
    'mediumspringgreen': [0,250,154,1], 'mediumturquoise': [72,209,204,1],
    'mediumvioletred': [199,21,133,1], 'midnightblue': [25,25,112,1],
    'mintcream': [245,255,250,1], 'mistyrose': [255,228,225,1],
    'moccasin': [255,228,181,1], 'navajowhite': [255,222,173,1],
    'navy': [0,0,128,1], 'oldlace': [253,245,230,1],
    'olive': [128,128,0,1], 'olivedrab': [107,142,35,1],
    'orange': [255,165,0,1], 'orangered': [255,69,0,1],
    'orchid': [218,112,214,1], 'palegoldenrod': [238,232,170,1],
    'palegreen': [152,251,152,1], 'paleturquoise': [175,238,238,1],
    'palevioletred': [219,112,147,1], 'papayawhip': [255,239,213,1],
    'peachpuff': [255,218,185,1], 'peru': [205,133,63,1],
    'pink': [255,192,203,1], 'plum': [221,160,221,1],
    'powderblue': [176,224,230,1], 'purple': [128,0,128,1],
    'red': [255,0,0,1], 'rosybrown': [188,143,143,1],
    'royalblue': [65,105,225,1], 'saddlebrown': [139,69,19,1],
    'salmon': [250,128,114,1], 'sandybrown': [244,164,96,1],
    'seagreen': [46,139,87,1], 'seashell': [255,245,238,1],
    'sienna': [160,82,45,1], 'silver': [192,192,192,1],
    'skyblue': [135,206,235,1], 'slateblue': [106,90,205,1],
    'slategray': [112,128,144,1], 'slategrey': [112,128,144,1],
    'snow': [255,250,250,1], 'springgreen': [0,255,127,1],
    'steelblue': [70,130,180,1], 'tan': [210,180,140,1],
    'teal': [0,128,128,1], 'thistle': [216,191,216,1],
    'tomato': [255,99,71,1], 'turquoise': [64,224,208,1],
    'violet': [238,130,238,1], 'wheat': [245,222,179,1],
    'white': [255,255,255,1], 'whitesmoke': [245,245,245,1],
    'yellow': [255,255,0,1], 'yellowgreen': [154,205,50,1]
};

function clampCssByte(i) {  // Clamp to integer 0 .. 255.
    i = Math.round(i);  // Seems to be what Chrome does (vs truncation).
    return i < 0 ? 0 : i > 255 ? 255 : i;
}

function clampCssFloat(f) {  // Clamp to float 0.0 .. 1.0.
    return f < 0 ? 0 : f > 1 ? 1 : f;
}

function parseCssInt(str) {  // int or percentage.
    if (str.length && str.charAt(str.length - 1) === '%') {
        return clampCssByte(parseFloat(str) / 100 * 255);
    }
    return clampCssByte(parseInt(str, 10));
}

function parseCssFloat(str) {  // float or percentage.
    if (str.length && str.charAt(str.length - 1) === '%') {
        return clampCssFloat(parseFloat(str) / 100);
    }
    return clampCssFloat(parseFloat(str));
}

function cssHueToRgb(m1, m2, h) {
    if (h < 0) {
        h += 1;
    }
    else if (h > 1) {
        h -= 1;
    }

    if (h * 6 < 1) {
        return m1 + (m2 - m1) * h * 6;
    }
    if (h * 2 < 1) {
        return m2;
    }
    if (h * 3 < 2) {
        return m1 + (m2 - m1) * (2/3 - h) * 6;
    }
    return m1;
}

function setRgba(out, r, g, b, a) {
    out[0] = r; out[1] = g; out[2] = b; out[3] = a;
    return out;
}
function copyRgba(out, a) {
    out[0] = a[0]; out[1] = a[1]; out[2] = a[2]; out[3] = a[3];
    return out;
}

var colorCache = new LRU(20);
var lastRemovedArr = null;

function putToCache(colorStr, rgbaArr) {
    // Reuse removed array
    if (lastRemovedArr) {
        copyRgba(lastRemovedArr, rgbaArr);
    }
    lastRemovedArr = colorCache.put(colorStr, lastRemovedArr || (rgbaArr.slice()));
}

/**
 * @param {string} colorStr
 * @param {Array.<number>} out
 * @return {Array.<number>}
 * @memberOf module:zrender/util/color
 */
function parse(colorStr, rgbaArr) {
    if (!colorStr) {
        return;
    }
    rgbaArr = rgbaArr || [];

    var cached = colorCache.get(colorStr);
    if (cached) {
        return copyRgba(rgbaArr, cached);
    }

    // colorStr may be not string
    colorStr = colorStr + '';
    // Remove all whitespace, not compliant, but should just be more accepting.
    var str = colorStr.replace(/ /g, '').toLowerCase();

    // Color keywords (and transparent) lookup.
    if (str in kCSSColorTable) {
        copyRgba(rgbaArr, kCSSColorTable[str]);
        putToCache(colorStr, rgbaArr);
        return rgbaArr;
    }

    // #abc and #abc123 syntax.
    if (str.charAt(0) === '#') {
        if (str.length === 4) {
            var iv = parseInt(str.substr(1), 16);  // TODO(deanm): Stricter parsing.
            if (!(iv >= 0 && iv <= 0xfff)) {
                setRgba(rgbaArr, 0, 0, 0, 1);
                return;  // Covers NaN.
            }
            setRgba(rgbaArr,
                ((iv & 0xf00) >> 4) | ((iv & 0xf00) >> 8),
                (iv & 0xf0) | ((iv & 0xf0) >> 4),
                (iv & 0xf) | ((iv & 0xf) << 4),
                1
            );
            putToCache(colorStr, rgbaArr);
            return rgbaArr;
        }
        else if (str.length === 7) {
            var iv = parseInt(str.substr(1), 16);  // TODO(deanm): Stricter parsing.
            if (!(iv >= 0 && iv <= 0xffffff)) {
                setRgba(rgbaArr, 0, 0, 0, 1);
                return;  // Covers NaN.
            }
            setRgba(rgbaArr,
                (iv & 0xff0000) >> 16,
                (iv & 0xff00) >> 8,
                iv & 0xff,
                1
            );
            putToCache(colorStr, rgbaArr);
            return rgbaArr;
        }

        return;
    }
    var op = str.indexOf('('), ep = str.indexOf(')');
    if (op !== -1 && ep + 1 === str.length) {
        var fname = str.substr(0, op);
        var params = str.substr(op + 1, ep - (op + 1)).split(',');
        var alpha = 1;  // To allow case fallthrough.
        switch (fname) {
            case 'rgba':
                if (params.length !== 4) {
                    setRgba(rgbaArr, 0, 0, 0, 1);
                    return;
                }
                alpha = parseCssFloat(params.pop()); // jshint ignore:line
            // Fall through.
            case 'rgb':
                if (params.length !== 3) {
                    setRgba(rgbaArr, 0, 0, 0, 1);
                    return;
                }
                setRgba(rgbaArr,
                    parseCssInt(params[0]),
                    parseCssInt(params[1]),
                    parseCssInt(params[2]),
                    alpha
                );
                putToCache(colorStr, rgbaArr);
                return rgbaArr;
            case 'hsla':
                if (params.length !== 4) {
                    setRgba(rgbaArr, 0, 0, 0, 1);
                    return;
                }
                params[3] = parseCssFloat(params[3]);
                hsla2rgba(params, rgbaArr);
                putToCache(colorStr, rgbaArr);
                return rgbaArr;
            case 'hsl':
                if (params.length !== 3) {
                    setRgba(rgbaArr, 0, 0, 0, 1);
                    return;
                }
                hsla2rgba(params, rgbaArr);
                putToCache(colorStr, rgbaArr);
                return rgbaArr;
            default:
                return;
        }
    }

    setRgba(rgbaArr, 0, 0, 0, 1);
    return;
}

/**
 * @param {Array.<number>} hsla
 * @param {Array.<number>} rgba
 * @return {Array.<number>} rgba
 */
function hsla2rgba(hsla, rgba) {
    var h = (((parseFloat(hsla[0]) % 360) + 360) % 360) / 360;  // 0 .. 1
    // NOTE(deanm): According to the CSS spec s/l should only be
    // percentages, but we don't bother and let float or percentage.
    var s = parseCssFloat(hsla[1]);
    var l = parseCssFloat(hsla[2]);
    var m2 = l <= 0.5 ? l * (s + 1) : l + s - l * s;
    var m1 = l * 2 - m2;

    rgba = rgba || [];
    setRgba(rgba,
        clampCssByte(cssHueToRgb(m1, m2, h + 1 / 3) * 255),
        clampCssByte(cssHueToRgb(m1, m2, h) * 255),
        clampCssByte(cssHueToRgb(m1, m2, h - 1 / 3) * 255),
        1
    );

    if (hsla.length === 4) {
        rgba[3] = hsla[3];
    }

    return rgba;
}

/**
 * @param {string} color
 * @param {number=} alpha 0 ~ 1
 * @return {string} Color string in rgba format.
 * @memberOf module:zrender/util/color
 */
function modifyAlpha(color, alpha) {
    color = parse(color);

    if (color && alpha != null) {
        color[3] = clampCssFloat(alpha);
        return stringify(color, 'rgba');
    }
}

/**
 * @param {Array.<number>} arrColor like [12,33,44,0.4]
 * @param {string} type 'rgba', 'hsva', ...
 * @return {string} Result color. (If input illegal, return undefined).
 */
function stringify(arrColor, type) {
    if (!arrColor || !arrColor.length) {
        return;
    }
    var colorStr = arrColor[0] + ',' + arrColor[1] + ',' + arrColor[2];
    if (type === 'rgba' || type === 'hsva' || type === 'hsla') {
        colorStr += ',' + arrColor[3];
    }
    return type + '(' + colorStr + ')';
}

/**
 * @module echarts/animation/Animator
 */

var arraySlice = Array.prototype.slice;

function defaultGetter(target, key) {
    return target[key];
}

function defaultSetter(target, key, value) {
    target[key] = value;
}

/**
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} percent
 * @return {number}
 */
function interpolateNumber(p0, p1, percent) {
    return (p1 - p0) * percent + p0;
}

/**
 * @param  {string} p0
 * @param  {string} p1
 * @param  {number} percent
 * @return {string}
 */
function interpolateString(p0, p1, percent) {
    return percent > 0.5 ? p1 : p0;
}

/**
 * @param  {Array} p0
 * @param  {Array} p1
 * @param  {number} percent
 * @param  {Array} out
 * @param  {number} arrDim
 */
function interpolateArray(p0, p1, percent, out, arrDim) {
    var len = p0.length;
    if (arrDim == 1) {
        for (var i = 0; i < len; i++) {
            out[i] = interpolateNumber(p0[i], p1[i], percent);
        }
    }
    else {
        var len2 = len && p0[0].length;
        for (var i = 0; i < len; i++) {
            for (var j = 0; j < len2; j++) {
                out[i][j] = interpolateNumber(
                    p0[i][j], p1[i][j], percent
                );
            }
        }
    }
}

// arr0 is source array, arr1 is target array.
// Do some preprocess to avoid error happened when interpolating from arr0 to arr1
function fillArr(arr0, arr1, arrDim) {
    var arr0Len = arr0.length;
    var arr1Len = arr1.length;
    if (arr0Len !== arr1Len) {
        // FIXME Not work for TypedArray
        var isPreviousLarger = arr0Len > arr1Len;
        if (isPreviousLarger) {
            // Cut the previous
            arr0.length = arr1Len;
        }
        else {
            // Fill the previous
            for (var i = arr0Len; i < arr1Len; i++) {
                arr0.push(
                    arrDim === 1 ? arr1[i] : arraySlice.call(arr1[i])
                );
            }
        }
    }
    // Handling NaN value
    var len2 = arr0[0] && arr0[0].length;
    for (var i = 0; i < arr0.length; i++) {
        if (arrDim === 1) {
            if (isNaN(arr0[i])) {
                arr0[i] = arr1[i];
            }
        }
        else {
            for (var j = 0; j < len2; j++) {
                if (isNaN(arr0[i][j])) {
                    arr0[i][j] = arr1[i][j];
                }
            }
        }
    }
}

/**
 * @param  {Array} arr0
 * @param  {Array} arr1
 * @param  {number} arrDim
 * @return {boolean}
 */
function isArraySame(arr0, arr1, arrDim) {
    if (arr0 === arr1) {
        return true;
    }
    var len = arr0.length;
    if (len !== arr1.length) {
        return false;
    }
    if (arrDim === 1) {
        for (var i = 0; i < len; i++) {
            if (arr0[i] !== arr1[i]) {
                return false;
            }
        }
    }
    else {
        var len2 = arr0[0].length;
        for (var i = 0; i < len; i++) {
            for (var j = 0; j < len2; j++) {
                if (arr0[i][j] !== arr1[i][j]) {
                    return false;
                }
            }
        }
    }
    return true;
}

/**
 * Catmull Rom interpolate array
 * @param  {Array} p0
 * @param  {Array} p1
 * @param  {Array} p2
 * @param  {Array} p3
 * @param  {number} t
 * @param  {number} t2
 * @param  {number} t3
 * @param  {Array} out
 * @param  {number} arrDim
 */
function catmullRomInterpolateArray(
    p0, p1, p2, p3, t, t2, t3, out, arrDim
) {
    var len = p0.length;
    if (arrDim == 1) {
        for (var i = 0; i < len; i++) {
            out[i] = catmullRomInterpolate(
                p0[i], p1[i], p2[i], p3[i], t, t2, t3
            );
        }
    }
    else {
        var len2 = p0[0].length;
        for (var i = 0; i < len; i++) {
            for (var j = 0; j < len2; j++) {
                out[i][j] = catmullRomInterpolate(
                    p0[i][j], p1[i][j], p2[i][j], p3[i][j],
                    t, t2, t3
                );
            }
        }
    }
}

/**
 * Catmull Rom interpolate number
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} p2
 * @param  {number} p3
 * @param  {number} t
 * @param  {number} t2
 * @param  {number} t3
 * @return {number}
 */
function catmullRomInterpolate(p0, p1, p2, p3, t, t2, t3) {
    var v0 = (p2 - p0) * 0.5;
    var v1 = (p3 - p1) * 0.5;
    return (2 * (p1 - p2) + v0 + v1) * t3
            + (-3 * (p1 - p2) - 2 * v0 - v1) * t2
            + v0 * t + p1;
}

function cloneValue(value) {
    if (isArrayLike(value)) {
        var len = value.length;
        if (isArrayLike(value[0])) {
            var ret = [];
            for (var i = 0; i < len; i++) {
                ret.push(arraySlice.call(value[i]));
            }
            return ret;
        }

        return arraySlice.call(value);
    }

    return value;
}

function rgba2String(rgba) {
    rgba[0] = Math.floor(rgba[0]);
    rgba[1] = Math.floor(rgba[1]);
    rgba[2] = Math.floor(rgba[2]);

    return 'rgba(' + rgba.join(',') + ')';
}

function getArrayDim(keyframes) {
    var lastValue = keyframes[keyframes.length - 1].value;
    return isArrayLike(lastValue && lastValue[0]) ? 2 : 1;
}

function createTrackClip(animator, easing, oneTrackDone, keyframes, propName, forceAnimate) {
    var getter = animator._getter;
    var setter = animator._setter;
    var useSpline = easing === 'spline';

    var trackLen = keyframes.length;
    if (!trackLen) {
        return;
    }
    // Guess data type
    var firstVal = keyframes[0].value;
    var isValueArray = isArrayLike(firstVal);
    var isValueColor = false;
    var isValueString = false;

    // For vertices morphing
    var arrDim = isValueArray ? getArrayDim(keyframes) : 0;

    var trackMaxTime;
    // Sort keyframe as ascending
    keyframes.sort(function(a, b) {
        return a.time - b.time;
    });

    trackMaxTime = keyframes[trackLen - 1].time;
    // Percents of each keyframe
    var kfPercents = [];
    // Value of each keyframe
    var kfValues = [];
    var prevValue = keyframes[0].value;
    var isAllValueEqual = true;
    for (var i = 0; i < trackLen; i++) {
        kfPercents.push(keyframes[i].time / trackMaxTime);
        // Assume value is a color when it is a string
        var value = keyframes[i].value;

        // Check if value is equal, deep check if value is array
        if (!((isValueArray && isArraySame(value, prevValue, arrDim))
            || (!isValueArray && value === prevValue))) {
            isAllValueEqual = false;
        }
        prevValue = value;

        // Try converting a string to a color array
        if (typeof value == 'string') {
            var colorArray = parse(value);
            if (colorArray) {
                value = colorArray;
                isValueColor = true;
            }
            else {
                isValueString = true;
            }
        }
        kfValues.push(value);
    }
    if (!forceAnimate && isAllValueEqual) {
        return;
    }

    var lastValue = kfValues[trackLen - 1];
    // Polyfill array and NaN value
    for (var i = 0; i < trackLen - 1; i++) {
        if (isValueArray) {
            fillArr(kfValues[i], lastValue, arrDim);
        }
        else {
            if (isNaN(kfValues[i]) && !isNaN(lastValue) && !isValueString && !isValueColor) {
                kfValues[i] = lastValue;
            }
        }
    }
    isValueArray && fillArr(getter(animator._target, propName), lastValue, arrDim);

    // Cache the key of last frame to speed up when
    // animation playback is sequency
    var lastFrame = 0;
    var lastFramePercent = 0;
    var start;
    var w;
    var p0;
    var p1;
    var p2;
    var p3;

    if (isValueColor) {
        var rgba = [0, 0, 0, 0];
    }

    var onframe = function (target, percent) {
        // Find the range keyframes
        // kf1-----kf2---------current--------kf3
        // find kf2 and kf3 and do interpolation
        var frame;
        // In the easing function like elasticOut, percent may less than 0
        if (percent < 0) {
            frame = 0;
        }
        else if (percent < lastFramePercent) {
            // Start from next key
            // PENDING start from lastFrame ?
            start = Math.min(lastFrame + 1, trackLen - 1);
            for (frame = start; frame >= 0; frame--) {
                if (kfPercents[frame] <= percent) {
                    break;
                }
            }
            // PENDING really need to do this ?
            frame = Math.min(frame, trackLen - 2);
        }
        else {
            for (frame = lastFrame; frame < trackLen; frame++) {
                if (kfPercents[frame] > percent) {
                    break;
                }
            }
            frame = Math.min(frame - 1, trackLen - 2);
        }
        lastFrame = frame;
        lastFramePercent = percent;

        var range = (kfPercents[frame + 1] - kfPercents[frame]);
        if (range === 0) {
            return;
        }
        else {
            w = (percent - kfPercents[frame]) / range;
        }
        if (useSpline) {
            p1 = kfValues[frame];
            p0 = kfValues[frame === 0 ? frame : frame - 1];
            p2 = kfValues[frame > trackLen - 2 ? trackLen - 1 : frame + 1];
            p3 = kfValues[frame > trackLen - 3 ? trackLen - 1 : frame + 2];
            if (isValueArray) {
                catmullRomInterpolateArray(
                    p0, p1, p2, p3, w, w * w, w * w * w,
                    getter(target, propName),
                    arrDim
                );
            }
            else {
                var value;
                if (isValueColor) {
                    value = catmullRomInterpolateArray(
                        p0, p1, p2, p3, w, w * w, w * w * w,
                        rgba, 1
                    );
                    value = rgba2String(rgba);
                }
                else if (isValueString) {
                    // String is step(0.5)
                    return interpolateString(p1, p2, w);
                }
                else {
                    value = catmullRomInterpolate(
                        p0, p1, p2, p3, w, w * w, w * w * w
                    );
                }
                setter(
                    target,
                    propName,
                    value
                );
            }
        }
        else {
            if (isValueArray) {
                interpolateArray(
                    kfValues[frame], kfValues[frame + 1], w,
                    getter(target, propName),
                    arrDim
                );
            }
            else {
                var value;
                if (isValueColor) {
                    interpolateArray(
                        kfValues[frame], kfValues[frame + 1], w,
                        rgba, 1
                    );
                    value = rgba2String(rgba);
                }
                else if (isValueString) {
                    // String is step(0.5)
                    return interpolateString(kfValues[frame], kfValues[frame + 1], w);
                }
                else {
                    value = interpolateNumber(kfValues[frame], kfValues[frame + 1], w);
                }
                setter(
                    target,
                    propName,
                    value
                );
            }
        }
    };

    var clip = new Clip({
        target: animator._target,
        life: trackMaxTime,
        loop: animator._loop,
        delay: animator._delay,
        onframe: onframe,
        ondestroy: oneTrackDone
    });

    if (easing && easing !== 'spline') {
        clip.easing = easing;
    }

    return clip;
}

/**
 * @alias module:zrender/animation/Animator
 * @constructor
 * @param {Object} target
 * @param {boolean} loop
 * @param {Function} getter
 * @param {Function} setter
 */
var Animator = function(target, loop, getter, setter) {
    this._tracks = {};
    this._target = target;

    this._loop = loop || false;

    this._getter = getter || defaultGetter;
    this._setter = setter || defaultSetter;

    this._clipCount = 0;

    this._delay = 0;

    this._doneList = [];

    this._onframeList = [];

    this._clipList = [];
};

Animator.prototype = {
    /**
     * 设置动画关键帧
     * @param  {number} time 关键帧时间，单位是ms
     * @param  {Object} props 关键帧的属性值，key-value表示
     * @return {module:zrender/animation/Animator}
     */
    when: function(time /* ms */, props) {
        var this$1 = this;

        var tracks = this._tracks;
        for (var propName in props) {
            if (!props.hasOwnProperty(propName)) {
                continue;
            }

            if (!tracks[propName]) {
                tracks[propName] = [];
                // Invalid value
                var value = this$1._getter(this$1._target, propName);
                if (value == null) {
                    // zrLog('Invalid property ' + propName);
                    continue;
                }
                // If time is 0
                //  Then props is given initialize value
                // Else
                //  Initialize value from current prop value
                if (time !== 0) {
                    tracks[propName].push({
                        time: 0,
                        value: cloneValue(value)
                    });
                }
            }
            tracks[propName].push({
                time: time,
                value: props[propName]
            });
        }
        return this;
    },
    /**
     * 添加动画每一帧的回调函数
     * @param  {Function} callback
     * @return {module:zrender/animation/Animator}
     */
    during: function (callback) {
        this._onframeList.push(callback);
        return this;
    },

    pause: function () {
        var this$1 = this;

        for (var i = 0; i < this._clipList.length; i++) {
            this$1._clipList[i].pause();
        }
        this._paused = true;
    },

    resume: function () {
        var this$1 = this;

        for (var i = 0; i < this._clipList.length; i++) {
            this$1._clipList[i].resume();
        }
        this._paused = false;
    },

    isPaused: function () {
        return !!this._paused;
    },

    _doneCallback: function () {
        var this$1 = this;

        // Clear all tracks
        this._tracks = {};
        // Clear all clips
        this._clipList.length = 0;

        var doneList = this._doneList;
        var len = doneList.length;
        for (var i = 0; i < len; i++) {
            doneList[i].call(this$1);
        }
    },
    /**
     * 开始执行动画
     * @param  {string|Function} [easing]
     *         动画缓动函数，详见{@link module:zrender/animation/easing}
     * @param  {boolean} forceAnimate
     * @return {module:zrender/animation/Animator}
     */
    start: function (easing, forceAnimate) {
        var this$1 = this;


        var self = this;
        var clipCount = 0;

        var oneTrackDone = function() {
            clipCount--;
            if (!clipCount) {
                self._doneCallback();
            }
        };

        var lastClip;
        for (var propName in this$1._tracks) {
            if (!this$1._tracks.hasOwnProperty(propName)) {
                continue;
            }
            var clip = createTrackClip(
                this$1, easing, oneTrackDone,
                this$1._tracks[propName], propName, forceAnimate
            );
            if (clip) {
                this$1._clipList.push(clip);
                clipCount++;

                // If start after added to animation
                if (this$1.animation) {
                    this$1.animation.addClip(clip);
                }

                lastClip = clip;
            }
        }

        // Add during callback on the last clip
        if (lastClip) {
            var oldOnFrame = lastClip.onframe;
            lastClip.onframe = function (target, percent) {
                oldOnFrame(target, percent);

                for (var i = 0; i < self._onframeList.length; i++) {
                    self._onframeList[i](target, percent);
                }
            };
        }

        // This optimization will help the case that in the upper application
        // the view may be refreshed frequently, where animation will be
        // called repeatly but nothing changed.
        if (!clipCount) {
            this._doneCallback();
        }
        return this;
    },
    /**
     * 停止动画
     * @param {boolean} forwardToLast If move to last frame before stop
     */
    stop: function (forwardToLast) {
        var this$1 = this;

        var clipList = this._clipList;
        var animation = this.animation;
        for (var i = 0; i < clipList.length; i++) {
            var clip = clipList[i];
            if (forwardToLast) {
                // Move to last frame before stop
                clip.onframe(this$1._target, 1);
            }
            animation && animation.removeClip(clip);
        }
        clipList.length = 0;
    },
    /**
     * 设置动画延迟开始的时间
     * @param  {number} time 单位ms
     * @return {module:zrender/animation/Animator}
     */
    delay: function (time) {
        this._delay = time;
        return this;
    },
    /**
     * 添加动画结束的回调
     * @param  {Function} cb
     * @return {module:zrender/animation/Animator}
     */
    done: function(cb) {
        if (cb) {
            this._doneList.push(cb);
        }
        return this;
    },

    /**
     * @return {Array.<module:zrender/animation/Clip>}
     */
    getClips: function () {
        return this._clipList;
    }
};

var dpr = 1;

// If in browser environment
if (typeof window !== 'undefined') {
    dpr = Math.max(window.devicePixelRatio || 1, 1);
}

/**
 * config默认配置项
 * @exports zrender/config
 * @author Kener (@Kener-林峰, kener.linfeng@gmail.com)
 */

/**
 * debug日志选项：catchBrushException为true下有效
 * 0 : 不生成debug数据，发布用
 * 1 : 异常抛出，调试用
 * 2 : 控制台输出，调试用
 */
var debugMode = 0;

// retina 屏幕优化
var devicePixelRatio = dpr;

var log = function () {
};

if (debugMode === 1) {
    log = function () {
        var arguments$1 = arguments;

        for (var k in arguments) {
            throw new Error(arguments$1[k]);
        }
    };
}
else if (debugMode > 1) {
    log = function () {
        var arguments$1 = arguments;

        for (var k in arguments) {
            console.log(arguments$1[k]);
        }
    };
}

var log$1 = log;

/**
 * @alias modue:zrender/mixin/Animatable
 * @constructor
 */
var Animatable = function () {

    /**
     * @type {Array.<module:zrender/animation/Animator>}
     * @readOnly
     */
    this.animators = [];
};

Animatable.prototype = {

    constructor: Animatable,

    /**
     * 动画
     *
     * @param {string} path The path to fetch value from object, like 'a.b.c'.
     * @param {boolean} [loop] Whether to loop animation.
     * @return {module:zrender/animation/Animator}
     * @example:
     *     el.animate('style', false)
     *         .when(1000, {x: 10} )
     *         .done(function(){ // Animation done })
     *         .start()
     */
    animate: function (path, loop) {
        var target;
        var animatingShape = false;
        var el = this;
        var zr = this.__zr;
        if (path) {
            var pathSplitted = path.split('.');
            var prop = el;
            // If animating shape
            animatingShape = pathSplitted[0] === 'shape';
            for (var i = 0, l = pathSplitted.length; i < l; i++) {
                if (!prop) {
                    continue;
                }
                prop = prop[pathSplitted[i]];
            }
            if (prop) {
                target = prop;
            }
        }
        else {
            target = el;
        }

        if (!target) {
            log$1(
                'Property "'
                + path
                + '" is not existed in element '
                + el.id
            );
            return;
        }

        var animators = el.animators;

        var animator = new Animator(target, loop);

        animator.during(function (target) {
            el.dirty(animatingShape);
        })
        .done(function () {
            // FIXME Animator will not be removed if use `Animator#stop` to stop animation
            animators.splice(indexOf$1(animators, animator), 1);
        });

        animators.push(animator);

        // If animate after added to the zrender
        if (zr) {
            zr.animation.addAnimator(animator);
        }

        return animator;
    },

    /**
     * 停止动画
     * @param {boolean} forwardToLast If move to last frame before stop
     */
    stopAnimation: function (forwardToLast) {
        var animators = this.animators;
        var len = animators.length;
        for (var i = 0; i < len; i++) {
            animators[i].stop(forwardToLast);
        }
        animators.length = 0;

        return this;
    },

    /**
     * Caution: this method will stop previous animation.
     * So do not use this method to one element twice before
     * animation starts, unless you know what you are doing.
     * @param {Object} target
     * @param {number} [time=500] Time in ms
     * @param {string} [easing='linear']
     * @param {number} [delay=0]
     * @param {Function} [callback]
     * @param {Function} [forceAnimate] Prevent stop animation and callback
     *        immediently when target values are the same as current values.
     *
     * @example
     *  // Animate position
     *  el.animateTo({
     *      position: [10, 10]
     *  }, function () { // done })
     *
     *  // Animate shape, style and position in 100ms, delayed 100ms, with cubicOut easing
     *  el.animateTo({
     *      shape: {
     *          width: 500
     *      },
     *      style: {
     *          fill: 'red'
     *      }
     *      position: [10, 10]
     *  }, 100, 100, 'cubicOut', function () { // done })
     */
        // TODO Return animation key
    animateTo: function (target, time, delay, easing, callback, forceAnimate) {
        // animateTo(target, time, easing, callback);
        if (isString$1(delay)) {
            callback = easing;
            easing = delay;
            delay = 0;
        }
        // animateTo(target, time, delay, callback);
        else if (isFunction$1(easing)) {
            callback = easing;
            easing = 'linear';
            delay = 0;
        }
        // animateTo(target, time, callback);
        else if (isFunction$1(delay)) {
            callback = delay;
            delay = 0;
        }
        // animateTo(target, callback)
        else if (isFunction$1(time)) {
            callback = time;
            time = 500;
        }
        // animateTo(target)
        else if (!time) {
            time = 500;
        }
        // Stop all previous animations
        this.stopAnimation();
        this._animateToShallow('', this, target, time, delay);

        // Animators may be removed immediately after start
        // if there is nothing to animate
        var animators = this.animators.slice();
        var count = animators.length;
        function done() {
            count--;
            if (!count) {
                callback && callback();
            }
        }

        // No animators. This should be checked before animators[i].start(),
        // because 'done' may be executed immediately if no need to animate.
        if (!count) {
            callback && callback();
        }
        // Start after all animators created
        // Incase any animator is done immediately when all animation properties are not changed
        for (var i = 0; i < animators.length; i++) {
            animators[i]
                .done(done)
                .start(easing, forceAnimate);
        }
    },

    /**
     * @private
     * @param {string} path=''
     * @param {Object} source=this
     * @param {Object} target
     * @param {number} [time=500]
     * @param {number} [delay=0]
     *
     * @example
     *  // Animate position
     *  el._animateToShallow({
     *      position: [10, 10]
     *  })
     *
     *  // Animate shape, style and position in 100ms, delayed 100ms
     *  el._animateToShallow({
     *      shape: {
     *          width: 500
     *      },
     *      style: {
     *          fill: 'red'
     *      }
     *      position: [10, 10]
     *  }, 100, 100)
     */
    _animateToShallow: function (path, source, target, time, delay) {
        var this$1 = this;

        var objShallow = {};
        var propertyCount = 0;
        for (var name in target) {
            if (!target.hasOwnProperty(name)) {
                continue;
            }

            if (source[name] != null) {
                if (isObject$1(target[name]) && !isArrayLike(target[name])) {
                    this$1._animateToShallow(
                        path ? path + '.' + name : name,
                        source[name],
                        target[name],
                        time,
                        delay
                    );
                }
                else {
                    objShallow[name] = target[name];
                    propertyCount++;
                }
            }
            else if (target[name] != null) {
                // Attr directly if not has property
                // FIXME, if some property not needed for element ?
                if (!path) {
                    this$1.attr(name, target[name]);
                }
                else {  // Shape or style
                    var props = {};
                    props[path] = {};
                    props[path][name] = target[name];
                    this$1.attr(props);
                }
            }
        }

        if (propertyCount > 0) {
            this.animate(path, false)
                .when(time == null ? 500 : time, objShallow)
                .delay(delay || 0);
        }

        return this;
    }
};

/**
 * @alias module:zrender/Element
 * @constructor
 * @extends {module:zrender/mixin/Animatable}
 * @extends {module:zrender/mixin/Transformable}
 * @extends {module:zrender/mixin/Eventful}
 */
var Element = function (opts) { // jshint ignore:line

    Transformable.call(this, opts);
    Eventful.call(this, opts);
    Animatable.call(this, opts);

    /**
     * 画布元素ID
     * @type {string}
     */
    this.id = opts.id || guid();
};

Element.prototype = {

    /**
     * 元素类型
     * Element type
     * @type {string}
     */
    type: 'element',

    /**
     * 元素名字
     * Element name
     * @type {string}
     */
    name: '',

    /**
     * ZRender 实例对象，会在 element 添加到 zrender 实例中后自动赋值
     * ZRender instance will be assigned when element is associated with zrender
     * @name module:/zrender/Element#__zr
     * @type {module:zrender/ZRender}
     */
    __zr: null,

    /**
     * 图形是否忽略，为true时忽略图形的绘制以及事件触发
     * If ignore drawing and events of the element object
     * @name module:/zrender/Element#ignore
     * @type {boolean}
     * @default false
     */
    ignore: false,

    /**
     * 用于裁剪的路径(shape)，所有 Group 内的路径在绘制时都会被这个路径裁剪
     * 该路径会继承被裁减对象的变换
     * @type {module:zrender/graphic/Path}
     * @see http://www.w3.org/TR/2dcontext/#clipping-region
     * @readOnly
     */
    clipPath: null,

    /**
     * Drift element
     * @param  {number} dx dx on the global space
     * @param  {number} dy dy on the global space
     */
    drift: function (dx, dy) {
        switch (this.draggable) {
            case 'horizontal':
                dy = 0;
                break;
            case 'vertical':
                dx = 0;
                break;
        }

        var m = this.transform;
        if (!m) {
            m = this.transform = [1, 0, 0, 1, 0, 0];
        }
        m[4] += dx;
        m[5] += dy;

        this.decomposeTransform();
        this.dirty(false);
    },

    /**
     * Hook before update
     */
    beforeUpdate: function () {},
    /**
     * Hook after update
     */
    afterUpdate: function () {},
    /**
     * Update each frame
     */
    update: function () {
        this.updateTransform();
    },

    /**
     * @param  {Function} cb
     * @param  {}   context
     */
    traverse: function (cb, context) {},

    /**
     * @protected
     */
    attrKV: function (key, value) {
        if (key === 'position' || key === 'scale' || key === 'origin') {
            // Copy the array
            if (value) {
                var target = this[key];
                if (!target) {
                    target = this[key] = [];
                }
                target[0] = value[0];
                target[1] = value[1];
            }
        }
        else {
            this[key] = value;
        }
    },

    /**
     * Hide the element
     */
    hide: function () {
        this.ignore = true;
        this.__zr && this.__zr.refresh();
    },

    /**
     * Show the element
     */
    show: function () {
        this.ignore = false;
        this.__zr && this.__zr.refresh();
    },

    /**
     * @param {string|Object} key
     * @param {*} value
     */
    attr: function (key, value) {
        var this$1 = this;

        if (typeof key === 'string') {
            this.attrKV(key, value);
        }
        else if (isObject$1(key)) {
            for (var name in key) {
                if (key.hasOwnProperty(name)) {
                    this$1.attrKV(name, key[name]);
                }
            }
        }

        this.dirty(false);

        return this;
    },

    /**
     * @param {module:zrender/graphic/Path} clipPath
     */
    setClipPath: function (clipPath) {
        var zr = this.__zr;
        if (zr) {
            clipPath.addSelfToZr(zr);
        }

        // Remove previous clip path
        if (this.clipPath && this.clipPath !== clipPath) {
            this.removeClipPath();
        }

        this.clipPath = clipPath;
        clipPath.__zr = zr;
        clipPath.__clipTarget = this;

        this.dirty(false);
    },

    /**
     */
    removeClipPath: function () {
        var clipPath = this.clipPath;
        if (clipPath) {
            if (clipPath.__zr) {
                clipPath.removeSelfFromZr(clipPath.__zr);
            }

            clipPath.__zr = null;
            clipPath.__clipTarget = null;
            this.clipPath = null;

            this.dirty(false);
        }
    },

    /**
     * Add self from zrender instance.
     * Not recursively because it will be invoked when element added to storage.
     * @param {module:zrender/ZRender} zr
     */
    addSelfToZr: function (zr) {
        this.__zr = zr;
        // 添加动画
        var animators = this.animators;
        if (animators) {
            for (var i = 0; i < animators.length; i++) {
                zr.animation.addAnimator(animators[i]);
            }
        }

        if (this.clipPath) {
            this.clipPath.addSelfToZr(zr);
        }
    },

    /**
     * Remove self from zrender instance.
     * Not recursively because it will be invoked when element added to storage.
     * @param {module:zrender/ZRender} zr
     */
    removeSelfFromZr: function (zr) {
        this.__zr = null;
        // 移除动画
        var animators = this.animators;
        if (animators) {
            for (var i = 0; i < animators.length; i++) {
                zr.animation.removeAnimator(animators[i]);
            }
        }

        if (this.clipPath) {
            this.clipPath.removeSelfFromZr(zr);
        }
    }
};

mixin(Element, Animatable);
mixin(Element, Transformable);
mixin(Element, Eventful);

/**
 * @module echarts/core/BoundingRect
 */

var v2ApplyTransform = applyTransform;
var mathMin = Math.min;
var mathMax = Math.max;

/**
 * @alias module:echarts/core/BoundingRect
 */
function BoundingRect(x, y, width, height) {

    if (width < 0) {
        x = x + width;
        width = -width;
    }
    if (height < 0) {
        y = y + height;
        height = -height;
    }

    /**
     * @type {number}
     */
    this.x = x;
    /**
     * @type {number}
     */
    this.y = y;
    /**
     * @type {number}
     */
    this.width = width;
    /**
     * @type {number}
     */
    this.height = height;
}

BoundingRect.prototype = {

    constructor: BoundingRect,

    /**
     * @param {module:echarts/core/BoundingRect} other
     */
    union: function (other) {
        var x = mathMin(other.x, this.x);
        var y = mathMin(other.y, this.y);

        this.width = mathMax(
                other.x + other.width,
                this.x + this.width
            ) - x;
        this.height = mathMax(
                other.y + other.height,
                this.y + this.height
            ) - y;
        this.x = x;
        this.y = y;
    },

    /**
     * @param {Array.<number>} m
     * @methods
     */
    applyTransform: (function () {
        var lt = [];
        var rb = [];
        var lb = [];
        var rt = [];
        return function (m) {
            // In case usage like this
            // el.getBoundingRect().applyTransform(el.transform)
            // And element has no transform
            if (!m) {
                return;
            }
            lt[0] = lb[0] = this.x;
            lt[1] = rt[1] = this.y;
            rb[0] = rt[0] = this.x + this.width;
            rb[1] = lb[1] = this.y + this.height;

            v2ApplyTransform(lt, lt, m);
            v2ApplyTransform(rb, rb, m);
            v2ApplyTransform(lb, lb, m);
            v2ApplyTransform(rt, rt, m);

            this.x = mathMin(lt[0], rb[0], lb[0], rt[0]);
            this.y = mathMin(lt[1], rb[1], lb[1], rt[1]);
            var maxX = mathMax(lt[0], rb[0], lb[0], rt[0]);
            var maxY = mathMax(lt[1], rb[1], lb[1], rt[1]);
            this.width = maxX - this.x;
            this.height = maxY - this.y;
        };
    })(),

    /**
     * Calculate matrix of transforming from self to target rect
     * @param  {module:zrender/core/BoundingRect} b
     * @return {Array.<number>}
     */
    calculateTransform: function (b) {
        var a = this;
        var sx = b.width / a.width;
        var sy = b.height / a.height;

        var m = create$1();

        // 矩阵右乘
        translate(m, m, [-a.x, -a.y]);
        scale(m, m, [sx, sy]);
        translate(m, m, [b.x, b.y]);

        return m;
    },

    /**
     * @param {(module:echarts/core/BoundingRect|Object)} b
     * @return {boolean}
     */
    intersect: function (b) {
        if (!b) {
            return false;
        }

        if (!(b instanceof BoundingRect)) {
            // Normalize negative width/height.
            b = BoundingRect.create(b);
        }

        var a = this;
        var ax0 = a.x;
        var ax1 = a.x + a.width;
        var ay0 = a.y;
        var ay1 = a.y + a.height;

        var bx0 = b.x;
        var bx1 = b.x + b.width;
        var by0 = b.y;
        var by1 = b.y + b.height;

        return ! (ax1 < bx0 || bx1 < ax0 || ay1 < by0 || by1 < ay0);
    },

    contain: function (x, y) {
        var rect = this;
        return x >= rect.x
            && x <= (rect.x + rect.width)
            && y >= rect.y
            && y <= (rect.y + rect.height);
    },

    /**
     * @return {module:echarts/core/BoundingRect}
     */
    clone: function () {
        return new BoundingRect(this.x, this.y, this.width, this.height);
    },

    /**
     * Copy from another rect
     */
    copy: function (other) {
        this.x = other.x;
        this.y = other.y;
        this.width = other.width;
        this.height = other.height;
    },

    plain: function () {
        return {
            x: this.x,
            y: this.y,
            width: this.width,
            height: this.height
        };
    }
};

/**
 * @param {Object|module:zrender/core/BoundingRect} rect
 * @param {number} rect.x
 * @param {number} rect.y
 * @param {number} rect.width
 * @param {number} rect.height
 * @return {module:zrender/core/BoundingRect}
 */
BoundingRect.create = function (rect) {
    return new BoundingRect(rect.x, rect.y, rect.width, rect.height);
};

var globalImageCache = new LRU(50);

/**
 * @param {string|HTMLImageElement|HTMLCanvasElement|Canvas} newImageOrSrc
 * @return {HTMLImageElement|HTMLCanvasElement|Canvas} image
 */
function findExistImage(newImageOrSrc) {
    if (typeof newImageOrSrc === 'string') {
        var cachedImgObj = globalImageCache.get(newImageOrSrc);
        return cachedImgObj && cachedImgObj.image;
    }
    else {
        return newImageOrSrc;
    }
}

/**
 * Caution: User should cache loaded images, but not just count on LRU.
 * Consider if required images more than LRU size, will dead loop occur?
 *
 * @param {string|HTMLImageElement|HTMLCanvasElement|Canvas} newImageOrSrc
 * @param {HTMLImageElement|HTMLCanvasElement|Canvas} image Existent image.
 * @param {module:zrender/Element} [hostEl] For calling `dirty`.
 * @param {Function} [cb] params: (image, cbPayload)
 * @param {Object} [cbPayload] Payload on cb calling.
 * @return {HTMLImageElement|HTMLCanvasElement|Canvas} image
 */
function createOrUpdateImage(newImageOrSrc, image, hostEl, cb, cbPayload) {
    if (!newImageOrSrc) {
        return image;
    }
    else if (typeof newImageOrSrc === 'string') {

        // Image should not be loaded repeatly.
        if ((image && image.__zrImageSrc === newImageOrSrc) || !hostEl) {
            return image;
        }

        // Only when there is no existent image or existent image src
        // is different, this method is responsible for load.
        var cachedImgObj = globalImageCache.get(newImageOrSrc);

        var pendingWrap = {hostEl: hostEl, cb: cb, cbPayload: cbPayload};

        if (cachedImgObj) {
            image = cachedImgObj.image;
            !isImageReady(image) && cachedImgObj.pending.push(pendingWrap);
        }
        else {

            if(!image || globalImageCache.get(image.__zrImageSrc)){
                image = new Image();
            }

            image.onload = imageOnLoad;

            globalImageCache.put(
                newImageOrSrc,
                image.__cachedImgObj = {
                    image: image,
                    pending: [pendingWrap]
                }
            );

            image.src = image.__zrImageSrc = newImageOrSrc;
        }

        return image;
    }
    // newImageOrSrc is an HTMLImageElement or HTMLCanvasElement or Canvas
    else {
        return newImageOrSrc;
    }
}

function imageOnLoad() {
    var this$1 = this;

    var cachedImgObj = this.__cachedImgObj;
    this.onload = this.__cachedImgObj = null;

    for (var i = 0; i < cachedImgObj.pending.length; i++) {
        var pendingWrap = cachedImgObj.pending[i];
        var cb = pendingWrap.cb;
        cb && cb(this$1, pendingWrap.cbPayload);
        pendingWrap.hostEl.dirty();
    }
    cachedImgObj.pending.length = 0;
}

function isImageReady(image) {
    return image && image.width && image.height;
}

var textWidthCache = {};
var textWidthCacheCounter = 0;

var TEXT_CACHE_MAX = 5000;
var STYLE_REG = /\{([a-zA-Z0-9_]+)\|([^}]*)\}/g;

var DEFAULT_FONT = '12px sans-serif';

// Avoid assign to an exported variable, for transforming to cjs.
var methods$1 = {};

/**
 * @public
 * @param {string} text
 * @param {string} font
 * @return {number} width
 */
function getWidth(text, font) {
    font = font || DEFAULT_FONT;
    var key = text + ':' + font;
    if (textWidthCache[key]) {
        return textWidthCache[key];
    }

    var textLines = (text + '').split('\n');
    var width = 0;

    for (var i = 0, l = textLines.length; i < l; i++) {
        // textContain.measureText may be overrided in SVG or VML
        width = Math.max(measureText(textLines[i], font).width, width);
    }

    if (textWidthCacheCounter > TEXT_CACHE_MAX) {
        textWidthCacheCounter = 0;
        textWidthCache = {};
    }
    textWidthCacheCounter++;
    textWidthCache[key] = width;

    return width;
}

/**
 * @public
 * @param {string} text
 * @param {string} font
 * @param {string} [textAlign='left']
 * @param {string} [textVerticalAlign='top']
 * @param {Array.<number>} [textPadding]
 * @param {Object} [rich]
 * @param {Object} [truncate]
 * @return {Object} {x, y, width, height, lineHeight}
 */
function getBoundingRect(text, font, textAlign, textVerticalAlign, textPadding, rich, truncate) {
    return rich
        ? getRichTextRect(text, font, textAlign, textVerticalAlign, textPadding, rich, truncate)
        : getPlainTextRect(text, font, textAlign, textVerticalAlign, textPadding, truncate);
}

function getPlainTextRect(text, font, textAlign, textVerticalAlign, textPadding, truncate) {
    var contentBlock = parsePlainText(text, font, textPadding, truncate);
    var outerWidth = getWidth(text, font);
    if (textPadding) {
        outerWidth += textPadding[1] + textPadding[3];
    }
    var outerHeight = contentBlock.outerHeight;

    var x = adjustTextX(0, outerWidth, textAlign);
    var y = adjustTextY(0, outerHeight, textVerticalAlign);

    var rect = new BoundingRect(x, y, outerWidth, outerHeight);
    rect.lineHeight = contentBlock.lineHeight;

    return rect;
}

function getRichTextRect(text, font, textAlign, textVerticalAlign, textPadding, rich, truncate) {
    var contentBlock = parseRichText(text, {
        rich: rich,
        truncate: truncate,
        font: font,
        textAlign: textAlign,
        textPadding: textPadding
    });
    var outerWidth = contentBlock.outerWidth;
    var outerHeight = contentBlock.outerHeight;

    var x = adjustTextX(0, outerWidth, textAlign);
    var y = adjustTextY(0, outerHeight, textVerticalAlign);

    return new BoundingRect(x, y, outerWidth, outerHeight);
}

/**
 * @public
 * @param {number} x
 * @param {number} width
 * @param {string} [textAlign='left']
 * @return {number} Adjusted x.
 */
function adjustTextX(x, width, textAlign) {
    // FIXME Right to left language
    if (textAlign === 'right') {
        x -= width;
    }
    else if (textAlign === 'center') {
        x -= width / 2;
    }
    return x;
}

/**
 * @public
 * @param {number} y
 * @param {number} height
 * @param {string} [textVerticalAlign='top']
 * @return {number} Adjusted y.
 */
function adjustTextY(y, height, textVerticalAlign) {
    if (textVerticalAlign === 'middle') {
        y -= height / 2;
    }
    else if (textVerticalAlign === 'bottom') {
        y -= height;
    }
    return y;
}

/**
 * @public
 * @param {stirng} textPosition
 * @param {Object} rect {x, y, width, height}
 * @param {number} distance
 * @return {Object} {x, y, textAlign, textVerticalAlign}
 */
function adjustTextPositionOnRect(textPosition, rect, distance) {

    var x = rect.x;
    var y = rect.y;

    var height = rect.height;
    var width = rect.width;
    var halfHeight = height / 2;

    var textAlign = 'left';
    var textVerticalAlign = 'top';

    switch (textPosition) {
        case 'left':
            x -= distance;
            y += halfHeight;
            textAlign = 'right';
            textVerticalAlign = 'middle';
            break;
        case 'right':
            x += distance + width;
            y += halfHeight;
            textVerticalAlign = 'middle';
            break;
        case 'top':
            x += width / 2;
            y -= distance;
            textAlign = 'center';
            textVerticalAlign = 'bottom';
            break;
        case 'bottom':
            x += width / 2;
            y += height + distance;
            textAlign = 'center';
            break;
        case 'inside':
            x += width / 2;
            y += halfHeight;
            textAlign = 'center';
            textVerticalAlign = 'middle';
            break;
        case 'insideLeft':
            x += distance;
            y += halfHeight;
            textVerticalAlign = 'middle';
            break;
        case 'insideRight':
            x += width - distance;
            y += halfHeight;
            textAlign = 'right';
            textVerticalAlign = 'middle';
            break;
        case 'insideTop':
            x += width / 2;
            y += distance;
            textAlign = 'center';
            break;
        case 'insideBottom':
            x += width / 2;
            y += height - distance;
            textAlign = 'center';
            textVerticalAlign = 'bottom';
            break;
        case 'insideTopLeft':
            x += distance;
            y += distance;
            break;
        case 'insideTopRight':
            x += width - distance;
            y += distance;
            textAlign = 'right';
            break;
        case 'insideBottomLeft':
            x += distance;
            y += height - distance;
            textVerticalAlign = 'bottom';
            break;
        case 'insideBottomRight':
            x += width - distance;
            y += height - distance;
            textAlign = 'right';
            textVerticalAlign = 'bottom';
            break;
    }

    return {
        x: x,
        y: y,
        textAlign: textAlign,
        textVerticalAlign: textVerticalAlign
    };
}

/**
 * Show ellipsis if overflow.
 *
 * @public
 * @param  {string} text
 * @param  {string} containerWidth
 * @param  {string} font
 * @param  {number} [ellipsis='...']
 * @param  {Object} [options]
 * @param  {number} [options.maxIterations=3]
 * @param  {number} [options.minChar=0] If truncate result are less
 *                  then minChar, ellipsis will not show, which is
 *                  better for user hint in some cases.
 * @param  {number} [options.placeholder=''] When all truncated, use the placeholder.
 * @return {string}
 */
function truncateText(text, containerWidth, font, ellipsis, options) {
    if (!containerWidth) {
        return '';
    }

    var textLines = (text + '').split('\n');
    options = prepareTruncateOptions(containerWidth, font, ellipsis, options);

    // FIXME
    // It is not appropriate that every line has '...' when truncate multiple lines.
    for (var i = 0, len = textLines.length; i < len; i++) {
        textLines[i] = truncateSingleLine(textLines[i], options);
    }

    return textLines.join('\n');
}

function prepareTruncateOptions(containerWidth, font, ellipsis, options) {
    options = extend$1({}, options);

    options.font = font;
    var ellipsis = retrieve2(ellipsis, '...');
    options.maxIterations = retrieve2(options.maxIterations, 2);
    var minChar = options.minChar = retrieve2(options.minChar, 0);
    // FIXME
    // Other languages?
    options.cnCharWidth = getWidth('国', font);
    // FIXME
    // Consider proportional font?
    var ascCharWidth = options.ascCharWidth = getWidth('a', font);
    options.placeholder = retrieve2(options.placeholder, '');

    // Example 1: minChar: 3, text: 'asdfzxcv', truncate result: 'asdf', but not: 'a...'.
    // Example 2: minChar: 3, text: '维度', truncate result: '维', but not: '...'.
    var contentWidth = containerWidth = Math.max(0, containerWidth - 1); // Reserve some gap.
    for (var i = 0; i < minChar && contentWidth >= ascCharWidth; i++) {
        contentWidth -= ascCharWidth;
    }

    var ellipsisWidth = getWidth(ellipsis);
    if (ellipsisWidth > contentWidth) {
        ellipsis = '';
        ellipsisWidth = 0;
    }

    contentWidth = containerWidth - ellipsisWidth;

    options.ellipsis = ellipsis;
    options.ellipsisWidth = ellipsisWidth;
    options.contentWidth = contentWidth;
    options.containerWidth = containerWidth;

    return options;
}

function truncateSingleLine(textLine, options) {
    var containerWidth = options.containerWidth;
    var font = options.font;
    var contentWidth = options.contentWidth;

    if (!containerWidth) {
        return '';
    }

    var lineWidth = getWidth(textLine, font);

    if (lineWidth <= containerWidth) {
        return textLine;
    }

    for (var j = 0;; j++) {
        if (lineWidth <= contentWidth || j >= options.maxIterations) {
            textLine += options.ellipsis;
            break;
        }

        var subLength = j === 0
            ? estimateLength(textLine, contentWidth, options.ascCharWidth, options.cnCharWidth)
            : lineWidth > 0
            ? Math.floor(textLine.length * contentWidth / lineWidth)
            : 0;

        textLine = textLine.substr(0, subLength);
        lineWidth = getWidth(textLine, font);
    }

    if (textLine === '') {
        textLine = options.placeholder;
    }

    return textLine;
}

function estimateLength(text, contentWidth, ascCharWidth, cnCharWidth) {
    var width = 0;
    var i = 0;
    for (var len = text.length; i < len && width < contentWidth; i++) {
        var charCode = text.charCodeAt(i);
        width += (0 <= charCode && charCode <= 127) ? ascCharWidth : cnCharWidth;
    }
    return i;
}

/**
 * @public
 * @param {string} font
 * @return {number} line height
 */
function getLineHeight(font) {
    // FIXME A rough approach.
    return getWidth('国', font);
}

/**
 * @public
 * @param {string} text
 * @param {string} font
 * @return {Object} width
 */
function measureText(text, font) {
    return methods$1.measureText(text, font);
}

// Avoid assign to an exported variable, for transforming to cjs.
methods$1.measureText = function (text, font) {
    var ctx = getContext();
    ctx.font = font || DEFAULT_FONT;
    return ctx.measureText(text);
};

/**
 * @public
 * @param {string} text
 * @param {string} font
 * @param {Object} [truncate]
 * @return {Object} block: {lineHeight, lines, height, outerHeight}
 *  Notice: for performance, do not calculate outerWidth util needed.
 */
function parsePlainText(text, font, padding, truncate) {
    text != null && (text += '');

    var lineHeight = getLineHeight(font);
    var lines = text ? text.split('\n') : [];
    var height = lines.length * lineHeight;
    var outerHeight = height;

    if (padding) {
        outerHeight += padding[0] + padding[2];
    }

    if (text && truncate) {
        var truncOuterHeight = truncate.outerHeight;
        var truncOuterWidth = truncate.outerWidth;
        if (truncOuterHeight != null && outerHeight > truncOuterHeight) {
            text = '';
            lines = [];
        }
        else if (truncOuterWidth != null) {
            var options = prepareTruncateOptions(
                truncOuterWidth - (padding ? padding[1] + padding[3] : 0),
                font,
                truncate.ellipsis,
                {minChar: truncate.minChar, placeholder: truncate.placeholder}
            );

            // FIXME
            // It is not appropriate that every line has '...' when truncate multiple lines.
            for (var i = 0, len = lines.length; i < len; i++) {
                lines[i] = truncateSingleLine(lines[i], options);
            }
        }
    }

    return {
        lines: lines,
        height: height,
        outerHeight: outerHeight,
        lineHeight: lineHeight
    };
}

/**
 * For example: 'some text {a|some text}other text{b|some text}xxx{c|}xxx'
 * Also consider 'bbbb{a|xxx\nzzz}xxxx\naaaa'.
 *
 * @public
 * @param {string} text
 * @param {Object} style
 * @return {Object} block
 * {
 *      width,
 *      height,
 *      lines: [{
 *          lineHeight,
 *          width,
 *          tokens: [[{
 *              styleName,
 *              text,
 *              width,      // include textPadding
 *              height,     // include textPadding
 *              textWidth, // pure text width
 *              textHeight, // pure text height
 *              lineHeihgt,
 *              font,
 *              textAlign,
 *              textVerticalAlign
 *          }], [...], ...]
 *      }, ...]
 * }
 * If styleName is undefined, it is plain text.
 */
function parseRichText(text, style) {
    var contentBlock = {lines: [], width: 0, height: 0};

    text != null && (text += '');
    if (!text) {
        return contentBlock;
    }

    var lastIndex = STYLE_REG.lastIndex = 0;
    var result;
    while ((result = STYLE_REG.exec(text)) != null) {
        var matchedIndex = result.index;
        if (matchedIndex > lastIndex) {
            pushTokens(contentBlock, text.substring(lastIndex, matchedIndex));
        }
        pushTokens(contentBlock, result[2], result[1]);
        lastIndex = STYLE_REG.lastIndex;
    }

    if (lastIndex < text.length) {
        pushTokens(contentBlock, text.substring(lastIndex, text.length));
    }

    var lines = contentBlock.lines;
    var contentHeight = 0;
    var contentWidth = 0;
    // For `textWidth: 100%`
    var pendingList = [];

    var stlPadding = style.textPadding;

    var truncate = style.truncate;
    var truncateWidth = truncate && truncate.outerWidth;
    var truncateHeight = truncate && truncate.outerHeight;
    if (stlPadding) {
        truncateWidth != null && (truncateWidth -= stlPadding[1] + stlPadding[3]);
        truncateHeight != null && (truncateHeight -= stlPadding[0] + stlPadding[2]);
    }

    // Calculate layout info of tokens.
    for (var i = 0; i < lines.length; i++) {
        var line = lines[i];
        var lineHeight = 0;
        var lineWidth = 0;

        for (var j = 0; j < line.tokens.length; j++) {
            var token = line.tokens[j];
            var tokenStyle = token.styleName && style.rich[token.styleName] || {};
            // textPadding should not inherit from style.
            var textPadding = token.textPadding = tokenStyle.textPadding;

            // textFont has been asigned to font by `normalizeStyle`.
            var font = token.font = tokenStyle.font || style.font;

            // textHeight can be used when textVerticalAlign is specified in token.
            var tokenHeight = token.textHeight = retrieve2(
                // textHeight should not be inherited, consider it can be specified
                // as box height of the block.
                tokenStyle.textHeight, getLineHeight(font)
            );
            textPadding && (tokenHeight += textPadding[0] + textPadding[2]);
            token.height = tokenHeight;
            token.lineHeight = retrieve3(
                tokenStyle.textLineHeight, style.textLineHeight, tokenHeight
            );

            token.textAlign = tokenStyle && tokenStyle.textAlign || style.textAlign;
            token.textVerticalAlign = tokenStyle && tokenStyle.textVerticalAlign || 'middle';

            if (truncateHeight != null && contentHeight + token.lineHeight > truncateHeight) {
                return {lines: [], width: 0, height: 0};
            }

            token.textWidth = getWidth(token.text, font);
            var tokenWidth = tokenStyle.textWidth;
            var tokenWidthNotSpecified = tokenWidth == null || tokenWidth === 'auto';

            // Percent width, can be `100%`, can be used in drawing separate
            // line when box width is needed to be auto.
            if (typeof tokenWidth === 'string' && tokenWidth.charAt(tokenWidth.length - 1) === '%') {
                token.percentWidth = tokenWidth;
                pendingList.push(token);
                tokenWidth = 0;
                // Do not truncate in this case, because there is no user case
                // and it is too complicated.
            }
            else {
                if (tokenWidthNotSpecified) {
                    tokenWidth = token.textWidth;

                    // FIXME: If image is not loaded and textWidth is not specified, calling
                    // `getBoundingRect()` will not get correct result.
                    var textBackgroundColor = tokenStyle.textBackgroundColor;
                    var bgImg = textBackgroundColor && textBackgroundColor.image;

                    // Use cases:
                    // (1) If image is not loaded, it will be loaded at render phase and call
                    // `dirty()` and `textBackgroundColor.image` will be replaced with the loaded
                    // image, and then the right size will be calculated here at the next tick.
                    // See `graphic/helper/text.js`.
                    // (2) If image loaded, and `textBackgroundColor.image` is image src string,
                    // use `imageHelper.findExistImage` to find cached image.
                    // `imageHelper.findExistImage` will always be called here before
                    // `imageHelper.createOrUpdateImage` in `graphic/helper/text.js#renderRichText`
                    // which ensures that image will not be rendered before correct size calcualted.
                    if (bgImg) {
                        bgImg = findExistImage(bgImg);
                        if (isImageReady(bgImg)) {
                            tokenWidth = Math.max(tokenWidth, bgImg.width * tokenHeight / bgImg.height);
                        }
                    }
                }

                var paddingW = textPadding ? textPadding[1] + textPadding[3] : 0;
                tokenWidth += paddingW;

                var remianTruncWidth = truncateWidth != null ? truncateWidth - lineWidth : null;

                if (remianTruncWidth != null && remianTruncWidth < tokenWidth) {
                    if (!tokenWidthNotSpecified || remianTruncWidth < paddingW) {
                        token.text = '';
                        token.textWidth = tokenWidth = 0;
                    }
                    else {
                        token.text = truncateText(
                            token.text, remianTruncWidth - paddingW, font, truncate.ellipsis,
                            {minChar: truncate.minChar}
                        );
                        token.textWidth = getWidth(token.text, font);
                        tokenWidth = token.textWidth + paddingW;
                    }
                }
            }

            lineWidth += (token.width = tokenWidth);
            tokenStyle && (lineHeight = Math.max(lineHeight, token.lineHeight));
        }

        line.width = lineWidth;
        line.lineHeight = lineHeight;
        contentHeight += lineHeight;
        contentWidth = Math.max(contentWidth, lineWidth);
    }

    contentBlock.outerWidth = contentBlock.width = retrieve2(style.textWidth, contentWidth);
    contentBlock.outerHeight = contentBlock.height = retrieve2(style.textHeight, contentHeight);

    if (stlPadding) {
        contentBlock.outerWidth += stlPadding[1] + stlPadding[3];
        contentBlock.outerHeight += stlPadding[0] + stlPadding[2];
    }

    for (var i = 0; i < pendingList.length; i++) {
        var token = pendingList[i];
        var percentWidth = token.percentWidth;
        // Should not base on outerWidth, because token can not be placed out of padding.
        token.width = parseInt(percentWidth, 10) / 100 * contentWidth;
    }

    return contentBlock;
}

function pushTokens(block, str, styleName) {
    var isEmptyStr = str === '';
    var strs = str.split('\n');
    var lines = block.lines;

    for (var i = 0; i < strs.length; i++) {
        var text = strs[i];
        var token = {
            styleName: styleName,
            text: text,
            isLineHolder: !text && !isEmptyStr
        };

        // The first token should be appended to the last line.
        if (!i) {
            var tokens = (lines[lines.length - 1] || (lines[0] = {tokens: []})).tokens;

            // Consider cases:
            // (1) ''.split('\n') => ['', '\n', ''], the '' at the first item
            // (which is a placeholder) should be replaced by new token.
            // (2) A image backage, where token likes {a|}.
            // (3) A redundant '' will affect textAlign in line.
            // (4) tokens with the same tplName should not be merged, because
            // they should be displayed in different box (with border and padding).
            var tokensLen = tokens.length;
            (tokensLen === 1 && tokens[0].isLineHolder)
                ? (tokens[0] = token)
                // Consider text is '', only insert when it is the "lineHolder" or
                // "emptyStr". Otherwise a redundant '' will affect textAlign in line.
                : ((text || !tokensLen || isEmptyStr) && tokens.push(token));
        }
        // Other tokens always start a new line.
        else {
            // If there is '', insert it as a placeholder.
            lines.push({tokens: [token]});
        }
    }
}

function makeFont(style) {
    // FIXME in node-canvas fontWeight is before fontStyle
    // Use `fontSize` `fontFamily` to check whether font properties are defined.
    return trim((style.fontSize || style.fontFamily) && [
        style.fontStyle,
        style.fontWeight,
        (style.fontSize || 12) + 'px',
        // If font properties are defined, `fontFamily` should not be ignored.
        style.fontFamily || 'sans-serif'
    ].join(' ')) || style.textFont || style.font;
}

function buildPath(ctx, shape) {
    var x = shape.x;
    var y = shape.y;
    var width = shape.width;
    var height = shape.height;
    var r = shape.r;
    var r1;
    var r2;
    var r3;
    var r4;

    // Convert width and height to positive for better borderRadius
    if (width < 0) {
        x = x + width;
        width = -width;
    }
    if (height < 0) {
        y = y + height;
        height = -height;
    }

    if (typeof r === 'number') {
        r1 = r2 = r3 = r4 = r;
    }
    else if (r instanceof Array) {
        if (r.length === 1) {
            r1 = r2 = r3 = r4 = r[0];
        }
        else if (r.length === 2) {
            r1 = r3 = r[0];
            r2 = r4 = r[1];
        }
        else if (r.length === 3) {
            r1 = r[0];
            r2 = r4 = r[1];
            r3 = r[2];
        }
        else {
            r1 = r[0];
            r2 = r[1];
            r3 = r[2];
            r4 = r[3];
        }
    }
    else {
        r1 = r2 = r3 = r4 = 0;
    }

    var total;
    if (r1 + r2 > width) {
        total = r1 + r2;
        r1 *= width / total;
        r2 *= width / total;
    }
    if (r3 + r4 > width) {
        total = r3 + r4;
        r3 *= width / total;
        r4 *= width / total;
    }
    if (r2 + r3 > height) {
        total = r2 + r3;
        r2 *= height / total;
        r3 *= height / total;
    }
    if (r1 + r4 > height) {
        total = r1 + r4;
        r1 *= height / total;
        r4 *= height / total;
    }
    ctx.moveTo(x + r1, y);
    ctx.lineTo(x + width - r2, y);
    r2 !== 0 && ctx.quadraticCurveTo(
        x + width, y, x + width, y + r2
    );
    ctx.lineTo(x + width, y + height - r3);
    r3 !== 0 && ctx.quadraticCurveTo(
        x + width, y + height, x + width - r3, y + height
    );
    ctx.lineTo(x + r4, y + height);
    r4 !== 0 && ctx.quadraticCurveTo(
        x, y + height, x, y + height - r4
    );
    ctx.lineTo(x, y + r1);
    r1 !== 0 && ctx.quadraticCurveTo(x, y, x + r1, y);
}

// TODO: Have not support 'start', 'end' yet.
var VALID_TEXT_ALIGN = {left: 1, right: 1, center: 1};
var VALID_TEXT_VERTICAL_ALIGN = {top: 1, bottom: 1, middle: 1};

/**
 * @param {module:zrender/graphic/Style} style
 * @return {module:zrender/graphic/Style} The input style.
 */
function normalizeTextStyle(style) {
    normalizeStyle(style);
    each$1(style.rich, normalizeStyle);
    return style;
}

function normalizeStyle(style) {
    if (style) {

        style.font = makeFont(style);

        var textAlign = style.textAlign;
        textAlign === 'middle' && (textAlign = 'center');
        style.textAlign = (
            textAlign == null || VALID_TEXT_ALIGN[textAlign]
        ) ? textAlign : 'left';

        // Compatible with textBaseline.
        var textVerticalAlign = style.textVerticalAlign || style.textBaseline;
        textVerticalAlign === 'center' && (textVerticalAlign = 'middle');
        style.textVerticalAlign = (
            textVerticalAlign == null || VALID_TEXT_VERTICAL_ALIGN[textVerticalAlign]
        ) ? textVerticalAlign : 'top';

        var textPadding = style.textPadding;
        if (textPadding) {
            style.textPadding = normalizeCssArray(style.textPadding);
        }
    }
}

/**
 * @param {CanvasRenderingContext2D} ctx
 * @param {string} text
 * @param {module:zrender/graphic/Style} style
 * @param {Object|boolean} [rect] {x, y, width, height}
 *                  If set false, rect text is not used.
 */
function renderText(hostEl, ctx, text, style, rect) {
    style.rich
        ? renderRichText(hostEl, ctx, text, style, rect)
        : renderPlainText(hostEl, ctx, text, style, rect);
}

function renderPlainText(hostEl, ctx, text, style, rect) {
    var font = setCtx(ctx, 'font', style.font || DEFAULT_FONT);

    var textPadding = style.textPadding;

    var contentBlock = hostEl.__textCotentBlock;
    if (!contentBlock || hostEl.__dirty) {
        contentBlock = hostEl.__textCotentBlock = parsePlainText(
            text, font, textPadding, style.truncate
        );
    }

    var outerHeight = contentBlock.outerHeight;

    var textLines = contentBlock.lines;
    var lineHeight = contentBlock.lineHeight;

    var boxPos = getBoxPosition(outerHeight, style, rect);
    var baseX = boxPos.baseX;
    var baseY = boxPos.baseY;
    var textAlign = boxPos.textAlign;
    var textVerticalAlign = boxPos.textVerticalAlign;

    // Origin of textRotation should be the base point of text drawing.
    applyTextRotation(ctx, style, rect, baseX, baseY);

    var boxY = adjustTextY(baseY, outerHeight, textVerticalAlign);
    var textX = baseX;
    var textY = boxY;

    var needDrawBg = needDrawBackground(style);
    if (needDrawBg || textPadding) {
        // Consider performance, do not call getTextWidth util necessary.
        var textWidth = getWidth(text, font);
        var outerWidth = textWidth;
        textPadding && (outerWidth += textPadding[1] + textPadding[3]);
        var boxX = adjustTextX(baseX, outerWidth, textAlign);

        needDrawBg && drawBackground(hostEl, ctx, style, boxX, boxY, outerWidth, outerHeight);

        if (textPadding) {
            textX = getTextXForPadding(baseX, textAlign, textPadding);
            textY += textPadding[0];
        }
    }

    setCtx(ctx, 'textAlign', textAlign || 'left');
    // Force baseline to be "middle". Otherwise, if using "top", the
    // text will offset downward a little bit in font "Microsoft YaHei".
    setCtx(ctx, 'textBaseline', 'middle');

    // Always set shadowBlur and shadowOffset to avoid leak from displayable.
    setCtx(ctx, 'shadowBlur', style.textShadowBlur || 0);
    setCtx(ctx, 'shadowColor', style.textShadowColor || 'transparent');
    setCtx(ctx, 'shadowOffsetX', style.textShadowOffsetX || 0);
    setCtx(ctx, 'shadowOffsetY', style.textShadowOffsetY || 0);

    // `textBaseline` is set as 'middle'.
    textY += lineHeight / 2;

    var textStrokeWidth = style.textStrokeWidth;
    var textStroke = getStroke(style.textStroke, textStrokeWidth);
    var textFill = getFill(style.textFill);

    if (textStroke) {
        setCtx(ctx, 'lineWidth', textStrokeWidth);
        setCtx(ctx, 'strokeStyle', textStroke);
    }
    if (textFill) {
        setCtx(ctx, 'fillStyle', textFill);
    }

    for (var i = 0; i < textLines.length; i++) {
        // Fill after stroke so the outline will not cover the main part.
        textStroke && ctx.strokeText(textLines[i], textX, textY);
        textFill && ctx.fillText(textLines[i], textX, textY);
        textY += lineHeight;
    }
}

function renderRichText(hostEl, ctx, text, style, rect) {
    var contentBlock = hostEl.__textCotentBlock;

    if (!contentBlock || hostEl.__dirty) {
        contentBlock = hostEl.__textCotentBlock = parseRichText(text, style);
    }

    drawRichText(hostEl, ctx, contentBlock, style, rect);
}

function drawRichText(hostEl, ctx, contentBlock, style, rect) {
    var contentWidth = contentBlock.width;
    var outerWidth = contentBlock.outerWidth;
    var outerHeight = contentBlock.outerHeight;
    var textPadding = style.textPadding;

    var boxPos = getBoxPosition(outerHeight, style, rect);
    var baseX = boxPos.baseX;
    var baseY = boxPos.baseY;
    var textAlign = boxPos.textAlign;
    var textVerticalAlign = boxPos.textVerticalAlign;

    // Origin of textRotation should be the base point of text drawing.
    applyTextRotation(ctx, style, rect, baseX, baseY);

    var boxX = adjustTextX(baseX, outerWidth, textAlign);
    var boxY = adjustTextY(baseY, outerHeight, textVerticalAlign);
    var xLeft = boxX;
    var lineTop = boxY;
    if (textPadding) {
        xLeft += textPadding[3];
        lineTop += textPadding[0];
    }
    var xRight = xLeft + contentWidth;

    needDrawBackground(style) && drawBackground(
        hostEl, ctx, style, boxX, boxY, outerWidth, outerHeight
    );

    for (var i = 0; i < contentBlock.lines.length; i++) {
        var line = contentBlock.lines[i];
        var tokens = line.tokens;
        var tokenCount = tokens.length;
        var lineHeight = line.lineHeight;
        var usedWidth = line.width;

        var leftIndex = 0;
        var lineXLeft = xLeft;
        var lineXRight = xRight;
        var rightIndex = tokenCount - 1;
        var token;

        while (
            leftIndex < tokenCount
            && (token = tokens[leftIndex], !token.textAlign || token.textAlign === 'left')
        ) {
            placeToken(hostEl, ctx, token, style, lineHeight, lineTop, lineXLeft, 'left');
            usedWidth -= token.width;
            lineXLeft += token.width;
            leftIndex++;
        }

        while (
            rightIndex >= 0
            && (token = tokens[rightIndex], token.textAlign === 'right')
        ) {
            placeToken(hostEl, ctx, token, style, lineHeight, lineTop, lineXRight, 'right');
            usedWidth -= token.width;
            lineXRight -= token.width;
            rightIndex--;
        }

        // The other tokens are placed as textAlign 'center' if there is enough space.
        lineXLeft += (contentWidth - (lineXLeft - xLeft) - (xRight - lineXRight) - usedWidth) / 2;
        while (leftIndex <= rightIndex) {
            token = tokens[leftIndex];
            // Consider width specified by user, use 'center' rather than 'left'.
            placeToken(hostEl, ctx, token, style, lineHeight, lineTop, lineXLeft + token.width / 2, 'center');
            lineXLeft += token.width;
            leftIndex++;
        }

        lineTop += lineHeight;
    }
}

function applyTextRotation(ctx, style, rect, x, y) {
    // textRotation only apply in RectText.
    if (rect && style.textRotation) {
        var origin = style.textOrigin;
        if (origin === 'center') {
            x = rect.width / 2 + rect.x;
            y = rect.height / 2 + rect.y;
        }
        else if (origin) {
            x = origin[0] + rect.x;
            y = origin[1] + rect.y;
        }

        ctx.translate(x, y);
        // Positive: anticlockwise
        ctx.rotate(-style.textRotation);
        ctx.translate(-x, -y);
    }
}

function placeToken(hostEl, ctx, token, style, lineHeight, lineTop, x, textAlign) {
    var tokenStyle = style.rich[token.styleName] || {};

    // 'ctx.textBaseline' is always set as 'middle', for sake of
    // the bias of "Microsoft YaHei".
    var textVerticalAlign = token.textVerticalAlign;
    var y = lineTop + lineHeight / 2;
    if (textVerticalAlign === 'top') {
        y = lineTop + token.height / 2;
    }
    else if (textVerticalAlign === 'bottom') {
        y = lineTop + lineHeight - token.height / 2;
    }

    !token.isLineHolder && needDrawBackground(tokenStyle) && drawBackground(
        hostEl,
        ctx,
        tokenStyle,
        textAlign === 'right'
            ? x - token.width
            : textAlign === 'center'
            ? x - token.width / 2
            : x,
        y - token.height / 2,
        token.width,
        token.height
    );

    var textPadding = token.textPadding;
    if (textPadding) {
        x = getTextXForPadding(x, textAlign, textPadding);
        y -= token.height / 2 - textPadding[2] - token.textHeight / 2;
    }

    setCtx(ctx, 'shadowBlur', retrieve3(tokenStyle.textShadowBlur, style.textShadowBlur, 0));
    setCtx(ctx, 'shadowColor', tokenStyle.textShadowColor || style.textShadowColor || 'transparent');
    setCtx(ctx, 'shadowOffsetX', retrieve3(tokenStyle.textShadowOffsetX, style.textShadowOffsetX, 0));
    setCtx(ctx, 'shadowOffsetY', retrieve3(tokenStyle.textShadowOffsetY, style.textShadowOffsetY, 0));

    setCtx(ctx, 'textAlign', textAlign);
    // Force baseline to be "middle". Otherwise, if using "top", the
    // text will offset downward a little bit in font "Microsoft YaHei".
    setCtx(ctx, 'textBaseline', 'middle');

    setCtx(ctx, 'font', token.font || DEFAULT_FONT);

    var textStroke = getStroke(tokenStyle.textStroke || style.textStroke, textStrokeWidth);
    var textFill = getFill(tokenStyle.textFill || style.textFill);
    var textStrokeWidth = retrieve2(tokenStyle.textStrokeWidth, style.textStrokeWidth);

    // Fill after stroke so the outline will not cover the main part.
    if (textStroke) {
        setCtx(ctx, 'lineWidth', textStrokeWidth);
        setCtx(ctx, 'strokeStyle', textStroke);
        ctx.strokeText(token.text, x, y);
    }
    if (textFill) {
        setCtx(ctx, 'fillStyle', textFill);
        ctx.fillText(token.text, x, y);
    }
}

function needDrawBackground(style) {
    return style.textBackgroundColor
        || (style.textBorderWidth && style.textBorderColor);
}

// style: {textBackgroundColor, textBorderWidth, textBorderColor, textBorderRadius}
// shape: {x, y, width, height}
function drawBackground(hostEl, ctx, style, x, y, width, height) {
    var textBackgroundColor = style.textBackgroundColor;
    var textBorderWidth = style.textBorderWidth;
    var textBorderColor = style.textBorderColor;
    var isPlainBg = isString$1(textBackgroundColor);

    setCtx(ctx, 'shadowBlur', style.textBoxShadowBlur || 0);
    setCtx(ctx, 'shadowColor', style.textBoxShadowColor || 'transparent');
    setCtx(ctx, 'shadowOffsetX', style.textBoxShadowOffsetX || 0);
    setCtx(ctx, 'shadowOffsetY', style.textBoxShadowOffsetY || 0);

    if (isPlainBg || (textBorderWidth && textBorderColor)) {
        ctx.beginPath();
        var textBorderRadius = style.textBorderRadius;
        if (!textBorderRadius) {
            ctx.rect(x, y, width, height);
        }
        else {
            buildPath(ctx, {
                x: x, y: y, width: width, height: height, r: textBorderRadius
            });
        }
        ctx.closePath();
    }

    if (isPlainBg) {
        setCtx(ctx, 'fillStyle', textBackgroundColor);
        ctx.fill();
    }
    else if (isObject$1(textBackgroundColor)) {
        var image = textBackgroundColor.image;

        image = createOrUpdateImage(
            image, null, hostEl, onBgImageLoaded, textBackgroundColor
        );
        if (image && isImageReady(image)) {
            ctx.drawImage(image, x, y, width, height);
        }
    }

    if (textBorderWidth && textBorderColor) {
        setCtx(ctx, 'lineWidth', textBorderWidth);
        setCtx(ctx, 'strokeStyle', textBorderColor);
        ctx.stroke();
    }
}

function onBgImageLoaded(image, textBackgroundColor) {
    // Replace image, so that `contain/text.js#parseRichText`
    // will get correct result in next tick.
    textBackgroundColor.image = image;
}

function getBoxPosition(blockHeiht, style, rect) {
    var baseX = style.x || 0;
    var baseY = style.y || 0;
    var textAlign = style.textAlign;
    var textVerticalAlign = style.textVerticalAlign;

    // Text position represented by coord
    if (rect) {
        var textPosition = style.textPosition;
        if (textPosition instanceof Array) {
            // Percent
            baseX = rect.x + parsePercent(textPosition[0], rect.width);
            baseY = rect.y + parsePercent(textPosition[1], rect.height);
        }
        else {
            var res = adjustTextPositionOnRect(
                textPosition, rect, style.textDistance
            );
            baseX = res.x;
            baseY = res.y;
            // Default align and baseline when has textPosition
            textAlign = textAlign || res.textAlign;
            textVerticalAlign = textVerticalAlign || res.textVerticalAlign;
        }

        // textOffset is only support in RectText, otherwise
        // we have to adjust boundingRect for textOffset.
        var textOffset = style.textOffset;
        if (textOffset) {
            baseX += textOffset[0];
            baseY += textOffset[1];
        }
    }

    return {
        baseX: baseX,
        baseY: baseY,
        textAlign: textAlign,
        textVerticalAlign: textVerticalAlign
    };
}

function setCtx(ctx, prop, value) {
    // FIXME ??? performance try
    // if (ctx.__currentValues[prop] !== value) {
        // ctx[prop] = ctx.__currentValues[prop] = value;

    ctx[prop] = fixShadow(ctx, prop, value);
    // }
    return ctx[prop];
}

/**
 * @param {string} [stroke] If specified, do not check style.textStroke.
 * @param {string} [lineWidth] If specified, do not check style.textStroke.
 * @param {number} style
 */
function getStroke(stroke, lineWidth) {
    return (stroke == null || lineWidth <= 0 || stroke === 'transparent' || stroke === 'none')
        ? null
        // TODO pattern and gradient?
        : (stroke.image || stroke.colorStops)
        ? '#000'
        : stroke;
}

function getFill(fill) {
    return (fill == null || fill === 'none')
        ? null
        // TODO pattern and gradient?
        : (fill.image || fill.colorStops)
        ? '#000'
        : fill;
}

function parsePercent(value, maxValue) {
    if (typeof value === 'string') {
        if (value.lastIndexOf('%') >= 0) {
            return parseFloat(value) / 100 * maxValue;
        }
        return parseFloat(value);
    }
    return value;
}

function getTextXForPadding(x, textAlign, textPadding) {
    return textAlign === 'right'
        ? (x - textPadding[1])
        : textAlign === 'center'
        ? (x + textPadding[3] / 2 - textPadding[1] / 2)
        : (x + textPadding[3]);
}

/**
 * @param {string} text
 * @param {module:zrender/Style} style
 * @return {boolean}
 */
function needDrawText(text, style) {
    return text != null
        && (text
            || style.textBackgroundColor
            || (style.textBorderWidth && style.textBorderColor)
            || style.textPadding
        );
}

/**
 * Mixin for drawing text in a element bounding rect
 * @module zrender/mixin/RectText
 */

var tmpRect = new BoundingRect();

var RectText = function () {};

RectText.prototype = {

    constructor: RectText,

    /**
     * Draw text in a rect with specified position.
     * @param  {CanvasRenderingContext2D} ctx
     * @param  {Object} rect Displayable rect
     */
    drawRectText: function (ctx, rect) {
        var style = this.style;

        rect = style.textRect || rect;

        // Optimize, avoid normalize every time.
        this.__dirty && normalizeTextStyle(style, true);

        var text = style.text;

        // Convert to string
        text != null && (text += '');

        if (!needDrawText(text, style)) {
            return;
        }

        // FIXME
        ctx.save();

        // Transform rect to view space
        var transform = this.transform;
        if (!style.transformText) {
            if (transform) {
                tmpRect.copy(rect);
                tmpRect.applyTransform(transform);
                rect = tmpRect;
            }
        }
        else {
            this.setTransform(ctx);
        }

        // transformText and textRotation can not be used at the same time.
        renderText(this, ctx, text, style, rect);

        ctx.restore();
    }
};

/**
 * 可绘制的图形基类
 * Base class of all displayable graphic objects
 * @module zrender/graphic/Displayable
 */

/**
 * @alias module:zrender/graphic/Displayable
 * @extends module:zrender/Element
 * @extends module:zrender/graphic/mixin/RectText
 */
function Displayable(opts) {
    var this$1 = this;


    opts = opts || {};

    Element.call(this, opts);

    // Extend properties
    for (var name in opts) {
        if (
            opts.hasOwnProperty(name) &&
            name !== 'style'
        ) {
            this$1[name] = opts[name];
        }
    }

    /**
     * @type {module:zrender/graphic/Style}
     */
    this.style = new Style(opts.style, this);

    this._rect = null;
    // Shapes for cascade clipping.
    this.__clipPaths = [];

    // FIXME Stateful must be mixined after style is setted
    // Stateful.call(this, opts);
}

Displayable.prototype = {

    constructor: Displayable,

    type: 'displayable',

    /**
     * Displayable 是否为脏，Painter 中会根据该标记判断是否需要是否需要重新绘制
     * Dirty flag. From which painter will determine if this displayable object needs brush
     * @name module:zrender/graphic/Displayable#__dirty
     * @type {boolean}
     */
    __dirty: true,

    /**
     * 图形是否可见，为true时不绘制图形，但是仍能触发鼠标事件
     * If ignore drawing of the displayable object. Mouse event will still be triggered
     * @name module:/zrender/graphic/Displayable#invisible
     * @type {boolean}
     * @default false
     */
    invisible: false,

    /**
     * @name module:/zrender/graphic/Displayable#z
     * @type {number}
     * @default 0
     */
    z: 0,

    /**
     * @name module:/zrender/graphic/Displayable#z
     * @type {number}
     * @default 0
     */
    z2: 0,

    /**
     * z层level，决定绘画在哪层canvas中
     * @name module:/zrender/graphic/Displayable#zlevel
     * @type {number}
     * @default 0
     */
    zlevel: 0,

    /**
     * 是否可拖拽
     * @name module:/zrender/graphic/Displayable#draggable
     * @type {boolean}
     * @default false
     */
    draggable: false,

    /**
     * 是否正在拖拽
     * @name module:/zrender/graphic/Displayable#draggable
     * @type {boolean}
     * @default false
     */
    dragging: false,

    /**
     * 是否相应鼠标事件
     * @name module:/zrender/graphic/Displayable#silent
     * @type {boolean}
     * @default false
     */
    silent: false,

    /**
     * If enable culling
     * @type {boolean}
     * @default false
     */
    culling: false,

    /**
     * Mouse cursor when hovered
     * @name module:/zrender/graphic/Displayable#cursor
     * @type {string}
     */
    cursor: 'pointer',

    /**
     * If hover area is bounding rect
     * @name module:/zrender/graphic/Displayable#rectHover
     * @type {string}
     */
    rectHover: false,

    /**
     * Render the element progressively when the value >= 0,
     * usefull for large data.
     * @type {number}
     */
    progressive: -1,

    beforeBrush: function (ctx) {},

    afterBrush: function (ctx) {},

    /**
     * 图形绘制方法
     * @param {CanvasRenderingContext2D} ctx
     */
    // Interface
    brush: function (ctx, prevEl) {},

    /**
     * 获取最小包围盒
     * @return {module:zrender/core/BoundingRect}
     */
    // Interface
    getBoundingRect: function () {},

    /**
     * 判断坐标 x, y 是否在图形上
     * If displayable element contain coord x, y
     * @param  {number} x
     * @param  {number} y
     * @return {boolean}
     */
    contain: function (x, y) {
        return this.rectContain(x, y);
    },

    /**
     * @param  {Function} cb
     * @param  {}   context
     */
    traverse: function (cb, context) {
        cb.call(context, this);
    },

    /**
     * 判断坐标 x, y 是否在图形的包围盒上
     * If bounding rect of element contain coord x, y
     * @param  {number} x
     * @param  {number} y
     * @return {boolean}
     */
    rectContain: function (x, y) {
        var coord = this.transformCoordToLocal(x, y);
        var rect = this.getBoundingRect();
        return rect.contain(coord[0], coord[1]);
    },

    /**
     * 标记图形元素为脏，并且在下一帧重绘
     * Mark displayable element dirty and refresh next frame
     */
    dirty: function () {
        this.__dirty = true;

        this._rect = null;

        this.__zr && this.__zr.refresh();
    },

    /**
     * 图形是否会触发事件
     * If displayable object binded any event
     * @return {boolean}
     */
    // TODO, 通过 bind 绑定的事件
    // isSilent: function () {
    //     return !(
    //         this.hoverable || this.draggable
    //         || this.onmousemove || this.onmouseover || this.onmouseout
    //         || this.onmousedown || this.onmouseup || this.onclick
    //         || this.ondragenter || this.ondragover || this.ondragleave
    //         || this.ondrop
    //     );
    // },
    /**
     * Alias for animate('style')
     * @param {boolean} loop
     */
    animateStyle: function (loop) {
        return this.animate('style', loop);
    },

    attrKV: function (key, value) {
        if (key !== 'style') {
            Element.prototype.attrKV.call(this, key, value);
        }
        else {
            this.style.set(value);
        }
    },

    /**
     * @param {Object|string} key
     * @param {*} value
     */
    setStyle: function (key, value) {
        this.style.set(key, value);
        this.dirty(false);
        return this;
    },

    /**
     * Use given style object
     * @param  {Object} obj
     */
    useStyle: function (obj) {
        this.style = new Style(obj, this);
        this.dirty(false);
        return this;
    }
};

inherits(Displayable, Element);

mixin(Displayable, RectText);

/**
 * 曲线辅助模块
 * @module zrender/core/curve
 * @author pissang(https://www.github.com/pissang)
 */

var mathPow = Math.pow;
var mathSqrt = Math.sqrt;

var EPSILON$1 = 1e-8;
var EPSILON_NUMERIC = 1e-4;

var THREE_SQRT = mathSqrt(3);
var ONE_THIRD = 1 / 3;

// 临时变量
var _v0 = create$2();
var _v1 = create$2();
var _v2 = create$2();

function isAroundZero(val) {
    return val > -EPSILON$1 && val < EPSILON$1;
}
function isNotAroundZero$1(val) {
    return val > EPSILON$1 || val < -EPSILON$1;
}
/**
 * 计算三次贝塞尔值
 * @memberOf module:zrender/core/curve
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} p2
 * @param  {number} p3
 * @param  {number} t
 * @return {number}
 */
function cubicAt(p0, p1, p2, p3, t) {
    var onet = 1 - t;
    return onet * onet * (onet * p0 + 3 * t * p1)
            + t * t * (t * p3 + 3 * onet * p2);
}

/**
 * 计算三次贝塞尔方程根，使用盛金公式
 * @memberOf module:zrender/core/curve
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} p2
 * @param  {number} p3
 * @param  {number} val
 * @param  {Array.<number>} roots
 * @return {number} 有效根数目
 */
function cubicRootAt(p0, p1, p2, p3, val, roots) {
    // Evaluate roots of cubic functions
    var a = p3 + 3 * (p1 - p2) - p0;
    var b = 3 * (p2 - p1 * 2 + p0);
    var c = 3 * (p1  - p0);
    var d = p0 - val;

    var A = b * b - 3 * a * c;
    var B = b * c - 9 * a * d;
    var C = c * c - 3 * b * d;

    var n = 0;

    if (isAroundZero(A) && isAroundZero(B)) {
        if (isAroundZero(b)) {
            roots[0] = 0;
        }
        else {
            var t1 = -c / b;  //t1, t2, t3, b is not zero
            if (t1 >= 0 && t1 <= 1) {
                roots[n++] = t1;
            }
        }
    }
    else {
        var disc = B * B - 4 * A * C;

        if (isAroundZero(disc)) {
            var K = B / A;
            var t1 = -b / a + K;  // t1, a is not zero
            var t2 = -K / 2;  // t2, t3
            if (t1 >= 0 && t1 <= 1) {
                roots[n++] = t1;
            }
            if (t2 >= 0 && t2 <= 1) {
                roots[n++] = t2;
            }
        }
        else if (disc > 0) {
            var discSqrt = mathSqrt(disc);
            var Y1 = A * b + 1.5 * a * (-B + discSqrt);
            var Y2 = A * b + 1.5 * a * (-B - discSqrt);
            if (Y1 < 0) {
                Y1 = -mathPow(-Y1, ONE_THIRD);
            }
            else {
                Y1 = mathPow(Y1, ONE_THIRD);
            }
            if (Y2 < 0) {
                Y2 = -mathPow(-Y2, ONE_THIRD);
            }
            else {
                Y2 = mathPow(Y2, ONE_THIRD);
            }
            var t1 = (-b - (Y1 + Y2)) / (3 * a);
            if (t1 >= 0 && t1 <= 1) {
                roots[n++] = t1;
            }
        }
        else {
            var T = (2 * A * b - 3 * a * B) / (2 * mathSqrt(A * A * A));
            var theta = Math.acos(T) / 3;
            var ASqrt = mathSqrt(A);
            var tmp = Math.cos(theta);

            var t1 = (-b - 2 * ASqrt * tmp) / (3 * a);
            var t2 = (-b + ASqrt * (tmp + THREE_SQRT * Math.sin(theta))) / (3 * a);
            var t3 = (-b + ASqrt * (tmp - THREE_SQRT * Math.sin(theta))) / (3 * a);
            if (t1 >= 0 && t1 <= 1) {
                roots[n++] = t1;
            }
            if (t2 >= 0 && t2 <= 1) {
                roots[n++] = t2;
            }
            if (t3 >= 0 && t3 <= 1) {
                roots[n++] = t3;
            }
        }
    }
    return n;
}

/**
 * 计算三次贝塞尔方程极限值的位置
 * @memberOf module:zrender/core/curve
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} p2
 * @param  {number} p3
 * @param  {Array.<number>} extrema
 * @return {number} 有效数目
 */
function cubicExtrema(p0, p1, p2, p3, extrema) {
    var b = 6 * p2 - 12 * p1 + 6 * p0;
    var a = 9 * p1 + 3 * p3 - 3 * p0 - 9 * p2;
    var c = 3 * p1 - 3 * p0;

    var n = 0;
    if (isAroundZero(a)) {
        if (isNotAroundZero$1(b)) {
            var t1 = -c / b;
            if (t1 >= 0 && t1 <=1) {
                extrema[n++] = t1;
            }
        }
    }
    else {
        var disc = b * b - 4 * a * c;
        if (isAroundZero(disc)) {
            extrema[0] = -b / (2 * a);
        }
        else if (disc > 0) {
            var discSqrt = mathSqrt(disc);
            var t1 = (-b + discSqrt) / (2 * a);
            var t2 = (-b - discSqrt) / (2 * a);
            if (t1 >= 0 && t1 <= 1) {
                extrema[n++] = t1;
            }
            if (t2 >= 0 && t2 <= 1) {
                extrema[n++] = t2;
            }
        }
    }
    return n;
}

/**
 * 投射点到三次贝塞尔曲线上，返回投射距离。
 * 投射点有可能会有一个或者多个，这里只返回其中距离最短的一个。
 * @param {number} x0
 * @param {number} y0
 * @param {number} x1
 * @param {number} y1
 * @param {number} x2
 * @param {number} y2
 * @param {number} x3
 * @param {number} y3
 * @param {number} x
 * @param {number} y
 * @param {Array.<number>} [out] 投射点
 * @return {number}
 */
function cubicProjectPoint(
    x0, y0, x1, y1, x2, y2, x3, y3,
    x, y, out
) {
    // http://pomax.github.io/bezierinfo/#projections
    var t;
    var interval = 0.005;
    var d = Infinity;
    var prev;
    var next;
    var d1;
    var d2;

    _v0[0] = x;
    _v0[1] = y;

    // 先粗略估计一下可能的最小距离的 t 值
    // PENDING
    for (var _t = 0; _t < 1; _t += 0.05) {
        _v1[0] = cubicAt(x0, x1, x2, x3, _t);
        _v1[1] = cubicAt(y0, y1, y2, y3, _t);
        d1 = distSquare(_v0, _v1);
        if (d1 < d) {
            t = _t;
            d = d1;
        }
    }
    d = Infinity;

    // At most 32 iteration
    for (var i = 0; i < 32; i++) {
        if (interval < EPSILON_NUMERIC) {
            break;
        }
        prev = t - interval;
        next = t + interval;
        // t - interval
        _v1[0] = cubicAt(x0, x1, x2, x3, prev);
        _v1[1] = cubicAt(y0, y1, y2, y3, prev);

        d1 = distSquare(_v1, _v0);

        if (prev >= 0 && d1 < d) {
            t = prev;
            d = d1;
        }
        else {
            // t + interval
            _v2[0] = cubicAt(x0, x1, x2, x3, next);
            _v2[1] = cubicAt(y0, y1, y2, y3, next);
            d2 = distSquare(_v2, _v0);

            if (next <= 1 && d2 < d) {
                t = next;
                d = d2;
            }
            else {
                interval *= 0.5;
            }
        }
    }
    // t
    if (out) {
        out[0] = cubicAt(x0, x1, x2, x3, t);
        out[1] = cubicAt(y0, y1, y2, y3, t);
    }
    // console.log(interval, i);
    return mathSqrt(d);
}

/**
 * 计算二次方贝塞尔值
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} p2
 * @param  {number} t
 * @return {number}
 */
function quadraticAt(p0, p1, p2, t) {
    var onet = 1 - t;
    return onet * (onet * p0 + 2 * t * p1) + t * t * p2;
}

/**
 * 计算二次方贝塞尔方程根
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} p2
 * @param  {number} t
 * @param  {Array.<number>} roots
 * @return {number} 有效根数目
 */
function quadraticRootAt(p0, p1, p2, val, roots) {
    var a = p0 - 2 * p1 + p2;
    var b = 2 * (p1 - p0);
    var c = p0 - val;

    var n = 0;
    if (isAroundZero(a)) {
        if (isNotAroundZero$1(b)) {
            var t1 = -c / b;
            if (t1 >= 0 && t1 <= 1) {
                roots[n++] = t1;
            }
        }
    }
    else {
        var disc = b * b - 4 * a * c;
        if (isAroundZero(disc)) {
            var t1 = -b / (2 * a);
            if (t1 >= 0 && t1 <= 1) {
                roots[n++] = t1;
            }
        }
        else if (disc > 0) {
            var discSqrt = mathSqrt(disc);
            var t1 = (-b + discSqrt) / (2 * a);
            var t2 = (-b - discSqrt) / (2 * a);
            if (t1 >= 0 && t1 <= 1) {
                roots[n++] = t1;
            }
            if (t2 >= 0 && t2 <= 1) {
                roots[n++] = t2;
            }
        }
    }
    return n;
}

/**
 * 计算二次贝塞尔方程极限值
 * @memberOf module:zrender/core/curve
 * @param  {number} p0
 * @param  {number} p1
 * @param  {number} p2
 * @return {number}
 */
function quadraticExtremum(p0, p1, p2) {
    var divider = p0 + p2 - 2 * p1;
    if (divider === 0) {
        // p1 is center of p0 and p2
        return 0.5;
    }
    else {
        return (p0 - p1) / divider;
    }
}

/**
 * 投射点到二次贝塞尔曲线上，返回投射距离。
 * 投射点有可能会有一个或者多个，这里只返回其中距离最短的一个。
 * @param {number} x0
 * @param {number} y0
 * @param {number} x1
 * @param {number} y1
 * @param {number} x2
 * @param {number} y2
 * @param {number} x
 * @param {number} y
 * @param {Array.<number>} out 投射点
 * @return {number}
 */
function quadraticProjectPoint(
    x0, y0, x1, y1, x2, y2,
    x, y, out
) {
    // http://pomax.github.io/bezierinfo/#projections
    var t;
    var interval = 0.005;
    var d = Infinity;

    _v0[0] = x;
    _v0[1] = y;

    // 先粗略估计一下可能的最小距离的 t 值
    // PENDING
    for (var _t = 0; _t < 1; _t += 0.05) {
        _v1[0] = quadraticAt(x0, x1, x2, _t);
        _v1[1] = quadraticAt(y0, y1, y2, _t);
        var d1 = distSquare(_v0, _v1);
        if (d1 < d) {
            t = _t;
            d = d1;
        }
    }
    d = Infinity;

    // At most 32 iteration
    for (var i = 0; i < 32; i++) {
        if (interval < EPSILON_NUMERIC) {
            break;
        }
        var prev = t - interval;
        var next = t + interval;
        // t - interval
        _v1[0] = quadraticAt(x0, x1, x2, prev);
        _v1[1] = quadraticAt(y0, y1, y2, prev);

        var d1 = distSquare(_v1, _v0);

        if (prev >= 0 && d1 < d) {
            t = prev;
            d = d1;
        }
        else {
            // t + interval
            _v2[0] = quadraticAt(x0, x1, x2, next);
            _v2[1] = quadraticAt(y0, y1, y2, next);
            var d2 = distSquare(_v2, _v0);
            if (next <= 1 && d2 < d) {
                t = next;
                d = d2;
            }
            else {
                interval *= 0.5;
            }
        }
    }
    // t
    if (out) {
        out[0] = quadraticAt(x0, x1, x2, t);
        out[1] = quadraticAt(y0, y1, y2, t);
    }
    // console.log(interval, i);
    return mathSqrt(d);
}

/**
 * @author Yi Shen(https://github.com/pissang)
 */

var mathMin$1 = Math.min;
var mathMax$1 = Math.max;
var mathSin = Math.sin;
var mathCos = Math.cos;
var PI2 = Math.PI * 2;

var start = create$2();
var end = create$2();
var extremity = create$2();

/**
 * @memberOf module:zrender/core/bbox
 * @param {number} x0
 * @param {number} y0
 * @param {number} x1
 * @param {number} y1
 * @param {Array.<number>} min
 * @param {Array.<number>} max
 */
function fromLine(x0, y0, x1, y1, min$$1, max$$1) {
    min$$1[0] = mathMin$1(x0, x1);
    min$$1[1] = mathMin$1(y0, y1);
    max$$1[0] = mathMax$1(x0, x1);
    max$$1[1] = mathMax$1(y0, y1);
}

var xDim = [];
var yDim = [];
/**
 * 从三阶贝塞尔曲线(p0, p1, p2, p3)中计算出最小包围盒，写入`min`和`max`中
 * @memberOf module:zrender/core/bbox
 * @param {number} x0
 * @param {number} y0
 * @param {number} x1
 * @param {number} y1
 * @param {number} x2
 * @param {number} y2
 * @param {number} x3
 * @param {number} y3
 * @param {Array.<number>} min
 * @param {Array.<number>} max
 */
function fromCubic(
    x0, y0, x1, y1, x2, y2, x3, y3, min$$1, max$$1
) {
    var cubicExtrema$$1 = cubicExtrema;
    var cubicAt$$1 = cubicAt;
    var i;
    var n = cubicExtrema$$1(x0, x1, x2, x3, xDim);
    min$$1[0] = Infinity;
    min$$1[1] = Infinity;
    max$$1[0] = -Infinity;
    max$$1[1] = -Infinity;

    for (i = 0; i < n; i++) {
        var x = cubicAt$$1(x0, x1, x2, x3, xDim[i]);
        min$$1[0] = mathMin$1(x, min$$1[0]);
        max$$1[0] = mathMax$1(x, max$$1[0]);
    }
    n = cubicExtrema$$1(y0, y1, y2, y3, yDim);
    for (i = 0; i < n; i++) {
        var y = cubicAt$$1(y0, y1, y2, y3, yDim[i]);
        min$$1[1] = mathMin$1(y, min$$1[1]);
        max$$1[1] = mathMax$1(y, max$$1[1]);
    }

    min$$1[0] = mathMin$1(x0, min$$1[0]);
    max$$1[0] = mathMax$1(x0, max$$1[0]);
    min$$1[0] = mathMin$1(x3, min$$1[0]);
    max$$1[0] = mathMax$1(x3, max$$1[0]);

    min$$1[1] = mathMin$1(y0, min$$1[1]);
    max$$1[1] = mathMax$1(y0, max$$1[1]);
    min$$1[1] = mathMin$1(y3, min$$1[1]);
    max$$1[1] = mathMax$1(y3, max$$1[1]);
}

/**
 * 从二阶贝塞尔曲线(p0, p1, p2)中计算出最小包围盒，写入`min`和`max`中
 * @memberOf module:zrender/core/bbox
 * @param {number} x0
 * @param {number} y0
 * @param {number} x1
 * @param {number} y1
 * @param {number} x2
 * @param {number} y2
 * @param {Array.<number>} min
 * @param {Array.<number>} max
 */
function fromQuadratic(x0, y0, x1, y1, x2, y2, min$$1, max$$1) {
    var quadraticExtremum$$1 = quadraticExtremum;
    var quadraticAt$$1 = quadraticAt;
    // Find extremities, where derivative in x dim or y dim is zero
    var tx =
        mathMax$1(
            mathMin$1(quadraticExtremum$$1(x0, x1, x2), 1), 0
        );
    var ty =
        mathMax$1(
            mathMin$1(quadraticExtremum$$1(y0, y1, y2), 1), 0
        );

    var x = quadraticAt$$1(x0, x1, x2, tx);
    var y = quadraticAt$$1(y0, y1, y2, ty);

    min$$1[0] = mathMin$1(x0, x2, x);
    min$$1[1] = mathMin$1(y0, y2, y);
    max$$1[0] = mathMax$1(x0, x2, x);
    max$$1[1] = mathMax$1(y0, y2, y);
}

/**
 * 从圆弧中计算出最小包围盒，写入`min`和`max`中
 * @method
 * @memberOf module:zrender/core/bbox
 * @param {number} x
 * @param {number} y
 * @param {number} rx
 * @param {number} ry
 * @param {number} startAngle
 * @param {number} endAngle
 * @param {number} anticlockwise
 * @param {Array.<number>} min
 * @param {Array.<number>} max
 */
function fromArc(
    x, y, rx, ry, startAngle, endAngle, anticlockwise, min$$1, max$$1
) {
    var vec2Min = min;
    var vec2Max = max;

    var diff = Math.abs(startAngle - endAngle);


    if (diff % PI2 < 1e-4 && diff > 1e-4) {
        // Is a circle
        min$$1[0] = x - rx;
        min$$1[1] = y - ry;
        max$$1[0] = x + rx;
        max$$1[1] = y + ry;
        return;
    }

    start[0] = mathCos(startAngle) * rx + x;
    start[1] = mathSin(startAngle) * ry + y;

    end[0] = mathCos(endAngle) * rx + x;
    end[1] = mathSin(endAngle) * ry + y;

    vec2Min(min$$1, start, end);
    vec2Max(max$$1, start, end);

    // Thresh to [0, Math.PI * 2]
    startAngle = startAngle % (PI2);
    if (startAngle < 0) {
        startAngle = startAngle + PI2;
    }
    endAngle = endAngle % (PI2);
    if (endAngle < 0) {
        endAngle = endAngle + PI2;
    }

    if (startAngle > endAngle && !anticlockwise) {
        endAngle += PI2;
    }
    else if (startAngle < endAngle && anticlockwise) {
        startAngle += PI2;
    }
    if (anticlockwise) {
        var tmp = endAngle;
        endAngle = startAngle;
        startAngle = tmp;
    }

    // var number = 0;
    // var step = (anticlockwise ? -Math.PI : Math.PI) / 2;
    for (var angle = 0; angle < endAngle; angle += Math.PI / 2) {
        if (angle > startAngle) {
            extremity[0] = mathCos(angle) * rx + x;
            extremity[1] = mathSin(angle) * ry + y;

            vec2Min(min$$1, extremity, min$$1);
            vec2Max(max$$1, extremity, max$$1);
        }
    }
}

/**
 * Path 代理，可以在`buildPath`中用于替代`ctx`, 会保存每个path操作的命令到pathCommands属性中
 * 可以用于 isInsidePath 判断以及获取boundingRect
 *
 * @module zrender/core/PathProxy
 * @author Yi Shen (http://www.github.com/pissang)
 */

var CMD = {
    M: 1,
    L: 2,
    C: 3,
    Q: 4,
    A: 5,
    Z: 6,
    // Rect
    R: 7
};

// var CMD_MEM_SIZE = {
//     M: 3,
//     L: 3,
//     C: 7,
//     Q: 5,
//     A: 9,
//     R: 5,
//     Z: 1
// };

var min$1 = [];
var max$1 = [];
var min2 = [];
var max2 = [];
var mathMin$2 = Math.min;
var mathMax$2 = Math.max;
var mathCos$1 = Math.cos;
var mathSin$1 = Math.sin;
var mathSqrt$1 = Math.sqrt;
var mathAbs = Math.abs;

var hasTypedArray = typeof Float32Array != 'undefined';

/**
 * @alias module:zrender/core/PathProxy
 * @constructor
 */
var PathProxy = function (notSaveData) {

    this._saveData = !(notSaveData || false);

    if (this._saveData) {
        /**
         * Path data. Stored as flat array
         * @type {Array.<Object>}
         */
        this.data = [];
    }

    this._ctx = null;
};

/**
 * 快速计算Path包围盒（并不是最小包围盒）
 * @return {Object}
 */
PathProxy.prototype = {

    constructor: PathProxy,

    _xi: 0,
    _yi: 0,

    _x0: 0,
    _y0: 0,
    // Unit x, Unit y. Provide for avoiding drawing that too short line segment
    _ux: 0,
    _uy: 0,

    _len: 0,

    _lineDash: null,

    _dashOffset: 0,

    _dashIdx: 0,

    _dashSum: 0,

    /**
     * @readOnly
     */
    setScale: function (sx, sy) {
        this._ux = mathAbs(1 / devicePixelRatio / sx) || 0;
        this._uy = mathAbs(1 / devicePixelRatio / sy) || 0;
    },

    getContext: function () {
        return this._ctx;
    },

    /**
     * @param  {CanvasRenderingContext2D} ctx
     * @return {module:zrender/core/PathProxy}
     */
    beginPath: function (ctx) {

        this._ctx = ctx;

        ctx && ctx.beginPath();

        ctx && (this.dpr = ctx.dpr);

        // Reset
        if (this._saveData) {
            this._len = 0;
        }

        if (this._lineDash) {
            this._lineDash = null;

            this._dashOffset = 0;
        }

        return this;
    },

    /**
     * @param  {number} x
     * @param  {number} y
     * @return {module:zrender/core/PathProxy}
     */
    moveTo: function (x, y) {
        this.addData(CMD.M, x, y);
        this._ctx && this._ctx.moveTo(x, y);

        // x0, y0, xi, yi 是记录在 _dashedXXXXTo 方法中使用
        // xi, yi 记录当前点, x0, y0 在 closePath 的时候回到起始点。
        // 有可能在 beginPath 之后直接调用 lineTo，这时候 x0, y0 需要
        // 在 lineTo 方法中记录，这里先不考虑这种情况，dashed line 也只在 IE10- 中不支持
        this._x0 = x;
        this._y0 = y;

        this._xi = x;
        this._yi = y;

        return this;
    },

    /**
     * @param  {number} x
     * @param  {number} y
     * @return {module:zrender/core/PathProxy}
     */
    lineTo: function (x, y) {
        var exceedUnit = mathAbs(x - this._xi) > this._ux
            || mathAbs(y - this._yi) > this._uy
            // Force draw the first segment
            || this._len < 5;

        this.addData(CMD.L, x, y);

        if (this._ctx && exceedUnit) {
            this._needsDash() ? this._dashedLineTo(x, y)
                : this._ctx.lineTo(x, y);
        }
        if (exceedUnit) {
            this._xi = x;
            this._yi = y;
        }

        return this;
    },

    /**
     * @param  {number} x1
     * @param  {number} y1
     * @param  {number} x2
     * @param  {number} y2
     * @param  {number} x3
     * @param  {number} y3
     * @return {module:zrender/core/PathProxy}
     */
    bezierCurveTo: function (x1, y1, x2, y2, x3, y3) {
        this.addData(CMD.C, x1, y1, x2, y2, x3, y3);
        if (this._ctx) {
            this._needsDash() ? this._dashedBezierTo(x1, y1, x2, y2, x3, y3)
                : this._ctx.bezierCurveTo(x1, y1, x2, y2, x3, y3);
        }
        this._xi = x3;
        this._yi = y3;
        return this;
    },

    /**
     * @param  {number} x1
     * @param  {number} y1
     * @param  {number} x2
     * @param  {number} y2
     * @return {module:zrender/core/PathProxy}
     */
    quadraticCurveTo: function (x1, y1, x2, y2) {
        this.addData(CMD.Q, x1, y1, x2, y2);
        if (this._ctx) {
            this._needsDash() ? this._dashedQuadraticTo(x1, y1, x2, y2)
                : this._ctx.quadraticCurveTo(x1, y1, x2, y2);
        }
        this._xi = x2;
        this._yi = y2;
        return this;
    },

    /**
     * @param  {number} cx
     * @param  {number} cy
     * @param  {number} r
     * @param  {number} startAngle
     * @param  {number} endAngle
     * @param  {boolean} anticlockwise
     * @return {module:zrender/core/PathProxy}
     */
    arc: function (cx, cy, r, startAngle, endAngle, anticlockwise) {
        this.addData(
            CMD.A, cx, cy, r, r, startAngle, endAngle - startAngle, 0, anticlockwise ? 0 : 1
        );
        this._ctx && this._ctx.arc(cx, cy, r, startAngle, endAngle, anticlockwise);

        this._xi = mathCos$1(endAngle) * r + cx;
        this._yi = mathSin$1(endAngle) * r + cx;
        return this;
    },

    // TODO
    arcTo: function (x1, y1, x2, y2, radius) {
        if (this._ctx) {
            this._ctx.arcTo(x1, y1, x2, y2, radius);
        }
        return this;
    },

    // TODO
    rect: function (x, y, w, h) {
        this._ctx && this._ctx.rect(x, y, w, h);
        this.addData(CMD.R, x, y, w, h);
        return this;
    },

    /**
     * @return {module:zrender/core/PathProxy}
     */
    closePath: function () {
        this.addData(CMD.Z);

        var ctx = this._ctx;
        var x0 = this._x0;
        var y0 = this._y0;
        if (ctx) {
            this._needsDash() && this._dashedLineTo(x0, y0);
            ctx.closePath();
        }

        this._xi = x0;
        this._yi = y0;
        return this;
    },

    /**
     * Context 从外部传入，因为有可能是 rebuildPath 完之后再 fill。
     * stroke 同样
     * @param {CanvasRenderingContext2D} ctx
     * @return {module:zrender/core/PathProxy}
     */
    fill: function (ctx) {
        ctx && ctx.fill();
        this.toStatic();
    },

    /**
     * @param {CanvasRenderingContext2D} ctx
     * @return {module:zrender/core/PathProxy}
     */
    stroke: function (ctx) {
        ctx && ctx.stroke();
        this.toStatic();
    },

    /**
     * 必须在其它绘制命令前调用
     * Must be invoked before all other path drawing methods
     * @return {module:zrender/core/PathProxy}
     */
    setLineDash: function (lineDash) {
        if (lineDash instanceof Array) {
            this._lineDash = lineDash;

            this._dashIdx = 0;

            var lineDashSum = 0;
            for (var i = 0; i < lineDash.length; i++) {
                lineDashSum += lineDash[i];
            }
            this._dashSum = lineDashSum;
        }
        return this;
    },

    /**
     * 必须在其它绘制命令前调用
     * Must be invoked before all other path drawing methods
     * @return {module:zrender/core/PathProxy}
     */
    setLineDashOffset: function (offset) {
        this._dashOffset = offset;
        return this;
    },

    /**
     *
     * @return {boolean}
     */
    len: function () {
        return this._len;
    },

    /**
     * 直接设置 Path 数据
     */
    setData: function (data) {
        var this$1 = this;


        var len$$1 = data.length;

        if (! (this.data && this.data.length == len$$1) && hasTypedArray) {
            this.data = new Float32Array(len$$1);
        }

        for (var i = 0; i < len$$1; i++) {
            this$1.data[i] = data[i];
        }

        this._len = len$$1;
    },

    /**
     * 添加子路径
     * @param {module:zrender/core/PathProxy|Array.<module:zrender/core/PathProxy>} path
     */
    appendPath: function (path) {
        var this$1 = this;

        if (!(path instanceof Array)) {
            path = [path];
        }
        var len$$1 = path.length;
        var appendSize = 0;
        var offset = this._len;
        for (var i = 0; i < len$$1; i++) {
            appendSize += path[i].len();
        }
        if (hasTypedArray && (this.data instanceof Float32Array)) {
            this.data = new Float32Array(offset + appendSize);
        }
        for (var i = 0; i < len$$1; i++) {
            var appendPathData = path[i].data;
            for (var k = 0; k < appendPathData.length; k++) {
                this$1.data[offset++] = appendPathData[k];
            }
        }
        this._len = offset;
    },

    /**
     * 填充 Path 数据。
     * 尽量复用而不申明新的数组。大部分图形重绘的指令数据长度都是不变的。
     */
    addData: function (cmd) {
        var arguments$1 = arguments;
        var this$1 = this;

        if (!this._saveData) {
            return;
        }

        var data = this.data;
        if (this._len + arguments.length > data.length) {
            // 因为之前的数组已经转换成静态的 Float32Array
            // 所以不够用时需要扩展一个新的动态数组
            this._expandData();
            data = this.data;
        }
        for (var i = 0; i < arguments.length; i++) {
            data[this$1._len++] = arguments$1[i];
        }

        this._prevCmd = cmd;
    },

    _expandData: function () {
        var this$1 = this;

        // Only if data is Float32Array
        if (!(this.data instanceof Array)) {
            var newData = [];
            for (var i = 0; i < this._len; i++) {
                newData[i] = this$1.data[i];
            }
            this.data = newData;
        }
    },

    /**
     * If needs js implemented dashed line
     * @return {boolean}
     * @private
     */
    _needsDash: function () {
        return this._lineDash;
    },

    _dashedLineTo: function (x1, y1) {
        var this$1 = this;

        var dashSum = this._dashSum;
        var offset = this._dashOffset;
        var lineDash = this._lineDash;
        var ctx = this._ctx;

        var x0 = this._xi;
        var y0 = this._yi;
        var dx = x1 - x0;
        var dy = y1 - y0;
        var dist$$1 = mathSqrt$1(dx * dx + dy * dy);
        var x = x0;
        var y = y0;
        var dash;
        var nDash = lineDash.length;
        var idx;
        dx /= dist$$1;
        dy /= dist$$1;

        if (offset < 0) {
            // Convert to positive offset
            offset = dashSum + offset;
        }
        offset %= dashSum;
        x -= offset * dx;
        y -= offset * dy;

        while ((dx > 0 && x <= x1) || (dx < 0 && x >= x1)
        || (dx == 0 && ((dy > 0 && y <= y1) || (dy < 0 && y >= y1)))) {
            idx = this$1._dashIdx;
            dash = lineDash[idx];
            x += dx * dash;
            y += dy * dash;
            this$1._dashIdx = (idx + 1) % nDash;
            // Skip positive offset
            if ((dx > 0 && x < x0) || (dx < 0 && x > x0) || (dy > 0 && y < y0) || (dy < 0 && y > y0)) {
                continue;
            }
            ctx[idx % 2 ? 'moveTo' : 'lineTo'](
                dx >= 0 ? mathMin$2(x, x1) : mathMax$2(x, x1),
                dy >= 0 ? mathMin$2(y, y1) : mathMax$2(y, y1)
            );
        }
        // Offset for next lineTo
        dx = x - x1;
        dy = y - y1;
        this._dashOffset = -mathSqrt$1(dx * dx + dy * dy);
    },

    // Not accurate dashed line to
    _dashedBezierTo: function (x1, y1, x2, y2, x3, y3) {
        var dashSum = this._dashSum;
        var offset = this._dashOffset;
        var lineDash = this._lineDash;
        var ctx = this._ctx;

        var x0 = this._xi;
        var y0 = this._yi;
        var t;
        var dx;
        var dy;
        var cubicAt$$1 = cubicAt;
        var bezierLen = 0;
        var idx = this._dashIdx;
        var nDash = lineDash.length;

        var x;
        var y;

        var tmpLen = 0;

        if (offset < 0) {
            // Convert to positive offset
            offset = dashSum + offset;
        }
        offset %= dashSum;
        // Bezier approx length
        for (t = 0; t < 1; t += 0.1) {
            dx = cubicAt$$1(x0, x1, x2, x3, t + 0.1)
                - cubicAt$$1(x0, x1, x2, x3, t);
            dy = cubicAt$$1(y0, y1, y2, y3, t + 0.1)
                - cubicAt$$1(y0, y1, y2, y3, t);
            bezierLen += mathSqrt$1(dx * dx + dy * dy);
        }

        // Find idx after add offset
        for (; idx < nDash; idx++) {
            tmpLen += lineDash[idx];
            if (tmpLen > offset) {
                break;
            }
        }
        t = (tmpLen - offset) / bezierLen;

        while (t <= 1) {

            x = cubicAt$$1(x0, x1, x2, x3, t);
            y = cubicAt$$1(y0, y1, y2, y3, t);

            // Use line to approximate dashed bezier
            // Bad result if dash is long
            idx % 2 ? ctx.moveTo(x, y)
                : ctx.lineTo(x, y);

            t += lineDash[idx] / bezierLen;

            idx = (idx + 1) % nDash;
        }

        // Finish the last segment and calculate the new offset
        (idx % 2 !== 0) && ctx.lineTo(x3, y3);
        dx = x3 - x;
        dy = y3 - y;
        this._dashOffset = -mathSqrt$1(dx * dx + dy * dy);
    },

    _dashedQuadraticTo: function (x1, y1, x2, y2) {
        // Convert quadratic to cubic using degree elevation
        var x3 = x2;
        var y3 = y2;
        x2 = (x2 + 2 * x1) / 3;
        y2 = (y2 + 2 * y1) / 3;
        x1 = (this._xi + 2 * x1) / 3;
        y1 = (this._yi + 2 * y1) / 3;

        this._dashedBezierTo(x1, y1, x2, y2, x3, y3);
    },

    /**
     * 转成静态的 Float32Array 减少堆内存占用
     * Convert dynamic array to static Float32Array
     */
    toStatic: function () {
        var data = this.data;
        if (data instanceof Array) {
            data.length = this._len;
            if (hasTypedArray) {
                this.data = new Float32Array(data);
            }
        }
    },

    /**
     * @return {module:zrender/core/BoundingRect}
     */
    getBoundingRect: function () {
        min$1[0] = min$1[1] = min2[0] = min2[1] = Number.MAX_VALUE;
        max$1[0] = max$1[1] = max2[0] = max2[1] = -Number.MAX_VALUE;

        var data = this.data;
        var xi = 0;
        var yi = 0;
        var x0 = 0;
        var y0 = 0;

        for (var i = 0; i < data.length;) {
            var cmd = data[i++];

            if (i == 1) {
                // 如果第一个命令是 L, C, Q
                // 则 previous point 同绘制命令的第一个 point
                //
                // 第一个命令为 Arc 的情况下会在后面特殊处理
                xi = data[i];
                yi = data[i + 1];

                x0 = xi;
                y0 = yi;
            }

            switch (cmd) {
                case CMD.M:
                    // moveTo 命令重新创建一个新的 subpath, 并且更新新的起点
                    // 在 closePath 的时候使用
                    x0 = data[i++];
                    y0 = data[i++];
                    xi = x0;
                    yi = y0;
                    min2[0] = x0;
                    min2[1] = y0;
                    max2[0] = x0;
                    max2[1] = y0;
                    break;
                case CMD.L:
                    fromLine(xi, yi, data[i], data[i + 1], min2, max2);
                    xi = data[i++];
                    yi = data[i++];
                    break;
                case CMD.C:
                    fromCubic(
                        xi, yi, data[i++], data[i++], data[i++], data[i++], data[i], data[i + 1],
                        min2, max2
                    );
                    xi = data[i++];
                    yi = data[i++];
                    break;
                case CMD.Q:
                    fromQuadratic(
                        xi, yi, data[i++], data[i++], data[i], data[i + 1],
                        min2, max2
                    );
                    xi = data[i++];
                    yi = data[i++];
                    break;
                case CMD.A:
                    // TODO Arc 判断的开销比较大
                    var cx = data[i++];
                    var cy = data[i++];
                    var rx = data[i++];
                    var ry = data[i++];
                    var startAngle = data[i++];
                    var endAngle = data[i++] + startAngle;
                    // TODO Arc 旋转
                    var psi = data[i++];
                    var anticlockwise = 1 - data[i++];

                    if (i == 1) {
                        // 直接使用 arc 命令
                        // 第一个命令起点还未定义
                        x0 = mathCos$1(startAngle) * rx + cx;
                        y0 = mathSin$1(startAngle) * ry + cy;
                    }

                    fromArc(
                        cx, cy, rx, ry, startAngle, endAngle,
                        anticlockwise, min2, max2
                    );

                    xi = mathCos$1(endAngle) * rx + cx;
                    yi = mathSin$1(endAngle) * ry + cy;
                    break;
                case CMD.R:
                    x0 = xi = data[i++];
                    y0 = yi = data[i++];
                    var width = data[i++];
                    var height = data[i++];
                    // Use fromLine
                    fromLine(x0, y0, x0 + width, y0 + height, min2, max2);
                    break;
                case CMD.Z:
                    xi = x0;
                    yi = y0;
                    break;
            }

            // Union
            min(min$1, min$1, min2);
            max(max$1, max$1, max2);
        }

        // No data
        if (i === 0) {
            min$1[0] = min$1[1] = max$1[0] = max$1[1] = 0;
        }

        return new BoundingRect(
            min$1[0], min$1[1], max$1[0] - min$1[0], max$1[1] - min$1[1]
        );
    },

    /**
     * Rebuild path from current data
     * Rebuild path will not consider javascript implemented line dash.
     * @param {CanvasRenderingContext2D} ctx
     */
    rebuildPath: function (ctx) {
        var d = this.data;
        var x0, y0;
        var xi, yi;
        var x, y;
        var ux = this._ux;
        var uy = this._uy;
        var len$$1 = this._len;
        for (var i = 0; i < len$$1;) {
            var cmd = d[i++];

            if (i == 1) {
                // 如果第一个命令是 L, C, Q
                // 则 previous point 同绘制命令的第一个 point
                //
                // 第一个命令为 Arc 的情况下会在后面特殊处理
                xi = d[i];
                yi = d[i + 1];

                x0 = xi;
                y0 = yi;
            }
            switch (cmd) {
                case CMD.M:
                    x0 = xi = d[i++];
                    y0 = yi = d[i++];
                    ctx.moveTo(xi, yi);
                    break;
                case CMD.L:
                    x = d[i++];
                    y = d[i++];
                    // Not draw too small seg between
                    if (mathAbs(x - xi) > ux || mathAbs(y - yi) > uy || i === len$$1 - 1) {
                        ctx.lineTo(x, y);
                        xi = x;
                        yi = y;
                    }
                    break;
                case CMD.C:
                    ctx.bezierCurveTo(
                        d[i++], d[i++], d[i++], d[i++], d[i++], d[i++]
                    );
                    xi = d[i - 2];
                    yi = d[i - 1];
                    break;
                case CMD.Q:
                    ctx.quadraticCurveTo(d[i++], d[i++], d[i++], d[i++]);
                    xi = d[i - 2];
                    yi = d[i - 1];
                    break;
                case CMD.A:
                    var cx = d[i++];
                    var cy = d[i++];
                    var rx = d[i++];
                    var ry = d[i++];
                    var theta = d[i++];
                    var dTheta = d[i++];
                    var psi = d[i++];
                    var fs = d[i++];
                    var r = (rx > ry) ? rx : ry;
                    var scaleX = (rx > ry) ? 1 : rx / ry;
                    var scaleY = (rx > ry) ? ry / rx : 1;
                    var isEllipse = Math.abs(rx - ry) > 1e-3;
                    var endAngle = theta + dTheta;
                    if (isEllipse) {
                        ctx.translate(cx, cy);
                        ctx.rotate(psi);
                        ctx.scale(scaleX, scaleY);
                        ctx.arc(0, 0, r, theta, endAngle, 1 - fs);
                        ctx.scale(1 / scaleX, 1 / scaleY);
                        ctx.rotate(-psi);
                        ctx.translate(-cx, -cy);
                    }
                    else {
                        ctx.arc(cx, cy, r, theta, endAngle, 1 - fs);
                    }

                    if (i == 1) {
                        // 直接使用 arc 命令
                        // 第一个命令起点还未定义
                        x0 = mathCos$1(theta) * rx + cx;
                        y0 = mathSin$1(theta) * ry + cy;
                    }
                    xi = mathCos$1(endAngle) * rx + cx;
                    yi = mathSin$1(endAngle) * ry + cy;
                    break;
                case CMD.R:
                    x0 = xi = d[i];
                    y0 = yi = d[i + 1];
                    ctx.rect(d[i++], d[i++], d[i++], d[i++]);
                    break;
                case CMD.Z:
                    ctx.closePath();
                    xi = x0;
                    yi = y0;
            }
        }
    }
};

PathProxy.CMD = CMD;

/**
 * 线段包含判断
 * @param  {number}  x0
 * @param  {number}  y0
 * @param  {number}  x1
 * @param  {number}  y1
 * @param  {number}  lineWidth
 * @param  {number}  x
 * @param  {number}  y
 * @return {boolean}
 */
function containStroke(x0, y0, x1, y1, lineWidth, x, y) {
    if (lineWidth === 0) {
        return false;
    }
    var _l = lineWidth;
    var _a = 0;
    var _b = x0;
    // Quick reject
    if (
        (y > y0 + _l && y > y1 + _l)
        || (y < y0 - _l && y < y1 - _l)
        || (x > x0 + _l && x > x1 + _l)
        || (x < x0 - _l && x < x1 - _l)
    ) {
        return false;
    }

    if (x0 !== x1) {
        _a = (y0 - y1) / (x0 - x1);
        _b = (x0 * y1 - x1 * y0) / (x0 - x1) ;
    }
    else {
        return Math.abs(x - x0) <= _l / 2;
    }
    var tmp = _a * x - y + _b;
    var _s = tmp * tmp / (_a * _a + 1);
    return _s <= _l / 2 * _l / 2;
}

/**
 * 三次贝塞尔曲线描边包含判断
 * @param  {number}  x0
 * @param  {number}  y0
 * @param  {number}  x1
 * @param  {number}  y1
 * @param  {number}  x2
 * @param  {number}  y2
 * @param  {number}  x3
 * @param  {number}  y3
 * @param  {number}  lineWidth
 * @param  {number}  x
 * @param  {number}  y
 * @return {boolean}
 */
function containStroke$1(x0, y0, x1, y1, x2, y2, x3, y3, lineWidth, x, y) {
    if (lineWidth === 0) {
        return false;
    }
    var _l = lineWidth;
    // Quick reject
    if (
        (y > y0 + _l && y > y1 + _l && y > y2 + _l && y > y3 + _l)
        || (y < y0 - _l && y < y1 - _l && y < y2 - _l && y < y3 - _l)
        || (x > x0 + _l && x > x1 + _l && x > x2 + _l && x > x3 + _l)
        || (x < x0 - _l && x < x1 - _l && x < x2 - _l && x < x3 - _l)
    ) {
        return false;
    }
    var d = cubicProjectPoint(
        x0, y0, x1, y1, x2, y2, x3, y3,
        x, y, null
    );
    return d <= _l / 2;
}

/**
 * 二次贝塞尔曲线描边包含判断
 * @param  {number}  x0
 * @param  {number}  y0
 * @param  {number}  x1
 * @param  {number}  y1
 * @param  {number}  x2
 * @param  {number}  y2
 * @param  {number}  lineWidth
 * @param  {number}  x
 * @param  {number}  y
 * @return {boolean}
 */
function containStroke$2(x0, y0, x1, y1, x2, y2, lineWidth, x, y) {
    if (lineWidth === 0) {
        return false;
    }
    var _l = lineWidth;
    // Quick reject
    if (
        (y > y0 + _l && y > y1 + _l && y > y2 + _l)
        || (y < y0 - _l && y < y1 - _l && y < y2 - _l)
        || (x > x0 + _l && x > x1 + _l && x > x2 + _l)
        || (x < x0 - _l && x < x1 - _l && x < x2 - _l)
    ) {
        return false;
    }
    var d = quadraticProjectPoint(
        x0, y0, x1, y1, x2, y2,
        x, y, null
    );
    return d <= _l / 2;
}

var PI2$1 = Math.PI * 2;

function normalizeRadian(angle) {
    angle %= PI2$1;
    if (angle < 0) {
        angle += PI2$1;
    }
    return angle;
}

var PI2$2 = Math.PI * 2;

/**
 * 圆弧描边包含判断
 * @param  {number}  cx
 * @param  {number}  cy
 * @param  {number}  r
 * @param  {number}  startAngle
 * @param  {number}  endAngle
 * @param  {boolean}  anticlockwise
 * @param  {number} lineWidth
 * @param  {number}  x
 * @param  {number}  y
 * @return {Boolean}
 */
function containStroke$3(
    cx, cy, r, startAngle, endAngle, anticlockwise,
    lineWidth, x, y
) {

    if (lineWidth === 0) {
        return false;
    }
    var _l = lineWidth;

    x -= cx;
    y -= cy;
    var d = Math.sqrt(x * x + y * y);

    if ((d - _l > r) || (d + _l < r)) {
        return false;
    }
    if (Math.abs(startAngle - endAngle) % PI2$2 < 1e-4) {
        // Is a circle
        return true;
    }
    if (anticlockwise) {
        var tmp = startAngle;
        startAngle = normalizeRadian(endAngle);
        endAngle = normalizeRadian(tmp);
    } else {
        startAngle = normalizeRadian(startAngle);
        endAngle = normalizeRadian(endAngle);
    }
    if (startAngle > endAngle) {
        endAngle += PI2$2;
    }

    var angle = Math.atan2(y, x);
    if (angle < 0) {
        angle += PI2$2;
    }
    return (angle >= startAngle && angle <= endAngle)
        || (angle + PI2$2 >= startAngle && angle + PI2$2 <= endAngle);
}

function windingLine(x0, y0, x1, y1, x, y) {
    if ((y > y0 && y > y1) || (y < y0 && y < y1)) {
        return 0;
    }
    // Ignore horizontal line
    if (y1 === y0) {
        return 0;
    }
    var dir = y1 < y0 ? 1 : -1;
    var t = (y - y0) / (y1 - y0);

    // Avoid winding error when intersection point is the connect point of two line of polygon
    if (t === 1 || t === 0) {
        dir = y1 < y0 ? 0.5 : -0.5;
    }

    var x_ = t * (x1 - x0) + x0;

    return x_ > x ? dir : 0;
}

var CMD$1 = PathProxy.CMD;
var PI2$3 = Math.PI * 2;

var EPSILON$2 = 1e-4;

function isAroundEqual(a, b) {
    return Math.abs(a - b) < EPSILON$2;
}

// 临时数组
var roots = [-1, -1, -1];
var extrema = [-1, -1];

function swapExtrema() {
    var tmp = extrema[0];
    extrema[0] = extrema[1];
    extrema[1] = tmp;
}

function windingCubic(x0, y0, x1, y1, x2, y2, x3, y3, x, y) {
    // Quick reject
    if (
        (y > y0 && y > y1 && y > y2 && y > y3)
        || (y < y0 && y < y1 && y < y2 && y < y3)
    ) {
        return 0;
    }
    var nRoots = cubicRootAt(y0, y1, y2, y3, y, roots);
    if (nRoots === 0) {
        return 0;
    }
    else {
        var w = 0;
        var nExtrema = -1;
        var y0_, y1_;
        for (var i = 0; i < nRoots; i++) {
            var t = roots[i];

            // Avoid winding error when intersection point is the connect point of two line of polygon
            var unit = (t === 0 || t === 1) ? 0.5 : 1;

            var x_ = cubicAt(x0, x1, x2, x3, t);
            if (x_ < x) { // Quick reject
                continue;
            }
            if (nExtrema < 0) {
                nExtrema = cubicExtrema(y0, y1, y2, y3, extrema);
                if (extrema[1] < extrema[0] && nExtrema > 1) {
                    swapExtrema();
                }
                y0_ = cubicAt(y0, y1, y2, y3, extrema[0]);
                if (nExtrema > 1) {
                    y1_ = cubicAt(y0, y1, y2, y3, extrema[1]);
                }
            }
            if (nExtrema == 2) {
                // 分成三段单调函数
                if (t < extrema[0]) {
                    w += y0_ < y0 ? unit : -unit;
                }
                else if (t < extrema[1]) {
                    w += y1_ < y0_ ? unit : -unit;
                }
                else {
                    w += y3 < y1_ ? unit : -unit;
                }
            }
            else {
                // 分成两段单调函数
                if (t < extrema[0]) {
                    w += y0_ < y0 ? unit : -unit;
                }
                else {
                    w += y3 < y0_ ? unit : -unit;
                }
            }
        }
        return w;
    }
}

function windingQuadratic(x0, y0, x1, y1, x2, y2, x, y) {
    // Quick reject
    if (
        (y > y0 && y > y1 && y > y2)
        || (y < y0 && y < y1 && y < y2)
    ) {
        return 0;
    }
    var nRoots = quadraticRootAt(y0, y1, y2, y, roots);
    if (nRoots === 0) {
        return 0;
    }
    else {
        var t = quadraticExtremum(y0, y1, y2);
        if (t >= 0 && t <= 1) {
            var w = 0;
            var y_ = quadraticAt(y0, y1, y2, t);
            for (var i = 0; i < nRoots; i++) {
                // Remove one endpoint.
                var unit = (roots[i] === 0 || roots[i] === 1) ? 0.5 : 1;

                var x_ = quadraticAt(x0, x1, x2, roots[i]);
                if (x_ < x) {   // Quick reject
                    continue;
                }
                if (roots[i] < t) {
                    w += y_ < y0 ? unit : -unit;
                }
                else {
                    w += y2 < y_ ? unit : -unit;
                }
            }
            return w;
        }
        else {
            // Remove one endpoint.
            var unit = (roots[0] === 0 || roots[0] === 1) ? 0.5 : 1;

            var x_ = quadraticAt(x0, x1, x2, roots[0]);
            if (x_ < x) {   // Quick reject
                return 0;
            }
            return y2 < y0 ? unit : -unit;
        }
    }
}

// TODO
// Arc 旋转
function windingArc(
    cx, cy, r, startAngle, endAngle, anticlockwise, x, y
) {
    y -= cy;
    if (y > r || y < -r) {
        return 0;
    }
    var tmp = Math.sqrt(r * r - y * y);
    roots[0] = -tmp;
    roots[1] = tmp;

    var diff = Math.abs(startAngle - endAngle);
    if (diff < 1e-4) {
        return 0;
    }
    if (diff % PI2$3 < 1e-4) {
        // Is a circle
        startAngle = 0;
        endAngle = PI2$3;
        var dir = anticlockwise ? 1 : -1;
        if (x >= roots[0] + cx && x <= roots[1] + cx) {
            return dir;
        } else {
            return 0;
        }
    }

    if (anticlockwise) {
        var tmp = startAngle;
        startAngle = normalizeRadian(endAngle);
        endAngle = normalizeRadian(tmp);
    }
    else {
        startAngle = normalizeRadian(startAngle);
        endAngle = normalizeRadian(endAngle);
    }
    if (startAngle > endAngle) {
        endAngle += PI2$3;
    }

    var w = 0;
    for (var i = 0; i < 2; i++) {
        var x_ = roots[i];
        if (x_ + cx > x) {
            var angle = Math.atan2(y, x_);
            var dir = anticlockwise ? 1 : -1;
            if (angle < 0) {
                angle = PI2$3 + angle;
            }
            if (
                (angle >= startAngle && angle <= endAngle)
                || (angle + PI2$3 >= startAngle && angle + PI2$3 <= endAngle)
            ) {
                if (angle > Math.PI / 2 && angle < Math.PI * 1.5) {
                    dir = -dir;
                }
                w += dir;
            }
        }
    }
    return w;
}

function containPath(data, lineWidth, isStroke, x, y) {
    var w = 0;
    var xi = 0;
    var yi = 0;
    var x0 = 0;
    var y0 = 0;

    for (var i = 0; i < data.length;) {
        var cmd = data[i++];
        // Begin a new subpath
        if (cmd === CMD$1.M && i > 1) {
            // Close previous subpath
            if (!isStroke) {
                w += windingLine(xi, yi, x0, y0, x, y);
            }
            // 如果被任何一个 subpath 包含
            // if (w !== 0) {
            //     return true;
            // }
        }

        if (i == 1) {
            // 如果第一个命令是 L, C, Q
            // 则 previous point 同绘制命令的第一个 point
            //
            // 第一个命令为 Arc 的情况下会在后面特殊处理
            xi = data[i];
            yi = data[i + 1];

            x0 = xi;
            y0 = yi;
        }

        switch (cmd) {
            case CMD$1.M:
                // moveTo 命令重新创建一个新的 subpath, 并且更新新的起点
                // 在 closePath 的时候使用
                x0 = data[i++];
                y0 = data[i++];
                xi = x0;
                yi = y0;
                break;
            case CMD$1.L:
                if (isStroke) {
                    if (containStroke(xi, yi, data[i], data[i + 1], lineWidth, x, y)) {
                        return true;
                    }
                }
                else {
                    // NOTE 在第一个命令为 L, C, Q 的时候会计算出 NaN
                    w += windingLine(xi, yi, data[i], data[i + 1], x, y) || 0;
                }
                xi = data[i++];
                yi = data[i++];
                break;
            case CMD$1.C:
                if (isStroke) {
                    if (containStroke$1(xi, yi,
                        data[i++], data[i++], data[i++], data[i++], data[i], data[i + 1],
                        lineWidth, x, y
                    )) {
                        return true;
                    }
                }
                else {
                    w += windingCubic(
                        xi, yi,
                        data[i++], data[i++], data[i++], data[i++], data[i], data[i + 1],
                        x, y
                    ) || 0;
                }
                xi = data[i++];
                yi = data[i++];
                break;
            case CMD$1.Q:
                if (isStroke) {
                    if (containStroke$2(xi, yi,
                        data[i++], data[i++], data[i], data[i + 1],
                        lineWidth, x, y
                    )) {
                        return true;
                    }
                }
                else {
                    w += windingQuadratic(
                        xi, yi,
                        data[i++], data[i++], data[i], data[i + 1],
                        x, y
                    ) || 0;
                }
                xi = data[i++];
                yi = data[i++];
                break;
            case CMD$1.A:
                // TODO Arc 判断的开销比较大
                var cx = data[i++];
                var cy = data[i++];
                var rx = data[i++];
                var ry = data[i++];
                var theta = data[i++];
                var dTheta = data[i++];
                // TODO Arc 旋转
                var psi = data[i++];
                var anticlockwise = 1 - data[i++];
                var x1 = Math.cos(theta) * rx + cx;
                var y1 = Math.sin(theta) * ry + cy;
                // 不是直接使用 arc 命令
                if (i > 1) {
                    w += windingLine(xi, yi, x1, y1, x, y);
                }
                else {
                    // 第一个命令起点还未定义
                    x0 = x1;
                    y0 = y1;
                }
                // zr 使用scale来模拟椭圆, 这里也对x做一定的缩放
                var _x = (x - cx) * ry / rx + cx;
                if (isStroke) {
                    if (containStroke$3(
                        cx, cy, ry, theta, theta + dTheta, anticlockwise,
                        lineWidth, _x, y
                    )) {
                        return true;
                    }
                }
                else {
                    w += windingArc(
                        cx, cy, ry, theta, theta + dTheta, anticlockwise,
                        _x, y
                    );
                }
                xi = Math.cos(theta + dTheta) * rx + cx;
                yi = Math.sin(theta + dTheta) * ry + cy;
                break;
            case CMD$1.R:
                x0 = xi = data[i++];
                y0 = yi = data[i++];
                var width = data[i++];
                var height = data[i++];
                var x1 = x0 + width;
                var y1 = y0 + height;
                if (isStroke) {
                    if (containStroke(x0, y0, x1, y0, lineWidth, x, y)
                        || containStroke(x1, y0, x1, y1, lineWidth, x, y)
                        || containStroke(x1, y1, x0, y1, lineWidth, x, y)
                        || containStroke(x0, y1, x0, y0, lineWidth, x, y)
                    ) {
                        return true;
                    }
                }
                else {
                    // FIXME Clockwise ?
                    w += windingLine(x1, y0, x1, y1, x, y);
                    w += windingLine(x0, y1, x0, y0, x, y);
                }
                break;
            case CMD$1.Z:
                if (isStroke) {
                    if (containStroke(
                        xi, yi, x0, y0, lineWidth, x, y
                    )) {
                        return true;
                    }
                }
                else {
                    // Close a subpath
                    w += windingLine(xi, yi, x0, y0, x, y);
                    // 如果被任何一个 subpath 包含
                    // FIXME subpaths may overlap
                    // if (w !== 0) {
                    //     return true;
                    // }
                }
                xi = x0;
                yi = y0;
                break;
        }
    }
    if (!isStroke && !isAroundEqual(yi, y0)) {
        w += windingLine(xi, yi, x0, y0, x, y) || 0;
    }
    return w !== 0;
}

function contain(pathData, x, y) {
    return containPath(pathData, 0, false, x, y);
}

function containStroke$4(pathData, lineWidth, x, y) {
    return containPath(pathData, lineWidth, true, x, y);
}

var Pattern = function (image, repeat) {
    // Should do nothing more in this constructor. Because gradient can be
    // declard by `color: {image: ...}`, where this constructor will not be called.

    this.image = image;
    this.repeat = repeat;

    // Can be cloned
    this.type = 'pattern';
};

Pattern.prototype.getCanvasPattern = function (ctx) {
    return ctx.createPattern(this.image, this.repeat || 'repeat');
};

var getCanvasPattern = Pattern.prototype.getCanvasPattern;

var abs = Math.abs;

var pathProxyForDraw = new PathProxy(true);
/**
 * @alias module:zrender/graphic/Path
 * @extends module:zrender/graphic/Displayable
 * @constructor
 * @param {Object} opts
 */
function Path(opts) {
    Displayable.call(this, opts);

    /**
     * @type {module:zrender/core/PathProxy}
     * @readOnly
     */
    this.path = null;
}

Path.prototype = {

    constructor: Path,

    type: 'path',

    __dirtyPath: true,

    strokeContainThreshold: 5,

    brush: function (ctx, prevEl) {
        var style = this.style;
        var path = this.path || pathProxyForDraw;
        var hasStroke = style.hasStroke();
        var hasFill = style.hasFill();
        var fill = style.fill;
        var stroke = style.stroke;
        var hasFillGradient = hasFill && !!(fill.colorStops);
        var hasStrokeGradient = hasStroke && !!(stroke.colorStops);
        var hasFillPattern = hasFill && !!(fill.image);
        var hasStrokePattern = hasStroke && !!(stroke.image);

        style.bind(ctx, this, prevEl);
        this.setTransform(ctx);

        if (this.__dirty) {
            var rect;
            // Update gradient because bounding rect may changed
            if (hasFillGradient) {
                rect = rect || this.getBoundingRect();
                this._fillGradient = style.getGradient(ctx, fill, rect);
            }
            if (hasStrokeGradient) {
                rect = rect || this.getBoundingRect();
                this._strokeGradient = style.getGradient(ctx, stroke, rect);
            }
        }
        // Use the gradient or pattern
        if (hasFillGradient) {
            // PENDING If may have affect the state
            ctx.fillStyle = this._fillGradient;
        }
        else if (hasFillPattern) {
            ctx.fillStyle = getCanvasPattern.call(fill, ctx);
        }
        if (hasStrokeGradient) {
            ctx.strokeStyle = this._strokeGradient;
        }
        else if (hasStrokePattern) {
            ctx.strokeStyle = getCanvasPattern.call(stroke, ctx);
        }

        var lineDash = style.lineDash;
        var lineDashOffset = style.lineDashOffset;

        var ctxLineDash = !!ctx.setLineDash;

        // Update path sx, sy
        var scale = this.getGlobalScale();
        path.setScale(scale[0], scale[1]);

        // Proxy context
        // Rebuild path in following 2 cases
        // 1. Path is dirty
        // 2. Path needs javascript implemented lineDash stroking.
        //    In this case, lineDash information will not be saved in PathProxy
        if (this.__dirtyPath
            || (lineDash && !ctxLineDash && hasStroke)
        ) {
            path.beginPath(ctx);

            // Setting line dash before build path
            if (lineDash && !ctxLineDash) {
                path.setLineDash(lineDash);
                path.setLineDashOffset(lineDashOffset);
            }

            this.buildPath(path, this.shape, false);

            // Clear path dirty flag
            if (this.path) {
                this.__dirtyPath = false;
            }
        }
        else {
            // Replay path building
            ctx.beginPath();
            this.path.rebuildPath(ctx);
        }

        hasFill && path.fill(ctx);

        if (lineDash && ctxLineDash) {
            ctx.setLineDash(lineDash);
            ctx.lineDashOffset = lineDashOffset;
        }

        hasStroke && path.stroke(ctx);

        if (lineDash && ctxLineDash) {
            // PENDING
            // Remove lineDash
            ctx.setLineDash([]);
        }

        this.restoreTransform(ctx);

        // Draw rect text
        if (style.text != null) {
            this.drawRectText(ctx, this.getBoundingRect());
        }
    },

    // When bundling path, some shape may decide if use moveTo to begin a new subpath or closePath
    // Like in circle
    buildPath: function (ctx, shapeCfg, inBundle) {},

    createPathProxy: function () {
        this.path = new PathProxy();
    },

    getBoundingRect: function () {
        var rect = this._rect;
        var style = this.style;
        var needsUpdateRect = !rect;
        if (needsUpdateRect) {
            var path = this.path;
            if (!path) {
                // Create path on demand.
                path = this.path = new PathProxy();
            }
            if (this.__dirtyPath) {
                path.beginPath();
                this.buildPath(path, this.shape, false);
            }
            rect = path.getBoundingRect();
        }
        this._rect = rect;

        if (style.hasStroke()) {
            // Needs update rect with stroke lineWidth when
            // 1. Element changes scale or lineWidth
            // 2. Shape is changed
            var rectWithStroke = this._rectWithStroke || (this._rectWithStroke = rect.clone());
            if (this.__dirty || needsUpdateRect) {
                rectWithStroke.copy(rect);
                // FIXME Must after updateTransform
                var w = style.lineWidth;
                // PENDING, Min line width is needed when line is horizontal or vertical
                var lineScale = style.strokeNoScale ? this.getLineScale() : 1;

                // Only add extra hover lineWidth when there are no fill
                if (!style.hasFill()) {
                    w = Math.max(w, this.strokeContainThreshold || 4);
                }
                // Consider line width
                // Line scale can't be 0;
                if (lineScale > 1e-10) {
                    rectWithStroke.width += w / lineScale;
                    rectWithStroke.height += w / lineScale;
                    rectWithStroke.x -= w / lineScale / 2;
                    rectWithStroke.y -= w / lineScale / 2;
                }
            }

            // Return rect with stroke
            return rectWithStroke;
        }

        return rect;
    },

    contain: function (x, y) {
        var localPos = this.transformCoordToLocal(x, y);
        var rect = this.getBoundingRect();
        var style = this.style;
        x = localPos[0];
        y = localPos[1];

        if (rect.contain(x, y)) {
            var pathData = this.path.data;
            if (style.hasStroke()) {
                var lineWidth = style.lineWidth;
                var lineScale = style.strokeNoScale ? this.getLineScale() : 1;
                // Line scale can't be 0;
                if (lineScale > 1e-10) {
                    // Only add extra hover lineWidth when there are no fill
                    if (!style.hasFill()) {
                        lineWidth = Math.max(lineWidth, this.strokeContainThreshold);
                    }
                    if (containStroke$4(
                        pathData, lineWidth / lineScale, x, y
                    )) {
                        return true;
                    }
                }
            }
            if (style.hasFill()) {
                return contain(pathData, x, y);
            }
        }
        return false;
    },

    /**
     * @param  {boolean} dirtyPath
     */
    dirty: function (dirtyPath) {
        if (dirtyPath == null) {
            dirtyPath = true;
        }
        // Only mark dirty, not mark clean
        if (dirtyPath) {
            this.__dirtyPath = dirtyPath;
            this._rect = null;
        }

        this.__dirty = true;

        this.__zr && this.__zr.refresh();

        // Used as a clipping path
        if (this.__clipTarget) {
            this.__clipTarget.dirty();
        }
    },

    /**
     * Alias for animate('shape')
     * @param {boolean} loop
     */
    animateShape: function (loop) {
        return this.animate('shape', loop);
    },

    // Overwrite attrKV
    attrKV: function (key, value) {
        // FIXME
        if (key === 'shape') {
            this.setShape(value);
            this.__dirtyPath = true;
            this._rect = null;
        }
        else {
            Displayable.prototype.attrKV.call(this, key, value);
        }
    },

    /**
     * @param {Object|string} key
     * @param {*} value
     */
    setShape: function (key, value) {
        var shape = this.shape;
        // Path from string may not have shape
        if (shape) {
            if (isObject$1(key)) {
                for (var name in key) {
                    if (key.hasOwnProperty(name)) {
                        shape[name] = key[name];
                    }
                }
            }
            else {
                shape[key] = value;
            }
            this.dirty(true);
        }
        return this;
    },

    getLineScale: function () {
        var m = this.transform;
        // Get the line scale.
        // Determinant of `m` means how much the area is enlarged by the
        // transformation. So its square root can be used as a scale factor
        // for width.
        return m && abs(m[0] - 1) > 1e-10 && abs(m[3] - 1) > 1e-10
            ? Math.sqrt(abs(m[0] * m[3] - m[2] * m[1]))
            : 1;
    }
};

/**
 * 扩展一个 Path element, 比如星形，圆等。
 * Extend a path element
 * @param {Object} props
 * @param {string} props.type Path type
 * @param {Function} props.init Initialize
 * @param {Function} props.buildPath Overwrite buildPath method
 * @param {Object} [props.style] Extended default style config
 * @param {Object} [props.shape] Extended default shape config
 */
Path.extend = function (defaults$$1) {
    var Sub = function (opts) {
        Path.call(this, opts);

        if (defaults$$1.style) {
            // Extend default style
            this.style.extendFrom(defaults$$1.style, false);
        }

        // Extend default shape
        var defaultShape = defaults$$1.shape;
        if (defaultShape) {
            this.shape = this.shape || {};
            var thisShape = this.shape;
            for (var name in defaultShape) {
                if (
                    ! thisShape.hasOwnProperty(name)
                    && defaultShape.hasOwnProperty(name)
                ) {
                    thisShape[name] = defaultShape[name];
                }
            }
        }

        defaults$$1.init && defaults$$1.init.call(this, opts);
    };

    inherits(Sub, Path);

    // FIXME 不能 extend position, rotation 等引用对象
    for (var name in defaults$$1) {
        // Extending prototype values and methods
        if (name !== 'style' && name !== 'shape') {
            Sub.prototype[name] = defaults$$1[name];
        }
    }

    return Sub;
};

inherits(Path, Displayable);

/**
 * 圆形
 * @module zrender/shape/Circle
 */

var Circle = Path.extend({

    type: 'circle',

    shape: {
        cx: 0,
        cy: 0,
        r: 0
    },


    buildPath : function (ctx, shape, inBundle) {
        // Better stroking in ShapeBundle
        // Always do it may have performence issue ( fill may be 2x more cost)
        if (inBundle) {
            ctx.moveTo(shape.cx + shape.r, shape.cy);
        }
        // else {
        //     if (ctx.allocate && !ctx.data.length) {
        //         ctx.allocate(ctx.CMD_MEM_SIZE.A);
        //     }
        // }
        // Better stroking in ShapeBundle
        // ctx.moveTo(shape.cx + shape.r, shape.cy);
        ctx.arc(shape.cx, shape.cy, shape.r, 0, Math.PI * 2, true);
    }
});

/**
 * @alias zrender/graphic/Text
 * @extends module:zrender/graphic/Displayable
 * @constructor
 * @param {Object} opts
 */
var Text = function (opts) { // jshint ignore:line
    Displayable.call(this, opts);
};

Text.prototype = {

    constructor: Text,

    type: 'text',

    brush: function (ctx, prevEl) {
        var style = this.style;

        // Optimize, avoid normalize every time.
        this.__dirty && normalizeTextStyle(style, true);

        // Use props with prefix 'text'.
        style.fill = style.stroke = style.shadowBlur = style.shadowColor =
            style.shadowOffsetX = style.shadowOffsetY = null;

        var text = style.text;
        // Convert to string
        text != null && (text += '');

        // Always bind style
        style.bind(ctx, this, prevEl);

        if (!needDrawText(text, style)) {
            return;
        }

        this.setTransform(ctx);

        renderText(this, ctx, text, style);

        this.restoreTransform(ctx);
    },

    getBoundingRect: function () {
        var style = this.style;

        // Optimize, avoid normalize every time.
        this.__dirty && normalizeTextStyle(style, true);

        if (!this._rect) {
            var text = style.text;
            text != null ? (text += '') : (text = '');

            var rect = getBoundingRect(
                style.text + '',
                style.font,
                style.textAlign,
                style.textVerticalAlign,
                style.textPadding,
                style.rich
            );

            rect.x += style.x || 0;
            rect.y += style.y || 0;

            if (getStroke(style.textStroke, style.textStrokeWidth)) {
                var w = style.textStrokeWidth;
                rect.x -= w / 2;
                rect.y -= w / 2;
                rect.width += w;
                rect.height += w;
            }

            this._rect = rect;
        }

        return this._rect;
    }
};

inherits(Text, Displayable);

var CMD$2 = PathProxy.CMD;

var points = [[], [], []];
var mathSqrt$2 = Math.sqrt;
var mathAtan2 = Math.atan2;

function transformPath (path, m) {
    var data = path.data;
    var cmd;
    var nPoint;
    var i;
    var j;
    var k;
    var p;

    var M = CMD$2.M;
    var C = CMD$2.C;
    var L = CMD$2.L;
    var R = CMD$2.R;
    var A = CMD$2.A;
    var Q = CMD$2.Q;

    for (i = 0, j = 0; i < data.length;) {
        cmd = data[i++];
        j = i;
        nPoint = 0;

        switch (cmd) {
            case M:
                nPoint = 1;
                break;
            case L:
                nPoint = 1;
                break;
            case C:
                nPoint = 3;
                break;
            case Q:
                nPoint = 2;
                break;
            case A:
                var x = m[4];
                var y = m[5];
                var sx = mathSqrt$2(m[0] * m[0] + m[1] * m[1]);
                var sy = mathSqrt$2(m[2] * m[2] + m[3] * m[3]);
                var angle = mathAtan2(-m[1] / sy, m[0] / sx);
                // cx
                data[i] *= sx;
                data[i++] += x;
                // cy
                data[i] *= sy;
                data[i++] += y;
                // Scale rx and ry
                // FIXME Assume psi is 0 here
                data[i++] *= sx;
                data[i++] *= sy;

                // Start angle
                data[i++] += angle;
                // end angle
                data[i++] += angle;
                // FIXME psi
                i += 2;
                j = i;
                break;
            case R:
                // x0, y0
                p[0] = data[i++];
                p[1] = data[i++];
                applyTransform(p, p, m);
                data[j++] = p[0];
                data[j++] = p[1];
                // x1, y1
                p[0] += data[i++];
                p[1] += data[i++];
                applyTransform(p, p, m);
                data[j++] = p[0];
                data[j++] = p[1];
        }

        for (k = 0; k < nPoint; k++) {
            var p = points[k];
            p[0] = data[i++];
            p[1] = data[i++];

            applyTransform(p, p, m);
            // Write back
            data[j++] = p[0];
            data[j++] = p[1];
        }
    }
}

// command chars
var cc = [
    'm', 'M', 'l', 'L', 'v', 'V', 'h', 'H', 'z', 'Z',
    'c', 'C', 'q', 'Q', 't', 'T', 's', 'S', 'a', 'A'
];

var mathSqrt$3 = Math.sqrt;
var mathSin$2 = Math.sin;
var mathCos$2 = Math.cos;
var PI = Math.PI;

var vMag = function(v) {
    return Math.sqrt(v[0] * v[0] + v[1] * v[1]);
};
var vRatio = function(u, v) {
    return (u[0] * v[0] + u[1] * v[1]) / (vMag(u) * vMag(v));
};
var vAngle = function(u, v) {
    return (u[0] * v[1] < u[1] * v[0] ? -1 : 1)
            * Math.acos(vRatio(u, v));
};

function processArc(x1, y1, x2, y2, fa, fs, rx, ry, psiDeg, cmd, path) {
    var psi = psiDeg * (PI / 180.0);
    var xp = mathCos$2(psi) * (x1 - x2) / 2.0
                + mathSin$2(psi) * (y1 - y2) / 2.0;
    var yp = -1 * mathSin$2(psi) * (x1 - x2) / 2.0
                + mathCos$2(psi) * (y1 - y2) / 2.0;

    var lambda = (xp * xp) / (rx * rx) + (yp * yp) / (ry * ry);

    if (lambda > 1) {
        rx *= mathSqrt$3(lambda);
        ry *= mathSqrt$3(lambda);
    }

    var f = (fa === fs ? -1 : 1)
        * mathSqrt$3((((rx * rx) * (ry * ry))
                - ((rx * rx) * (yp * yp))
                - ((ry * ry) * (xp * xp))) / ((rx * rx) * (yp * yp)
                + (ry * ry) * (xp * xp))
            ) || 0;

    var cxp = f * rx * yp / ry;
    var cyp = f * -ry * xp / rx;

    var cx = (x1 + x2) / 2.0
                + mathCos$2(psi) * cxp
                - mathSin$2(psi) * cyp;
    var cy = (y1 + y2) / 2.0
            + mathSin$2(psi) * cxp
            + mathCos$2(psi) * cyp;

    var theta = vAngle([ 1, 0 ], [ (xp - cxp) / rx, (yp - cyp) / ry ]);
    var u = [ (xp - cxp) / rx, (yp - cyp) / ry ];
    var v = [ (-1 * xp - cxp) / rx, (-1 * yp - cyp) / ry ];
    var dTheta = vAngle(u, v);

    if (vRatio(u, v) <= -1) {
        dTheta = PI;
    }
    if (vRatio(u, v) >= 1) {
        dTheta = 0;
    }
    if (fs === 0 && dTheta > 0) {
        dTheta = dTheta - 2 * PI;
    }
    if (fs === 1 && dTheta < 0) {
        dTheta = dTheta + 2 * PI;
    }

    path.addData(cmd, cx, cy, rx, ry, theta, dTheta, psi, fs);
}

function createPathProxyFromString(data) {
    if (!data) {
        return [];
    }

    // command string
    var cs = data.replace(/-/g, ' -')
        .replace(/  /g, ' ')
        .replace(/ /g, ',')
        .replace(/,,/g, ',');

    var n;
    // create pipes so that we can split the data
    for (n = 0; n < cc.length; n++) {
        cs = cs.replace(new RegExp(cc[n], 'g'), '|' + cc[n]);
    }

    // create array
    var arr = cs.split('|');
    // init context point
    var cpx = 0;
    var cpy = 0;

    var path = new PathProxy();
    var CMD = PathProxy.CMD;

    var prevCmd;
    for (n = 1; n < arr.length; n++) {
        var str = arr[n];
        var c = str.charAt(0);
        var off = 0;
        var p = str.slice(1).replace(/e,-/g, 'e-').split(',');
        var cmd;

        if (p.length > 0 && p[0] === '') {
            p.shift();
        }

        for (var i = 0; i < p.length; i++) {
            p[i] = parseFloat(p[i]);
        }
        while (off < p.length && !isNaN(p[off])) {
            if (isNaN(p[0])) {
                break;
            }
            var ctlPtx;
            var ctlPty;

            var rx;
            var ry;
            var psi;
            var fa;
            var fs;

            var x1 = cpx;
            var y1 = cpy;

            // convert l, H, h, V, and v to L
            switch (c) {
                case 'l':
                    cpx += p[off++];
                    cpy += p[off++];
                    cmd = CMD.L;
                    path.addData(cmd, cpx, cpy);
                    break;
                case 'L':
                    cpx = p[off++];
                    cpy = p[off++];
                    cmd = CMD.L;
                    path.addData(cmd, cpx, cpy);
                    break;
                case 'm':
                    cpx += p[off++];
                    cpy += p[off++];
                    cmd = CMD.M;
                    path.addData(cmd, cpx, cpy);
                    c = 'l';
                    break;
                case 'M':
                    cpx = p[off++];
                    cpy = p[off++];
                    cmd = CMD.M;
                    path.addData(cmd, cpx, cpy);
                    c = 'L';
                    break;
                case 'h':
                    cpx += p[off++];
                    cmd = CMD.L;
                    path.addData(cmd, cpx, cpy);
                    break;
                case 'H':
                    cpx = p[off++];
                    cmd = CMD.L;
                    path.addData(cmd, cpx, cpy);
                    break;
                case 'v':
                    cpy += p[off++];
                    cmd = CMD.L;
                    path.addData(cmd, cpx, cpy);
                    break;
                case 'V':
                    cpy = p[off++];
                    cmd = CMD.L;
                    path.addData(cmd, cpx, cpy);
                    break;
                case 'C':
                    cmd = CMD.C;
                    path.addData(
                        cmd, p[off++], p[off++], p[off++], p[off++], p[off++], p[off++]
                    );
                    cpx = p[off - 2];
                    cpy = p[off - 1];
                    break;
                case 'c':
                    cmd = CMD.C;
                    path.addData(
                        cmd,
                        p[off++] + cpx, p[off++] + cpy,
                        p[off++] + cpx, p[off++] + cpy,
                        p[off++] + cpx, p[off++] + cpy
                    );
                    cpx += p[off - 2];
                    cpy += p[off - 1];
                    break;
                case 'S':
                    ctlPtx = cpx;
                    ctlPty = cpy;
                    var len = path.len();
                    var pathData = path.data;
                    if (prevCmd === CMD.C) {
                        ctlPtx += cpx - pathData[len - 4];
                        ctlPty += cpy - pathData[len - 3];
                    }
                    cmd = CMD.C;
                    x1 = p[off++];
                    y1 = p[off++];
                    cpx = p[off++];
                    cpy = p[off++];
                    path.addData(cmd, ctlPtx, ctlPty, x1, y1, cpx, cpy);
                    break;
                case 's':
                    ctlPtx = cpx;
                    ctlPty = cpy;
                    var len = path.len();
                    var pathData = path.data;
                    if (prevCmd === CMD.C) {
                        ctlPtx += cpx - pathData[len - 4];
                        ctlPty += cpy - pathData[len - 3];
                    }
                    cmd = CMD.C;
                    x1 = cpx + p[off++];
                    y1 = cpy + p[off++];
                    cpx += p[off++];
                    cpy += p[off++];
                    path.addData(cmd, ctlPtx, ctlPty, x1, y1, cpx, cpy);
                    break;
                case 'Q':
                    x1 = p[off++];
                    y1 = p[off++];
                    cpx = p[off++];
                    cpy = p[off++];
                    cmd = CMD.Q;
                    path.addData(cmd, x1, y1, cpx, cpy);
                    break;
                case 'q':
                    x1 = p[off++] + cpx;
                    y1 = p[off++] + cpy;
                    cpx += p[off++];
                    cpy += p[off++];
                    cmd = CMD.Q;
                    path.addData(cmd, x1, y1, cpx, cpy);
                    break;
                case 'T':
                    ctlPtx = cpx;
                    ctlPty = cpy;
                    var len = path.len();
                    var pathData = path.data;
                    if (prevCmd === CMD.Q) {
                        ctlPtx += cpx - pathData[len - 4];
                        ctlPty += cpy - pathData[len - 3];
                    }
                    cpx = p[off++];
                    cpy = p[off++];
                    cmd = CMD.Q;
                    path.addData(cmd, ctlPtx, ctlPty, cpx, cpy);
                    break;
                case 't':
                    ctlPtx = cpx;
                    ctlPty = cpy;
                    var len = path.len();
                    var pathData = path.data;
                    if (prevCmd === CMD.Q) {
                        ctlPtx += cpx - pathData[len - 4];
                        ctlPty += cpy - pathData[len - 3];
                    }
                    cpx += p[off++];
                    cpy += p[off++];
                    cmd = CMD.Q;
                    path.addData(cmd, ctlPtx, ctlPty, cpx, cpy);
                    break;
                case 'A':
                    rx = p[off++];
                    ry = p[off++];
                    psi = p[off++];
                    fa = p[off++];
                    fs = p[off++];

                    x1 = cpx, y1 = cpy;
                    cpx = p[off++];
                    cpy = p[off++];
                    cmd = CMD.A;
                    processArc(
                        x1, y1, cpx, cpy, fa, fs, rx, ry, psi, cmd, path
                    );
                    break;
                case 'a':
                    rx = p[off++];
                    ry = p[off++];
                    psi = p[off++];
                    fa = p[off++];
                    fs = p[off++];

                    x1 = cpx, y1 = cpy;
                    cpx += p[off++];
                    cpy += p[off++];
                    cmd = CMD.A;
                    processArc(
                        x1, y1, cpx, cpy, fa, fs, rx, ry, psi, cmd, path
                    );
                    break;
            }
        }

        if (c === 'z' || c === 'Z') {
            cmd = CMD.Z;
            path.addData(cmd);
        }

        prevCmd = cmd;
    }

    path.toStatic();

    return path;
}

/**
 * Catmull-Rom spline 插值折线
 * @module zrender/shape/util/smoothSpline
 * @author pissang (https://www.github.com/pissang)
 *         Kener (@Kener-林峰, kener.linfeng@gmail.com)
 *         errorrik (errorrik@gmail.com)
 */

/**
 * @inner
 */
function interpolate(p0, p1, p2, p3, t, t2, t3) {
    var v0 = (p2 - p0) * 0.5;
    var v1 = (p3 - p1) * 0.5;
    return (2 * (p1 - p2) + v0 + v1) * t3
            + (-3 * (p1 - p2) - 2 * v0 - v1) * t2
            + v0 * t + p1;
}

/**
 * @alias module:zrender/shape/util/smoothSpline
 * @param {Array} points 线段顶点数组
 * @param {boolean} isLoop
 * @return {Array}
 */
function smoothSpline (points, isLoop) {
    var len$$1 = points.length;
    var ret = [];

    var distance$$1 = 0;
    for (var i = 1; i < len$$1; i++) {
        distance$$1 += distance(points[i - 1], points[i]);
    }

    var segs = distance$$1 / 2;
    segs = segs < len$$1 ? len$$1 : segs;
    for (var i = 0; i < segs; i++) {
        var pos = i / (segs - 1) * (isLoop ? len$$1 : len$$1 - 1);
        var idx = Math.floor(pos);

        var w = pos - idx;

        var p0;
        var p1 = points[idx % len$$1];
        var p2;
        var p3;
        if (!isLoop) {
            p0 = points[idx === 0 ? idx : idx - 1];
            p2 = points[idx > len$$1 - 2 ? len$$1 - 1 : idx + 1];
            p3 = points[idx > len$$1 - 3 ? len$$1 - 1 : idx + 2];
        }
        else {
            p0 = points[(idx - 1 + len$$1) % len$$1];
            p2 = points[(idx + 1) % len$$1];
            p3 = points[(idx + 2) % len$$1];
        }

        var w2 = w * w;
        var w3 = w * w2;

        ret.push([
            interpolate(p0[0], p1[0], p2[0], p3[0], w, w2, w3),
            interpolate(p0[1], p1[1], p2[1], p3[1], w, w2, w3)
        ]);
    }
    return ret;
}

/**
 * 贝塞尔平滑曲线
 * @module zrender/shape/util/smoothBezier
 * @author pissang (https://www.github.com/pissang)
 *         Kener (@Kener-林峰, kener.linfeng@gmail.com)
 *         errorrik (errorrik@gmail.com)
 */

/**
 * 贝塞尔平滑曲线
 * @alias module:zrender/shape/util/smoothBezier
 * @param {Array} points 线段顶点数组
 * @param {number} smooth 平滑等级, 0-1
 * @param {boolean} isLoop
 * @param {Array} constraint 将计算出来的控制点约束在一个包围盒内
 *                           比如 [[0, 0], [100, 100]], 这个包围盒会与
 *                           整个折线的包围盒做一个并集用来约束控制点。
 * @param {Array} 计算出来的控制点数组
 */
function smoothBezier (points, smooth, isLoop, constraint) {
    var cps = [];

    var v = [];
    var v1 = [];
    var v2 = [];
    var prevPoint;
    var nextPoint;

    var min$$1, max$$1;
    if (constraint) {
        min$$1 = [Infinity, Infinity];
        max$$1 = [-Infinity, -Infinity];
        for (var i = 0, len$$1 = points.length; i < len$$1; i++) {
            min(min$$1, min$$1, points[i]);
            max(max$$1, max$$1, points[i]);
        }
        // 与指定的包围盒做并集
        min(min$$1, min$$1, constraint[0]);
        max(max$$1, max$$1, constraint[1]);
    }

    for (var i = 0, len$$1 = points.length; i < len$$1; i++) {
        var point = points[i];

        if (isLoop) {
            prevPoint = points[i ? i - 1 : len$$1 - 1];
            nextPoint = points[(i + 1) % len$$1];
        }
        else {
            if (i === 0 || i === len$$1 - 1) {
                cps.push(clone$2(points[i]));
                continue;
            }
            else {
                prevPoint = points[i - 1];
                nextPoint = points[i + 1];
            }
        }

        sub(v, nextPoint, prevPoint);

        // use degree to scale the handle length
        scale$1(v, v, smooth);

        var d0 = distance(point, prevPoint);
        var d1 = distance(point, nextPoint);
        var sum = d0 + d1;
        if (sum !== 0) {
            d0 /= sum;
            d1 /= sum;
        }

        scale$1(v1, v, -d0);
        scale$1(v2, v, d1);
        var cp0 = add([], point, v1);
        var cp1 = add([], point, v2);
        if (constraint) {
            max(cp0, cp0, min$$1);
            min(cp0, cp0, max$$1);
            max(cp1, cp1, min$$1);
            min(cp1, cp1, max$$1);
        }
        cps.push(cp0);
        cps.push(cp1);
    }

    if (isLoop) {
        cps.push(cps.shift());
    }

    return cps;
}

function buildPath$1(ctx, shape, closePath) {
    var points = shape.points;
    var smooth = shape.smooth;
    if (points && points.length >= 2) {
        if (smooth && smooth !== 'spline') {
            var controlPoints = smoothBezier(
                points, smooth, closePath, shape.smoothConstraint
            );

            ctx.moveTo(points[0][0], points[0][1]);
            var len = points.length;
            for (var i = 0; i < (closePath ? len : len - 1); i++) {
                var cp1 = controlPoints[i * 2];
                var cp2 = controlPoints[i * 2 + 1];
                var p = points[(i + 1) % len];
                ctx.bezierCurveTo(
                    cp1[0], cp1[1], cp2[0], cp2[1], p[0], p[1]
                );
            }
        }
        else {
            if (smooth === 'spline') {
                points = smoothSpline(points, closePath);
            }

            ctx.moveTo(points[0][0], points[0][1]);
            for (var i = 1, l = points.length; i < l; i++) {
                ctx.lineTo(points[i][0], points[i][1]);
            }
        }

        closePath && ctx.closePath();
    }
}

/**
 * 多边形
 * @module zrender/shape/Polygon
 */

var Polygon = Path.extend({

    type: 'polygon',

    shape: {
        points: null,

        smooth: false,

        smoothConstraint: null
    },

    buildPath: function (ctx, shape) {
        buildPath$1(ctx, shape, true);
    }
});

/**
 * @module zrender/graphic/shape/Polyline
 */

var Polyline = Path.extend({

    type: 'polyline',

    shape: {
        points: null,

        smooth: false,

        smoothConstraint: null
    },

    style: {
        stroke: '#000',

        fill: null
    },

    buildPath: function (ctx, shape) {
        buildPath$1(ctx, shape, false);
    }
});

/**
 * 直线
 * @module zrender/graphic/shape/Line
 */

var Line = Path.extend({

    type: 'line',

    shape: {
        // Start point
        x1: 0,
        y1: 0,
        // End point
        x2: 0,
        y2: 0,

        percent: 1
    },

    style: {
        stroke: '#000',
        fill: null
    },

    buildPath: function (ctx, shape) {
        var x1 = shape.x1;
        var y1 = shape.y1;
        var x2 = shape.x2;
        var y2 = shape.y2;
        var percent = shape.percent;

        if (percent === 0) {
            return;
        }

        ctx.moveTo(x1, y1);

        if (percent < 1) {
            x2 = x1 * (1 - percent) + x2 * percent;
            y2 = y1 * (1 - percent) + y2 * percent;
        }
        ctx.lineTo(x2, y2);
    },

    /**
     * Get point at percent
     * @param  {number} percent
     * @return {Array.<number>}
     */
    pointAt: function (p) {
        var shape = this.shape;
        return [
            shape.x1 * (1 - p) + shape.x2 * p,
            shape.y1 * (1 - p) + shape.y2 * p
        ];
    }
});

/**
 * n角星（n>3）
 * @module zrender/graphic/shape/Star
 */

var PI$1 = Math.PI;
var cos = Math.cos;
var sin = Math.sin;

var Star = Path.extend({

    type: 'star',

    shape: {
        cx: 0,
        cy: 0,
        n: 3,
        r0: null,
        r: 0
    },

    buildPath: function (ctx, shape) {

        var n = shape.n;
        if (!n || n < 2) {
            return;
        }

        var x = shape.cx;
        var y = shape.cy;
        var r = shape.r;
        var r0 = shape.r0;

        // 如果未指定内部顶点外接圆半径，则自动计算
        if (r0 == null) {
            r0 = n > 4
                // 相隔的外部顶点的连线的交点，
                // 被取为内部交点，以此计算r0
                ? r * cos(2 * PI$1 / n) / cos(PI$1 / n)
                // 二三四角星的特殊处理
                : r / 3;
        }

        var dStep = PI$1 / n;
        var deg = -PI$1 / 2;
        var xStart = x + r * cos(deg);
        var yStart = y + r * sin(deg);
        deg += dStep;

        // 记录边界点，用于判断inside
        ctx.moveTo(xStart, yStart);
        for (var i = 0, end = n * 2 - 1, ri; i < end; i++) {
            ri = i % 2 === 0 ? r0 : r;
            ctx.lineTo(x + ri * cos(deg), y + ri * sin(deg));
            deg += dStep;
        }

        ctx.closePath();
    }
});

/**
 * 圆弧
 * @module zrender/graphic/shape/Arc
 */

var Arc = Path.extend({

    type: 'arc',

    shape: {

        cx: 0,

        cy: 0,

        r: 0,

        startAngle: 0,

        endAngle: Math.PI * 2,

        clockwise: true
    },

    style: {

        stroke: '#000',

        fill: null
    },

    buildPath: function (ctx, shape) {

        var x = shape.cx;
        var y = shape.cy;
        var r = Math.max(shape.r, 0);
        var startAngle = shape.startAngle;
        var endAngle = shape.endAngle;
        var clockwise = shape.clockwise;

        var unitX = Math.cos(startAngle);
        var unitY = Math.sin(startAngle);

        ctx.moveTo(unitX * r + x, unitY * r + y);
        ctx.arc(x, y, r, startAngle, endAngle, !clockwise);
    }
});

/**
 * 心形
 * @module zrender/graphic/shape/Heart
 */

var Heart = Path.extend({

    type: 'heart',

    shape: {
        cx: 0,
        cy: 0,
        width: 0,
        height: 0
    },

    buildPath: function (ctx, shape) {
        var x = shape.cx;
        var y = shape.cy;
        var a = shape.width;
        var b = shape.height;
        ctx.moveTo(x, y);
        ctx.bezierCurveTo(
            x + a / 2, y - b * 2 / 3,
            x + a * 2, y + b / 3,
            x, y + b
        );
        ctx.bezierCurveTo(
            x - a *  2, y + b / 3,
            x - a / 2, y - b * 2 / 3,
            x, y
        );
    }
});

/**
 * Group是一个容器，可以插入子节点，Group的变换也会被应用到子节点上
 * @module zrender/graphic/Group
 * @example
 *     var Group = require('zrender/container/Group');
 *     var Circle = require('zrender/graphic/shape/Circle');
 *     var g = new Group();
 *     g.position[0] = 100;
 *     g.position[1] = 100;
 *     g.add(new Circle({
 *         style: {
 *             x: 100,
 *             y: 100,
 *             r: 20,
 *         }
 *     }));
 *     zr.add(g);
 */

/**
 * @alias module:zrender/graphic/Group
 * @constructor
 * @extends module:zrender/mixin/Transformable
 * @extends module:zrender/mixin/Eventful
 */
var Group = function (opts) {
    var this$1 = this;


    opts = opts || {};

    Element.call(this, opts);

    for (var key in opts) {
        if (opts.hasOwnProperty(key)) {
            this$1[key] = opts[key];
        }
    }

    this._children = [];

    this.__storage = null;

    this.__dirty = true;
};

Group.prototype = {

    constructor: Group,

    isGroup: true,

    /**
     * @type {string}
     */
    type: 'group',

    /**
     * 所有子孙元素是否响应鼠标事件
     * @name module:/zrender/container/Group#silent
     * @type {boolean}
     * @default false
     */
    silent: false,

    /**
     * @return {Array.<module:zrender/Element>}
     */
    children: function () {
        return this._children.slice();
    },

    /**
     * 获取指定 index 的儿子节点
     * @param  {number} idx
     * @return {module:zrender/Element}
     */
    childAt: function (idx) {
        return this._children[idx];
    },

    /**
     * 获取指定名字的儿子节点
     * @param  {string} name
     * @return {module:zrender/Element}
     */
    childOfName: function (name) {
        var children = this._children;
        for (var i = 0; i < children.length; i++) {
            if (children[i].name === name) {
                return children[i];
            }
            }
    },

    /**
     * @return {number}
     */
    childCount: function () {
        return this._children.length;
    },

    /**
     * 添加子节点到最后
     * @param {module:zrender/Element} child
     */
    add: function (child) {
        if (child && child !== this && child.parent !== this) {

            this._children.push(child);

            this._doAdd(child);
        }

        return this;
    },

    /**
     * 添加子节点在 nextSibling 之前
     * @param {module:zrender/Element} child
     * @param {module:zrender/Element} nextSibling
     */
    addBefore: function (child, nextSibling) {
        if (child && child !== this && child.parent !== this
            && nextSibling && nextSibling.parent === this) {

            var children = this._children;
            var idx = children.indexOf(nextSibling);

            if (idx >= 0) {
                children.splice(idx, 0, child);
                this._doAdd(child);
            }
        }

        return this;
    },

    _doAdd: function (child) {
        if (child.parent) {
            child.parent.remove(child);
        }

        child.parent = this;

        var storage = this.__storage;
        var zr = this.__zr;
        if (storage && storage !== child.__storage) {

            storage.addToStorage(child);

            if (child instanceof Group) {
                child.addChildrenToStorage(storage);
            }
        }

        zr && zr.refresh();
    },

    /**
     * 移除子节点
     * @param {module:zrender/Element} child
     */
    remove: function (child) {
        var zr = this.__zr;
        var storage = this.__storage;
        var children = this._children;

        var idx = indexOf$1(children, child);
        if (idx < 0) {
            return this;
        }
        children.splice(idx, 1);

        child.parent = null;

        if (storage) {

            storage.delFromStorage(child);

            if (child instanceof Group) {
                child.delChildrenFromStorage(storage);
            }
        }

        zr && zr.refresh();

        return this;
    },

    /**
     * 移除所有子节点
     */
    removeAll: function () {
        var children = this._children;
        var storage = this.__storage;
        var child;
        var i;
        for (i = 0; i < children.length; i++) {
            child = children[i];
            if (storage) {
                storage.delFromStorage(child);
                if (child instanceof Group) {
                    child.delChildrenFromStorage(storage);
                }
            }
            child.parent = null;
        }
        children.length = 0;

        return this;
    },

    /**
     * 遍历所有子节点
     * @param  {Function} cb
     * @param  {}   context
     */
    eachChild: function (cb, context) {
        var children = this._children;
        for (var i = 0; i < children.length; i++) {
            var child = children[i];
            cb.call(context, child, i);
        }
        return this;
    },

    /**
     * 深度优先遍历所有子孙节点
     * @param  {Function} cb
     * @param  {}   context
     */
    traverse: function (cb, context) {
        var this$1 = this;

        for (var i = 0; i < this._children.length; i++) {
            var child = this$1._children[i];
            cb.call(context, child);

            if (child.type === 'group') {
                child.traverse(cb, context);
            }
        }
        return this;
    },

    addChildrenToStorage: function (storage) {
        var this$1 = this;

        for (var i = 0; i < this._children.length; i++) {
            var child = this$1._children[i];
            storage.addToStorage(child);
            if (child instanceof Group) {
                child.addChildrenToStorage(storage);
            }
        }
    },

    delChildrenFromStorage: function (storage) {
        var this$1 = this;

        for (var i = 0; i < this._children.length; i++) {
            var child = this$1._children[i];
            storage.delFromStorage(child);
            if (child instanceof Group) {
                child.delChildrenFromStorage(storage);
            }
        }
    },

    dirty: function () {
        this.__dirty = true;
        this.__zr && this.__zr.refresh();
        return this;
    },

    /**
     * @return {module:zrender/core/BoundingRect}
     */
    getBoundingRect: function (includeChildren) {
        // TODO Caching
        var rect = null;
        var tmpRect = new BoundingRect(0, 0, 0, 0);
        var children = includeChildren || this._children;
        var tmpMat = [];

        for (var i = 0; i < children.length; i++) {
            var child = children[i];
            if (child.ignore || child.invisible) {
                continue;
            }

            var childRect = child.getBoundingRect();
            var transform = child.getLocalTransform(tmpMat);
            // TODO
            // The boundingRect cacluated by transforming original
            // rect may be bigger than the actual bundingRect when rotation
            // is used. (Consider a circle rotated aginst its center, where
            // the actual boundingRect should be the same as that not be
            // rotated.) But we can not find better approach to calculate
            // actual boundingRect yet, considering performance.
            if (transform) {
                tmpRect.copy(childRect);
                tmpRect.applyTransform(transform);
                rect = rect || tmpRect.clone();
                rect.union(tmpRect);
            }
            else {
                rect = rect || childRect.clone();
                rect.union(childRect);
            }
        }
        return rect || tmpRect;
    }
};

inherits(Group, Element);

/**
 * @alias zrender/graphic/Image
 * @extends module:zrender/graphic/Displayable
 * @constructor
 * @param {Object} opts
 */
function ZImage(opts) {
    Displayable.call(this, opts);
}

ZImage.prototype = {

    constructor: ZImage,

    type: 'image',

    brush: function (ctx, prevEl) {
        var style = this.style;
        var src = style.image;

        // Must bind each time
        style.bind(ctx, this, prevEl);

        var image = this._image = createOrUpdateImage(
            src,
            this._image,
            this,
            this.onload
        );

        if (!image || !isImageReady(image)) {
            return;
        }

        // 图片已经加载完成
        // if (image.nodeName.toUpperCase() == 'IMG') {
        //     if (!image.complete) {
        //         return;
        //     }
        // }
        // Else is canvas

        var x = style.x || 0;
        var y = style.y || 0;
        var width = style.width;
        var height = style.height;
        var aspect = image.width / image.height;
        if (width == null && height != null) {
            // Keep image/height ratio
            width = height * aspect;
        }
        else if (height == null && width != null) {
            height = width / aspect;
        }
        else if (width == null && height == null) {
            width = image.width;
            height = image.height;
        }

        // 设置transform
        this.setTransform(ctx);

        if (style.sWidth && style.sHeight) {
            var sx = style.sx || 0;
            var sy = style.sy || 0;
            ctx.drawImage(
                image,
                sx, sy, style.sWidth, style.sHeight,
                x, y, width, height
            );
        }
        else if (style.sx && style.sy) {
            var sx = style.sx;
            var sy = style.sy;
            var sWidth = width - sx;
            var sHeight = height - sy;
            ctx.drawImage(
                image,
                sx, sy, sWidth, sHeight,
                x, y, width, height
            );
        }
        else {
            ctx.drawImage(image, x, y, width, height);
        }

        this.restoreTransform(ctx);

        // Draw rect text
        if (style.text != null) {
            this.drawRectText(ctx, this.getBoundingRect());
        }
    },

    getBoundingRect: function () {
        var style = this.style;
        if (! this._rect) {
            this._rect = new BoundingRect(
                style.x || 0, style.y || 0, style.width || 0, style.height || 0
            );
        }
        return this._rect;
    }
};

inherits(ZImage, Displayable);

/**
 * echarts设备环境识别
 *
 * @desc echarts基于Canvas，纯Javascript图表库，提供直观，生动，可交互，可个性化定制的数据统计图表。
 * @author firede[firede@firede.us]
 * @desc thanks zepto.
 */

var env = {};

if (typeof wx !== 'undefined' && wx.version) {
    // In Weixin Application
    env = {
        browser: {},
        os: {},
        node: false,
        wxa: true, // Weixin Application
        canvasSupported: true,
        svgSupported: false,
        touchEventsSupported: true
    };
}
else if (typeof document === 'undefined' && typeof self !== 'undefined') {
    // In worker
    env = {
        browser: {},
        os: {},
        node: false,
        worker: true,
        canvasSupported: true
    };
}
else if (typeof navigator === 'undefined') {
    // In node
    env = {
        browser: {},
        os: {},
        node: true,
        worker: false,
        // Assume canvas is supported
        canvasSupported: true,
        svgSupported: true
    };
}
else {
    env = detect(navigator.userAgent);
}

var env$1 = env;

// Zepto.js
// (c) 2010-2013 Thomas Fuchs
// Zepto.js may be freely distributed under the MIT license.

function detect(ua) {
    var os = {};
    var browser = {};
    // var webkit = ua.match(/Web[kK]it[\/]{0,1}([\d.]+)/);
    // var android = ua.match(/(Android);?[\s\/]+([\d.]+)?/);
    // var ipad = ua.match(/(iPad).*OS\s([\d_]+)/);
    // var ipod = ua.match(/(iPod)(.*OS\s([\d_]+))?/);
    // var iphone = !ipad && ua.match(/(iPhone\sOS)\s([\d_]+)/);
    // var webos = ua.match(/(webOS|hpwOS)[\s\/]([\d.]+)/);
    // var touchpad = webos && ua.match(/TouchPad/);
    // var kindle = ua.match(/Kindle\/([\d.]+)/);
    // var silk = ua.match(/Silk\/([\d._]+)/);
    // var blackberry = ua.match(/(BlackBerry).*Version\/([\d.]+)/);
    // var bb10 = ua.match(/(BB10).*Version\/([\d.]+)/);
    // var rimtabletos = ua.match(/(RIM\sTablet\sOS)\s([\d.]+)/);
    // var playbook = ua.match(/PlayBook/);
    // var chrome = ua.match(/Chrome\/([\d.]+)/) || ua.match(/CriOS\/([\d.]+)/);
    var firefox = ua.match(/Firefox\/([\d.]+)/);
    // var safari = webkit && ua.match(/Mobile\//) && !chrome;
    // var webview = ua.match(/(iPhone|iPod|iPad).*AppleWebKit(?!.*Safari)/) && !chrome;
    var ie = ua.match(/MSIE\s([\d.]+)/)
        // IE 11 Trident/7.0; rv:11.0
        || ua.match(/Trident\/.+?rv:(([\d.]+))/);
    var edge = ua.match(/Edge\/([\d.]+)/); // IE 12 and 12+

    var weChat = (/micromessenger/i).test(ua);

    // Todo: clean this up with a better OS/browser seperation:
    // - discern (more) between multiple browsers on android
    // - decide if kindle fire in silk mode is android or not
    // - Firefox on Android doesn't specify the Android version
    // - possibly devide in os, device and browser hashes

    // if (browser.webkit = !!webkit) browser.version = webkit[1];

    // if (android) os.android = true, os.version = android[2];
    // if (iphone && !ipod) os.ios = os.iphone = true, os.version = iphone[2].replace(/_/g, '.');
    // if (ipad) os.ios = os.ipad = true, os.version = ipad[2].replace(/_/g, '.');
    // if (ipod) os.ios = os.ipod = true, os.version = ipod[3] ? ipod[3].replace(/_/g, '.') : null;
    // if (webos) os.webos = true, os.version = webos[2];
    // if (touchpad) os.touchpad = true;
    // if (blackberry) os.blackberry = true, os.version = blackberry[2];
    // if (bb10) os.bb10 = true, os.version = bb10[2];
    // if (rimtabletos) os.rimtabletos = true, os.version = rimtabletos[2];
    // if (playbook) browser.playbook = true;
    // if (kindle) os.kindle = true, os.version = kindle[1];
    // if (silk) browser.silk = true, browser.version = silk[1];
    // if (!silk && os.android && ua.match(/Kindle Fire/)) browser.silk = true;
    // if (chrome) browser.chrome = true, browser.version = chrome[1];
    if (firefox) {
        browser.firefox = true;
        browser.version = firefox[1];
    }
    // if (safari && (ua.match(/Safari/) || !!os.ios)) browser.safari = true;
    // if (webview) browser.webview = true;

    if (ie) {
        browser.ie = true;
        browser.version = ie[1];
    }

    if (edge) {
        browser.edge = true;
        browser.version = edge[1];
    }

    // It is difficult to detect WeChat in Win Phone precisely, because ua can
    // not be set on win phone. So we do not consider Win Phone.
    if (weChat) {
        browser.weChat = true;
    }

    // os.tablet = !!(ipad || playbook || (android && !ua.match(/Mobile/)) ||
    //     (firefox && ua.match(/Tablet/)) || (ie && !ua.match(/Phone/) && ua.match(/Touch/)));
    // os.phone  = !!(!os.tablet && !os.ipod && (android || iphone || webos ||
    //     (chrome && ua.match(/Android/)) || (chrome && ua.match(/CriOS\/([\d.]+)/)) ||
    //     (firefox && ua.match(/Mobile/)) || (ie && ua.match(/Touch/))));

    return {
        browser: browser,
        os: os,
        node: false,
        // 原生canvas支持，改极端点了
        // canvasSupported : !(browser.ie && parseFloat(browser.version) < 9)
        canvasSupported: !!document.createElement('canvas').getContext,
        svgSupported: typeof SVGRect !== 'undefined',
        // works on most browsers
        // IE10/11 does not support touch event, and MS Edge supports them but not by
        // default, so we dont check navigator.maxTouchPoints for them here.
        touchEventsSupported: 'ontouchstart' in window && !browser.ie && !browser.edge,
        // <http://caniuse.com/#search=pointer%20event>.
        pointerEventsSupported: 'onpointerdown' in window
            // Firefox supports pointer but not by default, only MS browsers are reliable on pointer
            // events currently. So we dont use that on other browsers unless tested sufficiently.
            // Although IE 10 supports pointer event, it use old style and is different from the
            // standard. So we exclude that. (IE 10 is hardly used on touch device)
            && (browser.edge || (browser.ie && browser.version >= 11))
    };
}

// Fix weird bug in some version of IE11 (like 11.0.9600.178**),
// where exception "unexpected call to method or property access"
// might be thrown when calling ctx.fill or ctx.stroke after a path
// whose area size is zero is drawn and ctx.clip() is called and
// shadowBlur is set. See #4572, #3112, #5777.
// (e.g.,
//  ctx.moveTo(10, 10);
//  ctx.lineTo(20, 10);
//  ctx.closePath();
//  ctx.clip();
//  ctx.shadowBlur = 10;
//  ...
//  ctx.fill();
// )

var shadowTemp = [
    ['shadowBlur', 0],
    ['shadowColor', '#000'],
    ['shadowOffsetX', 0],
    ['shadowOffsetY', 0]
];

function fixClipWithShadow (orignalBrush) {

    // version string can be: '11.0'
    return (env$1.browser.ie && env$1.browser.version >= 11)

        ? function () {
            var clipPaths = this.__clipPaths;
            var style = this.style;
            var modified;

            if (clipPaths) {
                for (var i = 0; i < clipPaths.length; i++) {
                    var clipPath = clipPaths[i];
                    var shape = clipPath && clipPath.shape;
                    var type = clipPath && clipPath.type;

                    if (shape && (
                        (type === 'sector' && shape.startAngle === shape.endAngle)
                        || (type === 'rect' && (!shape.width || !shape.height))
                    )) {
                        for (var j = 0; j < shadowTemp.length; j++) {
                            // It is save to put shadowTemp static, because shadowTemp
                            // will be all modified each item brush called.
                            shadowTemp[j][2] = style[shadowTemp[j][0]];
                            style[shadowTemp[j][0]] = shadowTemp[j][1];
                        }
                        modified = true;
                        break;
                    }
                }
            }

            orignalBrush.apply(this, arguments);

            if (modified) {
                for (var j = 0; j < shadowTemp.length; j++) {
                    style[shadowTemp[j][0]] = shadowTemp[j][2];
                }
            }
        }

        : orignalBrush;
}

/**
 * ZRender提供的rect支持的圆角，类似于css中的border-radius.
 * 但是5.0中的则需要像SVG的rect一样支持rx和ry，这边重写一个新的rect
 */
var Rect = Path.extend({

    type: 'rect',

    shape: {
        x: 0,
        y: 0,
        width: 0,
        height: 0,
        r: 0
    },

    brush: fixClipWithShadow(Path.prototype.brush),

    buildPath: function (ctx, ref) {
        var x = ref.x;
        var y = ref.y;
        var width = ref.width;
        var height = ref.height;
        var r = ref.r;


        if (!r) {
            ctx.rect(x, y, width, height);
        } else {

            var min = Math.min;
            var w = width, hw = w / 2, h = height, hh = h / 2;
            var wr = min(hw, r), hr = min(hh, r);

            var left = x, right = x + w, top = y, bottom = y + h;

            var x1 = left + wr, x2 = right - wr,
                  y1 = top + hr,  y2 = bottom - hr;

            ctx.moveTo(x2, top);
            ctx.quadraticCurveTo(right, top, right, y1);
            ctx.lineTo(right, y2);
            ctx.quadraticCurveTo(right, bottom, x2, bottom);
            ctx.lineTo(x1, bottom);
            ctx.quadraticCurveTo(left, bottom, left, y2);
            ctx.lineTo(left, y1);
            ctx.quadraticCurveTo(left, top, x1, top);
            ctx.lineTo(x2, top);
        }
    }
});

var Sector = Path.extend({

    type: 'sector',

    shape: {

        cx: 0,

        cy: 0,

        r0: 0,

        r: 0,

        startAngle: 0,

        endAngle: Math.PI * 2,

        clockwise: true
    },

    brush: fixClipWithShadow(Path.prototype.brush),

    buildPath: function (ctx, shape) {

        var x = shape.cx;
        var y = shape.cy;
        var r0 = Math.max(shape.r0 || 0, 0);
        var r = Math.max(shape.r, 0);
        var startAngle = shape.startAngle;
        var endAngle = shape.endAngle;
        var clockwise = shape.clockwise;


        var cos = Math.cos, sin = Math.sin;

        var c1 = cos(startAngle), c2 = cos(endAngle),
            s1 = sin(startAngle), s2 = sin(endAngle);

        // hollow circle
        if(Math.abs(startAngle - endAngle) >= Math.PI * 2) {
            ctx.moveTo(c1 * r + x, s1 * r + y);
            ctx.arc(x, y, r, startAngle, endAngle, !clockwise);
            if(r0 !== 0) {
                ctx.moveTo(c2 * r0 + x, s2 * r0 + y);
                ctx.arc(x, y, r0, endAngle, startAngle, clockwise);
            }
        } else {
            ctx.moveTo(c1 * r0 + x, s1 * r0 + y);
            ctx.lineTo(c1 * r + x, s1 * r + y);
            ctx.arc(x, y, r, startAngle, endAngle, !clockwise);
            ctx.lineTo(c2 * r0 + x, s2 * r0 + y);

            (r0 !== 0) && ctx.arc(x, y, r0, endAngle, startAngle, clockwise);
        }

        ctx.closePath();
    }
});

/**
 * Created by Jeffrey on 2017/11/8.
 * 两端粗细不一的线
 * varLine : draws a line from A(x1,y1) to B(x2,y2)
 * that starts with a w1 width and ends with a w2 width.
 * @module zrender/graphic/shape/VarLine
 */

var sq = function (v) {
    return v * v
};

var VarLine = Path.extend({

    type: 'varLine',

    shape: {

        // Start point
        x1: 0,
        y1: 0,
        // End point
        x2: 0,
        y2: 0,

        w1: 0,
        w2: 0
    },

    style: {

        stroke: null,

        fill: '#000'
    },

    buildPath: function (ctx, shape) {
        var x1 = shape.x1,
            y1 = shape.y1,
            x2 = shape.x2,
            y2 = shape.y2,
            w1 = shape.w1,
            w2 = shape.w2;

        var dx = (x2 - x1),  shiftx = 0;
        var dy = (y2 - y1),  shifty = 0;
        w1 /= 2;   w2 /= 2; // we only use w1/2 and w2/2 for computations.
        // length of the AB vector
        var length = Math.sqrt(sq(dx) + sq(dy));
        if (!length) { return; } // exit if zero length
        dx /= length ;    dy /= length ;
        shiftx = - dy * w1 ;  // compute AA1 vector's x
        shifty =   dx * w1 ;  // compute AA1 vector's y
        var angle = Math.atan2(shifty, shiftx);
        ctx.moveTo(x1 + shiftx, y1 + shifty);
        ctx.arc(x1,y1, w1, angle, angle+Math.PI); // draw A1A2
        shiftx =  - dy * w2 ;  // compute BB1 vector's x
        shifty =    dx * w2 ;  // compute BB1 vector's y
        ctx.lineTo(x2 - shiftx, y2 - shifty); // draw A2B1
        ctx.arc(x2,y2, w2, angle+Math.PI, angle); // draw A1A2
        ctx.closePath(); // draw B2A1
    }
});

function ascending(a, b) {
  return a < b ? -1 : a > b ? 1 : a >= b ? 0 : NaN;
}

function bisector(compare) {
  if (compare.length === 1) { compare = ascendingComparator(compare); }
  return {
    left: function(a, x, lo, hi) {
      if (lo == null) { lo = 0; }
      if (hi == null) { hi = a.length; }
      while (lo < hi) {
        var mid = lo + hi >>> 1;
        if (compare(a[mid], x) < 0) { lo = mid + 1; }
        else { hi = mid; }
      }
      return lo;
    },
    right: function(a, x, lo, hi) {
      if (lo == null) { lo = 0; }
      if (hi == null) { hi = a.length; }
      while (lo < hi) {
        var mid = lo + hi >>> 1;
        if (compare(a[mid], x) > 0) { hi = mid; }
        else { lo = mid + 1; }
      }
      return lo;
    }
  };
}

function ascendingComparator(f) {
  return function(d, x) {
    return ascending(f(d), x);
  };
}

var ascendingBisect = bisector(ascending);

var noop$1 = {value: function() {}};

function dispatch() {
  var arguments$1 = arguments;

  for (var i = 0, n = arguments.length, _ = {}, t; i < n; ++i) {
    if (!(t = arguments$1[i] + "") || (t in _)) { throw new Error("illegal type: " + t); }
    _[t] = [];
  }
  return new Dispatch(_);
}

function Dispatch(_) {
  this._ = _;
}

function parseTypenames(typenames, types) {
  return typenames.trim().split(/^|\s+/).map(function(t) {
    var name = "", i = t.indexOf(".");
    if (i >= 0) { name = t.slice(i + 1), t = t.slice(0, i); }
    if (t && !types.hasOwnProperty(t)) { throw new Error("unknown type: " + t); }
    return {type: t, name: name};
  });
}

Dispatch.prototype = dispatch.prototype = {
  constructor: Dispatch,
  on: function(typename, callback) {
    var _ = this._,
        T = parseTypenames(typename + "", _),
        t,
        i = -1,
        n = T.length;

    // If no callback was specified, return the callback of the given type and name.
    if (arguments.length < 2) {
      while (++i < n) { if ((t = (typename = T[i]).type) && (t = get(_[t], typename.name))) { return t; } }
      return;
    }

    // If a type was specified, set the callback for the given type and name.
    // Otherwise, if a null callback was specified, remove callbacks of the given name.
    if (callback != null && typeof callback !== "function") { throw new Error("invalid callback: " + callback); }
    while (++i < n) {
      if (t = (typename = T[i]).type) { _[t] = set$1(_[t], typename.name, callback); }
      else if (callback == null) { for (t in _) { _[t] = set$1(_[t], typename.name, null); } }
    }

    return this;
  },
  copy: function() {
    var copy = {}, _ = this._;
    for (var t in _) { copy[t] = _[t].slice(); }
    return new Dispatch(copy);
  },
  call: function(type, that) {
    var arguments$1 = arguments;

    if ((n = arguments.length - 2) > 0) { for (var args = new Array(n), i = 0, n, t; i < n; ++i) { args[i] = arguments$1[i + 2]; } }
    if (!this._.hasOwnProperty(type)) { throw new Error("unknown type: " + type); }
    for (t = this._[type], i = 0, n = t.length; i < n; ++i) { t[i].value.apply(that, args); }
  },
  apply: function(type, that, args) {
    if (!this._.hasOwnProperty(type)) { throw new Error("unknown type: " + type); }
    for (var t = this._[type], i = 0, n = t.length; i < n; ++i) { t[i].value.apply(that, args); }
  }
};

function get(type, name) {
  for (var i = 0, n = type.length, c; i < n; ++i) {
    if ((c = type[i]).name === name) {
      return c.value;
    }
  }
}

function set$1(type, name, callback) {
  for (var i = 0, n = type.length; i < n; ++i) {
    if (type[i].name === name) {
      type[i] = noop$1, type = type.slice(0, i).concat(type.slice(i + 1));
      break;
    }
  }
  if (callback != null) { type.push({name: name, value: callback}); }
  return type;
}

var xhtml = "http://www.w3.org/1999/xhtml";

var namespaces = {
  svg: "http://www.w3.org/2000/svg",
  xhtml: xhtml,
  xlink: "http://www.w3.org/1999/xlink",
  xml: "http://www.w3.org/XML/1998/namespace",
  xmlns: "http://www.w3.org/2000/xmlns/"
};

function namespace(name) {
  var prefix = name += "", i = prefix.indexOf(":");
  if (i >= 0 && (prefix = name.slice(0, i)) !== "xmlns") { name = name.slice(i + 1); }
  return namespaces.hasOwnProperty(prefix) ? {space: namespaces[prefix], local: name} : name;
}

function creatorInherit(name) {
  return function() {
    var document = this.ownerDocument,
        uri = this.namespaceURI;
    return uri === xhtml && document.documentElement.namespaceURI === xhtml
        ? document.createElement(name)
        : document.createElementNS(uri, name);
  };
}

function creatorFixed(fullname) {
  return function() {
    return this.ownerDocument.createElementNS(fullname.space, fullname.local);
  };
}

function creator(name) {
  var fullname = namespace(name);
  return (fullname.local
      ? creatorFixed
      : creatorInherit)(fullname);
}

var matcher = function(selector) {
  return function() {
    return this.matches(selector);
  };
};

if (typeof document !== "undefined") {
  var element = document.documentElement;
  if (!element.matches) {
    var vendorMatches = element.webkitMatchesSelector
        || element.msMatchesSelector
        || element.mozMatchesSelector
        || element.oMatchesSelector;
    matcher = function(selector) {
      return function() {
        return vendorMatches.call(this, selector);
      };
    };
  }
}

var matcher$1 = matcher;

var filterEvents = {};

if (typeof document !== "undefined") {
  var element$1 = document.documentElement;
  if (!("onmouseenter" in element$1)) {
    filterEvents = {mouseenter: "mouseover", mouseleave: "mouseout"};
  }
}

function filterContextListener(listener, index, group) {
  listener = contextListener(listener, index, group);
  return function(event) {
    var related = event.relatedTarget;
    if (!related || (related !== this && !(related.compareDocumentPosition(this) & 8))) {
      listener.call(this, event);
    }
  };
}

function contextListener(listener, index, group) {
  return function(event1) {
    try {
      listener.call(this, this.__data__, index, group);
    } finally {
    }
  };
}

function parseTypenames$1(typenames) {
  return typenames.trim().split(/^|\s+/).map(function(t) {
    var name = "", i = t.indexOf(".");
    if (i >= 0) { name = t.slice(i + 1), t = t.slice(0, i); }
    return {type: t, name: name};
  });
}

function onRemove(typename) {
  return function() {
    var this$1 = this;

    var on = this.__on;
    if (!on) { return; }
    for (var j = 0, i = -1, m = on.length, o; j < m; ++j) {
      if (o = on[j], (!typename.type || o.type === typename.type) && o.name === typename.name) {
        this$1.removeEventListener(o.type, o.listener, o.capture);
      } else {
        on[++i] = o;
      }
    }
    if (++i) { on.length = i; }
    else { delete this.__on; }
  };
}

function onAdd(typename, value, capture) {
  var wrap = filterEvents.hasOwnProperty(typename.type) ? filterContextListener : contextListener;
  return function(d, i, group) {
    var this$1 = this;

    var on = this.__on, o, listener = wrap(value, i, group);
    if (on) { for (var j = 0, m = on.length; j < m; ++j) {
      if ((o = on[j]).type === typename.type && o.name === typename.name) {
        this$1.removeEventListener(o.type, o.listener, o.capture);
        this$1.addEventListener(o.type, o.listener = listener, o.capture = capture);
        o.value = value;
        return;
      }
    } }
    this.addEventListener(typename.type, listener, capture);
    o = {type: typename.type, name: typename.name, value: value, listener: listener, capture: capture};
    if (!on) { this.__on = [o]; }
    else { on.push(o); }
  };
}

function selection_on(typename, value, capture) {
  var this$1 = this;

  var typenames = parseTypenames$1(typename + ""), i, n = typenames.length, t;

  if (arguments.length < 2) {
    var on = this.node().__on;
    if (on) { for (var j = 0, m = on.length, o; j < m; ++j) {
      for (i = 0, o = on[j]; i < n; ++i) {
        if ((t = typenames[i]).type === o.type && t.name === o.name) {
          return o.value;
        }
      }
    } }
    return;
  }

  on = value ? onAdd : onRemove;
  if (capture == null) { capture = false; }
  for (i = 0; i < n; ++i) { this$1.each(on(typenames[i], value, capture)); }
  return this;
}

function none() {}

function selector(selector) {
  return selector == null ? none : function() {
    return this.querySelector(selector);
  };
}

function selection_select(select) {
  if (typeof select !== "function") { select = selector(select); }

  for (var groups = this._groups, m = groups.length, subgroups = new Array(m), j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, subgroup = subgroups[j] = new Array(n), node, subnode, i = 0; i < n; ++i) {
      if ((node = group[i]) && (subnode = select.call(node, node.__data__, i, group))) {
        if ("__data__" in node) { subnode.__data__ = node.__data__; }
        subgroup[i] = subnode;
      }
    }
  }

  return new Selection(subgroups, this._parents);
}

function empty() {
  return [];
}

function selectorAll(selector) {
  return selector == null ? empty : function() {
    return this.querySelectorAll(selector);
  };
}

function selection_selectAll(select) {
  if (typeof select !== "function") { select = selectorAll(select); }

  for (var groups = this._groups, m = groups.length, subgroups = [], parents = [], j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, node, i = 0; i < n; ++i) {
      if (node = group[i]) {
        subgroups.push(select.call(node, node.__data__, i, group));
        parents.push(node);
      }
    }
  }

  return new Selection(subgroups, parents);
}

function selection_filter(match) {
  if (typeof match !== "function") { match = matcher$1(match); }

  for (var groups = this._groups, m = groups.length, subgroups = new Array(m), j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, subgroup = subgroups[j] = [], node, i = 0; i < n; ++i) {
      if ((node = group[i]) && match.call(node, node.__data__, i, group)) {
        subgroup.push(node);
      }
    }
  }

  return new Selection(subgroups, this._parents);
}

function sparse(update) {
  return new Array(update.length);
}

function selection_enter() {
  return new Selection(this._enter || this._groups.map(sparse), this._parents);
}

function EnterNode(parent, datum) {
  this.ownerDocument = parent.ownerDocument;
  this.namespaceURI = parent.namespaceURI;
  this._next = null;
  this._parent = parent;
  this.__data__ = datum;
}

EnterNode.prototype = {
  constructor: EnterNode,
  appendChild: function(child) { return this._parent.insertBefore(child, this._next); },
  insertBefore: function(child, next) { return this._parent.insertBefore(child, next); },
  querySelector: function(selector) { return this._parent.querySelector(selector); },
  querySelectorAll: function(selector) { return this._parent.querySelectorAll(selector); }
};

function constant$1(x) {
  return function() {
    return x;
  };
}

var keyPrefix = "$"; // Protect against keys like “__proto__”.

function bindIndex(parent, group, enter, update, exit, data) {
  var i = 0,
      node,
      groupLength = group.length,
      dataLength = data.length;

  // Put any non-null nodes that fit into update.
  // Put any null nodes into enter.
  // Put any remaining data into enter.
  for (; i < dataLength; ++i) {
    if (node = group[i]) {
      node.__data__ = data[i];
      update[i] = node;
    } else {
      enter[i] = new EnterNode(parent, data[i]);
    }
  }

  // Put any non-null nodes that don’t fit into exit.
  for (; i < groupLength; ++i) {
    if (node = group[i]) {
      exit[i] = node;
    }
  }
}

function bindKey(parent, group, enter, update, exit, data, key) {
  var i,
      node,
      nodeByKeyValue = {},
      groupLength = group.length,
      dataLength = data.length,
      keyValues = new Array(groupLength),
      keyValue;

  // Compute the key for each node.
  // If multiple nodes have the same key, the duplicates are added to exit.
  for (i = 0; i < groupLength; ++i) {
    if (node = group[i]) {
      keyValues[i] = keyValue = keyPrefix + key.call(node, node.__data__, i, group);
      if (keyValue in nodeByKeyValue) {
        exit[i] = node;
      } else {
        nodeByKeyValue[keyValue] = node;
      }
    }
  }

  // Compute the key for each datum.
  // If there a node associated with this key, join and add it to update.
  // If there is not (or the key is a duplicate), add it to enter.
  for (i = 0; i < dataLength; ++i) {
    keyValue = keyPrefix + key.call(parent, data[i], i, data);
    if (node = nodeByKeyValue[keyValue]) {
      update[i] = node;
      node.__data__ = data[i];
      nodeByKeyValue[keyValue] = null;
    } else {
      enter[i] = new EnterNode(parent, data[i]);
    }
  }

  // Add any remaining nodes that were not bound to data to exit.
  for (i = 0; i < groupLength; ++i) {
    if ((node = group[i]) && (nodeByKeyValue[keyValues[i]] === node)) {
      exit[i] = node;
    }
  }
}

function selection_data(value, key) {
  if (!value) {
    data = new Array(this.size()), j = -1;
    this.each(function(d) { data[++j] = d; });
    return data;
  }

  var bind = key ? bindKey : bindIndex,
      parents = this._parents,
      groups = this._groups;

  if (typeof value !== "function") { value = constant$1(value); }

  for (var m = groups.length, update = new Array(m), enter = new Array(m), exit = new Array(m), j = 0; j < m; ++j) {
    var parent = parents[j],
        group = groups[j],
        groupLength = group.length,
        data = value.call(parent, parent && parent.__data__, j, parents),
        dataLength = data.length,
        enterGroup = enter[j] = new Array(dataLength),
        updateGroup = update[j] = new Array(dataLength),
        exitGroup = exit[j] = new Array(groupLength);

    bind(parent, group, enterGroup, updateGroup, exitGroup, data, key);

    // Now connect the enter nodes to their following update node, such that
    // appendChild can insert the materialized enter node before this node,
    // rather than at the end of the parent node.
    for (var i0 = 0, i1 = 0, previous, next; i0 < dataLength; ++i0) {
      if (previous = enterGroup[i0]) {
        if (i0 >= i1) { i1 = i0 + 1; }
        while (!(next = updateGroup[i1]) && ++i1 < dataLength){ }
        previous._next = next || null;
      }
    }
  }

  update = new Selection(update, parents);
  update._enter = enter;
  update._exit = exit;
  return update;
}

function selection_exit() {
  return new Selection(this._exit || this._groups.map(sparse), this._parents);
}

function selection_merge(selection$$1) {

  for (var groups0 = this._groups, groups1 = selection$$1._groups, m0 = groups0.length, m1 = groups1.length, m = Math.min(m0, m1), merges = new Array(m0), j = 0; j < m; ++j) {
    for (var group0 = groups0[j], group1 = groups1[j], n = group0.length, merge = merges[j] = new Array(n), node, i = 0; i < n; ++i) {
      if (node = group0[i] || group1[i]) {
        merge[i] = node;
      }
    }
  }

  for (; j < m0; ++j) {
    merges[j] = groups0[j];
  }

  return new Selection(merges, this._parents);
}

function selection_order() {

  for (var groups = this._groups, j = -1, m = groups.length; ++j < m;) {
    for (var group = groups[j], i = group.length - 1, next = group[i], node; --i >= 0;) {
      if (node = group[i]) {
        if (next && next !== node.nextSibling) { next.parentNode.insertBefore(node, next); }
        next = node;
      }
    }
  }

  return this;
}

function selection_sort(compare) {
  if (!compare) { compare = ascending$1; }

  function compareNode(a, b) {
    return a && b ? compare(a.__data__, b.__data__) : !a - !b;
  }

  for (var groups = this._groups, m = groups.length, sortgroups = new Array(m), j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, sortgroup = sortgroups[j] = new Array(n), node, i = 0; i < n; ++i) {
      if (node = group[i]) {
        sortgroup[i] = node;
      }
    }
    sortgroup.sort(compareNode);
  }

  return new Selection(sortgroups, this._parents).order();
}

function ascending$1(a, b) {
  return a < b ? -1 : a > b ? 1 : a >= b ? 0 : NaN;
}

function selection_call() {
  var callback = arguments[0];
  arguments[0] = this;
  callback.apply(null, arguments);
  return this;
}

function selection_nodes() {
  var nodes = new Array(this.size()), i = -1;
  this.each(function() { nodes[++i] = this; });
  return nodes;
}

function selection_node() {

  for (var groups = this._groups, j = 0, m = groups.length; j < m; ++j) {
    for (var group = groups[j], i = 0, n = group.length; i < n; ++i) {
      var node = group[i];
      if (node) { return node; }
    }
  }

  return null;
}

function selection_size() {
  var size = 0;
  this.each(function() { ++size; });
  return size;
}

function selection_empty() {
  return !this.node();
}

function selection_each(callback) {

  for (var groups = this._groups, j = 0, m = groups.length; j < m; ++j) {
    for (var group = groups[j], i = 0, n = group.length, node; i < n; ++i) {
      if (node = group[i]) { callback.call(node, node.__data__, i, group); }
    }
  }

  return this;
}

function attrRemove(name) {
  return function() {
    this.removeAttribute(name);
  };
}

function attrRemoveNS(fullname) {
  return function() {
    this.removeAttributeNS(fullname.space, fullname.local);
  };
}

function attrConstant(name, value) {
  return function() {
    this.setAttribute(name, value);
  };
}

function attrConstantNS(fullname, value) {
  return function() {
    this.setAttributeNS(fullname.space, fullname.local, value);
  };
}

function attrFunction(name, value) {
  return function() {
    var v = value.apply(this, arguments);
    if (v == null) { this.removeAttribute(name); }
    else { this.setAttribute(name, v); }
  };
}

function attrFunctionNS(fullname, value) {
  return function() {
    var v = value.apply(this, arguments);
    if (v == null) { this.removeAttributeNS(fullname.space, fullname.local); }
    else { this.setAttributeNS(fullname.space, fullname.local, v); }
  };
}

function selection_attr(name, value) {
  var fullname = namespace(name);

  if (arguments.length < 2) {
    var node = this.node();
    return fullname.local
        ? node.getAttributeNS(fullname.space, fullname.local)
        : node.getAttribute(fullname);
  }

  return this.each((value == null
      ? (fullname.local ? attrRemoveNS : attrRemove) : (typeof value === "function"
      ? (fullname.local ? attrFunctionNS : attrFunction)
      : (fullname.local ? attrConstantNS : attrConstant)))(fullname, value));
}

function defaultView(node) {
  return (node.ownerDocument && node.ownerDocument.defaultView) // node is a Node
      || (node.document && node) // node is a Window
      || node.defaultView; // node is a Document
}

function styleRemove(name) {
  return function() {
    this.style.removeProperty(name);
  };
}

function styleConstant(name, value, priority) {
  return function() {
    this.style.setProperty(name, value, priority);
  };
}

function styleFunction(name, value, priority) {
  return function() {
    var v = value.apply(this, arguments);
    if (v == null) { this.style.removeProperty(name); }
    else { this.style.setProperty(name, v, priority); }
  };
}

function selection_style(name, value, priority) {
  return arguments.length > 1
      ? this.each((value == null
            ? styleRemove : typeof value === "function"
            ? styleFunction
            : styleConstant)(name, value, priority == null ? "" : priority))
      : styleValue(this.node(), name);
}

function styleValue(node, name) {
  return node.style.getPropertyValue(name)
      || defaultView(node).getComputedStyle(node, null).getPropertyValue(name);
}

function propertyRemove(name) {
  return function() {
    delete this[name];
  };
}

function propertyConstant(name, value) {
  return function() {
    this[name] = value;
  };
}

function propertyFunction(name, value) {
  return function() {
    var v = value.apply(this, arguments);
    if (v == null) { delete this[name]; }
    else { this[name] = v; }
  };
}

function selection_property(name, value) {
  return arguments.length > 1
      ? this.each((value == null
          ? propertyRemove : typeof value === "function"
          ? propertyFunction
          : propertyConstant)(name, value))
      : this.node()[name];
}

function classArray(string) {
  return string.trim().split(/^|\s+/);
}

function classList(node) {
  return node.classList || new ClassList(node);
}

function ClassList(node) {
  this._node = node;
  this._names = classArray(node.getAttribute("class") || "");
}

ClassList.prototype = {
  add: function(name) {
    var i = this._names.indexOf(name);
    if (i < 0) {
      this._names.push(name);
      this._node.setAttribute("class", this._names.join(" "));
    }
  },
  remove: function(name) {
    var i = this._names.indexOf(name);
    if (i >= 0) {
      this._names.splice(i, 1);
      this._node.setAttribute("class", this._names.join(" "));
    }
  },
  contains: function(name) {
    return this._names.indexOf(name) >= 0;
  }
};

function classedAdd(node, names) {
  var list = classList(node), i = -1, n = names.length;
  while (++i < n) { list.add(names[i]); }
}

function classedRemove(node, names) {
  var list = classList(node), i = -1, n = names.length;
  while (++i < n) { list.remove(names[i]); }
}

function classedTrue(names) {
  return function() {
    classedAdd(this, names);
  };
}

function classedFalse(names) {
  return function() {
    classedRemove(this, names);
  };
}

function classedFunction(names, value) {
  return function() {
    (value.apply(this, arguments) ? classedAdd : classedRemove)(this, names);
  };
}

function selection_classed(name, value) {
  var names = classArray(name + "");

  if (arguments.length < 2) {
    var list = classList(this.node()), i = -1, n = names.length;
    while (++i < n) { if (!list.contains(names[i])) { return false; } }
    return true;
  }

  return this.each((typeof value === "function"
      ? classedFunction : value
      ? classedTrue
      : classedFalse)(names, value));
}

function textRemove() {
  this.textContent = "";
}

function textConstant(value) {
  return function() {
    this.textContent = value;
  };
}

function textFunction(value) {
  return function() {
    var v = value.apply(this, arguments);
    this.textContent = v == null ? "" : v;
  };
}

function selection_text(value) {
  return arguments.length
      ? this.each(value == null
          ? textRemove : (typeof value === "function"
          ? textFunction
          : textConstant)(value))
      : this.node().textContent;
}

function htmlRemove() {
  this.innerHTML = "";
}

function htmlConstant(value) {
  return function() {
    this.innerHTML = value;
  };
}

function htmlFunction(value) {
  return function() {
    var v = value.apply(this, arguments);
    this.innerHTML = v == null ? "" : v;
  };
}

function selection_html(value) {
  return arguments.length
      ? this.each(value == null
          ? htmlRemove : (typeof value === "function"
          ? htmlFunction
          : htmlConstant)(value))
      : this.node().innerHTML;
}

function raise() {
  if (this.nextSibling) { this.parentNode.appendChild(this); }
}

function selection_raise() {
  return this.each(raise);
}

function lower() {
  if (this.previousSibling) { this.parentNode.insertBefore(this, this.parentNode.firstChild); }
}

function selection_lower() {
  return this.each(lower);
}

function selection_append(name) {
  var create = typeof name === "function" ? name : creator(name);
  return this.select(function() {
    return this.appendChild(create.apply(this, arguments));
  });
}

function constantNull() {
  return null;
}

function selection_insert(name, before) {
  var create = typeof name === "function" ? name : creator(name),
      select = before == null ? constantNull : typeof before === "function" ? before : selector(before);
  return this.select(function() {
    return this.insertBefore(create.apply(this, arguments), select.apply(this, arguments) || null);
  });
}

function remove() {
  var parent = this.parentNode;
  if (parent) { parent.removeChild(this); }
}

function selection_remove() {
  return this.each(remove);
}

function selection_datum(value) {
  return arguments.length
      ? this.property("__data__", value)
      : this.node().__data__;
}

function dispatchEvent(node, type, params) {
  var window = defaultView(node),
      event = window.CustomEvent;

  if (typeof event === "function") {
    event = new event(type, params);
  } else {
    event = window.document.createEvent("Event");
    if (params) { event.initEvent(type, params.bubbles, params.cancelable), event.detail = params.detail; }
    else { event.initEvent(type, false, false); }
  }

  node.dispatchEvent(event);
}

function dispatchConstant(type, params) {
  return function() {
    return dispatchEvent(this, type, params);
  };
}

function dispatchFunction(type, params) {
  return function() {
    return dispatchEvent(this, type, params.apply(this, arguments));
  };
}

function selection_dispatch(type, params) {
  return this.each((typeof params === "function"
      ? dispatchFunction
      : dispatchConstant)(type, params));
}

var root = [null];

function Selection(groups, parents) {
  this._groups = groups;
  this._parents = parents;
}

function selection() {
  return new Selection([[document.documentElement]], root);
}

Selection.prototype = selection.prototype = {
  constructor: Selection,
  select: selection_select,
  selectAll: selection_selectAll,
  filter: selection_filter,
  data: selection_data,
  enter: selection_enter,
  exit: selection_exit,
  merge: selection_merge,
  order: selection_order,
  sort: selection_sort,
  call: selection_call,
  nodes: selection_nodes,
  node: selection_node,
  size: selection_size,
  empty: selection_empty,
  each: selection_each,
  attr: selection_attr,
  style: selection_style,
  property: selection_property,
  classed: selection_classed,
  text: selection_text,
  html: selection_html,
  raise: selection_raise,
  lower: selection_lower,
  append: selection_append,
  insert: selection_insert,
  remove: selection_remove,
  datum: selection_datum,
  on: selection_on,
  dispatch: selection_dispatch
};

function define(constructor, factory, prototype) {
  constructor.prototype = factory.prototype = prototype;
  prototype.constructor = constructor;
}

function extend$2(parent, definition) {
  var prototype = Object.create(parent.prototype);
  for (var key in definition) { prototype[key] = definition[key]; }
  return prototype;
}

function Color() {}

var darker = 0.7;
var brighter = 1 / darker;

var reI = "\\s*([+-]?\\d+)\\s*",
    reN = "\\s*([+-]?\\d*\\.?\\d+(?:[eE][+-]?\\d+)?)\\s*",
    reP = "\\s*([+-]?\\d*\\.?\\d+(?:[eE][+-]?\\d+)?)%\\s*",
    reHex3 = /^#([0-9a-f]{3})$/,
    reHex6 = /^#([0-9a-f]{6})$/,
    reRgbInteger = new RegExp("^rgb\\(" + [reI, reI, reI] + "\\)$"),
    reRgbPercent = new RegExp("^rgb\\(" + [reP, reP, reP] + "\\)$"),
    reRgbaInteger = new RegExp("^rgba\\(" + [reI, reI, reI, reN] + "\\)$"),
    reRgbaPercent = new RegExp("^rgba\\(" + [reP, reP, reP, reN] + "\\)$"),
    reHslPercent = new RegExp("^hsl\\(" + [reN, reP, reP] + "\\)$"),
    reHslaPercent = new RegExp("^hsla\\(" + [reN, reP, reP, reN] + "\\)$");

var named = {
  aliceblue: 0xf0f8ff,
  antiquewhite: 0xfaebd7,
  aqua: 0x00ffff,
  aquamarine: 0x7fffd4,
  azure: 0xf0ffff,
  beige: 0xf5f5dc,
  bisque: 0xffe4c4,
  black: 0x000000,
  blanchedalmond: 0xffebcd,
  blue: 0x0000ff,
  blueviolet: 0x8a2be2,
  brown: 0xa52a2a,
  burlywood: 0xdeb887,
  cadetblue: 0x5f9ea0,
  chartreuse: 0x7fff00,
  chocolate: 0xd2691e,
  coral: 0xff7f50,
  cornflowerblue: 0x6495ed,
  cornsilk: 0xfff8dc,
  crimson: 0xdc143c,
  cyan: 0x00ffff,
  darkblue: 0x00008b,
  darkcyan: 0x008b8b,
  darkgoldenrod: 0xb8860b,
  darkgray: 0xa9a9a9,
  darkgreen: 0x006400,
  darkgrey: 0xa9a9a9,
  darkkhaki: 0xbdb76b,
  darkmagenta: 0x8b008b,
  darkolivegreen: 0x556b2f,
  darkorange: 0xff8c00,
  darkorchid: 0x9932cc,
  darkred: 0x8b0000,
  darksalmon: 0xe9967a,
  darkseagreen: 0x8fbc8f,
  darkslateblue: 0x483d8b,
  darkslategray: 0x2f4f4f,
  darkslategrey: 0x2f4f4f,
  darkturquoise: 0x00ced1,
  darkviolet: 0x9400d3,
  deeppink: 0xff1493,
  deepskyblue: 0x00bfff,
  dimgray: 0x696969,
  dimgrey: 0x696969,
  dodgerblue: 0x1e90ff,
  firebrick: 0xb22222,
  floralwhite: 0xfffaf0,
  forestgreen: 0x228b22,
  fuchsia: 0xff00ff,
  gainsboro: 0xdcdcdc,
  ghostwhite: 0xf8f8ff,
  gold: 0xffd700,
  goldenrod: 0xdaa520,
  gray: 0x808080,
  green: 0x008000,
  greenyellow: 0xadff2f,
  grey: 0x808080,
  honeydew: 0xf0fff0,
  hotpink: 0xff69b4,
  indianred: 0xcd5c5c,
  indigo: 0x4b0082,
  ivory: 0xfffff0,
  khaki: 0xf0e68c,
  lavender: 0xe6e6fa,
  lavenderblush: 0xfff0f5,
  lawngreen: 0x7cfc00,
  lemonchiffon: 0xfffacd,
  lightblue: 0xadd8e6,
  lightcoral: 0xf08080,
  lightcyan: 0xe0ffff,
  lightgoldenrodyellow: 0xfafad2,
  lightgray: 0xd3d3d3,
  lightgreen: 0x90ee90,
  lightgrey: 0xd3d3d3,
  lightpink: 0xffb6c1,
  lightsalmon: 0xffa07a,
  lightseagreen: 0x20b2aa,
  lightskyblue: 0x87cefa,
  lightslategray: 0x778899,
  lightslategrey: 0x778899,
  lightsteelblue: 0xb0c4de,
  lightyellow: 0xffffe0,
  lime: 0x00ff00,
  limegreen: 0x32cd32,
  linen: 0xfaf0e6,
  magenta: 0xff00ff,
  maroon: 0x800000,
  mediumaquamarine: 0x66cdaa,
  mediumblue: 0x0000cd,
  mediumorchid: 0xba55d3,
  mediumpurple: 0x9370db,
  mediumseagreen: 0x3cb371,
  mediumslateblue: 0x7b68ee,
  mediumspringgreen: 0x00fa9a,
  mediumturquoise: 0x48d1cc,
  mediumvioletred: 0xc71585,
  midnightblue: 0x191970,
  mintcream: 0xf5fffa,
  mistyrose: 0xffe4e1,
  moccasin: 0xffe4b5,
  navajowhite: 0xffdead,
  navy: 0x000080,
  oldlace: 0xfdf5e6,
  olive: 0x808000,
  olivedrab: 0x6b8e23,
  orange: 0xffa500,
  orangered: 0xff4500,
  orchid: 0xda70d6,
  palegoldenrod: 0xeee8aa,
  palegreen: 0x98fb98,
  paleturquoise: 0xafeeee,
  palevioletred: 0xdb7093,
  papayawhip: 0xffefd5,
  peachpuff: 0xffdab9,
  peru: 0xcd853f,
  pink: 0xffc0cb,
  plum: 0xdda0dd,
  powderblue: 0xb0e0e6,
  purple: 0x800080,
  rebeccapurple: 0x663399,
  red: 0xff0000,
  rosybrown: 0xbc8f8f,
  royalblue: 0x4169e1,
  saddlebrown: 0x8b4513,
  salmon: 0xfa8072,
  sandybrown: 0xf4a460,
  seagreen: 0x2e8b57,
  seashell: 0xfff5ee,
  sienna: 0xa0522d,
  silver: 0xc0c0c0,
  skyblue: 0x87ceeb,
  slateblue: 0x6a5acd,
  slategray: 0x708090,
  slategrey: 0x708090,
  snow: 0xfffafa,
  springgreen: 0x00ff7f,
  steelblue: 0x4682b4,
  tan: 0xd2b48c,
  teal: 0x008080,
  thistle: 0xd8bfd8,
  tomato: 0xff6347,
  turquoise: 0x40e0d0,
  violet: 0xee82ee,
  wheat: 0xf5deb3,
  white: 0xffffff,
  whitesmoke: 0xf5f5f5,
  yellow: 0xffff00,
  yellowgreen: 0x9acd32
};

define(Color, color, {
  displayable: function() {
    return this.rgb().displayable();
  },
  toString: function() {
    return this.rgb() + "";
  }
});

function color(format) {
  var m;
  format = (format + "").trim().toLowerCase();
  return (m = reHex3.exec(format)) ? (m = parseInt(m[1], 16), new Rgb((m >> 8 & 0xf) | (m >> 4 & 0x0f0), (m >> 4 & 0xf) | (m & 0xf0), ((m & 0xf) << 4) | (m & 0xf), 1)) // #f00
      : (m = reHex6.exec(format)) ? rgbn(parseInt(m[1], 16)) // #ff0000
      : (m = reRgbInteger.exec(format)) ? new Rgb(m[1], m[2], m[3], 1) // rgb(255, 0, 0)
      : (m = reRgbPercent.exec(format)) ? new Rgb(m[1] * 255 / 100, m[2] * 255 / 100, m[3] * 255 / 100, 1) // rgb(100%, 0%, 0%)
      : (m = reRgbaInteger.exec(format)) ? rgba(m[1], m[2], m[3], m[4]) // rgba(255, 0, 0, 1)
      : (m = reRgbaPercent.exec(format)) ? rgba(m[1] * 255 / 100, m[2] * 255 / 100, m[3] * 255 / 100, m[4]) // rgb(100%, 0%, 0%, 1)
      : (m = reHslPercent.exec(format)) ? hsla(m[1], m[2] / 100, m[3] / 100, 1) // hsl(120, 50%, 50%)
      : (m = reHslaPercent.exec(format)) ? hsla(m[1], m[2] / 100, m[3] / 100, m[4]) // hsla(120, 50%, 50%, 1)
      : named.hasOwnProperty(format) ? rgbn(named[format])
      : format === "transparent" ? new Rgb(NaN, NaN, NaN, 0)
      : null;
}

function rgbn(n) {
  return new Rgb(n >> 16 & 0xff, n >> 8 & 0xff, n & 0xff, 1);
}

function rgba(r, g, b, a) {
  if (a <= 0) { r = g = b = NaN; }
  return new Rgb(r, g, b, a);
}

function rgbConvert(o) {
  if (!(o instanceof Color)) { o = color(o); }
  if (!o) { return new Rgb; }
  o = o.rgb();
  return new Rgb(o.r, o.g, o.b, o.opacity);
}

function rgb(r, g, b, opacity) {
  return arguments.length === 1 ? rgbConvert(r) : new Rgb(r, g, b, opacity == null ? 1 : opacity);
}

function Rgb(r, g, b, opacity) {
  this.r = +r;
  this.g = +g;
  this.b = +b;
  this.opacity = +opacity;
}

define(Rgb, rgb, extend$2(Color, {
  brighter: function(k) {
    k = k == null ? brighter : Math.pow(brighter, k);
    return new Rgb(this.r * k, this.g * k, this.b * k, this.opacity);
  },
  darker: function(k) {
    k = k == null ? darker : Math.pow(darker, k);
    return new Rgb(this.r * k, this.g * k, this.b * k, this.opacity);
  },
  rgb: function() {
    return this;
  },
  displayable: function() {
    return (0 <= this.r && this.r <= 255)
        && (0 <= this.g && this.g <= 255)
        && (0 <= this.b && this.b <= 255)
        && (0 <= this.opacity && this.opacity <= 1);
  },
  toString: function() {
    var a = this.opacity; a = isNaN(a) ? 1 : Math.max(0, Math.min(1, a));
    return (a === 1 ? "rgb(" : "rgba(")
        + Math.max(0, Math.min(255, Math.round(this.r) || 0)) + ", "
        + Math.max(0, Math.min(255, Math.round(this.g) || 0)) + ", "
        + Math.max(0, Math.min(255, Math.round(this.b) || 0))
        + (a === 1 ? ")" : ", " + a + ")");
  }
}));

function hsla(h, s, l, a) {
  if (a <= 0) { h = s = l = NaN; }
  else if (l <= 0 || l >= 1) { h = s = NaN; }
  else if (s <= 0) { h = NaN; }
  return new Hsl(h, s, l, a);
}

function hslConvert(o) {
  if (o instanceof Hsl) { return new Hsl(o.h, o.s, o.l, o.opacity); }
  if (!(o instanceof Color)) { o = color(o); }
  if (!o) { return new Hsl; }
  if (o instanceof Hsl) { return o; }
  o = o.rgb();
  var r = o.r / 255,
      g = o.g / 255,
      b = o.b / 255,
      min = Math.min(r, g, b),
      max = Math.max(r, g, b),
      h = NaN,
      s = max - min,
      l = (max + min) / 2;
  if (s) {
    if (r === max) { h = (g - b) / s + (g < b) * 6; }
    else if (g === max) { h = (b - r) / s + 2; }
    else { h = (r - g) / s + 4; }
    s /= l < 0.5 ? max + min : 2 - max - min;
    h *= 60;
  } else {
    s = l > 0 && l < 1 ? 0 : h;
  }
  return new Hsl(h, s, l, o.opacity);
}

function hsl(h, s, l, opacity) {
  return arguments.length === 1 ? hslConvert(h) : new Hsl(h, s, l, opacity == null ? 1 : opacity);
}

function Hsl(h, s, l, opacity) {
  this.h = +h;
  this.s = +s;
  this.l = +l;
  this.opacity = +opacity;
}

define(Hsl, hsl, extend$2(Color, {
  brighter: function(k) {
    k = k == null ? brighter : Math.pow(brighter, k);
    return new Hsl(this.h, this.s, this.l * k, this.opacity);
  },
  darker: function(k) {
    k = k == null ? darker : Math.pow(darker, k);
    return new Hsl(this.h, this.s, this.l * k, this.opacity);
  },
  rgb: function() {
    var h = this.h % 360 + (this.h < 0) * 360,
        s = isNaN(h) || isNaN(this.s) ? 0 : this.s,
        l = this.l,
        m2 = l + (l < 0.5 ? l : 1 - l) * s,
        m1 = 2 * l - m2;
    return new Rgb(
      hsl2rgb(h >= 240 ? h - 240 : h + 120, m1, m2),
      hsl2rgb(h, m1, m2),
      hsl2rgb(h < 120 ? h + 240 : h - 120, m1, m2),
      this.opacity
    );
  },
  displayable: function() {
    return (0 <= this.s && this.s <= 1 || isNaN(this.s))
        && (0 <= this.l && this.l <= 1)
        && (0 <= this.opacity && this.opacity <= 1);
  }
}));

/* From FvD 13.37, CSS Color Module Level 3 */
function hsl2rgb(h, m1, m2) {
  return (h < 60 ? m1 + (m2 - m1) * h / 60
      : h < 180 ? m2
      : h < 240 ? m1 + (m2 - m1) * (240 - h) / 60
      : m1) * 255;
}

var deg2rad = Math.PI / 180;
var rad2deg = 180 / Math.PI;

var Kn = 18,
    Xn = 0.950470, // D65 standard referent
    Yn = 1,
    Zn = 1.088830,
    t0 = 4 / 29,
    t1 = 6 / 29,
    t2 = 3 * t1 * t1,
    t3 = t1 * t1 * t1;

function labConvert(o) {
  if (o instanceof Lab) { return new Lab(o.l, o.a, o.b, o.opacity); }
  if (o instanceof Hcl) {
    var h = o.h * deg2rad;
    return new Lab(o.l, Math.cos(h) * o.c, Math.sin(h) * o.c, o.opacity);
  }
  if (!(o instanceof Rgb)) { o = rgbConvert(o); }
  var b = rgb2xyz(o.r),
      a = rgb2xyz(o.g),
      l = rgb2xyz(o.b),
      x = xyz2lab((0.4124564 * b + 0.3575761 * a + 0.1804375 * l) / Xn),
      y = xyz2lab((0.2126729 * b + 0.7151522 * a + 0.0721750 * l) / Yn),
      z = xyz2lab((0.0193339 * b + 0.1191920 * a + 0.9503041 * l) / Zn);
  return new Lab(116 * y - 16, 500 * (x - y), 200 * (y - z), o.opacity);
}

function lab(l, a, b, opacity) {
  return arguments.length === 1 ? labConvert(l) : new Lab(l, a, b, opacity == null ? 1 : opacity);
}

function Lab(l, a, b, opacity) {
  this.l = +l;
  this.a = +a;
  this.b = +b;
  this.opacity = +opacity;
}

define(Lab, lab, extend$2(Color, {
  brighter: function(k) {
    return new Lab(this.l + Kn * (k == null ? 1 : k), this.a, this.b, this.opacity);
  },
  darker: function(k) {
    return new Lab(this.l - Kn * (k == null ? 1 : k), this.a, this.b, this.opacity);
  },
  rgb: function() {
    var y = (this.l + 16) / 116,
        x = isNaN(this.a) ? y : y + this.a / 500,
        z = isNaN(this.b) ? y : y - this.b / 200;
    y = Yn * lab2xyz(y);
    x = Xn * lab2xyz(x);
    z = Zn * lab2xyz(z);
    return new Rgb(
      xyz2rgb( 3.2404542 * x - 1.5371385 * y - 0.4985314 * z), // D65 -> sRGB
      xyz2rgb(-0.9692660 * x + 1.8760108 * y + 0.0415560 * z),
      xyz2rgb( 0.0556434 * x - 0.2040259 * y + 1.0572252 * z),
      this.opacity
    );
  }
}));

function xyz2lab(t) {
  return t > t3 ? Math.pow(t, 1 / 3) : t / t2 + t0;
}

function lab2xyz(t) {
  return t > t1 ? t * t * t : t2 * (t - t0);
}

function xyz2rgb(x) {
  return 255 * (x <= 0.0031308 ? 12.92 * x : 1.055 * Math.pow(x, 1 / 2.4) - 0.055);
}

function rgb2xyz(x) {
  return (x /= 255) <= 0.04045 ? x / 12.92 : Math.pow((x + 0.055) / 1.055, 2.4);
}

function hclConvert(o) {
  if (o instanceof Hcl) { return new Hcl(o.h, o.c, o.l, o.opacity); }
  if (!(o instanceof Lab)) { o = labConvert(o); }
  var h = Math.atan2(o.b, o.a) * rad2deg;
  return new Hcl(h < 0 ? h + 360 : h, Math.sqrt(o.a * o.a + o.b * o.b), o.l, o.opacity);
}

function hcl(h, c, l, opacity) {
  return arguments.length === 1 ? hclConvert(h) : new Hcl(h, c, l, opacity == null ? 1 : opacity);
}

function Hcl(h, c, l, opacity) {
  this.h = +h;
  this.c = +c;
  this.l = +l;
  this.opacity = +opacity;
}

define(Hcl, hcl, extend$2(Color, {
  brighter: function(k) {
    return new Hcl(this.h, this.c, this.l + Kn * (k == null ? 1 : k), this.opacity);
  },
  darker: function(k) {
    return new Hcl(this.h, this.c, this.l - Kn * (k == null ? 1 : k), this.opacity);
  },
  rgb: function() {
    return labConvert(this).rgb();
  }
}));

var A = -0.14861,
    B = +1.78277,
    C = -0.29227,
    D = -0.90649,
    E = +1.97294,
    ED = E * D,
    EB = E * B,
    BC_DA = B * C - D * A;

function cubehelixConvert(o) {
  if (o instanceof Cubehelix) { return new Cubehelix(o.h, o.s, o.l, o.opacity); }
  if (!(o instanceof Rgb)) { o = rgbConvert(o); }
  var r = o.r / 255,
      g = o.g / 255,
      b = o.b / 255,
      l = (BC_DA * b + ED * r - EB * g) / (BC_DA + ED - EB),
      bl = b - l,
      k = (E * (g - l) - C * bl) / D,
      s = Math.sqrt(k * k + bl * bl) / (E * l * (1 - l)), // NaN if l=0 or l=1
      h = s ? Math.atan2(k, bl) * rad2deg - 120 : NaN;
  return new Cubehelix(h < 0 ? h + 360 : h, s, l, o.opacity);
}

function cubehelix(h, s, l, opacity) {
  return arguments.length === 1 ? cubehelixConvert(h) : new Cubehelix(h, s, l, opacity == null ? 1 : opacity);
}

function Cubehelix(h, s, l, opacity) {
  this.h = +h;
  this.s = +s;
  this.l = +l;
  this.opacity = +opacity;
}

define(Cubehelix, cubehelix, extend$2(Color, {
  brighter: function(k) {
    k = k == null ? brighter : Math.pow(brighter, k);
    return new Cubehelix(this.h, this.s, this.l * k, this.opacity);
  },
  darker: function(k) {
    k = k == null ? darker : Math.pow(darker, k);
    return new Cubehelix(this.h, this.s, this.l * k, this.opacity);
  },
  rgb: function() {
    var h = isNaN(this.h) ? 0 : (this.h + 120) * deg2rad,
        l = +this.l,
        a = isNaN(this.s) ? 0 : this.s * l * (1 - l),
        cosh = Math.cos(h),
        sinh = Math.sin(h);
    return new Rgb(
      255 * (l + a * (A * cosh + B * sinh)),
      255 * (l + a * (C * cosh + D * sinh)),
      255 * (l + a * (E * cosh)),
      this.opacity
    );
  }
}));

function constant$3(x) {
  return function() {
    return x;
  };
}

function linear(a, d) {
  return function(t) {
    return a + t * d;
  };
}

function exponential(a, b, y) {
  return a = Math.pow(a, y), b = Math.pow(b, y) - a, y = 1 / y, function(t) {
    return Math.pow(a + t * b, y);
  };
}

function hue(a, b) {
  var d = b - a;
  return d ? linear(a, d > 180 || d < -180 ? d - 360 * Math.round(d / 360) : d) : constant$3(isNaN(a) ? b : a);
}

function gamma(y) {
  return (y = +y) === 1 ? nogamma : function(a, b) {
    return b - a ? exponential(a, b, y) : constant$3(isNaN(a) ? b : a);
  };
}

function nogamma(a, b) {
  var d = b - a;
  return d ? linear(a, d) : constant$3(isNaN(a) ? b : a);
}

var interpolateRgb = (function rgbGamma(y) {
  var color$$1 = gamma(y);

  function rgb$$1(start, end) {
    var r = color$$1((start = rgb(start)).r, (end = rgb(end)).r),
        g = color$$1(start.g, end.g),
        b = color$$1(start.b, end.b),
        opacity = nogamma(start.opacity, end.opacity);
    return function(t) {
      start.r = r(t);
      start.g = g(t);
      start.b = b(t);
      start.opacity = opacity(t);
      return start + "";
    };
  }

  rgb$$1.gamma = rgbGamma;

  return rgb$$1;
})(1);

function array$1(a, b) {
  var nb = b ? b.length : 0,
      na = a ? Math.min(nb, a.length) : 0,
      x = new Array(na),
      c = new Array(nb),
      i;

  for (i = 0; i < na; ++i) { x[i] = interpolate$1(a[i], b[i]); }
  for (; i < nb; ++i) { c[i] = b[i]; }

  return function(t) {
    for (i = 0; i < na; ++i) { c[i] = x[i](t); }
    return c;
  };
}

function date(a, b) {
  var d = new Date;
  return a = +a, b -= a, function(t) {
    return d.setTime(a + b * t), d;
  };
}

function reinterpolate(a, b) {
  return a = +a, b -= a, function(t) {
    return a + b * t;
  };
}

function object(a, b) {
  var i = {},
      c = {},
      k;

  if (a === null || typeof a !== "object") { a = {}; }
  if (b === null || typeof b !== "object") { b = {}; }

  for (k in b) {
    if (k in a) {
      i[k] = interpolate$1(a[k], b[k]);
    } else {
      c[k] = b[k];
    }
  }

  return function(t) {
    for (k in i) { c[k] = i[k](t); }
    return c;
  };
}

var reA = /[-+]?(?:\d+\.?\d*|\.?\d+)(?:[eE][-+]?\d+)?/g,
    reB = new RegExp(reA.source, "g");

function zero(b) {
  return function() {
    return b;
  };
}

function one(b) {
  return function(t) {
    return b(t) + "";
  };
}

function interpolateString$1(a, b) {
  var bi = reA.lastIndex = reB.lastIndex = 0, // scan index for next number in b
      am, // current match in a
      bm, // current match in b
      bs, // string preceding current number in b, if any
      i = -1, // index in s
      s = [], // string constants and placeholders
      q = []; // number interpolators

  // Coerce inputs to strings.
  a = a + "", b = b + "";

  // Interpolate pairs of numbers in a & b.
  while ((am = reA.exec(a))
      && (bm = reB.exec(b))) {
    if ((bs = bm.index) > bi) { // a string precedes the next number in b
      bs = b.slice(bi, bs);
      if (s[i]) { s[i] += bs; } // coalesce with previous string
      else { s[++i] = bs; }
    }
    if ((am = am[0]) === (bm = bm[0])) { // numbers in a & b match
      if (s[i]) { s[i] += bm; } // coalesce with previous string
      else { s[++i] = bm; }
    } else { // interpolate non-matching numbers
      s[++i] = null;
      q.push({i: i, x: reinterpolate(am, bm)});
    }
    bi = reB.lastIndex;
  }

  // Add remains of b.
  if (bi < b.length) {
    bs = b.slice(bi);
    if (s[i]) { s[i] += bs; } // coalesce with previous string
    else { s[++i] = bs; }
  }

  // Special optimization for only a single match.
  // Otherwise, interpolate each of the numbers and rejoin the string.
  return s.length < 2 ? (q[0]
      ? one(q[0].x)
      : zero(b))
      : (b = q.length, function(t) {
          for (var i = 0, o; i < b; ++i) { s[(o = q[i]).i] = o.x(t); }
          return s.join("");
        });
}

function interpolate$1(a, b) {
  var t = typeof b, c;
  return b == null || t === "boolean" ? constant$3(b)
      : (t === "number" ? reinterpolate
      : t === "string" ? ((c = color(b)) ? (b = c, interpolateRgb) : interpolateString$1)
      : b instanceof color ? interpolateRgb
      : b instanceof Date ? date
      : Array.isArray(b) ? array$1
      : typeof b.valueOf !== "function" && typeof b.toString !== "function" || isNaN(b) ? object
      : reinterpolate)(a, b);
}

var degrees = 180 / Math.PI;

var identity$3 = {
  translateX: 0,
  translateY: 0,
  rotate: 0,
  skewX: 0,
  scaleX: 1,
  scaleY: 1
};

function decompose(a, b, c, d, e, f) {
  var scaleX, scaleY, skewX;
  if (scaleX = Math.sqrt(a * a + b * b)) { a /= scaleX, b /= scaleX; }
  if (skewX = a * c + b * d) { c -= a * skewX, d -= b * skewX; }
  if (scaleY = Math.sqrt(c * c + d * d)) { c /= scaleY, d /= scaleY, skewX /= scaleY; }
  if (a * d < b * c) { a = -a, b = -b, skewX = -skewX, scaleX = -scaleX; }
  return {
    translateX: e,
    translateY: f,
    rotate: Math.atan2(b, a) * degrees,
    skewX: Math.atan(skewX) * degrees,
    scaleX: scaleX,
    scaleY: scaleY
  };
}

var cssNode,
    cssRoot,
    cssView,
    svgNode;

function parseCss(value) {
  if (value === "none") { return identity$3; }
  if (!cssNode) { cssNode = document.createElement("DIV"), cssRoot = document.documentElement, cssView = document.defaultView; }
  cssNode.style.transform = value;
  value = cssView.getComputedStyle(cssRoot.appendChild(cssNode), null).getPropertyValue("transform");
  cssRoot.removeChild(cssNode);
  value = value.slice(7, -1).split(",");
  return decompose(+value[0], +value[1], +value[2], +value[3], +value[4], +value[5]);
}

function parseSvg(value) {
  if (value == null) { return identity$3; }
  if (!svgNode) { svgNode = document.createElementNS("http://www.w3.org/2000/svg", "g"); }
  svgNode.setAttribute("transform", value);
  if (!(value = svgNode.transform.baseVal.consolidate())) { return identity$3; }
  value = value.matrix;
  return decompose(value.a, value.b, value.c, value.d, value.e, value.f);
}

function interpolateTransform(parse, pxComma, pxParen, degParen) {

  function pop(s) {
    return s.length ? s.pop() + " " : "";
  }

  function translate(xa, ya, xb, yb, s, q) {
    if (xa !== xb || ya !== yb) {
      var i = s.push("translate(", null, pxComma, null, pxParen);
      q.push({i: i - 4, x: reinterpolate(xa, xb)}, {i: i - 2, x: reinterpolate(ya, yb)});
    } else if (xb || yb) {
      s.push("translate(" + xb + pxComma + yb + pxParen);
    }
  }

  function rotate(a, b, s, q) {
    if (a !== b) {
      if (a - b > 180) { b += 360; } else if (b - a > 180) { a += 360; } // shortest path
      q.push({i: s.push(pop(s) + "rotate(", null, degParen) - 2, x: reinterpolate(a, b)});
    } else if (b) {
      s.push(pop(s) + "rotate(" + b + degParen);
    }
  }

  function skewX(a, b, s, q) {
    if (a !== b) {
      q.push({i: s.push(pop(s) + "skewX(", null, degParen) - 2, x: reinterpolate(a, b)});
    } else if (b) {
      s.push(pop(s) + "skewX(" + b + degParen);
    }
  }

  function scale(xa, ya, xb, yb, s, q) {
    if (xa !== xb || ya !== yb) {
      var i = s.push(pop(s) + "scale(", null, ",", null, ")");
      q.push({i: i - 4, x: reinterpolate(xa, xb)}, {i: i - 2, x: reinterpolate(ya, yb)});
    } else if (xb !== 1 || yb !== 1) {
      s.push(pop(s) + "scale(" + xb + "," + yb + ")");
    }
  }

  return function(a, b) {
    var s = [], // string constants and placeholders
        q = []; // number interpolators
    a = parse(a), b = parse(b);
    translate(a.translateX, a.translateY, b.translateX, b.translateY, s, q);
    rotate(a.rotate, b.rotate, s, q);
    skewX(a.skewX, b.skewX, s, q);
    scale(a.scaleX, a.scaleY, b.scaleX, b.scaleY, s, q);
    a = b = null; // gc
    return function(t) {
      var i = -1, n = q.length, o;
      while (++i < n) { s[(o = q[i]).i] = o.x(t); }
      return s.join("");
    };
  };
}

var interpolateTransformCss = interpolateTransform(parseCss, "px, ", "px)", "deg)");
var interpolateTransformSvg = interpolateTransform(parseSvg, ", ", ")", ")");

var rho = Math.SQRT2;

function cubehelix$1(hue$$1) {
  return (function cubehelixGamma(y) {
    y = +y;

    function cubehelix$$1(start, end) {
      var h = hue$$1((start = cubehelix(start)).h, (end = cubehelix(end)).h),
          s = nogamma(start.s, end.s),
          l = nogamma(start.l, end.l),
          opacity = nogamma(start.opacity, end.opacity);
      return function(t) {
        start.h = h(t);
        start.s = s(t);
        start.l = l(Math.pow(t, y));
        start.opacity = opacity(t);
        return start + "";
      };
    }

    cubehelix$$1.gamma = cubehelixGamma;

    return cubehelix$$1;
  })(1);
}

cubehelix$1(hue);
var cubehelixLong = cubehelix$1(nogamma);

var frame = 0, // is an animation frame pending?
    timeout = 0, // is a timeout pending?
    interval = 0, // are any timers active?
    pokeDelay = 1000, // how frequently we check for clock skew
    taskHead,
    taskTail,
    clockLast = 0,
    clockNow = 0,
    clockSkew = 0,
    clock = typeof performance === "object" && performance.now ? performance : Date,
    setFrame = typeof window === "object" && window.requestAnimationFrame ? window.requestAnimationFrame.bind(window) : function(f) { setTimeout(f, 17); };

function now() {
  return clockNow || (setFrame(clearNow), clockNow = clock.now() + clockSkew);
}

function clearNow() {
  clockNow = 0;
}

function Timer() {
  this._call =
  this._time =
  this._next = null;
}

Timer.prototype = timer.prototype = {
  constructor: Timer,
  restart: function(callback, delay, time) {
    if (typeof callback !== "function") { throw new TypeError("callback is not a function"); }
    time = (time == null ? now() : +time) + (delay == null ? 0 : +delay);
    if (!this._next && taskTail !== this) {
      if (taskTail) { taskTail._next = this; }
      else { taskHead = this; }
      taskTail = this;
    }
    this._call = callback;
    this._time = time;
    sleep();
  },
  stop: function() {
    if (this._call) {
      this._call = null;
      this._time = Infinity;
      sleep();
    }
  }
};

function timer(callback, delay, time) {
  var t = new Timer;
  t.restart(callback, delay, time);
  return t;
}

function timerFlush() {
  now(); // Get the current time, if not already set.
  ++frame; // Pretend we’ve set an alarm, if we haven’t already.
  var t = taskHead, e;
  while (t) {
    if ((e = clockNow - t._time) >= 0) { t._call.call(null, e); }
    t = t._next;
  }
  --frame;
}

function wake() {
  clockNow = (clockLast = clock.now()) + clockSkew;
  frame = timeout = 0;
  try {
    timerFlush();
  } finally {
    frame = 0;
    nap();
    clockNow = 0;
  }
}

function poke() {
  var now = clock.now(), delay = now - clockLast;
  if (delay > pokeDelay) { clockSkew -= delay, clockLast = now; }
}

function nap() {
  var t0, t1 = taskHead, t2, time = Infinity;
  while (t1) {
    if (t1._call) {
      if (time > t1._time) { time = t1._time; }
      t0 = t1, t1 = t1._next;
    } else {
      t2 = t1._next, t1._next = null;
      t1 = t0 ? t0._next = t2 : taskHead = t2;
    }
  }
  taskTail = t0;
  sleep(time);
}

function sleep(time) {
  if (frame) { return; } // Soonest alarm already set, or will be.
  if (timeout) { timeout = clearTimeout(timeout); }
  var delay = time - clockNow; // Strictly less than if we recomputed clockNow.
  if (delay > 24) {
    if (time < Infinity) { timeout = setTimeout(wake, time - clock.now() - clockSkew); }
    if (interval) { interval = clearInterval(interval); }
  } else {
    if (!interval) { clockLast = clock.now(), interval = setInterval(poke, pokeDelay); }
    frame = 1, setFrame(wake);
  }
}

function timeout$1(callback, delay, time) {
  var t = new Timer;
  delay = delay == null ? 0 : +delay;
  t.restart(function(elapsed) {
    t.stop();
    callback(elapsed + delay);
  }, delay, time);
  return t;
}

function interval$1(callback, delay, time) {
  var t = new Timer, total = delay;
  if (delay == null) { return t.restart(callback, delay, time), t; }
  delay = +delay, time = time == null ? now() : +time;
  t.restart(function tick(elapsed) {
    elapsed += total;
    t.restart(tick, total += delay, time);
    callback(elapsed);
  }, delay, time);
  return t;
}

var emptyOn = dispatch("start", "end", "interrupt");
var emptyTween = [];

var CREATED = 0;
var SCHEDULED = 1;
var STARTING = 2;
var STARTED = 3;
var RUNNING = 4;
var ENDING = 5;
var ENDED = 6;

function schedule(node, name, id, index, group, timing) {
  var schedules = node.__transition;
  if (!schedules) { node.__transition = {}; }
  else if (id in schedules) { return; }
  create$3(node, id, {
    name: name,
    index: index, // For context during callback.
    group: group, // For context during callback.
    on: emptyOn,
    tween: emptyTween,
    time: timing.time,
    delay: timing.delay,
    duration: timing.duration,
    ease: timing.ease,
    timer: null,
    state: CREATED
  });
}

function init(node, id) {
  var schedule = get$1(node, id);
  if (schedule.state > CREATED) { throw new Error("too late; already scheduled"); }
  return schedule;
}

function set$2(node, id) {
  var schedule = get$1(node, id);
  if (schedule.state > STARTING) { throw new Error("too late; already started"); }
  return schedule;
}

function get$1(node, id) {
  var schedule = node.__transition;
  if (!schedule || !(schedule = schedule[id])) { throw new Error("transition not found"); }
  return schedule;
}

function create$3(node, id, self) {
  var schedules = node.__transition,
      tween;

  // Initialize the self timer when the transition is created.
  // Note the actual delay is not known until the first callback!
  schedules[id] = self;
  self.timer = timer(schedule, 0, self.time);

  function schedule(elapsed) {
    self.state = SCHEDULED;
    self.timer.restart(start, self.delay, self.time);

    // If the elapsed delay is less than our first sleep, start immediately.
    if (self.delay <= elapsed) { start(elapsed - self.delay); }
  }

  function start(elapsed) {
    var i, j, n, o;

    // If the state is not SCHEDULED, then we previously errored on start.
    if (self.state !== SCHEDULED) { return stop(); }

    for (i in schedules) {
      o = schedules[i];
      if (o.name !== self.name) { continue; }

      // While this element already has a starting transition during this frame,
      // defer starting an interrupting transition until that transition has a
      // chance to tick (and possibly end); see d3/d3-transition#54!
      if (o.state === STARTED) { return timeout$1(start); }

      // Interrupt the active transition, if any.
      // Dispatch the interrupt event.
      if (o.state === RUNNING) {
        o.state = ENDED;
        o.timer.stop();
        o.on.call("interrupt", node, node.__data__, o.index, o.group);
        delete schedules[i];
      }

      // Cancel any pre-empted transitions. No interrupt event is dispatched
      // because the cancelled transitions never started. Note that this also
      // removes this transition from the pending list!
      else if (+i < id) {
        o.state = ENDED;
        o.timer.stop();
        delete schedules[i];
      }
    }

    // Defer the first tick to end of the current frame; see d3/d3#1576.
    // Note the transition may be canceled after start and before the first tick!
    // Note this must be scheduled before the start event; see d3/d3-transition#16!
    // Assuming this is successful, subsequent callbacks go straight to tick.
    timeout$1(function() {
      if (self.state === STARTED) {
        self.state = RUNNING;
        self.timer.restart(tick, self.delay, self.time);
        tick(elapsed);
      }
    });

    // Dispatch the start event.
    // Note this must be done before the tween are initialized.
    self.state = STARTING;
    self.on.call("start", node, node.__data__, self.index, self.group);
    if (self.state !== STARTING) { return; } // interrupted
    self.state = STARTED;

    // Initialize the tween, deleting null tween.
    tween = new Array(n = self.tween.length);
    for (i = 0, j = -1; i < n; ++i) {
      if (o = self.tween[i].value.call(node, node.__data__, self.index, self.group)) {
        tween[++j] = o;
      }
    }
    tween.length = j + 1;
  }

  function tick(elapsed) {
    var t = elapsed < self.duration ? self.ease.call(null, elapsed / self.duration) : (self.timer.restart(stop), self.state = ENDING, 1),
        i = -1,
        n = tween.length;

    while (++i < n) {
      tween[i].call(null, t);
    }

    // Dispatch the end event.
    if (self.state === ENDING) {
      self.on.call("end", node, node.__data__, self.index, self.group);
      stop();
    }
  }

  function stop() {
    self.state = ENDED;
    self.timer.stop();
    delete schedules[id];
    for (var i in schedules) { return; } // eslint-disable-line no-unused-vars
    delete node.__transition;
  }
}

function interrupt(node, name) {
  var schedules = node.__transition,
      schedule$$1,
      active,
      empty = true,
      i;

  if (!schedules) { return; }

  name = name == null ? null : name + "";

  for (i in schedules) {
    if ((schedule$$1 = schedules[i]).name !== name) { empty = false; continue; }
    active = schedule$$1.state > STARTING && schedule$$1.state < ENDING;
    schedule$$1.state = ENDED;
    schedule$$1.timer.stop();
    if (active) { schedule$$1.on.call("interrupt", node, node.__data__, schedule$$1.index, schedule$$1.group); }
    delete schedules[i];
  }

  if (empty) { delete node.__transition; }
}

function selection_interrupt(name) {
  return this.each(function() {
    interrupt(this, name);
  });
}

function tweenRemove(id, name) {
  var tween0, tween1;
  return function() {
    var schedule$$1 = set$2(this, id),
        tween = schedule$$1.tween;

    // If this node shared tween with the previous node,
    // just assign the updated shared tween and we’re done!
    // Otherwise, copy-on-write.
    if (tween !== tween0) {
      tween1 = tween0 = tween;
      for (var i = 0, n = tween1.length; i < n; ++i) {
        if (tween1[i].name === name) {
          tween1 = tween1.slice();
          tween1.splice(i, 1);
          break;
        }
      }
    }

    schedule$$1.tween = tween1;
  };
}

function tweenFunction(id, name, value) {
  var tween0, tween1;
  if (typeof value !== "function") { throw new Error; }
  return function() {
    var schedule$$1 = set$2(this, id),
        tween = schedule$$1.tween;

    // If this node shared tween with the previous node,
    // just assign the updated shared tween and we’re done!
    // Otherwise, copy-on-write.
    if (tween !== tween0) {
      tween1 = (tween0 = tween).slice();
      for (var t = {name: name, value: value}, i = 0, n = tween1.length; i < n; ++i) {
        if (tween1[i].name === name) {
          tween1[i] = t;
          break;
        }
      }
      if (i === n) { tween1.push(t); }
    }

    schedule$$1.tween = tween1;
  };
}

function transition_tween(name, value) {
  var id = this._id;

  name += "";

  if (arguments.length < 2) {
    var tween = get$1(this.node(), id).tween;
    for (var i = 0, n = tween.length, t; i < n; ++i) {
      if ((t = tween[i]).name === name) {
        return t.value;
      }
    }
    return null;
  }

  return this.each((value == null ? tweenRemove : tweenFunction)(id, name, value));
}

function tweenValue(transition, name, value) {
  var id = transition._id;

  transition.each(function() {
    var schedule$$1 = set$2(this, id);
    (schedule$$1.value || (schedule$$1.value = {}))[name] = value.apply(this, arguments);
  });

  return function(node) {
    return get$1(node, id).value[name];
  };
}

function interpolate$2(a, b) {
  var c;
  return (typeof b === "number" ? reinterpolate
      : b instanceof color ? interpolateRgb
      : (c = color(b)) ? (b = c, interpolateRgb)
      : interpolateString$1)(a, b);
}

function attrRemove$1(name) {
  return function() {
    this.removeAttribute(name);
  };
}

function attrRemoveNS$1(fullname) {
  return function() {
    this.removeAttributeNS(fullname.space, fullname.local);
  };
}

function attrConstant$1(name, interpolate, value1) {
  var value00,
      interpolate0;
  return function() {
    var value0 = this.getAttribute(name);
    return value0 === value1 ? null
        : value0 === value00 ? interpolate0
        : interpolate0 = interpolate(value00 = value0, value1);
  };
}

function attrConstantNS$1(fullname, interpolate, value1) {
  var value00,
      interpolate0;
  return function() {
    var value0 = this.getAttributeNS(fullname.space, fullname.local);
    return value0 === value1 ? null
        : value0 === value00 ? interpolate0
        : interpolate0 = interpolate(value00 = value0, value1);
  };
}

function attrFunction$1(name, interpolate, value) {
  var value00,
      value10,
      interpolate0;
  return function() {
    var value0, value1 = value(this);
    if (value1 == null) { return void this.removeAttribute(name); }
    value0 = this.getAttribute(name);
    return value0 === value1 ? null
        : value0 === value00 && value1 === value10 ? interpolate0
        : interpolate0 = interpolate(value00 = value0, value10 = value1);
  };
}

function attrFunctionNS$1(fullname, interpolate, value) {
  var value00,
      value10,
      interpolate0;
  return function() {
    var value0, value1 = value(this);
    if (value1 == null) { return void this.removeAttributeNS(fullname.space, fullname.local); }
    value0 = this.getAttributeNS(fullname.space, fullname.local);
    return value0 === value1 ? null
        : value0 === value00 && value1 === value10 ? interpolate0
        : interpolate0 = interpolate(value00 = value0, value10 = value1);
  };
}

function transition_attr(name, value) {
  var fullname = namespace(name), i = fullname === "transform" ? interpolateTransformSvg : interpolate$2;
  return this.attrTween(name, typeof value === "function"
      ? (fullname.local ? attrFunctionNS$1 : attrFunction$1)(fullname, i, tweenValue(this, "attr." + name, value))
      : value == null ? (fullname.local ? attrRemoveNS$1 : attrRemove$1)(fullname)
      : (fullname.local ? attrConstantNS$1 : attrConstant$1)(fullname, i, value + ""));
}

function attrTweenNS(fullname, value) {
  function tween() {
    var node = this, i = value.apply(node, arguments);
    return i && function(t) {
      node.setAttributeNS(fullname.space, fullname.local, i(t));
    };
  }
  tween._value = value;
  return tween;
}

function attrTween(name, value) {
  function tween() {
    var node = this, i = value.apply(node, arguments);
    return i && function(t) {
      node.setAttribute(name, i(t));
    };
  }
  tween._value = value;
  return tween;
}

function transition_attrTween(name, value) {
  var key = "attr." + name;
  if (arguments.length < 2) { return (key = this.tween(key)) && key._value; }
  if (value == null) { return this.tween(key, null); }
  if (typeof value !== "function") { throw new Error; }
  var fullname = namespace(name);
  return this.tween(key, (fullname.local ? attrTweenNS : attrTween)(fullname, value));
}

function delayFunction(id, value) {
  return function() {
    init(this, id).delay = +value.apply(this, arguments);
  };
}

function delayConstant(id, value) {
  return value = +value, function() {
    init(this, id).delay = value;
  };
}

function transition_delay(value) {
  var id = this._id;

  return arguments.length
      ? this.each((typeof value === "function"
          ? delayFunction
          : delayConstant)(id, value))
      : get$1(this.node(), id).delay;
}

function durationFunction(id, value) {
  return function() {
    set$2(this, id).duration = +value.apply(this, arguments);
  };
}

function durationConstant(id, value) {
  return value = +value, function() {
    set$2(this, id).duration = value;
  };
}

function transition_duration(value) {
  var id = this._id;

  return arguments.length
      ? this.each((typeof value === "function"
          ? durationFunction
          : durationConstant)(id, value))
      : get$1(this.node(), id).duration;
}

function easeConstant(id, value) {
  if (typeof value !== "function") { throw new Error; }
  return function() {
    set$2(this, id).ease = value;
  };
}

function transition_ease(value) {
  var id = this._id;

  return arguments.length
      ? this.each(easeConstant(id, value))
      : get$1(this.node(), id).ease;
}

function transition_filter(match) {
  if (typeof match !== "function") { match = matcher$1(match); }

  for (var groups = this._groups, m = groups.length, subgroups = new Array(m), j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, subgroup = subgroups[j] = [], node, i = 0; i < n; ++i) {
      if ((node = group[i]) && match.call(node, node.__data__, i, group)) {
        subgroup.push(node);
      }
    }
  }

  return new Transition(subgroups, this._parents, this._name, this._id);
}

function transition_merge(transition$$1) {
  if (transition$$1._id !== this._id) { throw new Error; }

  for (var groups0 = this._groups, groups1 = transition$$1._groups, m0 = groups0.length, m1 = groups1.length, m = Math.min(m0, m1), merges = new Array(m0), j = 0; j < m; ++j) {
    for (var group0 = groups0[j], group1 = groups1[j], n = group0.length, merge = merges[j] = new Array(n), node, i = 0; i < n; ++i) {
      if (node = group0[i] || group1[i]) {
        merge[i] = node;
      }
    }
  }

  for (; j < m0; ++j) {
    merges[j] = groups0[j];
  }

  return new Transition(merges, this._parents, this._name, this._id);
}

function start$1(name) {
  return (name + "").trim().split(/^|\s+/).every(function(t) {
    var i = t.indexOf(".");
    if (i >= 0) { t = t.slice(0, i); }
    return !t || t === "start";
  });
}

function onFunction(id, name, listener) {
  var on0, on1, sit = start$1(name) ? init : set$2;
  return function() {
    var schedule$$1 = sit(this, id),
        on = schedule$$1.on;

    // If this node shared a dispatch with the previous node,
    // just assign the updated shared dispatch and we’re done!
    // Otherwise, copy-on-write.
    if (on !== on0) { (on1 = (on0 = on).copy()).on(name, listener); }

    schedule$$1.on = on1;
  };
}

function transition_on(name, listener) {
  var id = this._id;

  return arguments.length < 2
      ? get$1(this.node(), id).on.on(name)
      : this.each(onFunction(id, name, listener));
}

function removeFunction(id) {
  return function() {
    var this$1 = this;

    var parent = this.parentNode;
    for (var i in this$1.__transition) { if (+i !== id) { return; } }
    if (parent) { parent.removeChild(this); }
  };
}

function transition_remove() {
  return this.on("end.remove", removeFunction(this._id));
}

function transition_select(select$$1) {
  var name = this._name,
      id = this._id;

  if (typeof select$$1 !== "function") { select$$1 = selector(select$$1); }

  for (var groups = this._groups, m = groups.length, subgroups = new Array(m), j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, subgroup = subgroups[j] = new Array(n), node, subnode, i = 0; i < n; ++i) {
      if ((node = group[i]) && (subnode = select$$1.call(node, node.__data__, i, group))) {
        if ("__data__" in node) { subnode.__data__ = node.__data__; }
        subgroup[i] = subnode;
        schedule(subgroup[i], name, id, i, subgroup, get$1(node, id));
      }
    }
  }

  return new Transition(subgroups, this._parents, name, id);
}

function transition_selectAll(select$$1) {
  var name = this._name,
      id = this._id;

  if (typeof select$$1 !== "function") { select$$1 = selectorAll(select$$1); }

  for (var groups = this._groups, m = groups.length, subgroups = [], parents = [], j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, node, i = 0; i < n; ++i) {
      if (node = group[i]) {
        for (var children = select$$1.call(node, node.__data__, i, group), child, inherit = get$1(node, id), k = 0, l = children.length; k < l; ++k) {
          if (child = children[k]) {
            schedule(child, name, id, k, children, inherit);
          }
        }
        subgroups.push(children);
        parents.push(node);
      }
    }
  }

  return new Transition(subgroups, parents, name, id);
}

var Selection$1 = selection.prototype.constructor;

function transition_selection() {
  return new Selection$1(this._groups, this._parents);
}

function styleRemove$1(name, interpolate) {
  var value00,
      value10,
      interpolate0;
  return function() {
    var value0 = styleValue(this, name),
        value1 = (this.style.removeProperty(name), styleValue(this, name));
    return value0 === value1 ? null
        : value0 === value00 && value1 === value10 ? interpolate0
        : interpolate0 = interpolate(value00 = value0, value10 = value1);
  };
}

function styleRemoveEnd(name) {
  return function() {
    this.style.removeProperty(name);
  };
}

function styleConstant$1(name, interpolate, value1) {
  var value00,
      interpolate0;
  return function() {
    var value0 = styleValue(this, name);
    return value0 === value1 ? null
        : value0 === value00 ? interpolate0
        : interpolate0 = interpolate(value00 = value0, value1);
  };
}

function styleFunction$1(name, interpolate, value) {
  var value00,
      value10,
      interpolate0;
  return function() {
    var value0 = styleValue(this, name),
        value1 = value(this);
    if (value1 == null) { value1 = (this.style.removeProperty(name), styleValue(this, name)); }
    return value0 === value1 ? null
        : value0 === value00 && value1 === value10 ? interpolate0
        : interpolate0 = interpolate(value00 = value0, value10 = value1);
  };
}

function transition_style(name, value, priority) {
  var i = (name += "") === "transform" ? interpolateTransformCss : interpolate$2;
  return value == null ? this
          .styleTween(name, styleRemove$1(name, i))
          .on("end.style." + name, styleRemoveEnd(name))
      : this.styleTween(name, typeof value === "function"
          ? styleFunction$1(name, i, tweenValue(this, "style." + name, value))
          : styleConstant$1(name, i, value + ""), priority);
}

function styleTween(name, value, priority) {
  function tween() {
    var node = this, i = value.apply(node, arguments);
    return i && function(t) {
      node.style.setProperty(name, i(t), priority);
    };
  }
  tween._value = value;
  return tween;
}

function transition_styleTween(name, value, priority) {
  var key = "style." + (name += "");
  if (arguments.length < 2) { return (key = this.tween(key)) && key._value; }
  if (value == null) { return this.tween(key, null); }
  if (typeof value !== "function") { throw new Error; }
  return this.tween(key, styleTween(name, value, priority == null ? "" : priority));
}

function textConstant$1(value) {
  return function() {
    this.textContent = value;
  };
}

function textFunction$1(value) {
  return function() {
    var value1 = value(this);
    this.textContent = value1 == null ? "" : value1;
  };
}

function transition_text(value) {
  return this.tween("text", typeof value === "function"
      ? textFunction$1(tweenValue(this, "text", value))
      : textConstant$1(value == null ? "" : value + ""));
}

function transition_transition() {
  var name = this._name,
      id0 = this._id,
      id1 = newId();

  for (var groups = this._groups, m = groups.length, j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, node, i = 0; i < n; ++i) {
      if (node = group[i]) {
        var inherit = get$1(node, id0);
        schedule(node, name, id1, i, group, {
          time: inherit.time + inherit.delay + inherit.duration,
          delay: 0,
          duration: inherit.duration,
          ease: inherit.ease
        });
      }
    }
  }

  return new Transition(groups, this._parents, name, id1);
}

var id = 0;

function Transition(groups, parents, name, id) {
  this._groups = groups;
  this._parents = parents;
  this._name = name;
  this._id = id;
}

function transition(name) {
  return selection().transition(name);
}

function newId() {
  return ++id;
}

var selection_prototype = selection.prototype;

Transition.prototype = transition.prototype = {
  constructor: Transition,
  select: transition_select,
  selectAll: transition_selectAll,
  filter: transition_filter,
  merge: transition_merge,
  selection: transition_selection,
  transition: transition_transition,
  call: selection_prototype.call,
  nodes: selection_prototype.nodes,
  node: selection_prototype.node,
  size: selection_prototype.size,
  empty: selection_prototype.empty,
  each: selection_prototype.each,
  on: transition_on,
  attr: transition_attr,
  attrTween: transition_attrTween,
  style: transition_style,
  styleTween: transition_styleTween,
  text: transition_text,
  remove: transition_remove,
  tween: transition_tween,
  delay: transition_delay,
  duration: transition_duration,
  ease: transition_ease
};

function cubicInOut(t) {
  return ((t *= 2) <= 1 ? t * t * t : (t -= 2) * t * t + 2) / 2;
}

var pi = Math.PI;

var tau = 2 * Math.PI;

var defaultTiming = {
  time: null, // Set on use.
  delay: 0,
  duration: 250,
  ease: cubicInOut
};

function inherit(node, id) {
  var timing;
  while (!(timing = node.__transition) || !(timing = timing[id])) {
    if (!(node = node.parentNode)) {
      return defaultTiming.time = now(), defaultTiming;
    }
  }
  return timing;
}

function selection_transition(name) {
  var id,
      timing;

  if (name instanceof Transition) {
    id = name._id, name = name._name;
  } else {
    id = newId(), (timing = defaultTiming).time = now(), name = name == null ? null : name + "";
  }

  for (var groups = this._groups, m = groups.length, j = 0; j < m; ++j) {
    for (var group = groups[j], n = group.length, node, i = 0; i < n; ++i) {
      if (node = group[i]) {
        schedule(node, name, id, i, group, timing || inherit(node, id));
      }
    }
  }

  return new Transition(groups, this._parents, name, id);
}

selection.prototype.interrupt = selection_interrupt;
selection.prototype.transition = selection_transition;

var pi$1 = Math.PI;

var pi$2 = Math.PI,
    tau$2 = 2 * pi$2,
    epsilon$1 = 1e-6,
    tauEpsilon = tau$2 - epsilon$1;

function Path$1() {
  this._x0 = this._y0 = // start of current subpath
  this._x1 = this._y1 = null; // end of current subpath
  this._ = "";
}

function path() {
  return new Path$1;
}

Path$1.prototype = path.prototype = {
  constructor: Path$1,
  moveTo: function(x, y) {
    this._ += "M" + (this._x0 = this._x1 = +x) + "," + (this._y0 = this._y1 = +y);
  },
  closePath: function() {
    if (this._x1 !== null) {
      this._x1 = this._x0, this._y1 = this._y0;
      this._ += "Z";
    }
  },
  lineTo: function(x, y) {
    this._ += "L" + (this._x1 = +x) + "," + (this._y1 = +y);
  },
  quadraticCurveTo: function(x1, y1, x, y) {
    this._ += "Q" + (+x1) + "," + (+y1) + "," + (this._x1 = +x) + "," + (this._y1 = +y);
  },
  bezierCurveTo: function(x1, y1, x2, y2, x, y) {
    this._ += "C" + (+x1) + "," + (+y1) + "," + (+x2) + "," + (+y2) + "," + (this._x1 = +x) + "," + (this._y1 = +y);
  },
  arcTo: function(x1, y1, x2, y2, r) {
    x1 = +x1, y1 = +y1, x2 = +x2, y2 = +y2, r = +r;
    var x0 = this._x1,
        y0 = this._y1,
        x21 = x2 - x1,
        y21 = y2 - y1,
        x01 = x0 - x1,
        y01 = y0 - y1,
        l01_2 = x01 * x01 + y01 * y01;

    // Is the radius negative? Error.
    if (r < 0) { throw new Error("negative radius: " + r); }

    // Is this path empty? Move to (x1,y1).
    if (this._x1 === null) {
      this._ += "M" + (this._x1 = x1) + "," + (this._y1 = y1);
    }

    // Or, is (x1,y1) coincident with (x0,y0)? Do nothing.
    else if (!(l01_2 > epsilon$1)) {}

    // Or, are (x0,y0), (x1,y1) and (x2,y2) collinear?
    // Equivalently, is (x1,y1) coincident with (x2,y2)?
    // Or, is the radius zero? Line to (x1,y1).
    else if (!(Math.abs(y01 * x21 - y21 * x01) > epsilon$1) || !r) {
      this._ += "L" + (this._x1 = x1) + "," + (this._y1 = y1);
    }

    // Otherwise, draw an arc!
    else {
      var x20 = x2 - x0,
          y20 = y2 - y0,
          l21_2 = x21 * x21 + y21 * y21,
          l20_2 = x20 * x20 + y20 * y20,
          l21 = Math.sqrt(l21_2),
          l01 = Math.sqrt(l01_2),
          l = r * Math.tan((pi$2 - Math.acos((l21_2 + l01_2 - l20_2) / (2 * l21 * l01))) / 2),
          t01 = l / l01,
          t21 = l / l21;

      // If the start tangent is not coincident with (x0,y0), line to.
      if (Math.abs(t01 - 1) > epsilon$1) {
        this._ += "L" + (x1 + t01 * x01) + "," + (y1 + t01 * y01);
      }

      this._ += "A" + r + "," + r + ",0,0," + (+(y01 * x20 > x01 * y20)) + "," + (this._x1 = x1 + t21 * x21) + "," + (this._y1 = y1 + t21 * y21);
    }
  },
  arc: function(x, y, r, a0, a1, ccw) {
    x = +x, y = +y, r = +r;
    var dx = r * Math.cos(a0),
        dy = r * Math.sin(a0),
        x0 = x + dx,
        y0 = y + dy,
        cw = 1 ^ ccw,
        da = ccw ? a0 - a1 : a1 - a0;

    // Is the radius negative? Error.
    if (r < 0) { throw new Error("negative radius: " + r); }

    // Is this path empty? Move to (x0,y0).
    if (this._x1 === null) {
      this._ += "M" + x0 + "," + y0;
    }

    // Or, is (x0,y0) not coincident with the previous point? Line to (x0,y0).
    else if (Math.abs(this._x1 - x0) > epsilon$1 || Math.abs(this._y1 - y0) > epsilon$1) {
      this._ += "L" + x0 + "," + y0;
    }

    // Is this arc empty? We’re done.
    if (!r) { return; }

    // Does the angle go the wrong way? Flip the direction.
    if (da < 0) { da = da % tau$2 + tau$2; }

    // Is this a complete circle? Draw two arcs to complete the circle.
    if (da > tauEpsilon) {
      this._ += "A" + r + "," + r + ",0,1," + cw + "," + (x - dx) + "," + (y - dy) + "A" + r + "," + r + ",0,1," + cw + "," + (this._x1 = x0) + "," + (this._y1 = y0);
    }

    // Is this arc non-empty? Draw an arc!
    else if (da > epsilon$1) {
      this._ += "A" + r + "," + r + ",0," + (+(da >= pi$2)) + "," + cw + "," + (this._x1 = x + r * Math.cos(a1)) + "," + (this._y1 = y + r * Math.sin(a1));
    }
  },
  rect: function(x, y, w, h) {
    this._ += "M" + (this._x0 = this._x1 = +x) + "," + (this._y0 = this._y1 = +y) + "h" + (+w) + "v" + (+h) + "h" + (-w) + "Z";
  },
  toString: function() {
    return this._;
  }
};

var prefix = "$";

function Map() {}

Map.prototype = map$3.prototype = {
  constructor: Map,
  has: function(key) {
    return (prefix + key) in this;
  },
  get: function(key) {
    return this[prefix + key];
  },
  set: function(key, value) {
    this[prefix + key] = value;
    return this;
  },
  remove: function(key) {
    var property = prefix + key;
    return property in this && delete this[property];
  },
  clear: function() {
    var this$1 = this;

    for (var property in this$1) { if (property[0] === prefix) { delete this$1[property]; } }
  },
  keys: function() {
    var this$1 = this;

    var keys = [];
    for (var property in this$1) { if (property[0] === prefix) { keys.push(property.slice(1)); } }
    return keys;
  },
  values: function() {
    var this$1 = this;

    var values = [];
    for (var property in this$1) { if (property[0] === prefix) { values.push(this$1[property]); } }
    return values;
  },
  entries: function() {
    var this$1 = this;

    var entries = [];
    for (var property in this$1) { if (property[0] === prefix) { entries.push({key: property.slice(1), value: this$1[property]}); } }
    return entries;
  },
  size: function() {
    var this$1 = this;

    var size = 0;
    for (var property in this$1) { if (property[0] === prefix) { ++size; } }
    return size;
  },
  empty: function() {
    var this$1 = this;

    for (var property in this$1) { if (property[0] === prefix) { return false; } }
    return true;
  },
  each: function(f) {
    var this$1 = this;

    for (var property in this$1) { if (property[0] === prefix) { f(this$1[property], property.slice(1), this$1); } }
  }
};

function map$3(object, f) {
  var map = new Map;

  // Copy constructor.
  if (object instanceof Map) { object.each(function(value, key) { map.set(key, value); }); }

  // Index array by numeric index or specified key function.
  else if (Array.isArray(object)) {
    var i = -1,
        n = object.length,
        o;

    if (f == null) { while (++i < n) { map.set(i, object[i]); } }
    else { while (++i < n) { map.set(f(o = object[i], i, object), o); } }
  }

  // Convert object to map.
  else if (object) { for (var key in object) { map.set(key, object[key]); } }

  return map;
}

function Set() {}

var proto = map$3.prototype;

Set.prototype = set$3.prototype = {
  constructor: Set,
  has: proto.has,
  add: function(value) {
    value += "";
    this[prefix + value] = value;
    return this;
  },
  remove: proto.remove,
  clear: proto.clear,
  values: proto.keys,
  size: proto.size,
  empty: proto.empty,
  each: proto.each
};

function set$3(object, f) {
  var set = new Set;

  // Copy constructor.
  if (object instanceof Set) { object.each(function(value) { set.add(value); }); }

  // Otherwise, assume it’s an array.
  else if (object) {
    var i = -1, n = object.length;
    if (f == null) { while (++i < n) { set.add(object[i]); } }
    else { while (++i < n) { set.add(f(object[i], i, object)); } }
  }

  return set;
}

var EOL = {},
    EOF = {},
    QUOTE = 34,
    NEWLINE = 10,
    RETURN = 13;

function objectConverter(columns) {
  return new Function("d", "return {" + columns.map(function(name, i) {
    return JSON.stringify(name) + ": d[" + i + "]";
  }).join(",") + "}");
}

function customConverter(columns, f) {
  var object = objectConverter(columns);
  return function(row, i) {
    return f(object(row), i, columns);
  };
}

// Compute unique columns in order of discovery.
function inferColumns(rows) {
  var columnSet = Object.create(null),
      columns = [];

  rows.forEach(function(row) {
    for (var column in row) {
      if (!(column in columnSet)) {
        columns.push(columnSet[column] = column);
      }
    }
  });

  return columns;
}

function dsv(delimiter) {
  var reFormat = new RegExp("[\"" + delimiter + "\n\r]"),
      DELIMITER = delimiter.charCodeAt(0);

  function parse(text, f) {
    var convert, columns, rows = parseRows(text, function(row, i) {
      if (convert) { return convert(row, i - 1); }
      columns = row, convert = f ? customConverter(row, f) : objectConverter(row);
    });
    rows.columns = columns || [];
    return rows;
  }

  function parseRows(text, f) {
    var rows = [], // output rows
        N = text.length,
        I = 0, // current character index
        n = 0, // current line number
        t, // current token
        eof = N <= 0, // current token followed by EOF?
        eol = false; // current token followed by EOL?

    // Strip the trailing newline.
    if (text.charCodeAt(N - 1) === NEWLINE) { --N; }
    if (text.charCodeAt(N - 1) === RETURN) { --N; }

    function token() {
      if (eof) { return EOF; }
      if (eol) { return eol = false, EOL; }

      // Unescape quotes.
      var i, j = I, c;
      if (text.charCodeAt(j) === QUOTE) {
        while (I++ < N && text.charCodeAt(I) !== QUOTE || text.charCodeAt(++I) === QUOTE){ }
        if ((i = I) >= N) { eof = true; }
        else if ((c = text.charCodeAt(I++)) === NEWLINE) { eol = true; }
        else if (c === RETURN) { eol = true; if (text.charCodeAt(I) === NEWLINE) { ++I; } }
        return text.slice(j + 1, i - 1).replace(/""/g, "\"");
      }

      // Find next delimiter or newline.
      while (I < N) {
        if ((c = text.charCodeAt(i = I++)) === NEWLINE) { eol = true; }
        else if (c === RETURN) { eol = true; if (text.charCodeAt(I) === NEWLINE) { ++I; } }
        else if (c !== DELIMITER) { continue; }
        return text.slice(j, i);
      }

      // Return last token before EOF.
      return eof = true, text.slice(j, N);
    }

    while ((t = token()) !== EOF) {
      var row = [];
      while (t !== EOL && t !== EOF) { row.push(t), t = token(); }
      if (f && (row = f(row, n++)) == null) { continue; }
      rows.push(row);
    }

    return rows;
  }

  function format(rows, columns) {
    if (columns == null) { columns = inferColumns(rows); }
    return [columns.map(formatValue).join(delimiter)].concat(rows.map(function(row) {
      return columns.map(function(column) {
        return formatValue(row[column]);
      }).join(delimiter);
    })).join("\n");
  }

  function formatRows(rows) {
    return rows.map(formatRow).join("\n");
  }

  function formatRow(row) {
    return row.map(formatValue).join(delimiter);
  }

  function formatValue(text) {
    return text == null ? ""
        : reFormat.test(text += "") ? "\"" + text.replace(/"/g, "\"\"") + "\""
        : text;
  }

  return {
    parse: parse,
    parseRows: parseRows,
    format: format,
    formatRows: formatRows
  };
}

var csv = dsv(",");

var tsv = dsv("\t");

function tree_add(d) {
  var x = +this._x.call(null, d),
      y = +this._y.call(null, d);
  return add$1(this.cover(x, y), x, y, d);
}

function add$1(tree, x, y, d) {
  if (isNaN(x) || isNaN(y)) { return tree; } // ignore invalid points

  var parent,
      node = tree._root,
      leaf = {data: d},
      x0 = tree._x0,
      y0 = tree._y0,
      x1 = tree._x1,
      y1 = tree._y1,
      xm,
      ym,
      xp,
      yp,
      right,
      bottom,
      i,
      j;

  // If the tree is empty, initialize the root as a leaf.
  if (!node) { return tree._root = leaf, tree; }

  // Find the existing leaf for the new point, or add it.
  while (node.length) {
    if (right = x >= (xm = (x0 + x1) / 2)) { x0 = xm; } else { x1 = xm; }
    if (bottom = y >= (ym = (y0 + y1) / 2)) { y0 = ym; } else { y1 = ym; }
    if (parent = node, !(node = node[i = bottom << 1 | right])) { return parent[i] = leaf, tree; }
  }

  // Is the new point is exactly coincident with the existing point?
  xp = +tree._x.call(null, node.data);
  yp = +tree._y.call(null, node.data);
  if (x === xp && y === yp) { return leaf.next = node, parent ? parent[i] = leaf : tree._root = leaf, tree; }

  // Otherwise, split the leaf node until the old and new point are separated.
  do {
    parent = parent ? parent[i] = new Array(4) : tree._root = new Array(4);
    if (right = x >= (xm = (x0 + x1) / 2)) { x0 = xm; } else { x1 = xm; }
    if (bottom = y >= (ym = (y0 + y1) / 2)) { y0 = ym; } else { y1 = ym; }
  } while ((i = bottom << 1 | right) === (j = (yp >= ym) << 1 | (xp >= xm)));
  return parent[j] = node, parent[i] = leaf, tree;
}

function addAll(data) {
  var this$1 = this;

  var d, i, n = data.length,
      x,
      y,
      xz = new Array(n),
      yz = new Array(n),
      x0 = Infinity,
      y0 = Infinity,
      x1 = -Infinity,
      y1 = -Infinity;

  // Compute the points and their extent.
  for (i = 0; i < n; ++i) {
    if (isNaN(x = +this$1._x.call(null, d = data[i])) || isNaN(y = +this$1._y.call(null, d))) { continue; }
    xz[i] = x;
    yz[i] = y;
    if (x < x0) { x0 = x; }
    if (x > x1) { x1 = x; }
    if (y < y0) { y0 = y; }
    if (y > y1) { y1 = y; }
  }

  // If there were no (valid) points, inherit the existing extent.
  if (x1 < x0) { x0 = this._x0, x1 = this._x1; }
  if (y1 < y0) { y0 = this._y0, y1 = this._y1; }

  // Expand the tree to cover the new points.
  this.cover(x0, y0).cover(x1, y1);

  // Add the new points.
  for (i = 0; i < n; ++i) {
    add$1(this$1, xz[i], yz[i], data[i]);
  }

  return this;
}

function tree_cover(x, y) {
  if (isNaN(x = +x) || isNaN(y = +y)) { return this; } // ignore invalid points

  var x0 = this._x0,
      y0 = this._y0,
      x1 = this._x1,
      y1 = this._y1;

  // If the quadtree has no extent, initialize them.
  // Integer extent are necessary so that if we later double the extent,
  // the existing quadrant boundaries don’t change due to floating point error!
  if (isNaN(x0)) {
    x1 = (x0 = Math.floor(x)) + 1;
    y1 = (y0 = Math.floor(y)) + 1;
  }

  // Otherwise, double repeatedly to cover.
  else if (x0 > x || x > x1 || y0 > y || y > y1) {
    var z = x1 - x0,
        node = this._root,
        parent,
        i;

    switch (i = (y < (y0 + y1) / 2) << 1 | (x < (x0 + x1) / 2)) {
      case 0: {
        do { parent = new Array(4), parent[i] = node, node = parent; }
        while (z *= 2, x1 = x0 + z, y1 = y0 + z, x > x1 || y > y1);
        break;
      }
      case 1: {
        do { parent = new Array(4), parent[i] = node, node = parent; }
        while (z *= 2, x0 = x1 - z, y1 = y0 + z, x0 > x || y > y1);
        break;
      }
      case 2: {
        do { parent = new Array(4), parent[i] = node, node = parent; }
        while (z *= 2, x1 = x0 + z, y0 = y1 - z, x > x1 || y0 > y);
        break;
      }
      case 3: {
        do { parent = new Array(4), parent[i] = node, node = parent; }
        while (z *= 2, x0 = x1 - z, y0 = y1 - z, x0 > x || y0 > y);
        break;
      }
    }

    if (this._root && this._root.length) { this._root = node; }
  }

  // If the quadtree covers the point already, just return.
  else { return this; }

  this._x0 = x0;
  this._y0 = y0;
  this._x1 = x1;
  this._y1 = y1;
  return this;
}

function tree_data() {
  var data = [];
  this.visit(function(node) {
    if (!node.length) { do { data.push(node.data); } while (node = node.next) }
  });
  return data;
}

function tree_extent(_) {
  return arguments.length
      ? this.cover(+_[0][0], +_[0][1]).cover(+_[1][0], +_[1][1])
      : isNaN(this._x0) ? undefined : [[this._x0, this._y0], [this._x1, this._y1]];
}

function Quad(node, x0, y0, x1, y1) {
  this.node = node;
  this.x0 = x0;
  this.y0 = y0;
  this.x1 = x1;
  this.y1 = y1;
}

function tree_find(x, y, radius) {
  var this$1 = this;

  var data,
      x0 = this._x0,
      y0 = this._y0,
      x1,
      y1,
      x2,
      y2,
      x3 = this._x1,
      y3 = this._y1,
      quads = [],
      node = this._root,
      q,
      i;

  if (node) { quads.push(new Quad(node, x0, y0, x3, y3)); }
  if (radius == null) { radius = Infinity; }
  else {
    x0 = x - radius, y0 = y - radius;
    x3 = x + radius, y3 = y + radius;
    radius *= radius;
  }

  while (q = quads.pop()) {

    // Stop searching if this quadrant can’t contain a closer node.
    if (!(node = q.node)
        || (x1 = q.x0) > x3
        || (y1 = q.y0) > y3
        || (x2 = q.x1) < x0
        || (y2 = q.y1) < y0) { continue; }

    // Bisect the current quadrant.
    if (node.length) {
      var xm = (x1 + x2) / 2,
          ym = (y1 + y2) / 2;

      quads.push(
        new Quad(node[3], xm, ym, x2, y2),
        new Quad(node[2], x1, ym, xm, y2),
        new Quad(node[1], xm, y1, x2, ym),
        new Quad(node[0], x1, y1, xm, ym)
      );

      // Visit the closest quadrant first.
      if (i = (y >= ym) << 1 | (x >= xm)) {
        q = quads[quads.length - 1];
        quads[quads.length - 1] = quads[quads.length - 1 - i];
        quads[quads.length - 1 - i] = q;
      }
    }

    // Visit this point. (Visiting coincident points isn’t necessary!)
    else {
      var dx = x - +this$1._x.call(null, node.data),
          dy = y - +this$1._y.call(null, node.data),
          d2 = dx * dx + dy * dy;
      if (d2 < radius) {
        var d = Math.sqrt(radius = d2);
        x0 = x - d, y0 = y - d;
        x3 = x + d, y3 = y + d;
        data = node.data;
      }
    }
  }

  return data;
}

function tree_remove(d) {
  var this$1 = this;

  if (isNaN(x = +this._x.call(null, d)) || isNaN(y = +this._y.call(null, d))) { return this; } // ignore invalid points

  var parent,
      node = this._root,
      retainer,
      previous,
      next,
      x0 = this._x0,
      y0 = this._y0,
      x1 = this._x1,
      y1 = this._y1,
      x,
      y,
      xm,
      ym,
      right,
      bottom,
      i,
      j;

  // If the tree is empty, initialize the root as a leaf.
  if (!node) { return this; }

  // Find the leaf node for the point.
  // While descending, also retain the deepest parent with a non-removed sibling.
  if (node.length) { while (true) {
    if (right = x >= (xm = (x0 + x1) / 2)) { x0 = xm; } else { x1 = xm; }
    if (bottom = y >= (ym = (y0 + y1) / 2)) { y0 = ym; } else { y1 = ym; }
    if (!(parent = node, node = node[i = bottom << 1 | right])) { return this$1; }
    if (!node.length) { break; }
    if (parent[(i + 1) & 3] || parent[(i + 2) & 3] || parent[(i + 3) & 3]) { retainer = parent, j = i; }
  } }

  // Find the point to remove.
  while (node.data !== d) { if (!(previous = node, node = node.next)) { return this$1; } }
  if (next = node.next) { delete node.next; }

  // If there are multiple coincident points, remove just the point.
  if (previous) { return next ? previous.next = next : delete previous.next, this; }

  // If this is the root point, remove it.
  if (!parent) { return this._root = next, this; }

  // Remove this leaf.
  next ? parent[i] = next : delete parent[i];

  // If the parent now contains exactly one leaf, collapse superfluous parents.
  if ((node = parent[0] || parent[1] || parent[2] || parent[3])
      && node === (parent[3] || parent[2] || parent[1] || parent[0])
      && !node.length) {
    if (retainer) { retainer[j] = node; }
    else { this._root = node; }
  }

  return this;
}

function removeAll(data) {
  var this$1 = this;

  for (var i = 0, n = data.length; i < n; ++i) { this$1.remove(data[i]); }
  return this;
}

function tree_root() {
  return this._root;
}

function tree_size() {
  var size = 0;
  this.visit(function(node) {
    if (!node.length) { do { ++size; } while (node = node.next) }
  });
  return size;
}

function tree_visit(callback) {
  var quads = [], q, node = this._root, child, x0, y0, x1, y1;
  if (node) { quads.push(new Quad(node, this._x0, this._y0, this._x1, this._y1)); }
  while (q = quads.pop()) {
    if (!callback(node = q.node, x0 = q.x0, y0 = q.y0, x1 = q.x1, y1 = q.y1) && node.length) {
      var xm = (x0 + x1) / 2, ym = (y0 + y1) / 2;
      if (child = node[3]) { quads.push(new Quad(child, xm, ym, x1, y1)); }
      if (child = node[2]) { quads.push(new Quad(child, x0, ym, xm, y1)); }
      if (child = node[1]) { quads.push(new Quad(child, xm, y0, x1, ym)); }
      if (child = node[0]) { quads.push(new Quad(child, x0, y0, xm, ym)); }
    }
  }
  return this;
}

function tree_visitAfter(callback) {
  var quads = [], next = [], q;
  if (this._root) { quads.push(new Quad(this._root, this._x0, this._y0, this._x1, this._y1)); }
  while (q = quads.pop()) {
    var node = q.node;
    if (node.length) {
      var child, x0 = q.x0, y0 = q.y0, x1 = q.x1, y1 = q.y1, xm = (x0 + x1) / 2, ym = (y0 + y1) / 2;
      if (child = node[0]) { quads.push(new Quad(child, x0, y0, xm, ym)); }
      if (child = node[1]) { quads.push(new Quad(child, xm, y0, x1, ym)); }
      if (child = node[2]) { quads.push(new Quad(child, x0, ym, xm, y1)); }
      if (child = node[3]) { quads.push(new Quad(child, xm, ym, x1, y1)); }
    }
    next.push(q);
  }
  while (q = next.pop()) {
    callback(q.node, q.x0, q.y0, q.x1, q.y1);
  }
  return this;
}

function defaultX(d) {
  return d[0];
}

function tree_x(_) {
  return arguments.length ? (this._x = _, this) : this._x;
}

function defaultY(d) {
  return d[1];
}

function tree_y(_) {
  return arguments.length ? (this._y = _, this) : this._y;
}

function quadtree(nodes, x, y) {
  var tree = new Quadtree(x == null ? defaultX : x, y == null ? defaultY : y, NaN, NaN, NaN, NaN);
  return nodes == null ? tree : tree.addAll(nodes);
}

function Quadtree(x, y, x0, y0, x1, y1) {
  this._x = x;
  this._y = y;
  this._x0 = x0;
  this._y0 = y0;
  this._x1 = x1;
  this._y1 = y1;
  this._root = undefined;
}

function leaf_copy(leaf) {
  var copy = {data: leaf.data}, next = copy;
  while (leaf = leaf.next) { next = next.next = {data: leaf.data}; }
  return copy;
}

var treeProto = quadtree.prototype = Quadtree.prototype;

treeProto.copy = function() {
  var copy = new Quadtree(this._x, this._y, this._x0, this._y0, this._x1, this._y1),
      node = this._root,
      nodes,
      child;

  if (!node) { return copy; }

  if (!node.length) { return copy._root = leaf_copy(node), copy; }

  nodes = [{source: node, target: copy._root = new Array(4)}];
  while (node = nodes.pop()) {
    for (var i = 0; i < 4; ++i) {
      if (child = node.source[i]) {
        if (child.length) { nodes.push({source: child, target: node.target[i] = new Array(4)}); }
        else { node.target[i] = leaf_copy(child); }
      }
    }
  }

  return copy;
};

treeProto.add = tree_add;
treeProto.addAll = addAll;
treeProto.cover = tree_cover;
treeProto.data = tree_data;
treeProto.extent = tree_extent;
treeProto.find = tree_find;
treeProto.remove = tree_remove;
treeProto.removeAll = removeAll;
treeProto.root = tree_root;
treeProto.size = tree_size;
treeProto.visit = tree_visit;
treeProto.visitAfter = tree_visitAfter;
treeProto.x = tree_x;
treeProto.y = tree_y;

var initialAngle = Math.PI * (3 - Math.sqrt(5));

// Computes the decimal coefficient and exponent of the specified number x with
// significant digits p, where x is positive and p is in [1, 21] or undefined.
// For example, formatDecimal(1.23) returns ["123", 0].
function formatDecimal(x, p) {
  if ((i = (x = p ? x.toExponential(p - 1) : x.toExponential()).indexOf("e")) < 0) { return null; } // NaN, ±Infinity
  var i, coefficient = x.slice(0, i);

  // The string returned by toExponential either has the form \d\.\d+e[-+]\d+
  // (e.g., 1.2e+3) or the form \de[-+]\d+ (e.g., 1e+3).
  return [
    coefficient.length > 1 ? coefficient[0] + coefficient.slice(2) : coefficient,
    +x.slice(i + 1)
  ];
}

function exponent$1(x) {
  return x = formatDecimal(Math.abs(x)), x ? x[1] : NaN;
}

function formatGroup(grouping, thousands) {
  return function(value, width) {
    var i = value.length,
        t = [],
        j = 0,
        g = grouping[0],
        length = 0;

    while (i > 0 && g > 0) {
      if (length + g + 1 > width) { g = Math.max(1, width - length); }
      t.push(value.substring(i -= g, i + g));
      if ((length += g + 1) > width) { break; }
      g = grouping[j = (j + 1) % grouping.length];
    }

    return t.reverse().join(thousands);
  };
}

function formatNumerals(numerals) {
  return function(value) {
    return value.replace(/[0-9]/g, function(i) {
      return numerals[+i];
    });
  };
}

function formatDefault(x, p) {
  x = x.toPrecision(p);

  out: for (var n = x.length, i = 1, i0 = -1, i1; i < n; ++i) {
    switch (x[i]) {
      case ".": i0 = i1 = i; break;
      case "0": if (i0 === 0) { i0 = i; } i1 = i; break;
      case "e": break out;
      default: if (i0 > 0) { i0 = 0; } break;
    }
  }

  return i0 > 0 ? x.slice(0, i0) + x.slice(i1 + 1) : x;
}

var prefixExponent;

function formatPrefixAuto(x, p) {
  var d = formatDecimal(x, p);
  if (!d) { return x + ""; }
  var coefficient = d[0],
      exponent = d[1],
      i = exponent - (prefixExponent = Math.max(-8, Math.min(8, Math.floor(exponent / 3))) * 3) + 1,
      n = coefficient.length;
  return i === n ? coefficient
      : i > n ? coefficient + new Array(i - n + 1).join("0")
      : i > 0 ? coefficient.slice(0, i) + "." + coefficient.slice(i)
      : "0." + new Array(1 - i).join("0") + formatDecimal(x, Math.max(0, p + i - 1))[0]; // less than 1y!
}

function formatRounded(x, p) {
  var d = formatDecimal(x, p);
  if (!d) { return x + ""; }
  var coefficient = d[0],
      exponent = d[1];
  return exponent < 0 ? "0." + new Array(-exponent).join("0") + coefficient
      : coefficient.length > exponent + 1 ? coefficient.slice(0, exponent + 1) + "." + coefficient.slice(exponent + 1)
      : coefficient + new Array(exponent - coefficient.length + 2).join("0");
}

var formatTypes = {
  "": formatDefault,
  "%": function(x, p) { return (x * 100).toFixed(p); },
  "b": function(x) { return Math.round(x).toString(2); },
  "c": function(x) { return x + ""; },
  "d": function(x) { return Math.round(x).toString(10); },
  "e": function(x, p) { return x.toExponential(p); },
  "f": function(x, p) { return x.toFixed(p); },
  "g": function(x, p) { return x.toPrecision(p); },
  "o": function(x) { return Math.round(x).toString(8); },
  "p": function(x, p) { return formatRounded(x * 100, p); },
  "r": formatRounded,
  "s": formatPrefixAuto,
  "X": function(x) { return Math.round(x).toString(16).toUpperCase(); },
  "x": function(x) { return Math.round(x).toString(16); }
};

// [[fill]align][sign][symbol][0][width][,][.precision][type]
var re = /^(?:(.)?([<>=^]))?([+\-\( ])?([$#])?(0)?(\d+)?(,)?(\.\d+)?([a-z%])?$/i;

function formatSpecifier(specifier) {
  return new FormatSpecifier(specifier);
}

formatSpecifier.prototype = FormatSpecifier.prototype; // instanceof

function FormatSpecifier(specifier) {
  if (!(match = re.exec(specifier))) { throw new Error("invalid format: " + specifier); }

  var match,
      fill = match[1] || " ",
      align = match[2] || ">",
      sign = match[3] || "-",
      symbol = match[4] || "",
      zero = !!match[5],
      width = match[6] && +match[6],
      comma = !!match[7],
      precision = match[8] && +match[8].slice(1),
      type = match[9] || "";

  // The "n" type is an alias for ",g".
  if (type === "n") { comma = true, type = "g"; }

  // Map invalid types to the default format.
  else if (!formatTypes[type]) { type = ""; }

  // If zero fill is specified, padding goes after sign and before digits.
  if (zero || (fill === "0" && align === "=")) { zero = true, fill = "0", align = "="; }

  this.fill = fill;
  this.align = align;
  this.sign = sign;
  this.symbol = symbol;
  this.zero = zero;
  this.width = width;
  this.comma = comma;
  this.precision = precision;
  this.type = type;
}

FormatSpecifier.prototype.toString = function() {
  return this.fill
      + this.align
      + this.sign
      + this.symbol
      + (this.zero ? "0" : "")
      + (this.width == null ? "" : Math.max(1, this.width | 0))
      + (this.comma ? "," : "")
      + (this.precision == null ? "" : "." + Math.max(0, this.precision | 0))
      + this.type;
};

function identity$4(x) {
  return x;
}

var prefixes = ["y","z","a","f","p","n","µ","m","","k","M","G","T","P","E","Z","Y"];

function formatLocale(locale) {
  var group = locale.grouping && locale.thousands ? formatGroup(locale.grouping, locale.thousands) : identity$4,
      currency = locale.currency,
      decimal = locale.decimal,
      numerals = locale.numerals ? formatNumerals(locale.numerals) : identity$4,
      percent = locale.percent || "%";

  function newFormat(specifier) {
    specifier = formatSpecifier(specifier);

    var fill = specifier.fill,
        align = specifier.align,
        sign = specifier.sign,
        symbol = specifier.symbol,
        zero = specifier.zero,
        width = specifier.width,
        comma = specifier.comma,
        precision = specifier.precision,
        type = specifier.type;

    // Compute the prefix and suffix.
    // For SI-prefix, the suffix is lazily computed.
    var prefix = symbol === "$" ? currency[0] : symbol === "#" && /[boxX]/.test(type) ? "0" + type.toLowerCase() : "",
        suffix = symbol === "$" ? currency[1] : /[%p]/.test(type) ? percent : "";

    // What format function should we use?
    // Is this an integer type?
    // Can this type generate exponential notation?
    var formatType = formatTypes[type],
        maybeSuffix = !type || /[defgprs%]/.test(type);

    // Set the default precision if not specified,
    // or clamp the specified precision to the supported range.
    // For significant precision, it must be in [1, 21].
    // For fixed precision, it must be in [0, 20].
    precision = precision == null ? (type ? 6 : 12)
        : /[gprs]/.test(type) ? Math.max(1, Math.min(21, precision))
        : Math.max(0, Math.min(20, precision));

    function format(value) {
      var valuePrefix = prefix,
          valueSuffix = suffix,
          i, n, c;

      if (type === "c") {
        valueSuffix = formatType(value) + valueSuffix;
        value = "";
      } else {
        value = +value;

        // Perform the initial formatting.
        var valueNegative = value < 0;
        value = formatType(Math.abs(value), precision);

        // If a negative value rounds to zero during formatting, treat as positive.
        if (valueNegative && +value === 0) { valueNegative = false; }

        // Compute the prefix and suffix.
        valuePrefix = (valueNegative ? (sign === "(" ? sign : "-") : sign === "-" || sign === "(" ? "" : sign) + valuePrefix;
        valueSuffix = valueSuffix + (type === "s" ? prefixes[8 + prefixExponent / 3] : "") + (valueNegative && sign === "(" ? ")" : "");

        // Break the formatted value into the integer “value” part that can be
        // grouped, and fractional or exponential “suffix” part that is not.
        if (maybeSuffix) {
          i = -1, n = value.length;
          while (++i < n) {
            if (c = value.charCodeAt(i), 48 > c || c > 57) {
              valueSuffix = (c === 46 ? decimal + value.slice(i + 1) : value.slice(i)) + valueSuffix;
              value = value.slice(0, i);
              break;
            }
          }
        }
      }

      // If the fill character is not "0", grouping is applied before padding.
      if (comma && !zero) { value = group(value, Infinity); }

      // Compute the padding.
      var length = valuePrefix.length + value.length + valueSuffix.length,
          padding = length < width ? new Array(width - length + 1).join(fill) : "";

      // If the fill character is "0", grouping is applied after padding.
      if (comma && zero) { value = group(padding + value, padding.length ? width - valueSuffix.length : Infinity), padding = ""; }

      // Reconstruct the final output based on the desired alignment.
      switch (align) {
        case "<": value = valuePrefix + value + valueSuffix + padding; break;
        case "=": value = valuePrefix + padding + value + valueSuffix; break;
        case "^": value = padding.slice(0, length = padding.length >> 1) + valuePrefix + value + valueSuffix + padding.slice(length); break;
        default: value = padding + valuePrefix + value + valueSuffix; break;
      }

      return numerals(value);
    }

    format.toString = function() {
      return specifier + "";
    };

    return format;
  }

  function formatPrefix(specifier, value) {
    var f = newFormat((specifier = formatSpecifier(specifier), specifier.type = "f", specifier)),
        e = Math.max(-8, Math.min(8, Math.floor(exponent$1(value) / 3))) * 3,
        k = Math.pow(10, -e),
        prefix = prefixes[8 + e / 3];
    return function(value) {
      return f(k * value) + prefix;
    };
  }

  return {
    format: newFormat,
    formatPrefix: formatPrefix
  };
}

var locale;
var format$1;
var formatPrefix;

defaultLocale({
  decimal: ".",
  thousands: ",",
  grouping: [3],
  currency: ["$", ""]
});

function defaultLocale(definition) {
  locale = formatLocale(definition);
  format$1 = locale.format;
  formatPrefix = locale.formatPrefix;
  return locale;
}

// Adds floating point numbers with twice the normal precision.
// Reference: J. R. Shewchuk, Adaptive Precision Floating-Point Arithmetic and
// Fast Robust Geometric Predicates, Discrete & Computational Geometry 18(3)
// 305–363 (1997).
// Code adapted from GeographicLib by Charles F. F. Karney,
// http://geographiclib.sourceforge.net/

function adder() {
  return new Adder;
}

function Adder() {
  this.reset();
}

Adder.prototype = {
  constructor: Adder,
  reset: function() {
    this.s = // rounded value
    this.t = 0; // exact error
  },
  add: function(y) {
    add$2(temp, y, this.t);
    add$2(this, temp.s, this.s);
    if (this.s) { this.t += temp.t; }
    else { this.s = temp.t; }
  },
  valueOf: function() {
    return this.s;
  }
};

var temp = new Adder;

function add$2(adder, a, b) {
  var x = adder.s = a + b,
      bv = x - a,
      av = x - bv;
  adder.t = (a - av) + (b - bv);
}

var pi$3 = Math.PI;

var areaRingSum = adder();

var areaSum = adder();

var deltaSum = adder();

var sum$1 = adder();

var lengthSum = adder();

var areaSum$1 = adder(),
    areaRingSum$1 = adder();

var lengthSum$1 = adder();

// Returns the 2D cross product of AB and AC vectors, i.e., the z-component of

var t0$1 = new Date,
    t1$1 = new Date;

function newInterval(floori, offseti, count, field) {

  function interval(date) {
    return floori(date = new Date(+date)), date;
  }

  interval.floor = interval;

  interval.ceil = function(date) {
    return floori(date = new Date(date - 1)), offseti(date, 1), floori(date), date;
  };

  interval.round = function(date) {
    var d0 = interval(date),
        d1 = interval.ceil(date);
    return date - d0 < d1 - date ? d0 : d1;
  };

  interval.offset = function(date, step) {
    return offseti(date = new Date(+date), step == null ? 1 : Math.floor(step)), date;
  };

  interval.range = function(start, stop, step) {
    var range = [], previous;
    start = interval.ceil(start);
    step = step == null ? 1 : Math.floor(step);
    if (!(start < stop) || !(step > 0)) { return range; } // also handles Invalid Date
    do { range.push(previous = new Date(+start)), offseti(start, step), floori(start); }
    while (previous < start && start < stop);
    return range;
  };

  interval.filter = function(test) {
    return newInterval(function(date) {
      if (date >= date) { while (floori(date), !test(date)) { date.setTime(date - 1); } }
    }, function(date, step) {
      if (date >= date) {
        if (step < 0) { while (++step <= 0) {
          while (offseti(date, -1), !test(date)) {} // eslint-disable-line no-empty
        } } else { while (--step >= 0) {
          while (offseti(date, +1), !test(date)) {} // eslint-disable-line no-empty
        } }
      }
    });
  };

  if (count) {
    interval.count = function(start, end) {
      t0$1.setTime(+start), t1$1.setTime(+end);
      floori(t0$1), floori(t1$1);
      return Math.floor(count(t0$1, t1$1));
    };

    interval.every = function(step) {
      step = Math.floor(step);
      return !isFinite(step) || !(step > 0) ? null
          : !(step > 1) ? interval
          : interval.filter(field
              ? function(d) { return field(d) % step === 0; }
              : function(d) { return interval.count(0, d) % step === 0; });
    };
  }

  return interval;
}

var millisecond = newInterval(function() {
  // noop
}, function(date, step) {
  date.setTime(+date + step);
}, function(start, end) {
  return end - start;
});

// An optimized implementation for this simple case.
millisecond.every = function(k) {
  k = Math.floor(k);
  if (!isFinite(k) || !(k > 0)) { return null; }
  if (!(k > 1)) { return millisecond; }
  return newInterval(function(date) {
    date.setTime(Math.floor(date / k) * k);
  }, function(date, step) {
    date.setTime(+date + step * k);
  }, function(start, end) {
    return (end - start) / k;
  });
};

var durationSecond = 1e3;
var durationMinute = 6e4;
var durationHour = 36e5;
var durationDay = 864e5;
var durationWeek = 6048e5;

var second = newInterval(function(date) {
  date.setTime(Math.floor(date / durationSecond) * durationSecond);
}, function(date, step) {
  date.setTime(+date + step * durationSecond);
}, function(start, end) {
  return (end - start) / durationSecond;
}, function(date) {
  return date.getUTCSeconds();
});

var minute = newInterval(function(date) {
  date.setTime(Math.floor(date / durationMinute) * durationMinute);
}, function(date, step) {
  date.setTime(+date + step * durationMinute);
}, function(start, end) {
  return (end - start) / durationMinute;
}, function(date) {
  return date.getMinutes();
});

var hour = newInterval(function(date) {
  var offset = date.getTimezoneOffset() * durationMinute % durationHour;
  if (offset < 0) { offset += durationHour; }
  date.setTime(Math.floor((+date - offset) / durationHour) * durationHour + offset);
}, function(date, step) {
  date.setTime(+date + step * durationHour);
}, function(start, end) {
  return (end - start) / durationHour;
}, function(date) {
  return date.getHours();
});

var day = newInterval(function(date) {
  date.setHours(0, 0, 0, 0);
}, function(date, step) {
  date.setDate(date.getDate() + step);
}, function(start, end) {
  return (end - start - (end.getTimezoneOffset() - start.getTimezoneOffset()) * durationMinute) / durationDay;
}, function(date) {
  return date.getDate() - 1;
});

function weekday(i) {
  return newInterval(function(date) {
    date.setDate(date.getDate() - (date.getDay() + 7 - i) % 7);
    date.setHours(0, 0, 0, 0);
  }, function(date, step) {
    date.setDate(date.getDate() + step * 7);
  }, function(start, end) {
    return (end - start - (end.getTimezoneOffset() - start.getTimezoneOffset()) * durationMinute) / durationWeek;
  });
}

var sunday = weekday(0);
var monday = weekday(1);
var tuesday = weekday(2);
var wednesday = weekday(3);
var thursday = weekday(4);
var friday = weekday(5);
var saturday = weekday(6);

var month = newInterval(function(date) {
  date.setDate(1);
  date.setHours(0, 0, 0, 0);
}, function(date, step) {
  date.setMonth(date.getMonth() + step);
}, function(start, end) {
  return end.getMonth() - start.getMonth() + (end.getFullYear() - start.getFullYear()) * 12;
}, function(date) {
  return date.getMonth();
});

var year = newInterval(function(date) {
  date.setMonth(0, 1);
  date.setHours(0, 0, 0, 0);
}, function(date, step) {
  date.setFullYear(date.getFullYear() + step);
}, function(start, end) {
  return end.getFullYear() - start.getFullYear();
}, function(date) {
  return date.getFullYear();
});

// An optimized implementation for this simple case.
year.every = function(k) {
  return !isFinite(k = Math.floor(k)) || !(k > 0) ? null : newInterval(function(date) {
    date.setFullYear(Math.floor(date.getFullYear() / k) * k);
    date.setMonth(0, 1);
    date.setHours(0, 0, 0, 0);
  }, function(date, step) {
    date.setFullYear(date.getFullYear() + step * k);
  });
};

var utcMinute = newInterval(function(date) {
  date.setUTCSeconds(0, 0);
}, function(date, step) {
  date.setTime(+date + step * durationMinute);
}, function(start, end) {
  return (end - start) / durationMinute;
}, function(date) {
  return date.getUTCMinutes();
});

var utcHour = newInterval(function(date) {
  date.setUTCMinutes(0, 0, 0);
}, function(date, step) {
  date.setTime(+date + step * durationHour);
}, function(start, end) {
  return (end - start) / durationHour;
}, function(date) {
  return date.getUTCHours();
});

var utcDay = newInterval(function(date) {
  date.setUTCHours(0, 0, 0, 0);
}, function(date, step) {
  date.setUTCDate(date.getUTCDate() + step);
}, function(start, end) {
  return (end - start) / durationDay;
}, function(date) {
  return date.getUTCDate() - 1;
});

function utcWeekday(i) {
  return newInterval(function(date) {
    date.setUTCDate(date.getUTCDate() - (date.getUTCDay() + 7 - i) % 7);
    date.setUTCHours(0, 0, 0, 0);
  }, function(date, step) {
    date.setUTCDate(date.getUTCDate() + step * 7);
  }, function(start, end) {
    return (end - start) / durationWeek;
  });
}

var utcSunday = utcWeekday(0);
var utcMonday = utcWeekday(1);
var utcTuesday = utcWeekday(2);
var utcWednesday = utcWeekday(3);
var utcThursday = utcWeekday(4);
var utcFriday = utcWeekday(5);
var utcSaturday = utcWeekday(6);

var utcMonth = newInterval(function(date) {
  date.setUTCDate(1);
  date.setUTCHours(0, 0, 0, 0);
}, function(date, step) {
  date.setUTCMonth(date.getUTCMonth() + step);
}, function(start, end) {
  return end.getUTCMonth() - start.getUTCMonth() + (end.getUTCFullYear() - start.getUTCFullYear()) * 12;
}, function(date) {
  return date.getUTCMonth();
});

var utcYear = newInterval(function(date) {
  date.setUTCMonth(0, 1);
  date.setUTCHours(0, 0, 0, 0);
}, function(date, step) {
  date.setUTCFullYear(date.getUTCFullYear() + step);
}, function(start, end) {
  return end.getUTCFullYear() - start.getUTCFullYear();
}, function(date) {
  return date.getUTCFullYear();
});

// An optimized implementation for this simple case.
utcYear.every = function(k) {
  return !isFinite(k = Math.floor(k)) || !(k > 0) ? null : newInterval(function(date) {
    date.setUTCFullYear(Math.floor(date.getUTCFullYear() / k) * k);
    date.setUTCMonth(0, 1);
    date.setUTCHours(0, 0, 0, 0);
  }, function(date, step) {
    date.setUTCFullYear(date.getUTCFullYear() + step * k);
  });
};

function localDate(d) {
  if (0 <= d.y && d.y < 100) {
    var date = new Date(-1, d.m, d.d, d.H, d.M, d.S, d.L);
    date.setFullYear(d.y);
    return date;
  }
  return new Date(d.y, d.m, d.d, d.H, d.M, d.S, d.L);
}

function utcDate(d) {
  if (0 <= d.y && d.y < 100) {
    var date = new Date(Date.UTC(-1, d.m, d.d, d.H, d.M, d.S, d.L));
    date.setUTCFullYear(d.y);
    return date;
  }
  return new Date(Date.UTC(d.y, d.m, d.d, d.H, d.M, d.S, d.L));
}

function newYear(y) {
  return {y: y, m: 0, d: 1, H: 0, M: 0, S: 0, L: 0};
}

function formatLocale$1(locale) {
  var locale_dateTime = locale.dateTime,
      locale_date = locale.date,
      locale_time = locale.time,
      locale_periods = locale.periods,
      locale_weekdays = locale.days,
      locale_shortWeekdays = locale.shortDays,
      locale_months = locale.months,
      locale_shortMonths = locale.shortMonths;

  var periodRe = formatRe(locale_periods),
      periodLookup = formatLookup(locale_periods),
      weekdayRe = formatRe(locale_weekdays),
      weekdayLookup = formatLookup(locale_weekdays),
      shortWeekdayRe = formatRe(locale_shortWeekdays),
      shortWeekdayLookup = formatLookup(locale_shortWeekdays),
      monthRe = formatRe(locale_months),
      monthLookup = formatLookup(locale_months),
      shortMonthRe = formatRe(locale_shortMonths),
      shortMonthLookup = formatLookup(locale_shortMonths);

  var formats = {
    "a": formatShortWeekday,
    "A": formatWeekday,
    "b": formatShortMonth,
    "B": formatMonth,
    "c": null,
    "d": formatDayOfMonth,
    "e": formatDayOfMonth,
    "f": formatMicroseconds,
    "H": formatHour24,
    "I": formatHour12,
    "j": formatDayOfYear,
    "L": formatMilliseconds,
    "m": formatMonthNumber,
    "M": formatMinutes,
    "p": formatPeriod,
    "Q": formatUnixTimestamp,
    "s": formatUnixTimestampSeconds,
    "S": formatSeconds,
    "u": formatWeekdayNumberMonday,
    "U": formatWeekNumberSunday,
    "V": formatWeekNumberISO,
    "w": formatWeekdayNumberSunday,
    "W": formatWeekNumberMonday,
    "x": null,
    "X": null,
    "y": formatYear,
    "Y": formatFullYear,
    "Z": formatZone,
    "%": formatLiteralPercent
  };

  var utcFormats = {
    "a": formatUTCShortWeekday,
    "A": formatUTCWeekday,
    "b": formatUTCShortMonth,
    "B": formatUTCMonth,
    "c": null,
    "d": formatUTCDayOfMonth,
    "e": formatUTCDayOfMonth,
    "f": formatUTCMicroseconds,
    "H": formatUTCHour24,
    "I": formatUTCHour12,
    "j": formatUTCDayOfYear,
    "L": formatUTCMilliseconds,
    "m": formatUTCMonthNumber,
    "M": formatUTCMinutes,
    "p": formatUTCPeriod,
    "Q": formatUnixTimestamp,
    "s": formatUnixTimestampSeconds,
    "S": formatUTCSeconds,
    "u": formatUTCWeekdayNumberMonday,
    "U": formatUTCWeekNumberSunday,
    "V": formatUTCWeekNumberISO,
    "w": formatUTCWeekdayNumberSunday,
    "W": formatUTCWeekNumberMonday,
    "x": null,
    "X": null,
    "y": formatUTCYear,
    "Y": formatUTCFullYear,
    "Z": formatUTCZone,
    "%": formatLiteralPercent
  };

  var parses = {
    "a": parseShortWeekday,
    "A": parseWeekday,
    "b": parseShortMonth,
    "B": parseMonth,
    "c": parseLocaleDateTime,
    "d": parseDayOfMonth,
    "e": parseDayOfMonth,
    "f": parseMicroseconds,
    "H": parseHour24,
    "I": parseHour24,
    "j": parseDayOfYear,
    "L": parseMilliseconds,
    "m": parseMonthNumber,
    "M": parseMinutes,
    "p": parsePeriod,
    "Q": parseUnixTimestamp,
    "s": parseUnixTimestampSeconds,
    "S": parseSeconds,
    "u": parseWeekdayNumberMonday,
    "U": parseWeekNumberSunday,
    "V": parseWeekNumberISO,
    "w": parseWeekdayNumberSunday,
    "W": parseWeekNumberMonday,
    "x": parseLocaleDate,
    "X": parseLocaleTime,
    "y": parseYear,
    "Y": parseFullYear,
    "Z": parseZone,
    "%": parseLiteralPercent
  };

  // These recursive directive definitions must be deferred.
  formats.x = newFormat(locale_date, formats);
  formats.X = newFormat(locale_time, formats);
  formats.c = newFormat(locale_dateTime, formats);
  utcFormats.x = newFormat(locale_date, utcFormats);
  utcFormats.X = newFormat(locale_time, utcFormats);
  utcFormats.c = newFormat(locale_dateTime, utcFormats);

  function newFormat(specifier, formats) {
    return function(date) {
      var string = [],
          i = -1,
          j = 0,
          n = specifier.length,
          c,
          pad,
          format;

      if (!(date instanceof Date)) { date = new Date(+date); }

      while (++i < n) {
        if (specifier.charCodeAt(i) === 37) {
          string.push(specifier.slice(j, i));
          if ((pad = pads[c = specifier.charAt(++i)]) != null) { c = specifier.charAt(++i); }
          else { pad = c === "e" ? " " : "0"; }
          if (format = formats[c]) { c = format(date, pad); }
          string.push(c);
          j = i + 1;
        }
      }

      string.push(specifier.slice(j, i));
      return string.join("");
    };
  }

  function newParse(specifier, newDate) {
    return function(string) {
      var d = newYear(1900),
          i = parseSpecifier(d, specifier, string += "", 0),
          week, day$$1;
      if (i != string.length) { return null; }

      // If a UNIX timestamp is specified, return it.
      if ("Q" in d) { return new Date(d.Q); }

      // The am-pm flag is 0 for AM, and 1 for PM.
      if ("p" in d) { d.H = d.H % 12 + d.p * 12; }

      // Convert day-of-week and week-of-year to day-of-year.
      if ("V" in d) {
        if (d.V < 1 || d.V > 53) { return null; }
        if (!("w" in d)) { d.w = 1; }
        if ("Z" in d) {
          week = utcDate(newYear(d.y)), day$$1 = week.getUTCDay();
          week = day$$1 > 4 || day$$1 === 0 ? utcMonday.ceil(week) : utcMonday(week);
          week = utcDay.offset(week, (d.V - 1) * 7);
          d.y = week.getUTCFullYear();
          d.m = week.getUTCMonth();
          d.d = week.getUTCDate() + (d.w + 6) % 7;
        } else {
          week = newDate(newYear(d.y)), day$$1 = week.getDay();
          week = day$$1 > 4 || day$$1 === 0 ? monday.ceil(week) : monday(week);
          week = day.offset(week, (d.V - 1) * 7);
          d.y = week.getFullYear();
          d.m = week.getMonth();
          d.d = week.getDate() + (d.w + 6) % 7;
        }
      } else if ("W" in d || "U" in d) {
        if (!("w" in d)) { d.w = "u" in d ? d.u % 7 : "W" in d ? 1 : 0; }
        day$$1 = "Z" in d ? utcDate(newYear(d.y)).getUTCDay() : newDate(newYear(d.y)).getDay();
        d.m = 0;
        d.d = "W" in d ? (d.w + 6) % 7 + d.W * 7 - (day$$1 + 5) % 7 : d.w + d.U * 7 - (day$$1 + 6) % 7;
      }

      // If a time zone is specified, all fields are interpreted as UTC and then
      // offset according to the specified time zone.
      if ("Z" in d) {
        d.H += d.Z / 100 | 0;
        d.M += d.Z % 100;
        return utcDate(d);
      }

      // Otherwise, all fields are in local time.
      return newDate(d);
    };
  }

  function parseSpecifier(d, specifier, string, j) {
    var i = 0,
        n = specifier.length,
        m = string.length,
        c,
        parse;

    while (i < n) {
      if (j >= m) { return -1; }
      c = specifier.charCodeAt(i++);
      if (c === 37) {
        c = specifier.charAt(i++);
        parse = parses[c in pads ? specifier.charAt(i++) : c];
        if (!parse || ((j = parse(d, string, j)) < 0)) { return -1; }
      } else if (c != string.charCodeAt(j++)) {
        return -1;
      }
    }

    return j;
  }

  function parsePeriod(d, string, i) {
    var n = periodRe.exec(string.slice(i));
    return n ? (d.p = periodLookup[n[0].toLowerCase()], i + n[0].length) : -1;
  }

  function parseShortWeekday(d, string, i) {
    var n = shortWeekdayRe.exec(string.slice(i));
    return n ? (d.w = shortWeekdayLookup[n[0].toLowerCase()], i + n[0].length) : -1;
  }

  function parseWeekday(d, string, i) {
    var n = weekdayRe.exec(string.slice(i));
    return n ? (d.w = weekdayLookup[n[0].toLowerCase()], i + n[0].length) : -1;
  }

  function parseShortMonth(d, string, i) {
    var n = shortMonthRe.exec(string.slice(i));
    return n ? (d.m = shortMonthLookup[n[0].toLowerCase()], i + n[0].length) : -1;
  }

  function parseMonth(d, string, i) {
    var n = monthRe.exec(string.slice(i));
    return n ? (d.m = monthLookup[n[0].toLowerCase()], i + n[0].length) : -1;
  }

  function parseLocaleDateTime(d, string, i) {
    return parseSpecifier(d, locale_dateTime, string, i);
  }

  function parseLocaleDate(d, string, i) {
    return parseSpecifier(d, locale_date, string, i);
  }

  function parseLocaleTime(d, string, i) {
    return parseSpecifier(d, locale_time, string, i);
  }

  function formatShortWeekday(d) {
    return locale_shortWeekdays[d.getDay()];
  }

  function formatWeekday(d) {
    return locale_weekdays[d.getDay()];
  }

  function formatShortMonth(d) {
    return locale_shortMonths[d.getMonth()];
  }

  function formatMonth(d) {
    return locale_months[d.getMonth()];
  }

  function formatPeriod(d) {
    return locale_periods[+(d.getHours() >= 12)];
  }

  function formatUTCShortWeekday(d) {
    return locale_shortWeekdays[d.getUTCDay()];
  }

  function formatUTCWeekday(d) {
    return locale_weekdays[d.getUTCDay()];
  }

  function formatUTCShortMonth(d) {
    return locale_shortMonths[d.getUTCMonth()];
  }

  function formatUTCMonth(d) {
    return locale_months[d.getUTCMonth()];
  }

  function formatUTCPeriod(d) {
    return locale_periods[+(d.getUTCHours() >= 12)];
  }

  return {
    format: function(specifier) {
      var f = newFormat(specifier += "", formats);
      f.toString = function() { return specifier; };
      return f;
    },
    parse: function(specifier) {
      var p = newParse(specifier += "", localDate);
      p.toString = function() { return specifier; };
      return p;
    },
    utcFormat: function(specifier) {
      var f = newFormat(specifier += "", utcFormats);
      f.toString = function() { return specifier; };
      return f;
    },
    utcParse: function(specifier) {
      var p = newParse(specifier, utcDate);
      p.toString = function() { return specifier; };
      return p;
    }
  };
}

var pads = {"-": "", "_": " ", "0": "0"},
    numberRe = /^\s*\d+/, // note: ignores next directive
    percentRe = /^%/,
    requoteRe = /[\\^$*+?|[\]().{}]/g;

function pad(value, fill, width) {
  var sign = value < 0 ? "-" : "",
      string = (sign ? -value : value) + "",
      length = string.length;
  return sign + (length < width ? new Array(width - length + 1).join(fill) + string : string);
}

function requote(s) {
  return s.replace(requoteRe, "\\$&");
}

function formatRe(names) {
  return new RegExp("^(?:" + names.map(requote).join("|") + ")", "i");
}

function formatLookup(names) {
  var map = {}, i = -1, n = names.length;
  while (++i < n) { map[names[i].toLowerCase()] = i; }
  return map;
}

function parseWeekdayNumberSunday(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 1));
  return n ? (d.w = +n[0], i + n[0].length) : -1;
}

function parseWeekdayNumberMonday(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 1));
  return n ? (d.u = +n[0], i + n[0].length) : -1;
}

function parseWeekNumberSunday(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.U = +n[0], i + n[0].length) : -1;
}

function parseWeekNumberISO(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.V = +n[0], i + n[0].length) : -1;
}

function parseWeekNumberMonday(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.W = +n[0], i + n[0].length) : -1;
}

function parseFullYear(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 4));
  return n ? (d.y = +n[0], i + n[0].length) : -1;
}

function parseYear(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.y = +n[0] + (+n[0] > 68 ? 1900 : 2000), i + n[0].length) : -1;
}

function parseZone(d, string, i) {
  var n = /^(Z)|([+-]\d\d)(?::?(\d\d))?/.exec(string.slice(i, i + 6));
  return n ? (d.Z = n[1] ? 0 : -(n[2] + (n[3] || "00")), i + n[0].length) : -1;
}

function parseMonthNumber(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.m = n[0] - 1, i + n[0].length) : -1;
}

function parseDayOfMonth(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.d = +n[0], i + n[0].length) : -1;
}

function parseDayOfYear(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 3));
  return n ? (d.m = 0, d.d = +n[0], i + n[0].length) : -1;
}

function parseHour24(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.H = +n[0], i + n[0].length) : -1;
}

function parseMinutes(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.M = +n[0], i + n[0].length) : -1;
}

function parseSeconds(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 2));
  return n ? (d.S = +n[0], i + n[0].length) : -1;
}

function parseMilliseconds(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 3));
  return n ? (d.L = +n[0], i + n[0].length) : -1;
}

function parseMicroseconds(d, string, i) {
  var n = numberRe.exec(string.slice(i, i + 6));
  return n ? (d.L = Math.floor(n[0] / 1000), i + n[0].length) : -1;
}

function parseLiteralPercent(d, string, i) {
  var n = percentRe.exec(string.slice(i, i + 1));
  return n ? i + n[0].length : -1;
}

function parseUnixTimestamp(d, string, i) {
  var n = numberRe.exec(string.slice(i));
  return n ? (d.Q = +n[0], i + n[0].length) : -1;
}

function parseUnixTimestampSeconds(d, string, i) {
  var n = numberRe.exec(string.slice(i));
  return n ? (d.Q = (+n[0]) * 1000, i + n[0].length) : -1;
}

function formatDayOfMonth(d, p) {
  return pad(d.getDate(), p, 2);
}

function formatHour24(d, p) {
  return pad(d.getHours(), p, 2);
}

function formatHour12(d, p) {
  return pad(d.getHours() % 12 || 12, p, 2);
}

function formatDayOfYear(d, p) {
  return pad(1 + day.count(year(d), d), p, 3);
}

function formatMilliseconds(d, p) {
  return pad(d.getMilliseconds(), p, 3);
}

function formatMicroseconds(d, p) {
  return formatMilliseconds(d, p) + "000";
}

function formatMonthNumber(d, p) {
  return pad(d.getMonth() + 1, p, 2);
}

function formatMinutes(d, p) {
  return pad(d.getMinutes(), p, 2);
}

function formatSeconds(d, p) {
  return pad(d.getSeconds(), p, 2);
}

function formatWeekdayNumberMonday(d) {
  var day$$1 = d.getDay();
  return day$$1 === 0 ? 7 : day$$1;
}

function formatWeekNumberSunday(d, p) {
  return pad(sunday.count(year(d), d), p, 2);
}

function formatWeekNumberISO(d, p) {
  var day$$1 = d.getDay();
  d = (day$$1 >= 4 || day$$1 === 0) ? thursday(d) : thursday.ceil(d);
  return pad(thursday.count(year(d), d) + (year(d).getDay() === 4), p, 2);
}

function formatWeekdayNumberSunday(d) {
  return d.getDay();
}

function formatWeekNumberMonday(d, p) {
  return pad(monday.count(year(d), d), p, 2);
}

function formatYear(d, p) {
  return pad(d.getFullYear() % 100, p, 2);
}

function formatFullYear(d, p) {
  return pad(d.getFullYear() % 10000, p, 4);
}

function formatZone(d) {
  var z = d.getTimezoneOffset();
  return (z > 0 ? "-" : (z *= -1, "+"))
      + pad(z / 60 | 0, "0", 2)
      + pad(z % 60, "0", 2);
}

function formatUTCDayOfMonth(d, p) {
  return pad(d.getUTCDate(), p, 2);
}

function formatUTCHour24(d, p) {
  return pad(d.getUTCHours(), p, 2);
}

function formatUTCHour12(d, p) {
  return pad(d.getUTCHours() % 12 || 12, p, 2);
}

function formatUTCDayOfYear(d, p) {
  return pad(1 + utcDay.count(utcYear(d), d), p, 3);
}

function formatUTCMilliseconds(d, p) {
  return pad(d.getUTCMilliseconds(), p, 3);
}

function formatUTCMicroseconds(d, p) {
  return formatUTCMilliseconds(d, p) + "000";
}

function formatUTCMonthNumber(d, p) {
  return pad(d.getUTCMonth() + 1, p, 2);
}

function formatUTCMinutes(d, p) {
  return pad(d.getUTCMinutes(), p, 2);
}

function formatUTCSeconds(d, p) {
  return pad(d.getUTCSeconds(), p, 2);
}

function formatUTCWeekdayNumberMonday(d) {
  var dow = d.getUTCDay();
  return dow === 0 ? 7 : dow;
}

function formatUTCWeekNumberSunday(d, p) {
  return pad(utcSunday.count(utcYear(d), d), p, 2);
}

function formatUTCWeekNumberISO(d, p) {
  var day$$1 = d.getUTCDay();
  d = (day$$1 >= 4 || day$$1 === 0) ? utcThursday(d) : utcThursday.ceil(d);
  return pad(utcThursday.count(utcYear(d), d) + (utcYear(d).getUTCDay() === 4), p, 2);
}

function formatUTCWeekdayNumberSunday(d) {
  return d.getUTCDay();
}

function formatUTCWeekNumberMonday(d, p) {
  return pad(utcMonday.count(utcYear(d), d), p, 2);
}

function formatUTCYear(d, p) {
  return pad(d.getUTCFullYear() % 100, p, 2);
}

function formatUTCFullYear(d, p) {
  return pad(d.getUTCFullYear() % 10000, p, 4);
}

function formatUTCZone() {
  return "+0000";
}

function formatLiteralPercent() {
  return "%";
}

function formatUnixTimestamp(d) {
  return +d;
}

function formatUnixTimestampSeconds(d) {
  return Math.floor(+d / 1000);
}

var locale$1;
var timeFormat;
var timeParse;
var utcFormat;
var utcParse;

defaultLocale$1({
  dateTime: "%x, %X",
  date: "%-m/%-d/%Y",
  time: "%-I:%M:%S %p",
  periods: ["AM", "PM"],
  days: ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"],
  shortDays: ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"],
  months: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"],
  shortMonths: ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
});

function defaultLocale$1(definition) {
  locale$1 = formatLocale$1(definition);
  timeFormat = locale$1.format;
  timeParse = locale$1.parse;
  utcFormat = locale$1.utcFormat;
  utcParse = locale$1.utcParse;
  return locale$1;
}

var isoSpecifier = "%Y-%m-%dT%H:%M:%S.%LZ";

function formatIsoNative(date) {
  return date.toISOString();
}

var formatIso = Date.prototype.toISOString
    ? formatIsoNative
    : utcFormat(isoSpecifier);

function parseIsoNative(string) {
  var date = new Date(string);
  return isNaN(date) ? null : date;
}

var parseIso = +new Date("2000-01-01T00:00:00.000Z")
    ? parseIsoNative
    : utcParse(isoSpecifier);

function colors(s) {
  return s.match(/.{6}/g).map(function(x) {
    return "#" + x;
  });
}

colors("1f77b4ff7f0e2ca02cd627289467bd8c564be377c27f7f7fbcbd2217becf");

colors("393b795254a36b6ecf9c9ede6379398ca252b5cf6bcedb9c8c6d31bd9e39e7ba52e7cb94843c39ad494ad6616be7969c7b4173a55194ce6dbdde9ed6");

colors("3182bd6baed69ecae1c6dbefe6550dfd8d3cfdae6bfdd0a231a35474c476a1d99bc7e9c0756bb19e9ac8bcbddcdadaeb636363969696bdbdbdd9d9d9");

colors("1f77b4aec7e8ff7f0effbb782ca02c98df8ad62728ff98969467bdc5b0d58c564bc49c94e377c2f7b6d27f7f7fc7c7c7bcbd22dbdb8d17becf9edae5");

cubehelixLong(cubehelix(300, 0.5, 0.0), cubehelix(-240, 0.5, 1.0));

var warm = cubehelixLong(cubehelix(-100, 0.75, 0.35), cubehelix(80, 1.50, 0.8));

var cool = cubehelixLong(cubehelix(260, 0.75, 0.35), cubehelix(80, 1.50, 0.8));

var rainbow = cubehelix();

function ramp(range) {
  var n = range.length;
  return function(t) {
    return range[Math.max(0, Math.min(n - 1, Math.floor(t * n)))];
  };
}

ramp(colors("44015444025645045745055946075a46085c460a5d460b5e470d60470e6147106347116447136548146748166848176948186a481a6c481b6d481c6e481d6f481f70482071482173482374482475482576482677482878482979472a7a472c7a472d7b472e7c472f7d46307e46327e46337f463480453581453781453882443983443a83443b84433d84433e85423f854240864241864142874144874045884046883f47883f48893e49893e4a893e4c8a3d4d8a3d4e8a3c4f8a3c508b3b518b3b528b3a538b3a548c39558c39568c38588c38598c375a8c375b8d365c8d365d8d355e8d355f8d34608d34618d33628d33638d32648e32658e31668e31678e31688e30698e306a8e2f6b8e2f6c8e2e6d8e2e6e8e2e6f8e2d708e2d718e2c718e2c728e2c738e2b748e2b758e2a768e2a778e2a788e29798e297a8e297b8e287c8e287d8e277e8e277f8e27808e26818e26828e26828e25838e25848e25858e24868e24878e23888e23898e238a8d228b8d228c8d228d8d218e8d218f8d21908d21918c20928c20928c20938c1f948c1f958b1f968b1f978b1f988b1f998a1f9a8a1e9b8a1e9c891e9d891f9e891f9f881fa0881fa1881fa1871fa28720a38620a48621a58521a68522a78522a88423a98324aa8325ab8225ac8226ad8127ad8128ae8029af7f2ab07f2cb17e2db27d2eb37c2fb47c31b57b32b67a34b67935b77937b87838b9773aba763bbb753dbc743fbc7340bd7242be7144bf7046c06f48c16e4ac16d4cc26c4ec36b50c46a52c56954c56856c66758c7655ac8645cc8635ec96260ca6063cb5f65cb5e67cc5c69cd5b6ccd5a6ece5870cf5773d05675d05477d1537ad1517cd2507fd34e81d34d84d44b86d54989d5488bd6468ed64590d74393d74195d84098d83e9bd93c9dd93ba0da39a2da37a5db36a8db34aadc32addc30b0dd2fb2dd2db5de2bb8de29bade28bddf26c0df25c2df23c5e021c8e020cae11fcde11dd0e11cd2e21bd5e21ad8e219dae319dde318dfe318e2e418e5e419e7e419eae51aece51befe51cf1e51df4e61ef6e620f8e621fbe723fde725"));

var magma = ramp(colors("00000401000501010601010802010902020b02020d03030f03031204041405041606051806051a07061c08071e0907200a08220b09240c09260d0a290e0b2b100b2d110c2f120d31130d34140e36150e38160f3b180f3d19103f1a10421c10441d11471e114920114b21114e22115024125325125527125829115a2a115c2c115f2d11612f116331116533106734106936106b38106c390f6e3b0f703d0f713f0f72400f74420f75440f764510774710784910784a10794c117a4e117b4f127b51127c52137c54137d56147d57157e59157e5a167e5c167f5d177f5f187f601880621980641a80651a80671b80681c816a1c816b1d816d1d816e1e81701f81721f817320817521817621817822817922827b23827c23827e24828025828125818326818426818627818827818928818b29818c29818e2a81902a81912b81932b80942c80962c80982d80992d809b2e7f9c2e7f9e2f7fa02f7fa1307ea3307ea5317ea6317da8327daa337dab337cad347cae347bb0357bb2357bb3367ab5367ab73779b83779ba3878bc3978bd3977bf3a77c03a76c23b75c43c75c53c74c73d73c83e73ca3e72cc3f71cd4071cf4070d0416fd2426fd3436ed5446dd6456cd8456cd9466bdb476adc4869de4968df4a68e04c67e24d66e34e65e44f64e55064e75263e85362e95462ea5661eb5760ec5860ed5a5fee5b5eef5d5ef05f5ef1605df2625df2645cf3655cf4675cf4695cf56b5cf66c5cf66e5cf7705cf7725cf8745cf8765cf9785df9795df97b5dfa7d5efa7f5efa815ffb835ffb8560fb8761fc8961fc8a62fc8c63fc8e64fc9065fd9266fd9467fd9668fd9869fd9a6afd9b6bfe9d6cfe9f6dfea16efea36ffea571fea772fea973feaa74feac76feae77feb078feb27afeb47bfeb67cfeb77efeb97ffebb81febd82febf84fec185fec287fec488fec68afec88cfeca8dfecc8ffecd90fecf92fed194fed395fed597fed799fed89afdda9cfddc9efddea0fde0a1fde2a3fde3a5fde5a7fde7a9fde9aafdebacfcecaefceeb0fcf0b2fcf2b4fcf4b6fcf6b8fcf7b9fcf9bbfcfbbdfcfdbf"));

var inferno = ramp(colors("00000401000501010601010802010a02020c02020e03021004031204031405041706041907051b08051d09061f0a07220b07240c08260d08290e092b10092d110a30120a32140b34150b37160b39180c3c190c3e1b0c411c0c431e0c451f0c48210c4a230c4c240c4f260c51280b53290b552b0b572d0b592f0a5b310a5c320a5e340a5f3609613809623909633b09643d09653e0966400a67420a68440a68450a69470b6a490b6a4a0c6b4c0c6b4d0d6c4f0d6c510e6c520e6d540f6d550f6d57106e59106e5a116e5c126e5d126e5f136e61136e62146e64156e65156e67166e69166e6a176e6c186e6d186e6f196e71196e721a6e741a6e751b6e771c6d781c6d7a1d6d7c1d6d7d1e6d7f1e6c801f6c82206c84206b85216b87216b88226a8a226a8c23698d23698f24699025689225689326679526679727669827669a28659b29649d29649f2a63a02a63a22b62a32c61a52c60a62d60a82e5fa92e5eab2f5ead305dae305cb0315bb1325ab3325ab43359b63458b73557b93556ba3655bc3754bd3853bf3952c03a51c13a50c33b4fc43c4ec63d4dc73e4cc83f4bca404acb4149cc4248ce4347cf4446d04545d24644d34743d44842d54a41d74b3fd84c3ed94d3dda4e3cdb503bdd513ade5238df5337e05536e15635e25734e35933e45a31e55c30e65d2fe75e2ee8602de9612bea632aeb6429eb6628ec6726ed6925ee6a24ef6c23ef6e21f06f20f1711ff1731df2741cf3761bf37819f47918f57b17f57d15f67e14f68013f78212f78410f8850ff8870ef8890cf98b0bf98c0af98e09fa9008fa9207fa9407fb9606fb9706fb9906fb9b06fb9d07fc9f07fca108fca309fca50afca60cfca80dfcaa0ffcac11fcae12fcb014fcb216fcb418fbb61afbb81dfbba1ffbbc21fbbe23fac026fac228fac42afac62df9c72ff9c932f9cb35f8cd37f8cf3af7d13df7d340f6d543f6d746f5d949f5db4cf4dd4ff4df53f4e156f3e35af3e55df2e661f2e865f2ea69f1ec6df1ed71f1ef75f1f179f2f27df2f482f3f586f3f68af4f88ef5f992f6fa96f8fb9af9fc9dfafda1fcffa4"));

var plasma = ramp(colors("0d088710078813078916078a19068c1b068d1d068e20068f2206902406912605912805922a05932c05942e05952f059631059733059735049837049938049a3a049a3c049b3e049c3f049c41049d43039e44039e46039f48039f4903a04b03a14c02a14e02a25002a25102a35302a35502a45601a45801a45901a55b01a55c01a65e01a66001a66100a76300a76400a76600a76700a86900a86a00a86c00a86e00a86f00a87100a87201a87401a87501a87701a87801a87a02a87b02a87d03a87e03a88004a88104a78305a78405a78606a68707a68808a68a09a58b0aa58d0ba58e0ca48f0da4910ea3920fa39410a29511a19613a19814a099159f9a169f9c179e9d189d9e199da01a9ca11b9ba21d9aa31e9aa51f99a62098a72197a82296aa2395ab2494ac2694ad2793ae2892b02991b12a90b22b8fb32c8eb42e8db52f8cb6308bb7318ab83289ba3388bb3488bc3587bd3786be3885bf3984c03a83c13b82c23c81c33d80c43e7fc5407ec6417dc7427cc8437bc9447aca457acb4679cc4778cc4977cd4a76ce4b75cf4c74d04d73d14e72d24f71d35171d45270d5536fd5546ed6556dd7566cd8576bd9586ada5a6ada5b69db5c68dc5d67dd5e66de5f65de6164df6263e06363e16462e26561e26660e3685fe4695ee56a5de56b5de66c5ce76e5be76f5ae87059e97158e97257ea7457eb7556eb7655ec7754ed7953ed7a52ee7b51ef7c51ef7e50f07f4ff0804ef1814df1834cf2844bf3854bf3874af48849f48948f58b47f58c46f68d45f68f44f79044f79143f79342f89441f89540f9973ff9983ef99a3efa9b3dfa9c3cfa9e3bfb9f3afba139fba238fca338fca537fca636fca835fca934fdab33fdac33fdae32fdaf31fdb130fdb22ffdb42ffdb52efeb72dfeb82cfeba2cfebb2bfebd2afebe2afec029fdc229fdc328fdc527fdc627fdc827fdca26fdcb26fccd25fcce25fcd025fcd225fbd324fbd524fbd724fad824fada24f9dc24f9dd25f8df25f8e125f7e225f7e425f6e626f6e826f5e926f5eb27f4ed27f3ee27f3f027f2f227f1f426f1f525f0f724f0f921"));

function constant$10(x) {
  return function constant() {
    return x;
  };
}

var pi$4 = Math.PI;

function Linear(context) {
  this._context = context;
}

Linear.prototype = {
  areaStart: function() {
    this._line = 0;
  },
  areaEnd: function() {
    this._line = NaN;
  },
  lineStart: function() {
    this._point = 0;
  },
  lineEnd: function() {
    if (this._line || (this._line !== 0 && this._point === 1)) { this._context.closePath(); }
    this._line = 1 - this._line;
  },
  point: function(x, y) {
    x = +x, y = +y;
    switch (this._point) {
      case 0: this._point = 1; this._line ? this._context.lineTo(x, y) : this._context.moveTo(x, y); break;
      case 1: this._point = 2; // proceed
      default: this._context.lineTo(x, y); break;
    }
  }
};

function normal$1(context) {
  return new Linear(context);
}

function x$3(p) {
  return p[0];
}

function y$3(p) {
  return p[1];
}

function LinePathGenerator() {
  var x$$1 = x$3,
      y$$1 = y$3,
      defined = constant$10(true),
      context = null,
      curve = normal$1,
      output = null;

  function line(data) {
    var i,
        n = data.length,
        d,
        defined0 = false,
        buffer;

    if (context == null) { output = curve(buffer = path()); }

    for (i = 0; i <= n; ++i) {
      if (!(i < n && defined(d = data[i], i, data)) === defined0) {
        if (defined0 = !defined0) { output.lineStart(); }
        else { output.lineEnd(); }
      }
      if (defined0) { output.point(+x$$1(d, i, data), +y$$1(d, i, data)); }
    }

    if (buffer) { return output = null, buffer + "" || null; }
  }

  line.x = function(_) {
    return arguments.length ? (x$$1 = typeof _ === "function" ? _ : constant$10(+_), line) : x$$1;
  };

  line.y = function(_) {
    return arguments.length ? (y$$1 = typeof _ === "function" ? _ : constant$10(+_), line) : y$$1;
  };

  line.defined = function(_) {
    return arguments.length ? (defined = typeof _ === "function" ? _ : constant$10(!!_), line) : defined;
  };

  line.curve = function(_) {
    return arguments.length ? (curve = _, context != null && (output = curve(context)), line) : curve;
  };

  line.context = function(_) {
    return arguments.length ? (_ == null ? context = output = null : output = curve(context = _), line) : context;
  };

  return line;
}

function areaPathGenerator() {
  var x0 = x$3,
      x1 = null,
      y0 = constant$10(0),
      y1 = y$3,
      defined = constant$10(true),
      context = null,
      curve = normal$1,
      output = null;

  function area(data) {
    var i,
        j,
        k,
        n = data.length,
        d,
        defined0 = false,
        buffer,
        x0z = new Array(n),
        y0z = new Array(n);

    if (context == null) { output = curve(buffer = path()); }

    for (i = 0; i <= n; ++i) {
      if (!(i < n && defined(d = data[i], i, data)) === defined0) {
        if (defined0 = !defined0) {
          j = i;
          output.areaStart();
          output.lineStart();
        } else {
          output.lineEnd();
          output.lineStart();
          for (k = i - 1; k >= j; --k) {
            output.point(x0z[k], y0z[k]);
          }
          output.lineEnd();
          output.areaEnd();
        }
      }
      if (defined0) {
        x0z[i] = +x0(d, i, data), y0z[i] = +y0(d, i, data);
        output.point(x1 ? +x1(d, i, data) : x0z[i], y1 ? +y1(d, i, data) : y0z[i]);
      }
    }

    if (buffer) { return output = null, buffer + "" || null; }
  }

  function arealine() {
    return LinePathGenerator().defined(defined).curve(curve).context(context);
  }

  area.x = function(_) {
    return arguments.length ? (x0 = typeof _ === "function" ? _ : constant$10(+_), x1 = null, area) : x0;
  };

  area.x0 = function(_) {
    return arguments.length ? (x0 = typeof _ === "function" ? _ : constant$10(+_), area) : x0;
  };

  area.x1 = function(_) {
    return arguments.length ? (x1 = _ == null ? null : typeof _ === "function" ? _ : constant$10(+_), area) : x1;
  };

  area.y = function(_) {
    return arguments.length ? (y0 = typeof _ === "function" ? _ : constant$10(+_), y1 = null, area) : y0;
  };

  area.y0 = function(_) {
    return arguments.length ? (y0 = typeof _ === "function" ? _ : constant$10(+_), area) : y0;
  };

  area.y1 = function(_) {
    return arguments.length ? (y1 = _ == null ? null : typeof _ === "function" ? _ : constant$10(+_), area) : y1;
  };

  area.lineX0 =
  area.lineY0 = function() {
    return arealine().x(x0).y(y0);
  };

  area.lineY1 = function() {
    return arealine().x(x0).y(y1);
  };

  area.lineX1 = function() {
    return arealine().x(x1).y(y0);
  };

  area.defined = function(_) {
    return arguments.length ? (defined = typeof _ === "function" ? _ : constant$10(!!_), area) : defined;
  };

  area.curve = function(_) {
    return arguments.length ? (curve = _, context != null && (output = curve(context)), area) : curve;
  };

  area.context = function(_) {
    return arguments.length ? (_ == null ? context = output = null : output = curve(context = _), area) : context;
  };

  return area;
}

function point$3(that, x, y) {
  that._context.bezierCurveTo(
    that._x1 + that._k * (that._x2 - that._x0),
    that._y1 + that._k * (that._y2 - that._y0),
    that._x2 + that._k * (that._x1 - x),
    that._y2 + that._k * (that._y1 - y),
    that._x2,
    that._y2
  );
}

function Cardinal(context, tension) {
  this._context = context;
  this._k = (1 - tension) / 6;
}

Cardinal.prototype = {
  areaStart: function() {
    this._line = 0;
  },
  areaEnd: function() {
    this._line = NaN;
  },
  lineStart: function() {
    this._x0 = this._x1 = this._x2 =
    this._y0 = this._y1 = this._y2 = NaN;
    this._point = 0;
  },
  lineEnd: function() {
    switch (this._point) {
      case 2: this._context.lineTo(this._x2, this._y2); break;
      case 3: point$3(this, this._x1, this._y1); break;
    }
    if (this._line || (this._line !== 0 && this._point === 1)) { this._context.closePath(); }
    this._line = 1 - this._line;
  },
  point: function(x, y) {
    x = +x, y = +y;
    switch (this._point) {
      case 0: this._point = 1; this._line ? this._context.lineTo(x, y) : this._context.moveTo(x, y); break;
      case 1: this._point = 2; this._x1 = x, this._y1 = y; break;
      case 2: this._point = 3; // proceed
      default: point$3(this, x, y); break;
    }
    this._x0 = this._x1, this._x1 = this._x2, this._x2 = x;
    this._y0 = this._y1, this._y1 = this._y2, this._y2 = y;
  }
};

var curve = (function custom(tension) {

  function cardinal(context) {
    return new Cardinal(context, tension);
  }

  cardinal.tension = function(tension) {
    return custom(+tension);
  };

  return cardinal;
})(0);

function sign$1(x) {
  return x < 0 ? -1 : 1;
}

// Calculate the slopes of the tangents (Hermite-type interpolation) based on
// the following paper: Steffen, M. 1990. A Simple Method for Monotonic
// Interpolation in One Dimension. Astronomy and Astrophysics, Vol. 239, NO.
// NOV(II), P. 443, 1990.
function slope3(that, x2, y2) {
  var h0 = that._x1 - that._x0,
      h1 = x2 - that._x1,
      s0 = (that._y1 - that._y0) / (h0 || h1 < 0 && -0),
      s1 = (y2 - that._y1) / (h1 || h0 < 0 && -0),
      p = (s0 * h1 + s1 * h0) / (h0 + h1);
  return (sign$1(s0) + sign$1(s1)) * Math.min(Math.abs(s0), Math.abs(s1), 0.5 * Math.abs(p)) || 0;
}

// Calculate a one-sided slope.
function slope2(that, t) {
  var h = that._x1 - that._x0;
  return h ? (3 * (that._y1 - that._y0) / h - t) / 2 : t;
}

// According to https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Representations
// "you can express cubic Hermite interpolation in terms of cubic Bézier curves
// with respect to the four values p0, p0 + m0 / 3, p1 - m1 / 3, p1".
function point$5(that, t0, t1) {
  var x0 = that._x0,
      y0 = that._y0,
      x1 = that._x1,
      y1 = that._y1,
      dx = (x1 - x0) / 3;
  that._context.bezierCurveTo(x0 + dx, y0 + dx * t0, x1 - dx, y1 - dx * t1, x1, y1);
}

function MonotoneX(context) {
  this._context = context;
}

MonotoneX.prototype = {
  areaStart: function() {
    this._line = 0;
  },
  areaEnd: function() {
    this._line = NaN;
  },
  lineStart: function() {
    this._x0 = this._x1 =
    this._y0 = this._y1 =
    this._t0 = NaN;
    this._point = 0;
  },
  lineEnd: function() {
    switch (this._point) {
      case 2: this._context.lineTo(this._x1, this._y1); break;
      case 3: point$5(this, this._t0, slope2(this, this._t0)); break;
    }
    if (this._line || (this._line !== 0 && this._point === 1)) { this._context.closePath(); }
    this._line = 1 - this._line;
  },
  point: function(x, y) {
    var t1 = NaN;

    x = +x, y = +y;
    if (x === this._x1 && y === this._y1) { return; } // Ignore coincident points.
    switch (this._point) {
      case 0: this._point = 1; this._line ? this._context.lineTo(x, y) : this._context.moveTo(x, y); break;
      case 1: this._point = 2; break;
      case 2: this._point = 3; point$5(this, slope2(this, t1 = slope3(this, x, y)), t1); break;
      default: point$5(this, this._t0, t1 = slope3(this, x, y)); break;
    }

    this._x0 = this._x1, this._x1 = x;
    this._y0 = this._y1, this._y1 = y;
    this._t0 = t1;
  }
};

function MonotoneY(context) {
  this._context = new ReflectContext(context);
}

(MonotoneY.prototype = Object.create(MonotoneX.prototype)).point = function(x, y) {
  MonotoneX.prototype.point.call(this, y, x);
};

function ReflectContext(context) {
  this._context = context;
}

ReflectContext.prototype = {
  moveTo: function(x, y) { this._context.moveTo(y, x); },
  closePath: function() { this._context.closePath(); },
  lineTo: function(x, y) { this._context.lineTo(y, x); },
  bezierCurveTo: function(x1, y1, x2, y2, x, y) { this._context.bezierCurveTo(y1, x1, y2, x2, y, x); }
};

function Step(context, t) {
  this._context = context;
  this._t = t;
}

Step.prototype = {
  areaStart: function() {
    this._line = 0;
  },
  areaEnd: function() {
    this._line = NaN;
  },
  lineStart: function() {
    this._x = this._y = NaN;
    this._point = 0;
  },
  lineEnd: function() {
    if (0 < this._t && this._t < 1 && this._point === 2) { this._context.lineTo(this._x, this._y); }
    if (this._line || (this._line !== 0 && this._point === 1)) { this._context.closePath(); }
    if (this._line >= 0) { this._t = 1 - this._t, this._line = 1 - this._line; }
  },
  point: function(x, y) {
    x = +x, y = +y;
    switch (this._point) {
      case 0: this._point = 1; this._line ? this._context.lineTo(x, y) : this._context.moveTo(x, y); break;
      case 1: this._point = 2; // proceed
      default: {
        if (this._t <= 0) {
          this._context.lineTo(this._x, y);
          this._context.lineTo(x, y);
        } else {
          var x1 = this._x * (1 - this._t) + x * this._t;
          this._context.lineTo(x1, this._y);
          this._context.lineTo(x1, y);
        }
        break;
      }
    }
    this._x = x, this._y = y;
  }
};

function stepAfter(context) {
  return new Step(context, 1);
}

/**
 * Created by eason on 2017/8/15.
 */
var PI$2 = Math.PI;

var minMax = function (array){
    var min = Number.MAX_VALUE, max = -Number.MAX_VALUE;

    for(var i = 0, len = array.length; i < len; i++){

        min = Math.min(min, array[i]);

        max = Math.max(max, array[i]);

    }

    return [min, max]

};

var toNum = function (num){
    return isNaN(num) ? 0 : +num
};

/**
 * revised from _.baseClamp
 *
 * @param {number} number The number to clamp.
 * @param {number} [lower] The lower bound.
 * @param {number} upper The upper bound.
 * @returns {number} Returns the clamped number.
 */
var clamp = function (number, lower, upper){
    if (number === number) {
        if (upper !== undefined) {
            number = number <= upper ? number : upper;
        }
        if (lower !== undefined) {
            number = number >= lower ? number : lower;
        }
    }
    return toNum(number);
};

/**
 * 考虑到用h("path", props)不方便更新且性能不佳，
 * 这边把折线图的绘制抽象成一个shape，考虑到线的粗细不一致，像水中漂浮的海带一样，故命名为海带线
 */

var xFunc = function (d) { return d.x; };
var yFunc = function (d) { return d.y; };

var LineGenerator = LinePathGenerator().x(xFunc).y(yFunc);

var STYLE_NORMAL = "normal", STYLE_STEP   = "step", STYLE_CURVE  = "curve";

var curveFunc = function (style) {
    switch (style.toLowerCase()) {
        case STYLE_STEP:
            return stepAfter;

        case STYLE_CURVE:
            return curve;

        default:
            return normal$1;
    }
};

var KelpLine = Path.extend({

    type: 'kelpLine',

    shape: {

        points: null,

        isHorizontal: true,

        style: "normal"  // normal || step || curve
    },

    style: {
        stroke: 'none',

        fill: null
    },

    buildPath: function (ctx, ref) {
        var points = ref.points;
        var style = ref.style; if ( style === void 0 ) style = STYLE_NORMAL;
        var isHorizontal = ref.isHorizontal; if ( isHorizontal === void 0 ) isHorizontal = true;
        var stroke = ref.stroke; if ( stroke === void 0 ) stroke = true;


        var painter = LineGenerator.context(ctx).curve(curveFunc(style));

        // points share the same size.
        if(stroke) {
            painter(points);
            return;
        }

        // step kelp
        if(style === "step") {
            painter(getStepKeyPoint(points, isHorizontal));
            return;
        }

        var ref$1 = getNormalAndCurveKeyPoints(points, style === "normal");
        var top = ref$1[0];
        var bottom = ref$1[1];

        // curve
        if(style === "curve" || style === "normal"){
            var ref$2 = getTopAndBottomPoints(points);
            var top$1 = ref$2[0];
            var bottom$1 = ref$2[1];
            var ref$3 = top$1[0];
            var x1 = ref$3.x;
            var y1 = ref$3.y;
            var ref$4 = bottom$1[0];
            var x2 = ref$4.x;
            var y2 = ref$4.y;

            painter(top$1);
            ctx.lineTo(x2, y2);
            painter(bottom$1);
            ctx.lineTo(x1, y1);
        }
        // normal
        else {
            var ref$5 = top[0][0];
            var x1$1 = ref$5.x;
            var y1$1 = ref$5.y;
            var ref$6 = bottom[0][0];
            var x2$1 = ref$6.x;
            var y2$1 = ref$6.y;

            var lineOrArc = function (points, reverse$$1){
                for(var i = 1; i < points.length; i++){
                    var ps = points[i], single = ps.length === 1;

                    if(single) {
                        var ref = ps[0];
                        var x = ref.x;
                        var y = ref.y;
                        ctx.lineTo(x ,y);
                    } else {
                        var p1 = ps[0];
                        var p2 = ps[1];
                        var p3 = ps[2];
                        var radius = ps[3];
                        var x1 = p1.x;
                        var y1 = p1.y;
                        var x2 = p2.x;
                        var y2 = p2.y;
                        var x3 = p3.x;
                        var y3 = p3.y;

                        if(reverse$$1) {
                            ctx.lineTo(x3, y3);
                            ctx.arcTo(x2, y2, x1, y1, radius);
                        } else {
                            ctx.lineTo(x1, y1);
                            ctx.arcTo(x2, y2, x3, y3, radius);
                        }
                    }

                }
            };

            ctx.moveTo(x1$1, y1$1);

            lineOrArc(top);

            ctx.lineTo(x2$1, y2$1);

            lineOrArc(bottom, true);

            ctx.lineTo(x1$1, y1$1);


        }
    }
});


function getTopAndBottomPoints(points) {

    var top = [], bottom = [];

    map(points, function (ref) {
        var x = ref.x;
        var y = ref.y;
        var size = ref.size;

        var r = size / 2;
        top.push({x: x, y: y - r});
        bottom.unshift({x: x, y: y + r});
    });

    return [top, bottom];
}

/**
 * step线关键点计算
 * @param points
 * @param isHorizontal
 */
function getStepKeyPoint(points, isHorizontal){

    var newPoints = [];

    forEach(points, function (p, i) {
        var next = points[i+1], pre = points[i-1];
        var up = next ? p.y > next.y : false,
            down = pre ? p.y > pre.y : false;

        newPoints.push({
            x: p.x,
            y: p.y,
            size: p.size,
            down: down
        });

        if(i < points.length - 1) {
            newPoints.push({
                x: next.x,
                y: p.y,
                size: (p.size + next.size) / 2,
                up: up
            });
        }
    });

    var len = newPoints.length;

    var p1 = map(newPoints, function (p, i) {

        var isOdd = i % 2;
        var right = p.x + p.size / 2,
            left = p.x - p.size / 2;

        var x = isOdd ? (p.up ? left : right) : (p.down ? right : left);

        if(i === 0) {
            x = p.x;
        } else if( i === len - 1) {
            x = p.down ? right : left;
        }

        return {
            x: x,
            y: i === len - 1 ? p.y : (p.y - p.size / 2)
        }
    });

    var p2 = map(newPoints.reverse(), function (p, i) {
        var isOdd = i % 2;
        var right = p.x + p.size / 2,
            left = p.x - p.size / 2;

        var x = isOdd ? (p.up ? right : left) : (p.down ? left : right);

        if(i === len - 1){
            x = p.x;
        } else if(i === 0) {
            x = p.down ? left : right;
        }

        return {
            x: x,
            y: i === 0 ? p.y : (p.y + p.size / 2)
        }
    });

    return p1.concat(p2);
}


/**
 * normal&curve样式的线关键点计算
 * @param points
 * @param isNormal
 * @returns {*[]}
 */
function getNormalAndCurveKeyPoints(points, isNormal) {

    var topPoints = [], bottomPoints = [];

    var len = points.length;

    for(var i = 0; i < len; i++) {
        var preNode = points[i - 1], node = points[i], nextNode = points[i+1];
        var ref = calculateKeyPoint(preNode, node, nextNode, isNormal);
        var top = ref[0];
        var bottom = ref[1];
        topPoints.push(isNormal ? top : top[0]);
        bottomPoints.push(isNormal ? bottom : bottom[0]);
    }

    return [topPoints, bottomPoints.reverse()];
}

/**
 * 根据一个点的preNode和nextNode，计算出绘制需要的两个关键点top&bottom
 * @param preNode
 * @param node
 * @param nextNode
 * @param isNormal
 * @returns {*[]}
 */
function calculateKeyPoint(preNode, node, nextNode, isNormal) {

    // preNode:{x0, y0}
    // node:{x1, y1}
    // nextNode: {x2, y2}

    var x1 = node.x;
    var y1 = node.y;
    var s1 = node.size; var c1 = [x1, y1];

    var top , bottom;
    // 第一个点
    if(preNode == null) {
        var x2 = nextNode.x;
        var y2 = nextNode.y;
        var ref = getAngleInfo(x1, y1, x2, y2);
        var a1 = ref[0];
        var a2 = ref[1];

        top = [getAnglePoint(c1, s1, a1)];
        bottom = [getAnglePoint(c1, s1, a2)];
    }
    // 最后一个点
    else if(nextNode == null) {
        var x0 = preNode.x;
        var y0 = preNode.y;
        var ref$1 = getAngleInfo(x0, y0, x1, y1);
        var a1$1 = ref$1[0];
        var a2$1 = ref$1[1];

        top = [getAnglePoint(c1, s1, a1$1)];
        bottom = [getAnglePoint(c1, s1, a2$1)];
    }
    // 中间的点
    else {
        // 计算出四个关键点并判断是相交取相交点或者不想交取三点（p1, c, p2）;
        var x0$1 = preNode.x;
        var y0$1 = preNode.y;
        var s0 = preNode.size;
        var c0 = [x0$1, y0$1];
        var x2$1 = nextNode.x;
        var y2$1 = nextNode.y;
        var s2 = nextNode.size;
        var c2 = [x2$1,y2$1];

        var ref$2 = getAngleInfo(x0$1, y0$1, x1, y1);
        var a11 = ref$2[0];
        var a12 = ref$2[1];
        var ref$3 = getAngleInfo(x1, y1, x2$1, y2$1);
        var a21 = ref$3[0];
        var a22 = ref$3[1];

        var p01 = getAnglePoint(c0, s0, a11), p02 = getAnglePoint(c0, s0, a12);
        var p11 = getAnglePoint(c1, s1, a11), p12 = getAnglePoint(c1, s1, a12);
        var p13 = getAnglePoint(c1, s1, a21), p14 = getAnglePoint(c1, s1, a22);
        var p21 = getAnglePoint(c2, s2, a21), p22 = getAnglePoint(c2, s2, a22);

        var line1 = [p01, p11], line2 = [p13, p21], line3 = [p02, p12], line4 = [p14, p22];

        var intersect1 = testIntersect(line1, line2), intersect2 = testIntersect(line3, line4);

        var radius = s1 / 2;

        top = [intersect1];
        bottom = [intersect2];

        if(intersect1 == null) {
            var cp = calculateCurvePoint(c1, radius, p11, p13);
            top = isNormal ? [p11, cp, p13, radius] : [cp];
        }
        if(intersect2 == null) {
            var cp$1 = calculateCurvePoint(c1, radius, p12, p14);
            bottom = isNormal ? [p12, cp$1, p14, radius] : [cp$1];
        }
    }

    return [top, bottom];
}

function calculateCurvePoint(center, r, p1, p2){
    var _x = (p1.x + p2.x) / 2, _y = (p1.y + p2.y) / 2;
    var cx = center[0];
    var cy = center[1];
    var dx = _x - cx, dy = _y - cy;
    var k = r / Math.sqrt(dx * dx + dy * dy);

    var x = k * dx + cx,
        y = k * dy + cy;

    return {
        x: x,
        y: y
    }
}

// 判断两根长度有限的线是否相交
function testIntersect(line1, line2){

    var line1_0 = line1[0];
    var x11 = line1_0.x;
    var y11 = line1_0.y;
    var line1_1 = line1[1];
    var x12 = line1_1.x;
    var y12 = line1_1.y;
    var line2_0 = line2[0];
    var x21 = line2_0.x;
    var y21 = line2_0.y;
    var line2_1 = line2[1];
    var x22 = line2_1.x;
    var y22 = line2_1.y;

    var k1 = calculateK(line1), k2 = calculateK(line2);

    if(Math.abs(k1) === Math.abs(k2)) {
        return null;
    }

    // y = k1x + b1 & y = k2x + b2 => x = (b1 - b2) / (k2 - k1)

    var b1 = y11 - k1 * x11, b2 = y21 - k2 * x21;
    var x =  (b1 - b2) / (k2 - k1), y = k1 * x + b1;

    var ref = minMax([x11, x12]);
    var minX = ref[0];
    var maxX = ref[1];
    var ref$1 = minMax([y11, y12]);
    var minY = ref$1[0];
    var maxY = ref$1[1];

    // 相交
    if(x >= minX && x <= maxX && y >= minY && y <= maxY) {
        return {x: x,y: y};
    } else {
        return null;
    }

}

function calculateK(ref ){
    var ref_0 = ref[0];
    var x1 = ref_0.x;
    var y1 = ref_0.y;
    var ref_1 = ref[1];
    var x2 = ref_1.x;
    var y2 = ref_1.y;

    return (y2 - y1) / (x2 - x1);
}

function calculateAngle(x1, y1, x2, y2){
    var three = y1 < y2 && x1 > x2;
    var two = y1 > y2 && x1 > x2;
    return Math.atan((y1 - y2) / (x2 - x1)) + ((three || two) ? Math.PI : 0);
}

function getAngleInfo(x1, y1, x2, y2){
    var angle = calculateAngle(x1, y1, x2, y2);
    var halfPI = Math.PI / 2;
    return [-angle - halfPI, halfPI - angle];
}

function getAnglePoint(ref, r, angle){
    var x = ref[0];
    var y = ref[1];

    return {
        x: x + r * 0.5 * Math.cos(angle),
        y: y + r * 0.5 * Math.sin(angle)
    };
}

/**
 * 闭合圆环海带线-主要用于线雷达图
 * 这边把折线图的绘制抽象成一个shape，考虑到线的粗细不一致，像水中漂浮的海带一样，故命名为海带线
 */

var xFunc$1 = function (d) { return d.x; };
var yFunc$1 = function (d) { return d.y; };

var LineGenerator$1 = LinePathGenerator().x(xFunc$1).y(yFunc$1).curve(normal$1);

var RadarLine = Path.extend({

    type: 'radarLine',

    shape: {

        points: null,

        center: [0, 0],  // 圆心

    },

    style: {
        stroke: 'none',

        fill: null
    },

    buildPath: function (ctx, ref) {
        var points = ref.points;
        var center = ref.center;


        var painter = LineGenerator$1.context(ctx);

        var ref$1 = dealPoints(points, center);
        var p1 = ref$1[0];
        var p2 = ref$1[1];

        var ref$2 = p1[0];
        var x1 = ref$2.x;
        var y1 = ref$2.y;
        var ref$3 = p2[0];
        var x2 = ref$3.x;
        var y2 = ref$3.y;

        // 分两次绘制
        painter(p1);
        ctx.lineTo(x1, y1);
        painter(p2);
        ctx.lineTo(x2, y2);
    }
});

/**
 * 不同线宽时，需要根据原始点的位置和线宽，重新生成一组点，用来生成LinePath
 * @param points
 */
function dealPoints(points){
    // 外圈点
    var p1 = map(points, function (ref) {
        var x = ref.x;
        var y = ref.y;
        var width = ref.width;
        var angle = ref.angle;


        var ref$1 = getPointPositionOnCircle(width / 2, angle);
        var dx = ref$1[0];
        var dy = ref$1[1];

        return {
            x:x + dx,
            y: y + dy
        }
    });

    // 内圈点
    var p2 = map(points, function (p, index) {

        // 反向绘制hollow
        var ref = points[points.length - index - 1];
        var x = ref.x;
        var y = ref.y;
        var width = ref.width;
        var angle = ref.angle;

        var ref$1 = getPointPositionOnCircle(width / 2, angle + Math.PI);
        var dx = ref$1[0];
        var dy = ref$1[1];

        return {
            x:x + dx,
            y: y + dy
        }
    });

    return [p1, p2];
}

function getPointPositionOnCircle(radius, angle){
    return [
        radius * Math.sin(angle),
        -radius * Math.cos(angle)
    ];
}

/**
 * 闭合圆环海带线-主要用于线雷达图
 * 这边把折线图的绘制抽象成一个shape，考虑到线的粗细不一致，像水中漂浮的海带一样，故命名为海带线
 */

var RadarArea = Path.extend({

    type: 'radarArea',

    shape: {

        points: null,

        center: [0, 0],  // 圆心

    },

    style: {

        stroke: 'none',

        fill: null
    },

    buildPath: function (ctx, ref) {
        var points = ref.points;


        console.log(points);

        var ref$1 = points[0];
        var x = ref$1.x;
        var y = ref$1.y;

        forEach(points, function (ref, index) {
            var x = ref.x;
            var y = ref.y;


            ctx[index === 0 ? "moveTo" : "lineTo"](x, y);

        });

        ctx.lineTo(x, y);
    }
});

var xFunc$2 = function (d) { return d.x; };
var y1Func = function (d) { return d.y; };
var y0Func = function (d) { return d.y0; };

var horizontalAreaGenerator = areaPathGenerator().x(xFunc$2).y0(y0Func).y1(y1Func);
var verticalAreaGenerator   = areaPathGenerator().y(xFunc$2).x0(y0Func).x1(y1Func);

var STYLE_NORMAL$1 = "normal",
    STYLE_STEP$1   = "step",
    STYLE_CURVE$1  = "curve";

var Area = Path.extend({

    type: 'area',

    shape: {

        points: null,

        isHorizontal: true,

        style: "normal",  // normal || step || curve
    },

    style: {

        stroke: 'none',

        fill: null
    },

    buildPath: function (ctx, ref) {
        var points = ref.points;
        var style = ref.style; if ( style === void 0 ) style = STYLE_NORMAL$1;
        var isHorizontal = ref.isHorizontal;


        var curveFunc;

        switch (style.toLowerCase()) {
            case STYLE_STEP$1:
                curveFunc = stepAfter;
                break;
            case STYLE_CURVE$1:
                curveFunc = curve;
                break;
            default:
                curveFunc = normal$1;
        }

        var generator = isHorizontal ? horizontalAreaGenerator : verticalAreaGenerator;

        var painter = generator.context(ctx).curve(curveFunc);

        painter(points);

    }
});

var Pointer = Path.extend({

    type: 'pointer',

    shape: {
        r: 0,
        r0: 0
    },

    style: {
        fill: null
    },


    buildPath: function (ctx, shape) {

        var r = shape.r;
        var r0 = shape.r0;

        var cx = 0, cy = 0;

        var left = cx - r,
            right  = cx + r0,
            top    = cy - r0,
            bottom = cy + r0;

        // return `M${[left, cy].join()}L${[cx, top].join()}Q${[right, cy].join()},${[cx, bottom].join()}Z`;

        ctx.moveTo(left, cy);
        ctx.lineTo(cx, top);
        ctx.quadraticCurveTo(right, cy, cx, bottom);
        ctx.lineTo(left, cy);

    }
});

/**
 * 圆角Arc， ZRender提供的arc方法不支持圆角， 而slotGauge中用于需要用形状来clip SweepGradient，按照之前的arc+stroke不能满足需求
 * 这边自定义个一个圆角的Arc，之前使用d3.arc虽然支持圆角，但是圆角是向内的，这就导致startAngle = endAngle = 0，画出的不是一个圆
 * 这里简单实现一个外圆角的圆弧Graphic
 */

var PI$3 = Math.PI;

function getPointPos(radius, angle) {
    var x = radius * Math.cos(angle),
        y = radius * Math.sin(angle);

    return [x, y];
}

var RoundCornerArc = Path.extend({

    type: "roundCornerArc",

    shape: {
        r:0,
        r0:0,
        startAngle: 0,
        endAngle:0
    },

    style:{

    },

    /**
     * 外圆角圆弧path生成
     * @param ctx
     * @param r
     * @param r0
     * @param startAngle
     * @param endAngle
     */
    buildPath: function (ctx, ref) {
        var r = ref.r;
        var r0 = ref.r0;
        var startAngle = ref.startAngle;
        var endAngle = ref.endAngle;


        var r1 = (r + r0) / 2, rr = Math.abs(r - r0) / 2;
        var p1 = getPointPos(r1, startAngle);
        var p2 = getPointPos(r1, endAngle);

        var cx = 0, cy = 0;

        // 内圆圆弧->结束角度处外接圆弧->外圆圆弧->起始角度处外接圆弧
        ctx.arc(cx, cy, r0, startAngle,endAngle);
        ctx.arc(p2[0], p2[1], rr, endAngle, endAngle + PI$3);
        ctx.arc(cx, cy, r, endAngle, startAngle, true);
        ctx.arc(p1[0], p1[1], rr, startAngle, startAngle - PI$3, true);
    }
});

/**
 * 之前的180度指针仪表盘的表盘是使用一个半圆的arc外加下面一个下边圆角矩形，后来和曹笑确定了一下，表盘还有外部阴影等，这就导致冲突
 * 这边用自定义一个180度指针仪表盘的表盘形状，用于绘制180度指针仪表盘的表盘
 */

var PI$4 = Math.PI;

var SEMI_PLOT_RECT_HEIGHT = 0.14;  // 180度仪表盘底盘超出部分
var SEMI_PLOT_BORDER_RADIUS = 0.04;  // 180度仪表盘底盘圆角

var SemiPointerGaugePlot = Path.extend({

    type: "semiPointerGaugePlot",

    shape: {
        cx: 0,
        cy: 0,
        r: 0,
    },

    style: {
    },

    buildPath: function (ctx, ref) {
        var cx = ref.cx;
        var cy = ref.cy;
        var r = ref.r;


        var bh = r * SEMI_PLOT_RECT_HEIGHT;
        var br = r * SEMI_PLOT_BORDER_RADIUS;

        var dx = r - br, dy = bh - br, y2 = cy + dy;

        // 左侧端点180度大圆弧至右侧端点->右侧端点连线右下方圆角->右下方圆角->右下方圆角连线左下角圆角->左下方圆角->下方圆角连线左下方端点
        ctx.arc(cx, cy, r, PI$4, PI$4 * 2);
        ctx.lineTo(cx + r, y2);
        ctx.arc(cx + dx, y2, br, 0, PI$4 / 2);
        ctx.lineTo(cx - dx, cy + bh);
        ctx.arc(cx - dx, y2, br, PI$4 / 2, PI$4);
        ctx.lineTo(cx - r, cy);
    }
});

/**
 * RegionLine for region exhibition of fake point in timing series prediction
 */

// http://ui.finebi.com:7777/UI/05-%E5%9B%BE%E8%A1%A8/#g=1&p=%E6%97%B6%E5%BA%8F%E9%A2%84%E6%B5%8B
var LINE_WIDTH = 2.5;
var WIDTH      = 7.5;

var RegionLine = Path.extend({

    type: 'regionLine',

    shape: {
        x1: 0,
        y1: 0,

        x2: 0,
        y2: 0
    },

    style: {
        lineWidth: LINE_WIDTH
    },

    buildPath: function (ctx, ref) {
        var x1 = ref.x1;
        var y1 = ref.y1;
        var x2 = ref.x2;
        var y2 = ref.y2;


        var horizontal = x1 === x2;

        if(horizontal && y1 === y2) {
            return;
        }

        var width = WIDTH, halfWidth = width / 2;

        if (horizontal) {

            ctx.moveTo(x1 - halfWidth, y1);
            ctx.lineTo(x1 + halfWidth, y1);
            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);
            ctx.moveTo(x1 - halfWidth, y2);
            ctx.lineTo(x1 + halfWidth, y2);

        } else {

            ctx.moveTo(x1, y1 - halfWidth);
            ctx.lineTo(x1, y1 + halfWidth);
            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);
            ctx.moveTo(x2, y2 - halfWidth);
            ctx.lineTo(x2, y2 + halfWidth);

        }

    }
});

var AreaMap = Path.extend({

    type: 'areaMap',

    shape: {

        rings: null
    },

    buildPath: function (ctx, ref) {
        var rings = ref.rings;


        var i, j, len, len2, points, p;

        for (i = 0, len = rings.length; i < len; i++) {

            points = rings[i];

            for (j = 0, len2 = points.length; j < len2; j++) {
                p = points[j];
                ctx[j ? "lineTo" : "moveTo"](p.x, p.y);
            }

        }
    }
});

/**
 * Created by Jeffrey on 2018/1/27.
 */

var graphic = {
    Group: Group,
    Rect: Rect,
    Circle: Circle,
    Text: Text,
    Heart: Heart,
    Polygon: Polygon,
    Polyline: Polyline,
    Line: Line,
    Sector: Sector,
    Star: Star,
    Arc: Arc,
    Image: ZImage,

    Pointer: Pointer,
    VarLine: VarLine,
    KelpLine: KelpLine,
    RadarLine: RadarLine,
    RadarArea: RadarArea,
    Area: Area,
    RoundCornerArc: RoundCornerArc,
    SemiPointerGaugePlot: SemiPointerGaugePlot,
    RegionLine: RegionLine,
    AreaMap: AreaMap
};

forOwn(graphic, function (ctor, k) {
    var type = k[0].toLowerCase() + k.substr(1);
    {
        // a trick to show shape's name in console
        ctor = eval(("(class " + k + " extends ctor {})"));
    }
    graphic[type] = function (cfg) {
        return new ctor(cfg)
    };
});

// todo
// `path` generated by `createFromString` cannot change the shape
// we should avoid using `path` in most case
graphic.path = function (opt) {
    var pathProxy;
    var opts = opt || {};
    opts.buildPath = function (path, shape) {
        pathProxy = createPathProxyFromString(shape.str);
        if (path.setData) {
            path.setData(pathProxy.data);
            // Svg and vml renderer don't have context
            var ctx = path.getContext();
            if (ctx) {
                path.rebuildPath(ctx);
            }
        }
        else {
            var ctx = path;
            pathProxy.rebuildPath(ctx);
        }
    };

    opts.applyTransform = function (m) {
        transformPath(pathProxy, m);

        this.dirty(true);
    };

    return new Path(opts);
};

/**
 * position/scale/origin in attr may be frozen
 * @param attr
 * @returns {*}
 */
function createHelper (attr) {
    forEach(TransformNames, function (k) {
        if (attr[k]) {
            attr[k] = attr[k].slice();
        }
    });
    return attr
}

/** Create an element with the given nodeName.
 *	@param {String} nodeName
 *	@param {Object} [attr]   - for `path` to initialize
 *	@returns {Element} node
 */
function createNode(nodeName, attr) {
    var node;
    if (nodeName === 'path') {
        node = graphic.path(createHelper(attr));
    } else {
        node = graphic[nodeName]();
    }
    return node
}

/** Remove a child node from its parent if attached.
 *	@param {Element} node		The node to remove
 */
function removeNode(node) {
    // let parentNode = node.parentNode;
    // if (parentNode) parentNode.removeChild(node);

    // todo, zr has several remove methods.
    // Is there any difference between `delFromStorage` and `group.remove`...
    var parent = node.parent || node.__zr;
    if (parent) {
        parent.remove(node);
    }
    if (node.__zr) {
        // remove animation
        node.removeSelfFromZr(node.__zr);
    }
}

var pairEqual = function (a, b) { return a[0] === b[0] && a[1] === b[1]; };

/** Set a named attribute on the given Node, with special behavior for some names and event handlers.
 *	If `value` is `null`, the attribute/handler will be removed.
 *	@param {Element} node	- An element to mutate
 *	@param {string} name	- The name/key to set, such as an event or attribute name
 *	@param {any} value	    - An attribute value, such as a function to be used as an event handler
 *	@param {Boolean} isPure	- Do we need a deep (2th) comparison (e.g. position...)
 *	@returns {boolean} dirty
 */
function setAccessor(node, name, value, isPure) {

    // We get the `key` from node directly, so no ignore.
    // But as a convention, `key` shouldn't be used outside the diff.
    //
    // if (name==='key') {
    //     // ignore
    // }
    // else

    if (name==='ref') {
        var old = node[name];

        // if (old) old(null);
        // if (value) value(node);
        // node[name] = value;

        if (old !== value) {
            old && old(null);
            value && value(node);
            node[name] = value;
        }

    }
    // clip, compare inside `setClip`
    else if (name === 'clip') {
        setClip(node, value, isPure);
    }
    // array (Element.attrKV), a deeper compare
    else if (TransformNamesMap[name]) {
        return attrHelper(node, name, value, isPure);
    }
    // as simple value (no deep comparison)
    // update if it's impure or not equal
    else if ( !(isPure && value === node[name]) ) {
        // todo, compare attrKV & set manually
        node.attrKV(name, value);
        return true
    }
    // todo, gradient?
}

/**
 * position, scale, origin helper
 * @param shape
 * @param key
 * @param value
 * @param isPure
 * @returns {boolean} dirty
 */
function attrHelper (shape, key, value, isPure) {
    shape[key] = shape[key] || [];
    var target = shape[key];
    if (value) {
        // update if it's impure or not equal
        if ( !(isPure && pairEqual(value, target)) ) {
            try{
                target[0] = value[0];
                target[1] = value[1];
            } catch (e){

            }

            return true
        }
        // else no update

    } else {
        // FIXME
        // bug:
        // [] -> null
        // but position/scale/origin defaults are different
        // how to get a proper solution
    }
    return false
}

/**
 * Set a clip to shape
 * @param shape    - zr's element
 * @param cfg      - attr.clip
 * @param isPure   - deep (2th) comparison
 */
function setClip (shape, cfg, isPure) {
    var vclip = shape._vclip;

    // the same type, update
    if (vclip && vclip.type === cfg.type) {

        // update, or compare & update
        if ( !(isPure && isMatch(vclip.shape, cfg.shape)) ) {
            vclip.setShape(cfg.shape);
        }

        var helper = function (key) { return (key in cfg)
            && attrHelper(vclip, key, cfg[key], isPure)
            && shape.dirty(false); };

        helper('position');
        helper('scale');
        helper('origin');

    } else {
        shape._vclip = graphic[cfg.type](createHelper(cfg));
        shape.setClipPath(shape._vclip);
    }
}

/** Queue of components that have been mounted and are awaiting componentDidMount */
var mounts = [];

/** Diff recursion count, used to track the end of the diff cycle. */
var diffLevel = 0;

/** Global flag indicating if the diff is performing hydration */
var hydrating = false;

/** Invoke queued componentDidMount lifecycle methods */
function flushMounts() {
	var c;
	while ((c=mounts.pop())) {
		if (options.afterMount) { options.afterMount(c); }
		if (c.componentDidMount) { c.componentDidMount(); }
	}
}


/** Apply differences in a given vnode (and it's deep children) to a real DOM Node.
 *	@param {Element} [dom=null]		A DOM node to mutate into the shape of the `vnode`
 *	@param {VNode} vnode			A VNode (with descendants forming a tree) representing the desired DOM structure
 *	@returns {Element} dom			The created/mutated element
 *	@private
 */
function diff(dom, vnode, context, mountAll, parent, componentRoot) {
	// diffLevel having been 0 here indicates initial entry into the diff (not a subdiff)
	if (!diffLevel++) {
		// when first starting the diff, check if we're diffing an SVG or within an SVG
		// isSvgMode = parent!=null && parent.ownerSVGElement!==undefined;

		// hydration is indicated by the existing element to be diffed not having a prop cache
		hydrating = dom!=null && !(ATTR_KEY in dom);
	}

	var ret = idiff(dom, vnode, context, mountAll, componentRoot);

	// append the element if its a new parent
	if (parent && ret.parent!==parent) { parent.add(ret); }

	// diffLevel being reduced to 0 means we're exiting the diff
	if (!--diffLevel) {
		hydrating = false;
		// invoke queued componentDidMount lifecycle methods
		if (!componentRoot) { flushMounts(); }
	}

	return ret;
}


/** Internals of `diff()`, separated to allow bypassing diffLevel / mount flushing. */
function idiff(dom, vnode, context, mountAll, componentRoot) {
	var out = dom;

	// empty values (null, undefined, booleans) render as empty Text nodes
	// if (vnode==null || typeof vnode==='boolean') vnode = '';


	// Fast case: Strings & Numbers create/update Text nodes.
	// if (typeof vnode==='string' || typeof vnode==='number') {
    //
	// 	// update if it's already a Text node:
	// 	if (dom && dom.splitText!==undefined && dom.parentNode && (!dom._component || componentRoot)) {
	// 		/* istanbul ignore if */ /* Browser quirk that can't be covered: https://github.com/developit/preact/commit/fd4f21f5c45dfd75151bd27b4c217d8003aa5eb9 */
	// 		if (dom.nodeValue!=vnode) {
	// 			dom.nodeValue = vnode;
	// 		}
	// 	}
	// 	else {
	// 		// it wasn't a Text node: replace it with one and recycle the old Element
	// 		out = document.createTextNode(vnode);
	// 		if (dom) {
	// 			if (dom.parentNode) dom.parentNode.replaceChild(out, dom);
	// 			recollectNodeTree(dom, true);
	// 		}
	// 	}
    //
	// 	out[ATTR_KEY] = true;
    //
	// 	return out;
	// }

    /**
     * no Text Node
     */

    /**
     * by Jeffrey
     * base node -> null
     * dom空vnode可以看作空白text节点
     * 我们没有text node，直接去掉
     */
    if (!vnode) {
        dom && dom.parent && dom.parent.remove(dom);
        return
    }


	// If the VNode represents a Component, perform a component diff:
	var vnodeName = vnode.nodeName;
	if (typeof vnodeName==='function') {
		return buildComponentFromVNode(dom, vnode, context, mountAll);
	}


	// Tracks entering and exiting SVG namespace when descending through the tree.
	// isSvgMode = vnodeName==='svg' ? true : vnodeName==='foreignObject' ? false : isSvgMode;


	// If there's no existing element or it's the wrong type, create a new one:
	// vnodeName = String(vnodeName);
	if (!dom || !isNamedNode(dom, vnodeName)) {
		out = createNode(vnodeName, vnode.attributes);

		if (dom) {
			// move children into the replacement node
			// while (dom.firstChild) out.appendChild(dom.firstChild);
			// todo, batch add
			while (dom.childCount && dom.childCount()) {
				out.add(dom.childAt(0));
			}

			// if the previous Element was mounted into the DOM, replace it inline
			// if (dom.parentNode) dom.parentNode.replaceChild(out, dom);
			if (dom.parent) {
				// todo, replace
				dom.parent.addBefore(out, dom);
				dom.parent.remove(dom);
			}

			// recycle the old element (skips non-Element node types)
			recollectNodeTree(dom, true);
		}
	}


	var fc = out._children && out._children[0],
		props = out[ATTR_KEY],
		vchildren = vnode.children;

    // todo, can we move this flag behind the `diffAttributes` to indicate the first render?
	if (props==null) {
		props = out[ATTR_KEY] = true;
		// for (let a=out.attributes, i=a.length; i--; ) props[a[i].name] = a[i].value;
	}

	// // Optimization: fast-path for elements containing a single TextNode:
	// if (!hydrating && vchildren && vchildren.length===1 && typeof vchildren[0]==='string' && fc!=null && fc.splitText!==undefined && fc.nextSibling==null) {
	// 	if (fc.nodeValue!=vchildren[0]) {
	// 		fc.nodeValue = vchildren[0];
	// 	}
	// }
	// // otherwise, if there are existing or new children, diff them:
	// else
	if (vchildren && vchildren.length || fc!=null) {
		innerDiffNode(out, vchildren, context, mountAll, hydrating);
	}

	// Apply attributes/props from VNode to the DOM Element:
	diffAttributes(out, vnode.attributes/* props*/);

	// restore previous SVG mode: (in case we're exiting an SVG namespace)
	// isSvgMode = prevSvgMode;

	return out;
}


/** Apply child and attribute changes between a VNode and a DOM Node to the DOM.
 *	@param {Element} dom			Element whose children should be compared & mutated
 *	@param {Array} vchildren		Array of VNodes to compare to `dom.childNodes`
 *	@param {Object} context			Implicitly descendant context object (from most recent `getChildContext()`)
 *	@param {Boolean} mountAll
 *	@param {Boolean} isHydrating	If `true`, consumes externally created elements similar to hydration
 */
function innerDiffNode(dom, vchildren, context, mountAll, isHydrating) {
	var originalChildren = dom._children,
		children = [],
		keyed = {},
		keyedLen = 0,
		min = 0,
		len = originalChildren.length,
		childrenLen = 0,
		vlen = vchildren ? vchildren.length : 0,
		j, c, f, vchild, child;

	// Build up a map of keyed children and an Array of unkeyed children:
	if (len!==0) {
		for (var i=0; i<len; i++) {
			var child$1 = originalChildren[i],
				// props = child[ATTR_KEY],
				props = child$1,
				key = vlen && props ? child$1._component ? child$1._component.__key : props.key : null;
			if (key!=null) {
				keyedLen++;
				keyed[key] = child$1;
			}
			// todo, remove this condition
			else if (props || (child$1.splitText!==undefined ? (isHydrating ? child$1.nodeValue.trim() : true) : isHydrating)) {
				children[childrenLen++] = child$1;
			}
		}
	}

	if (vlen!==0) {
		for (var i$1=0; i$1<vlen; i$1++) {
			vchild = vchildren[i$1];
			child = null;

			// attempt to find a node based on key matching
			var key$1 = vchild.key;
			if (key$1!=null) {
				if (keyedLen && keyed[key$1]!==undefined) {
					child = keyed[key$1];
					keyed[key$1] = undefined;
					keyedLen--;
				}
			}
			// attempt to pluck a node of the same type from the existing children
			else if (!child && min<childrenLen) {
				for (j=min; j<childrenLen; j++) {
					if (children[j]!==undefined && isSameNodeType(c = children[j], vchild, isHydrating)) {
						child = c;
						children[j] = undefined;
						if (j===childrenLen-1) { childrenLen--; }
						if (j===min) { min++; }
						break;
					}
				}
			}

			// morph the matched/found/created DOM child to match vchild (deep)
			child = idiff(child, vchild, context, mountAll);

			f = originalChildren[i$1];
			if (child && child!==dom && child!==f) {
				if (f==null) {
					dom.add(child);
				}
				// else if (child===f.nextSibling) {
				// todo, optimize
                else if (child===dom._children[dom._children.indexOf(f) + 1]) {
                    removeNode(f);
                }
				else {
					dom.addBefore(child, f);
				}
			}
		}
	}


	// remove unused keyed children:
	if (keyedLen) {
		for (var i$2 in keyed) { if (keyed[i$2]!==undefined) { recollectNodeTree(keyed[i$2], false); } }
	}

	// remove orphaned unkeyed children:
	while (min<=childrenLen) {
		if ((child = children[childrenLen--])!==undefined) { recollectNodeTree(child, false); }
	}
}



/** Recursively recycle (or just unmount) a node and its descendants.
 *	@param {Node} node						DOM node to start unmount/removal from
 *	@param {Boolean} [unmountOnly=false]	If `true`, only triggers unmount lifecycle, skips removal
 */
function recollectNodeTree(node, unmountOnly) {
	var component = node._component;
	if (component) {
		// if node is owned by a Component, unmount that component (ends up recursing back here)
		unmountComponent(component);
	}
	else {
		// If the node's VNode had a ref function, invoke it with null here.
		// (this is part of the React spec, and smart for unsetting references)
		// if (node[ATTR_KEY]!=null && node[ATTR_KEY].ref) node[ATTR_KEY].ref(null);
        if (node.ref) { node.ref(null); }

		if (unmountOnly===false || node[ATTR_KEY]==null) {
			removeNode(node);
		}

		removeChildren(node);
	}
}


/** Recollect/unmount all children.
 *	- we use .lastChild here because it causes less reflow than .firstChild
 *	- it's also cheaper than accessing the .childNodes Live NodeList
 */
function removeChildren(node) {
	var children = node._children;
	if(children){
	    forEach(children, function (child) {
			child && recollectNodeTree(child, true);
        });
	}
}


/** Apply differences in attributes from a VNode to the given DOM Element.
 *	@param {Group|Path} element - Element (zr's) with attributes to diff `attrs` against
 *	@param {Object} attrs		- The desired end-state key-value attribute pairs
 *	// @param {Object} old		- Current/previous attributes (from previous VNode or element's prop cache)
 *  old 空
 * 添加key:
 * 'clip': 添加clip
 * 'pure': 比较group属性防止无用刷新（当多层时）
 *
 * @example
 * {
 *     pure: true,
 *     clip: {
 *         type: 'rect',
 *         shape: {},
 *         position: []
 *     }
 * }
 */
function diffAttributes(element, attrs) {
	var obj;

	var name;

	// // remove attributes no longer present on the vnode by setting them to undefined
	// for (name in old) {
	// 	if (!(attrs && attrs[name]!=null) && old[name]!=null) {
	// 		setAccessor(dom, name, old[name], old[name] = undefined, isSvgMode);
	// 	}
	// }
    //
	// // add new & update changed attributes
	// for (name in attrs) {
	// 	if (name!=='children' && name!=='innerHTML' && (!(name in old) || attrs[name]!==(name==='value' || name==='checked' ? dom[name] : old[name]))) {
	// 		setAccessor(dom, name, old[name], old[name] = attrs[name], isSvgMode);
	// 	}
	// }

	// zr-class.attr:
	//
    // Element.attr = attrKVs + dirty
    // Element.attrKV 特殊处理了position, scale, origin
    //
    // Group extends Element
    // Group.attr = Element.attr
    //
    // Displayable extends Element
    // Displayable.attrKV特殊处理了style
    //
    // Path extends Displayable
    // Path.attrKV特殊处理了shape

    if (!attrs) {
        return
    }

    var isPure = attrs.pure; // `pure` -> compare attrs

    // default is dirty
    var dirty = true;

    // otherwise we don't know if it's dirty now, compare
    if (isPure) {
        dirty = false;
    }

    // here may be a bug:
    // h('group', { foo: 1, bar: 2 }) -> h('group', { foo: 1 })
    // but why not set it to `null` explicitly

	if (element.animators.length) {
        // stop previous animation
        element.stopAnimation();
	}

	// todo, 搞一搞这里居然已经变得这么冗余的关系……
	var isFirstRender = element[ATTR_KEY] !== RENDERED;

    // no transition in the first render unless `appear` set
    if (attrs.transition
		&& (!isFirstRender || attrs.transition.appear)) {

        var transition = attrs.transition;

        var properties = transition.properties || AnimationNamesMap;

        var appear;
        if (isFirstRender) {
            appear = transition.appear && transition.appear(attrs);
        }

        for (name in attrs) {
            if (!attrs.hasOwnProperty(name)
                || name === 'transition'
                || name === 'transitionEnd') {
                continue
            }

            var value = attrs[name];

            var appearValue = appear && appear[name];

            if (properties[name]
				&& (!isFirstRender || appearValue)) {

            	// gradient no animation
            	if (name === 'style') {
            		if(typeof value.fill === 'object' || typeof element.style.fill === 'object'){
            			element.setStyle('fill', value.fill);
                    	delete value.fill;
            		}
					if(typeof value.rich === 'object' || typeof element.style.rich === 'object'){
						element.setStyle('rich', value.rich);
						delete value.rich;
					}
				}

				// zr's animator.when(0) logic is buggy...
				if (appearValue) {
                	element.attrKV(name, appearValue);
                }

                // todo,
				// animate doesn't support animate('key')...
				// We can make a custom fn...
				// And add transition key support
				if (name === 'rotation' || name === "origin") {
            		element.animate('')
                           .delay(transition.delay)
                           .when(transition.duration, ( obj = {}, obj[name] = value, obj))
                           .start(transition.ease, appearValue);
				} else {
                    element.animate(name)
                           .delay(transition.delay)
                           .when(transition.duration, value)
                           .start(transition.ease, appearValue);
                }

            } else {
                var result = setAccessor(element, name, value, isPure);
                dirty = dirty || result;
            }
        }

    } else {
        for (name in attrs) {
            if (!attrs.hasOwnProperty(name)
                || name === 'transition'
                || name === 'transitionEnd') {
                continue
            }
            var value$1 = attrs[name];
			var result$1 = setAccessor(element, name, value$1, isPure);
			dirty = dirty || result$1;
        }
	}

    var end = attrs['transitionEnd'];
    var animators = element.animators;
    if (end && animators.length) {
    	animators[animators.length - 1].done(function () {
    		end(element);
		});
	}

    dirty && element.dirty(false);

    element[ATTR_KEY] = RENDERED;
}

/** Retains a pool of Components for re-use, keyed on component name.
 *	Note: since component names are not unique or even necessarily available, these are primarily a form of sharding.
 *	@private
 */
var components = {};


/** Create a component. Normalizes differences between PFC's and classful Components. */
function createComponent(Ctor, props, context) {
	var list = components[Ctor.name],
		inst;

	if (Ctor.prototype && Ctor.prototype.render) {
		inst = new Ctor(props, context);
		Component.call(inst, props, context);
	}
	else {
		inst = new Component(props, context);
		inst.constructor = Ctor;
		inst.render = doRender;
	}


	if (list) {
		for (var i=list.length; i--; ) {
			if (list[i].constructor===Ctor) {
				inst.nextBase = list[i].nextBase;
				list.splice(i, 1);
				break;
			}
		}
	}
	return inst;
}


/** The `.render()` method for a PFC backing instance. */
function doRender(props, state, context) {
	return this.constructor(props, context);
}

/** Set a component's `props` (generally derived from JSX attributes).
 *	@param {Object} props
 *	@param {Object} [opts]
 *	@param {boolean} [opts.renderSync=false]	If `true` and {@link options.syncComponentUpdates} is `true`, triggers synchronous rendering.
 *	@param {boolean} [opts.render=true]			If `false`, no render will be triggered.
 */
function setComponentProps(component, props, opts, context, mountAll) {
	if (component._disable) { return; }
	component._disable = true;

	if ((component.__ref = props.ref)) { delete props.ref; }
	if ((component.__key = props.key)) { delete props.key; }

	if (!component.base || mountAll) {
		if (component.componentWillMount) { component.componentWillMount(); }
	}
	else if (component.componentWillReceiveProps) {
		component.componentWillReceiveProps(props, context);
	}

	if (context && context!==component.context) {
		if (!component.prevContext) { component.prevContext = component.context; }
		component.context = context;
	}

	if (!component.prevProps) { component.prevProps = component.props; }
	component.props = props;

	component._disable = false;

	if (opts!==NO_RENDER) {
		if (opts===SYNC_RENDER || options.syncComponentUpdates!==false || !component.base) {
			renderComponent(component, SYNC_RENDER, mountAll);
		}
		else {
			enqueueRender(component);
		}
	}

	if (component.__ref) { component.__ref(component); }
}



/** Render a Component, triggering necessary lifecycle events and taking High-Order Components into account.
 *	@param {Component} component
 *	@param {Object} [opts]
 *	@param {boolean} [opts.build=false]		If `true`, component will build and store a DOM node if not already associated with one.
 *	@private
 */
function renderComponent(component, opts, mountAll, isChild) {
	if (component._disable) { return; }

	var props = component.props,
		state = component.state,
		context = component.context,
		previousProps = component.prevProps || props,
		previousState = component.prevState || state,
		previousContext = component.prevContext || context,
		isUpdate = component.base,
		nextBase = component.nextBase,
		initialBase = isUpdate || nextBase,
		initialChildComponent = component._component,
		skip = false,
		rendered, inst, cbase;

	// if updating
	if (isUpdate) {
		component.props = previousProps;
		component.state = previousState;
		component.context = previousContext;
		if (opts!==FORCE_RENDER
			&& component.shouldComponentUpdate
			&& component.shouldComponentUpdate(props, state, context) === false) {
			skip = true;
		}
		else if (component.componentWillUpdate) {
			component.componentWillUpdate(props, state, context);
		}
		component.props = props;
		component.state = state;
		component.context = context;
	}

	component.prevProps = component.prevState = component.prevContext = component.nextBase = null;
	component._dirty = false;

	if (!skip) {
		rendered = component.render(props, state, context);

		// context to pass to the child, can be updated via (grand-)parent component
		if (component.getChildContext) {
			context = extend(extend({}, context), component.getChildContext());
		}

		var childComponent = rendered && rendered.nodeName,
			toUnmount, base;

		if (typeof childComponent==='function') {
			// set up high order component link

			var childProps = getNodeProps(rendered);
			inst = initialChildComponent;

			if (inst && inst.constructor===childComponent && childProps.key==inst.__key) {
				setComponentProps(inst, childProps, SYNC_RENDER, context, false);
			}
			else {
				toUnmount = inst;

				component._component = inst = createComponent(childComponent, childProps, context);
				inst.nextBase = inst.nextBase || nextBase;
				inst._parentComponent = component;
				setComponentProps(inst, childProps, NO_RENDER, context, false);
				renderComponent(inst, SYNC_RENDER, mountAll, true);
			}

			base = inst.base;
		}
		else {
			cbase = initialBase;

			// destroy high order component link
			toUnmount = initialChildComponent;
			if (toUnmount) {
				cbase = component._component = null;
			}

			if (initialBase || opts===SYNC_RENDER) {
				if (cbase) { cbase._component = null; }
				base = diff(cbase, rendered, context, mountAll || !isUpdate, initialBase && initialBase.parent, true);
			}
		}

		if (initialBase && base!==initialBase && inst!==initialChildComponent) {
			var baseParent = initialBase.parent;
			if (baseParent && base!==baseParent) {
				// baseParent.replaceChild(base, initialBase);
				baseParent.addBefore(base, initialBase);
				baseParent.remove(initialBase);

				if (!toUnmount) {
					initialBase._component = null;
					recollectNodeTree(initialBase, false);
				}
			}
		}

		if (toUnmount) {
			unmountComponent(toUnmount);
		}

		component.base = base;
		if (base && !isChild) {
			var componentRef = component,
				t = component;
			while ((t=t._parentComponent)) {
				(componentRef = t).base = base;
			}
			base._component = componentRef;
			base._componentConstructor = componentRef.constructor;
		}
	}

	if (!isUpdate || mountAll) {
		mounts.unshift(component);
	}
	else if (!skip) {
		// Ensure that pending componentDidMount() hooks of child components
		// are called before the componentDidUpdate() hook in the parent.
		// Note: disabled as it causes duplicate hooks, see https://github.com/developit/preact/issues/750
		// flushMounts();

		if (component.componentDidUpdate) {
			component.componentDidUpdate(previousProps, previousState, previousContext);
		}
		if (options.afterUpdate) { options.afterUpdate(component); }
	}

	if (component._renderCallbacks!=null) {
		while (component._renderCallbacks.length) { component._renderCallbacks.pop().call(component); }
	}

	if (!diffLevel && !isChild) { flushMounts(); }
}



/** Apply the Component referenced by a VNode to the DOM.
 *	@param {Element} dom	The DOM node to mutate
 *	@param {VNode} vnode	A Component-referencing VNode
 *	@returns {Element} dom	The created/mutated element
 *	@private
 */
function buildComponentFromVNode(dom, vnode, context, mountAll) {
	var c = dom && dom._component,
		originalComponent = c,
		oldDom = dom,
		isDirectOwner = c && dom._componentConstructor===vnode.nodeName,
		isOwner = isDirectOwner,
		props = getNodeProps(vnode);
	while (c && !isOwner && (c=c._parentComponent)) {
		isOwner = c.constructor===vnode.nodeName;
	}

	if (c && isOwner && (!mountAll || c._component)) {
		setComponentProps(c, props, ASYNC_RENDER, context, mountAll);
		dom = c.base;
	}
	else {
		if (originalComponent && !isDirectOwner) {
			unmountComponent(originalComponent);
			dom = oldDom = null;
		}

		c = createComponent(vnode.nodeName, props, context);
		if (dom && !c.nextBase) {
			c.nextBase = dom;
			// passing dom/oldDom as nextBase will recycle it if unused, so bypass recycling on L229:
			oldDom = null;
		}
		setComponentProps(c, props, SYNC_RENDER, context, mountAll);
		dom = c.base;

		if (oldDom && dom!==oldDom) {
			oldDom._component = null;
			recollectNodeTree(oldDom, false);
		}
	}

	return dom;
}



/** Remove a component from the DOM and recycle it.
 *	@param {Component} component	The Component instance to unmount
 *	@private
 */
function unmountComponent(component) {
	if (options.beforeUnmount) { options.beforeUnmount(component); }

	var base = component.base;

	component._disable = true;

	if (component.componentWillUnmount) { component.componentWillUnmount(); }

	component.base = null;

	// recursively tear down & recollect high-order component children:
	var inner = component._component;
	if (inner) {
		unmountComponent(inner);
	}
	else if (base) {
		if (base[ATTR_KEY] && base[ATTR_KEY].ref) { base[ATTR_KEY].ref(null); }

		component.nextBase = base;

		removeNode(base);
		// 收集之后重新使用，重新setProps的时候，会导致一些属性不会被置为默认，这边先注释掉
		// collectComponent(component);

		removeChildren(base);
	}

	if (component.__ref) { component.__ref(null); }
}

/** Base Component class.
 *	Provides `setState()` and `forceUpdate()`, which trigger rendering.
 *	@public
 *
 *	@example
 *	class MyFoo extends Component {
 *		render(props, state) {
 *			return <div />;
 *		}
 *	}
 */
function Component(props, context) {
	this._dirty = true;

	/** @public
	 *	@type {object}
	 */
	this.context = context;

	/** @public
	 *	@type {object}
	 */
	this.props = props;

	/** @public
	 *	@type {object}
	 */
	this.state = this.state || {};
}


extend(Component.prototype, {

	/** Returns a `boolean` indicating if the component should re-render when receiving the given `props` and `state`.
	 *	@param {object} nextProps
	 *	@param {object} nextState
	 *	@param {object} nextContext
	 *	@returns {Boolean} should the component re-render
	 *	@name shouldComponentUpdate
	 *	@function
	 */


	/** Update component state by copying properties from `state` to `this.state`.
	 *	@param {object} state		A hash of state properties to update with new values
	 *	@param {function} callback	A function to be called once component state is updated
	 */
	setState: function(state, callback) {
		var s = this.state;
		if (!this.prevState) { this.prevState = extend({}, s); }
		extend(s, typeof state==='function' ? state(s, this.props) : state);
		if (callback) { (this._renderCallbacks = (this._renderCallbacks || [])).push(callback); }
		enqueueRender(this);
	},


	/** Immediately perform a synchronous re-render of the component.
	 *	@param {function} callback		A function to be called after component is re-rendered.
	 *	@private
	 */
	forceUpdate: function(callback) {
		if (callback) { (this._renderCallbacks = (this._renderCallbacks || [])).push(callback); }
		renderComponent(this, FORCE_RENDER);
	},


	/** Accepts `props` and `state`, and returns a new Virtual DOM tree to build.
	 *	Virtual DOM is generally constructed via [JSX](http://jasonformat.com/wtf-is-jsx).
	 *	@param {object} props		Props (eg: JSX attributes) received from parent element/component
	 *	@param {object} state		The component's current state
	 *	@param {object} context		Context object (if a parent component has provided context)
	 *	@returns VNode
	 */
	render: function() {}

});

var PureComponent$$1 = (function (Component$$1) {
    function PureComponent$$1 (props, context) {
        Component$$1.call(this, props, context);
    }

    if ( Component$$1 ) PureComponent$$1.__proto__ = Component$$1;
    PureComponent$$1.prototype = create( Component$$1 && Component$$1.prototype );
    PureComponent$$1.prototype.constructor = PureComponent$$1;

    PureComponent$$1.prototype.shouldComponentUpdate = function (nextProps, nextState) {
        return !shallowEqual(this.props, nextProps) || !shallowEqual(this.state, nextState)
    };

    return PureComponent$$1;
}(Component));

// TODO Draggable for group
// FIXME Draggable on element which has parent rotation or scale
function Draggable() {

    this.on('mousedown', this._dragStart, this);
    this.on('mousemove', this._drag, this);
    this.on('mouseup', this._dragEnd, this);
    this.on('globalout', this._dragEnd, this);
    // this._dropTarget = null;
    // this._draggingTarget = null;

    // this._x = 0;
    // this._y = 0;
}

Draggable.prototype = {

    constructor: Draggable,

    _dragStart: function (e) {
        var draggingTarget = e.target;
        if (draggingTarget && draggingTarget.draggable) {
            this._draggingTarget = draggingTarget;
            draggingTarget.dragging = true;
            this._x = e.offsetX;
            this._y = e.offsetY;

            this.dispatchToElement(param(draggingTarget, e), 'dragstart', e.event);
        }
    },

    _drag: function (e) {
        var draggingTarget = this._draggingTarget;
        if (draggingTarget) {

            var x = e.offsetX;
            var y = e.offsetY;

            var dx = x - this._x;
            var dy = y - this._y;
            this._x = x;
            this._y = y;

            draggingTarget.drift(dx, dy, e);
            this.dispatchToElement(param(draggingTarget, e), 'drag', e.event);

            var dropTarget = this.findHover(x, y, draggingTarget).target;
            var lastDropTarget = this._dropTarget;
            this._dropTarget = dropTarget;

            if (draggingTarget !== dropTarget) {
                if (lastDropTarget && dropTarget !== lastDropTarget) {
                    this.dispatchToElement(param(lastDropTarget, e), 'dragleave', e.event);
                }
                if (dropTarget && dropTarget !== lastDropTarget) {
                    this.dispatchToElement(param(dropTarget, e), 'dragenter', e.event);
                }
            }
        }
    },

    _dragEnd: function (e) {
        var draggingTarget = this._draggingTarget;

        if (draggingTarget) {
            draggingTarget.dragging = false;
        }

        this.dispatchToElement(param(draggingTarget, e), 'dragend', e.event);

        if (this._dropTarget) {
            this.dispatchToElement(param(this._dropTarget, e), 'drop', e.event);
        }

        this._draggingTarget = null;
        this._dropTarget = null;
    }

};

function param(target, e) {
    return {target: target, topTarget: e && e.topTarget};
}

/**
 * Handler
 * @module zrender/Handler
 * @author Kener (@Kener-林峰, kener.linfeng@gmail.com)
 *         errorrik (errorrik@gmail.com)
 *         pissang (shenyi.914@gmail.com)
 */

var SILENT = 'silent';

function makeEventPacket(eveType, targetInfo, event) {
    return {
        type: eveType,
        event: event,
        // target can only be an element that is not silent.
        target: targetInfo.target,
        // topTarget can be a silent element.
        topTarget: targetInfo.topTarget,
        cancelBubble: false,
        offsetX: event.zrX,
        offsetY: event.zrY,
        gestureEvent: event.gestureEvent,
        pinchX: event.pinchX,
        pinchY: event.pinchY,
        pinchScale: event.pinchScale,
        wheelDelta: event.zrDelta,
        zrByTouch: event.zrByTouch,
        which: event.which
    };
}

function EmptyProxy () {}
EmptyProxy.prototype.dispose = function () {};

var handlerNames = [
    'click', 'dblclick', 'mousewheel', 'mouseout',
    'mouseup', 'mousedown', 'mousemove', 'contextmenu'
];
/**
 * @alias module:zrender/Handler
 * @constructor
 * @extends module:zrender/mixin/Eventful
 * @param {module:zrender/Storage} storage Storage instance.
 * @param {module:zrender/Painter} painter Painter instance.
 * @param {module:zrender/dom/HandlerProxy} proxy HandlerProxy instance.
 * @param {HTMLElement} painterRoot painter.root (not painter.getViewportRoot()).
 */
var Handler = function(storage, painter, proxy, painterRoot) {
    Eventful.call(this);

    this.storage = storage;

    this.painter = painter;

    this.painterRoot = painterRoot;

    proxy = proxy || new EmptyProxy();

    /**
     * Proxy of event. can be Dom, WebGLSurface, etc.
     */
    this.proxy = null;

    /**
     * {target, topTarget, x, y}
     * @private
     * @type {Object}
     */
    this._hovered = {};

    /**
     * @private
     * @type {Date}
     */
    this._lastTouchMoment;

    /**
     * @private
     * @type {number}
     */
    this._lastX;

    /**
     * @private
     * @type {number}
     */
    this._lastY;


    Draggable.call(this);

    this.setHandlerProxy(proxy);
};

Handler.prototype = {

    constructor: Handler,

    setHandlerProxy: function (proxy) {
        if (this.proxy) {
            this.proxy.dispose();
        }

        if (proxy) {
            each$1(handlerNames, function (name) {
                proxy.on && proxy.on(name, this[name], this);
            }, this);
            // Attach handler
            proxy.handler = this;
        }
        this.proxy = proxy;
    },

    mousemove: function (event) {
        var x = event.zrX;
        var y = event.zrY;

        var lastHovered = this._hovered;
        var lastHoveredTarget = lastHovered.target;

        // If lastHoveredTarget is removed from zr (detected by '__zr') by some API call
        // (like 'setOption' or 'dispatchAction') in event handlers, we should find
        // lastHovered again here. Otherwise 'mouseout' can not be triggered normally.
        // See #6198.
        if (lastHoveredTarget && !lastHoveredTarget.__zr) {
            lastHovered = this.findHover(lastHovered.x, lastHovered.y);
            lastHoveredTarget = lastHovered.target;
        }

        var hovered = this._hovered = this.findHover(x, y);
        var hoveredTarget = hovered.target;

        var proxy = this.proxy;
        proxy.setCursor && proxy.setCursor(hoveredTarget ? hoveredTarget.cursor : 'default');

        // Mouse out on previous hovered element
        if (lastHoveredTarget && hoveredTarget !== lastHoveredTarget) {
            this.dispatchToElement(lastHovered, 'mouseout', event);
        }

        // Mouse moving on one element
        this.dispatchToElement(hovered, 'mousemove', event);

        // Mouse over on a new element
        if (hoveredTarget && hoveredTarget !== lastHoveredTarget) {
            this.dispatchToElement(hovered, 'mouseover', event);
        }
    },

    mouseout: function (event) {
        this.dispatchToElement(this._hovered, 'mouseout', event);

        // There might be some doms created by upper layer application
        // at the same level of painter.getViewportRoot() (e.g., tooltip
        // dom created by echarts), where 'globalout' event should not
        // be triggered when mouse enters these doms. (But 'mouseout'
        // should be triggered at the original hovered element as usual).
        var element = event.toElement || event.relatedTarget;
        var innerDom;
        do {
            element = element && element.parentNode;
        }
        while (element && element.nodeType != 9 && !(
            innerDom = element === this.painterRoot
        ));

        !innerDom && this.trigger('globalout', {event: event});
    },

    /**
     * Resize
     */
    resize: function (event) {
        this._hovered = {};
    },

    /**
     * Dispatch event
     * @param {string} eventName
     * @param {event=} eventArgs
     */
    dispatch: function (eventName, eventArgs) {
        var handler = this[eventName];
        handler && handler.call(this, eventArgs);
    },

    /**
     * Dispose
     */
    dispose: function () {

        this.proxy.dispose();

        this.storage =
        this.proxy =
        this.painter = null;
    },

    /**
     * 设置默认的cursor style
     * @param {string} [cursorStyle='default'] 例如 crosshair
     */
    setCursorStyle: function (cursorStyle) {
        var proxy = this.proxy;
        proxy.setCursor && proxy.setCursor(cursorStyle);
    },

    /**
     * 事件分发代理
     *
     * @private
     * @param {Object} targetInfo {target, topTarget} 目标图形元素
     * @param {string} eventName 事件名称
     * @param {Object} event 事件对象
     */
    dispatchToElement: function (targetInfo, eventName, event) {
        targetInfo = targetInfo || {};
        var el = targetInfo.target;
        if (el && el.silent) {
            return;
        }
        var eventHandler = 'on' + eventName;
        var eventPacket = makeEventPacket(eventName, targetInfo, event);

        while (el) {
            el[eventHandler]
                && (eventPacket.cancelBubble = el[eventHandler].call(el, eventPacket));

            el.trigger(eventName, eventPacket);

            el = el.parent;

            if (eventPacket.cancelBubble) {
                break;
            }
        }

        if (!eventPacket.cancelBubble) {
            // 冒泡到顶级 zrender 对象
            this.trigger(eventName, eventPacket);
            // 分发事件到用户自定义层
            // 用户有可能在全局 click 事件中 dispose，所以需要判断下 painter 是否存在
            this.painter && this.painter.eachOtherLayer(function (layer) {
                if (typeof(layer[eventHandler]) == 'function') {
                    layer[eventHandler].call(layer, eventPacket);
                }
                if (layer.trigger) {
                    layer.trigger(eventName, eventPacket);
                }
            });
        }
    },

    /**
     * @private
     * @param {number} x
     * @param {number} y
     * @param {module:zrender/graphic/Displayable} exclude
     * @return {model:zrender/Element}
     * @method
     */
    findHover: function(x, y, exclude) {
        var list = this.storage.getDisplayList();
        var out = {x: x, y: y};

        for (var i = list.length - 1; i >= 0 ; i--) {
            var hoverCheckResult;
            if (list[i] !== exclude
                // getDisplayList may include ignored item in VML mode
                && !list[i].ignore
                && (hoverCheckResult = isHover(list[i], x, y))
            ) {
                !out.topTarget && (out.topTarget = list[i]);
                if (hoverCheckResult !== SILENT) {
                    out.target = list[i];
                    break;
                }
            }
        }

        return out;
    }
};

// Common handlers
each$1(['click', 'mousedown', 'mouseup', 'mousewheel', 'dblclick', 'contextmenu'], function (name) {
    Handler.prototype[name] = function (event) {
        // Find hover again to avoid click event is dispatched manually. Or click is triggered without mouseover
        var hovered = this.findHover(event.zrX, event.zrY);
        var hoveredTarget = hovered.target;

        if (name === 'mousedown') {
            this._downEl = hoveredTarget;
            this._downPoint = [event.zrX, event.zrY];
            // In case click triggered before mouseup
            this._upEl = hoveredTarget;
        }
        else if (name === 'mouseup') {
            this._upEl = hoveredTarget;
        }
        else if (name === 'click') {
            if (this._downEl !== this._upEl
                // Original click event is triggered on the whole canvas element,
                // including the case that `mousedown` - `mousemove` - `mouseup`,
                // which should be filtered, otherwise it will bring trouble to
                // pan and zoom.
                || !this._downPoint
                // Arbitrary value
                || dist(this._downPoint, [event.zrX, event.zrY]) > 4
            ) {
                return;
            }
            this._downPoint = null;
        }

        this.dispatchToElement(hovered, name, event);
    };
});

function isHover(displayable, x, y) {
    if (displayable[displayable.rectHover ? 'rectContain' : 'contain'](x, y)) {
        var el = displayable;
        var isSilent;
        while (el) {
            // If clipped by ancestor.
            // FIXME: If clipPath has neither stroke nor fill,
            // el.clipPath.contain(x, y) will always return false.
            if (el.clipPath && !el.clipPath.contain(x, y))  {
                return false;
            }
            if (el.silent) {
                isSilent = true;
            }
            el = el.parent;
        }
        return isSilent ? SILENT : true;
    }

    return false;
}

mixin(Handler, Eventful);
mixin(Handler, Draggable);

// https://github.com/mziccard/node-timsort
var DEFAULT_MIN_MERGE = 32;

var DEFAULT_MIN_GALLOPING = 7;

function minRunLength(n) {
    var r = 0;

    while (n >= DEFAULT_MIN_MERGE) {
        r |= n & 1;
        n >>= 1;
    }

    return n + r;
}

function makeAscendingRun(array, lo, hi, compare) {
    var runHi = lo + 1;

    if (runHi === hi) {
        return 1;
    }

    if (compare(array[runHi++], array[lo]) < 0) {
        while (runHi < hi && compare(array[runHi], array[runHi - 1]) < 0) {
            runHi++;
        }

        reverseRun(array, lo, runHi);
    }
    else {
        while (runHi < hi && compare(array[runHi], array[runHi - 1]) >= 0) {
            runHi++;
        }
    }

    return runHi - lo;
}

function reverseRun(array, lo, hi) {
    hi--;

    while (lo < hi) {
        var t = array[lo];
        array[lo++] = array[hi];
        array[hi--] = t;
    }
}

function binaryInsertionSort(array, lo, hi, start, compare) {
    if (start === lo) {
        start++;
    }

    for (; start < hi; start++) {
        var pivot = array[start];

        var left = lo;
        var right = start;
        var mid;

        while (left < right) {
            mid = left + right >>> 1;

            if (compare(pivot, array[mid]) < 0) {
                right = mid;
            }
            else {
                left = mid + 1;
            }
        }

        var n = start - left;

        switch (n) {
            case 3:
                array[left + 3] = array[left + 2];

            case 2:
                array[left + 2] = array[left + 1];

            case 1:
                array[left + 1] = array[left];
                break;
            default:
                while (n > 0) {
                    array[left + n] = array[left + n - 1];
                    n--;
                }
        }

        array[left] = pivot;
    }
}

function gallopLeft(value, array, start, length, hint, compare) {
    var lastOffset = 0;
    var maxOffset = 0;
    var offset = 1;

    if (compare(value, array[start + hint]) > 0) {
        maxOffset = length - hint;

        while (offset < maxOffset && compare(value, array[start + hint + offset]) > 0) {
            lastOffset = offset;
            offset = (offset << 1) + 1;

            if (offset <= 0) {
                offset = maxOffset;
            }
        }

        if (offset > maxOffset) {
            offset = maxOffset;
        }

        lastOffset += hint;
        offset += hint;
    }
    else {
        maxOffset = hint + 1;
        while (offset < maxOffset && compare(value, array[start + hint - offset]) <= 0) {
            lastOffset = offset;
            offset = (offset << 1) + 1;

            if (offset <= 0) {
                offset = maxOffset;
            }
        }
        if (offset > maxOffset) {
            offset = maxOffset;
        }

        var tmp = lastOffset;
        lastOffset = hint - offset;
        offset = hint - tmp;
    }

    lastOffset++;
    while (lastOffset < offset) {
        var m = lastOffset + (offset - lastOffset >>> 1);

        if (compare(value, array[start + m]) > 0) {
            lastOffset = m + 1;
        }
        else {
            offset = m;
        }
    }
    return offset;
}

function gallopRight(value, array, start, length, hint, compare) {
    var lastOffset = 0;
    var maxOffset = 0;
    var offset = 1;

    if (compare(value, array[start + hint]) < 0) {
        maxOffset = hint + 1;

        while (offset < maxOffset && compare(value, array[start + hint - offset]) < 0) {
            lastOffset = offset;
            offset = (offset << 1) + 1;

            if (offset <= 0) {
                offset = maxOffset;
            }
        }

        if (offset > maxOffset) {
            offset = maxOffset;
        }

        var tmp = lastOffset;
        lastOffset = hint - offset;
        offset = hint - tmp;
    }
    else {
        maxOffset = length - hint;

        while (offset < maxOffset && compare(value, array[start + hint + offset]) >= 0) {
            lastOffset = offset;
            offset = (offset << 1) + 1;

            if (offset <= 0) {
                offset = maxOffset;
            }
        }

        if (offset > maxOffset) {
            offset = maxOffset;
        }

        lastOffset += hint;
        offset += hint;
    }

    lastOffset++;

    while (lastOffset < offset) {
        var m = lastOffset + (offset - lastOffset >>> 1);

        if (compare(value, array[start + m]) < 0) {
            offset = m;
        }
        else {
            lastOffset = m + 1;
        }
    }

    return offset;
}

function TimSort(array, compare) {
    var minGallop = DEFAULT_MIN_GALLOPING;
    var length = 0;
    var runStart;
    var runLength;
    var stackSize = 0;

    length = array.length;

    var tmp = [];

    runStart = [];
    runLength = [];

    function pushRun(_runStart, _runLength) {
        runStart[stackSize] = _runStart;
        runLength[stackSize] = _runLength;
        stackSize += 1;
    }

    function mergeRuns() {
        while (stackSize > 1) {
            var n = stackSize - 2;

            if (n >= 1 && runLength[n - 1] <= runLength[n] + runLength[n + 1] || n >= 2 && runLength[n - 2] <= runLength[n] + runLength[n - 1]) {
                if (runLength[n - 1] < runLength[n + 1]) {
                    n--;
                }
            }
            else if (runLength[n] > runLength[n + 1]) {
                break;
            }
            mergeAt(n);
        }
    }

    function forceMergeRuns() {
        while (stackSize > 1) {
            var n = stackSize - 2;

            if (n > 0 && runLength[n - 1] < runLength[n + 1]) {
                n--;
            }

            mergeAt(n);
        }
    }

    function mergeAt(i) {
        var start1 = runStart[i];
        var length1 = runLength[i];
        var start2 = runStart[i + 1];
        var length2 = runLength[i + 1];

        runLength[i] = length1 + length2;

        if (i === stackSize - 3) {
            runStart[i + 1] = runStart[i + 2];
            runLength[i + 1] = runLength[i + 2];
        }

        stackSize--;

        var k = gallopRight(array[start2], array, start1, length1, 0, compare);
        start1 += k;
        length1 -= k;

        if (length1 === 0) {
            return;
        }

        length2 = gallopLeft(array[start1 + length1 - 1], array, start2, length2, length2 - 1, compare);

        if (length2 === 0) {
            return;
        }

        if (length1 <= length2) {
            mergeLow(start1, length1, start2, length2);
        }
        else {
            mergeHigh(start1, length1, start2, length2);
        }
    }

    function mergeLow(start1, length1, start2, length2) {
        var i = 0;

        for (i = 0; i < length1; i++) {
            tmp[i] = array[start1 + i];
        }

        var cursor1 = 0;
        var cursor2 = start2;
        var dest = start1;

        array[dest++] = array[cursor2++];

        if (--length2 === 0) {
            for (i = 0; i < length1; i++) {
                array[dest + i] = tmp[cursor1 + i];
            }
            return;
        }

        if (length1 === 1) {
            for (i = 0; i < length2; i++) {
                array[dest + i] = array[cursor2 + i];
            }
            array[dest + length2] = tmp[cursor1];
            return;
        }

        var _minGallop = minGallop;
        var count1, count2, exit;

        while (1) {
            count1 = 0;
            count2 = 0;
            exit = false;

            do {
                if (compare(array[cursor2], tmp[cursor1]) < 0) {
                    array[dest++] = array[cursor2++];
                    count2++;
                    count1 = 0;

                    if (--length2 === 0) {
                        exit = true;
                        break;
                    }
                }
                else {
                    array[dest++] = tmp[cursor1++];
                    count1++;
                    count2 = 0;
                    if (--length1 === 1) {
                        exit = true;
                        break;
                    }
                }
            } while ((count1 | count2) < _minGallop);

            if (exit) {
                break;
            }

            do {
                count1 = gallopRight(array[cursor2], tmp, cursor1, length1, 0, compare);

                if (count1 !== 0) {
                    for (i = 0; i < count1; i++) {
                        array[dest + i] = tmp[cursor1 + i];
                    }

                    dest += count1;
                    cursor1 += count1;
                    length1 -= count1;
                    if (length1 <= 1) {
                        exit = true;
                        break;
                    }
                }

                array[dest++] = array[cursor2++];

                if (--length2 === 0) {
                    exit = true;
                    break;
                }

                count2 = gallopLeft(tmp[cursor1], array, cursor2, length2, 0, compare);

                if (count2 !== 0) {
                    for (i = 0; i < count2; i++) {
                        array[dest + i] = array[cursor2 + i];
                    }

                    dest += count2;
                    cursor2 += count2;
                    length2 -= count2;

                    if (length2 === 0) {
                        exit = true;
                        break;
                    }
                }
                array[dest++] = tmp[cursor1++];

                if (--length1 === 1) {
                    exit = true;
                    break;
                }

                _minGallop--;
            } while (count1 >= DEFAULT_MIN_GALLOPING || count2 >= DEFAULT_MIN_GALLOPING);

            if (exit) {
                break;
            }

            if (_minGallop < 0) {
                _minGallop = 0;
            }

            _minGallop += 2;
        }

        minGallop = _minGallop;

        minGallop < 1 && (minGallop = 1);

        if (length1 === 1) {
            for (i = 0; i < length2; i++) {
                array[dest + i] = array[cursor2 + i];
            }
            array[dest + length2] = tmp[cursor1];
        }
        else if (length1 === 0) {
            throw new Error();
            // throw new Error('mergeLow preconditions were not respected');
        }
        else {
            for (i = 0; i < length1; i++) {
                array[dest + i] = tmp[cursor1 + i];
            }
        }
    }

    function mergeHigh (start1, length1, start2, length2) {
        var i = 0;

        for (i = 0; i < length2; i++) {
            tmp[i] = array[start2 + i];
        }

        var cursor1 = start1 + length1 - 1;
        var cursor2 = length2 - 1;
        var dest = start2 + length2 - 1;
        var customCursor = 0;
        var customDest = 0;

        array[dest--] = array[cursor1--];

        if (--length1 === 0) {
            customCursor = dest - (length2 - 1);

            for (i = 0; i < length2; i++) {
                array[customCursor + i] = tmp[i];
            }

            return;
        }

        if (length2 === 1) {
            dest -= length1;
            cursor1 -= length1;
            customDest = dest + 1;
            customCursor = cursor1 + 1;

            for (i = length1 - 1; i >= 0; i--) {
                array[customDest + i] = array[customCursor + i];
            }

            array[dest] = tmp[cursor2];
            return;
        }

        var _minGallop = minGallop;

        while (true) {
            var count1 = 0;
            var count2 = 0;
            var exit = false;

            do {
                if (compare(tmp[cursor2], array[cursor1]) < 0) {
                    array[dest--] = array[cursor1--];
                    count1++;
                    count2 = 0;
                    if (--length1 === 0) {
                        exit = true;
                        break;
                    }
                }
                else {
                    array[dest--] = tmp[cursor2--];
                    count2++;
                    count1 = 0;
                    if (--length2 === 1) {
                        exit = true;
                        break;
                    }
                }
            } while ((count1 | count2) < _minGallop);

            if (exit) {
                break;
            }

            do {
                count1 = length1 - gallopRight(tmp[cursor2], array, start1, length1, length1 - 1, compare);

                if (count1 !== 0) {
                    dest -= count1;
                    cursor1 -= count1;
                    length1 -= count1;
                    customDest = dest + 1;
                    customCursor = cursor1 + 1;

                    for (i = count1 - 1; i >= 0; i--) {
                        array[customDest + i] = array[customCursor + i];
                    }

                    if (length1 === 0) {
                        exit = true;
                        break;
                    }
                }

                array[dest--] = tmp[cursor2--];

                if (--length2 === 1) {
                    exit = true;
                    break;
                }

                count2 = length2 - gallopLeft(array[cursor1], tmp, 0, length2, length2 - 1, compare);

                if (count2 !== 0) {
                    dest -= count2;
                    cursor2 -= count2;
                    length2 -= count2;
                    customDest = dest + 1;
                    customCursor = cursor2 + 1;

                    for (i = 0; i < count2; i++) {
                        array[customDest + i] = tmp[customCursor + i];
                    }

                    if (length2 <= 1) {
                        exit = true;
                        break;
                    }
                }

                array[dest--] = array[cursor1--];

                if (--length1 === 0) {
                    exit = true;
                    break;
                }

                _minGallop--;
            } while (count1 >= DEFAULT_MIN_GALLOPING || count2 >= DEFAULT_MIN_GALLOPING);

            if (exit) {
                break;
            }

            if (_minGallop < 0) {
                _minGallop = 0;
            }

            _minGallop += 2;
        }

        minGallop = _minGallop;

        if (minGallop < 1) {
            minGallop = 1;
        }

        if (length2 === 1) {
            dest -= length1;
            cursor1 -= length1;
            customDest = dest + 1;
            customCursor = cursor1 + 1;

            for (i = length1 - 1; i >= 0; i--) {
                array[customDest + i] = array[customCursor + i];
            }

            array[dest] = tmp[cursor2];
        }
        else if (length2 === 0) {
            throw new Error();
            // throw new Error('mergeHigh preconditions were not respected');
        }
        else {
            customCursor = dest - (length2 - 1);
            for (i = 0; i < length2; i++) {
                array[customCursor + i] = tmp[i];
            }
        }
    }

    this.mergeRuns = mergeRuns;
    this.forceMergeRuns = forceMergeRuns;
    this.pushRun = pushRun;
}

function sort(array, compare, lo, hi) {
    if (!lo) {
        lo = 0;
    }
    if (!hi) {
        hi = array.length;
    }

    var remaining = hi - lo;

    if (remaining < 2) {
        return;
    }

    var runLength = 0;

    if (remaining < DEFAULT_MIN_MERGE) {
        runLength = makeAscendingRun(array, lo, hi, compare);
        binaryInsertionSort(array, lo, hi, lo + runLength, compare);
        return;
    }

    var ts = new TimSort(array, compare);

    var minRun = minRunLength(remaining);

    do {
        runLength = makeAscendingRun(array, lo, hi, compare);
        if (runLength < minRun) {
            var force = remaining;
            if (force > minRun) {
                force = minRun;
            }

            binaryInsertionSort(array, lo, lo + force, lo + runLength, compare);
            runLength = force;
        }

        ts.pushRun(lo, runLength);
        ts.mergeRuns();

        remaining -= runLength;
        lo += runLength;
    } while (remaining !== 0);

    ts.forceMergeRuns();
}

/**
 * Storage内容仓库模块
 * @module zrender/Storage
 * @author Kener (@Kener-林峰, kener.linfeng@gmail.com)
 * @author errorrik (errorrik@gmail.com)
 * @author pissang (https://github.com/pissang/)
 */

function shapeCompareFunc(a, b) {
    if (a.zlevel === b.zlevel) {
        if (a.z === b.z) {
            // if (a.z2 === b.z2) {
            //     // FIXME Slow has renderidx compare
            //     // http://stackoverflow.com/questions/20883421/sorting-in-javascript-should-every-compare-function-have-a-return-0-statement
            //     // https://github.com/v8/v8/blob/47cce544a31ed5577ffe2963f67acb4144ee0232/src/js/array.js#L1012
            //     return a.__renderidx - b.__renderidx;
            // }
            return a.z2 - b.z2;
        }
        return a.z - b.z;
    }
    return a.zlevel - b.zlevel;
}
/**
 * 内容仓库 (M)
 * @alias module:zrender/Storage
 * @constructor
 */
var Storage = function () { // jshint ignore:line
    this._roots = [];

    this._displayList = [];

    this._displayListLen = 0;
};

Storage.prototype = {

    constructor: Storage,

    /**
     * @param  {Function} cb
     *
     */
    traverse: function (cb, context) {
        var this$1 = this;

        for (var i = 0; i < this._roots.length; i++) {
            this$1._roots[i].traverse(cb, context);
        }
    },

    /**
     * 返回所有图形的绘制队列
     * @param {boolean} [update=false] 是否在返回前更新该数组
     * @param {boolean} [includeIgnore=false] 是否包含 ignore 的数组, 在 update 为 true 的时候有效
     *
     * 详见{@link module:zrender/graphic/Displayable.prototype.updateDisplayList}
     * @return {Array.<module:zrender/graphic/Displayable>}
     */
    getDisplayList: function (update, includeIgnore) {
        includeIgnore = includeIgnore || false;
        if (update) {
            this.updateDisplayList(includeIgnore);
        }
        return this._displayList;
    },

    /**
     * 更新图形的绘制队列。
     * 每次绘制前都会调用，该方法会先深度优先遍历整个树，更新所有Group和Shape的变换并且把所有可见的Shape保存到数组中，
     * 最后根据绘制的优先级（zlevel > z > 插入顺序）排序得到绘制队列
     * @param {boolean} [includeIgnore=false] 是否包含 ignore 的数组
     */
    updateDisplayList: function (includeIgnore) {
        var this$1 = this;

        this._displayListLen = 0;
        var roots = this._roots;
        var displayList = this._displayList;
        for (var i = 0, len = roots.length; i < len; i++) {
            this$1._updateAndAddDisplayable(roots[i], null, includeIgnore);
        }
        displayList.length = this._displayListLen;

        // for (var i = 0, len = displayList.length; i < len; i++) {
        //     displayList[i].__renderidx = i;
        // }

        // displayList.sort(shapeCompareFunc);
        env$1.canvasSupported && sort(displayList, shapeCompareFunc);
    },

    _updateAndAddDisplayable: function (el, clipPaths, includeIgnore) {
        var this$1 = this;


        if (el.ignore && !includeIgnore) {
            return;
        }

        el.beforeUpdate();

        if (el.__dirty) {

            el.update();

        }

        el.afterUpdate();

        var userSetClipPath = el.clipPath;
        if (userSetClipPath) {

            // FIXME 效率影响
            if (clipPaths) {
                clipPaths = clipPaths.slice();
            }
            else {
                clipPaths = [];
            }

            var currentClipPath = userSetClipPath;
            var parentClipPath = el;
            // Recursively add clip path
            while (currentClipPath) {
                // clipPath 的变换是基于使用这个 clipPath 的元素
                currentClipPath.parent = parentClipPath;
                currentClipPath.updateTransform();

                clipPaths.push(currentClipPath);

                parentClipPath = currentClipPath;
                currentClipPath = currentClipPath.clipPath;
            }
        }

        if (el.isGroup) {
            var children = el._children;

            for (var i = 0; i < children.length; i++) {
                var child = children[i];

                // Force to mark as dirty if group is dirty
                // FIXME __dirtyPath ?
                if (el.__dirty) {
                    child.__dirty = true;
                }

                this$1._updateAndAddDisplayable(child, clipPaths, includeIgnore);
            }

            // Mark group clean here
            el.__dirty = false;

        }
        else {
            el.__clipPaths = clipPaths;

            this._displayList[this._displayListLen++] = el;
        }
    },

    /**
     * 添加图形(Shape)或者组(Group)到根节点
     * @param {module:zrender/Element} el
     */
    addRoot: function (el) {
        if (el.__storage === this) {
            return;
        }

        if (el instanceof Group) {
            el.addChildrenToStorage(this);
        }

        this.addToStorage(el);
        this._roots.push(el);
    },

    /**
     * 删除指定的图形(Shape)或者组(Group)
     * @param {string|Array.<string>} [el] 如果为空清空整个Storage
     */
    delRoot: function (el) {
        var this$1 = this;

        if (el == null) {
            // 不指定el清空
            for (var i = 0; i < this._roots.length; i++) {
                var root = this$1._roots[i];
                if (root instanceof Group) {
                    root.delChildrenFromStorage(this$1);
                }
            }

            this._roots = [];
            this._displayList = [];
            this._displayListLen = 0;

            return;
        }

        if (el instanceof Array) {
            for (var i = 0, l = el.length; i < l; i++) {
                this$1.delRoot(el[i]);
            }
            return;
        }


        var idx = indexOf$1(this._roots, el);
        if (idx >= 0) {
            this.delFromStorage(el);
            this._roots.splice(idx, 1);
            if (el instanceof Group) {
                el.delChildrenFromStorage(this);
            }
        }
    },

    addToStorage: function (el) {
        el.__storage = this;
        el.dirty(false);

        return this;
    },

    delFromStorage: function (el) {
        if (el) {
            el.__storage = null;
        }

        return this;
    },

    /**
     * 清空并且释放Storage
     */
    dispose: function () {
        this._renderList =
        this._roots = null;
    },

    displayableSortFunc: shapeCompareFunc
};

/**
 * @module zrender/Layer
 * @author pissang(https://www.github.com/pissang)
 */

function returnFalse() {
    return false;
}

/**
 * 创建dom
 *
 * @inner
 * @param {string} id dom id 待用
 * @param {Painter} painter painter instance
 * @param {number} number
 */
function createDom(id, painter, dpr) {
    var newDom = createCanvas();
    var width = painter.getWidth();
    var height = painter.getHeight();

    var newDomStyle = newDom.style;
    // 没append呢，请原谅我这样写，清晰~
    newDomStyle.position = 'absolute';
    newDomStyle.left = 0;
    newDomStyle.top = 0;
    newDomStyle.width = width + 'px';
    newDomStyle.height = height + 'px';
    newDom.width = width * dpr;
    newDom.height = height * dpr;

    // id不作为索引用，避免可能造成的重名，定义为私有属性
    newDom.setAttribute('data-zr-dom-id', id);
    return newDom;
}

/**
 * @alias module:zrender/Layer
 * @constructor
 * @extends module:zrender/mixin/Transformable
 * @param {string} id
 * @param {module:zrender/Painter} painter
 * @param {number} [dpr]
 */
var Layer = function(id, painter, dpr) {
    var dom;
    dpr = dpr || devicePixelRatio;
    if (typeof id === 'string') {
        dom = createDom(id, painter, dpr);
    }
    // Not using isDom because in node it will return false
    else if (isObject$1(id)) {
        dom = id;
        id = dom.id;
    }
    this.id = id;
    this.dom = dom;

    var domStyle = dom.style;
    if (domStyle) { // Not in node
        dom.onselectstart = returnFalse; // 避免页面选中的尴尬
        domStyle['-webkit-user-select'] = 'none';
        domStyle['user-select'] = 'none';
        domStyle['-webkit-touch-callout'] = 'none';
        domStyle['-webkit-tap-highlight-color'] = 'rgba(0,0,0,0)';
        domStyle['padding'] = 0;
        domStyle['margin'] = 0;
        domStyle['border-width'] = 0;
    }

    this.domBack = null;
    this.ctxBack = null;

    this.painter = painter;

    this.config = null;

    // Configs
    /**
     * 每次清空画布的颜色
     * @type {string}
     * @default 0
     */
    this.clearColor = 0;
    /**
     * 是否开启动态模糊
     * @type {boolean}
     * @default false
     */
    this.motionBlur = false;
    /**
     * 在开启动态模糊的时候使用，与上一帧混合的alpha值，值越大尾迹越明显
     * @type {number}
     * @default 0.7
     */
    this.lastFrameAlpha = 0.7;

    /**
     * Layer dpr
     * @type {number}
     */
    this.dpr = dpr;
};

Layer.prototype = {

    constructor: Layer,

    elCount: 0,

    __dirty: true,

    initContext: function () {
        this.ctx = this.dom.getContext('2d');
        this.ctx.__currentValues = {};
        this.ctx.dpr = this.dpr;
    },

    createBackBuffer: function () {
        var dpr = this.dpr;

        this.domBack = createDom('back-' + this.id, this.painter, dpr);
        this.ctxBack = this.domBack.getContext('2d');
        this.ctxBack.__currentValues = {};

        if (dpr != 1) {
            this.ctxBack.scale(dpr, dpr);
        }
    },

    /**
     * @param  {number} width
     * @param  {number} height
     */
    resize: function (width, height) {
        var dpr = this.dpr;

        var dom = this.dom;
        var domStyle = dom.style;
        var domBack = this.domBack;

        domStyle.width = width + 'px';
        domStyle.height = height + 'px';

        dom.width = width * dpr;
        dom.height = height * dpr;

        if (domBack) {
            domBack.width = width * dpr;
            domBack.height = height * dpr;

            if (dpr != 1) {
                this.ctxBack.scale(dpr, dpr);
            }
        }
    },

    /**
     * 清空该层画布
     * @param {boolean} clearAll Clear all with out motion blur
     */
    clear: function (clearAll) {
        var dom = this.dom;
        var ctx = this.ctx;
        var width = dom.width;
        var height = dom.height;

        var clearColor = this.clearColor;
        var haveMotionBLur = this.motionBlur && !clearAll;
        var lastFrameAlpha = this.lastFrameAlpha;

        var dpr = this.dpr;

        if (haveMotionBLur) {
            if (!this.domBack) {
                this.createBackBuffer();
            }

            this.ctxBack.globalCompositeOperation = 'copy';
            this.ctxBack.drawImage(
                dom, 0, 0,
                width / dpr,
                height / dpr
            );
        }

        ctx.clearRect(0, 0, width, height);
        if (clearColor) {
            var clearColorGradientOrPattern;
            // Gradient
            if (clearColor.colorStops) {
                // Cache canvas gradient
                clearColorGradientOrPattern = clearColor.__canvasGradient || Style.getGradient(ctx, clearColor, {
                    x: 0,
                    y: 0,
                    width: width,
                    height: height
                });

                clearColor.__canvasGradient = clearColorGradientOrPattern;
            }
            // Pattern
            else if (clearColor.image) {
                clearColorGradientOrPattern = Pattern.prototype.getCanvasPattern.call(clearColor, ctx);
            }
            ctx.save();
            ctx.fillStyle = clearColorGradientOrPattern || clearColor;
            ctx.fillRect(0, 0, width, height);
            ctx.restore();
        }

        if (haveMotionBLur) {
            var domBack = this.domBack;
            ctx.save();
            ctx.globalAlpha = lastFrameAlpha;
            ctx.drawImage(domBack, 0, 0, width, height);
            ctx.restore();
        }
    }
};

var requestAnimationFrame = (
    typeof window !== 'undefined'
    && (
        (window.requestAnimationFrame && window.requestAnimationFrame.bind(window))
        // https://github.com/ecomfe/zrender/issues/189#issuecomment-224919809
        || (window.msRequestAnimationFrame && window.msRequestAnimationFrame.bind(window))
        || window.mozRequestAnimationFrame
        || window.webkitRequestAnimationFrame
    )
) || function (func) {
    setTimeout(func, 16);
};

/**
 * Default canvas painter
 * @module zrender/Painter
 * @author Kener (@Kener-林峰, kener.linfeng@gmail.com)
 *         errorrik (errorrik@gmail.com)
 *         pissang (https://www.github.com/pissang)
 */

// PENDIGN
// Layer exceeds MAX_PROGRESSIVE_LAYER_NUMBER may have some problem when flush directly second time.
//
// Maximum progressive layer. When exceeding this number. All elements will be drawed in the last layer.
var MAX_PROGRESSIVE_LAYER_NUMBER = 5;

function parseInt10(val) {
    return parseInt(val, 10);
}

function isLayerValid(layer) {
    if (!layer) {
        return false;
    }

    if (layer.__builtin__) {
        return true;
    }

    if (typeof(layer.resize) !== 'function'
        || typeof(layer.refresh) !== 'function'
    ) {
        return false;
    }

    return true;
}

function preProcessLayer(layer) {
    layer.__unusedCount++;
}

function postProcessLayer(layer) {
    if (layer.__unusedCount == 1) {
        layer.clear();
    }
}

var tmpRect$1 = new BoundingRect(0, 0, 0, 0);
var viewRect = new BoundingRect(0, 0, 0, 0);
function isDisplayableCulled(el, width, height) {
    tmpRect$1.copy(el.getBoundingRect());
    if (el.transform) {
        tmpRect$1.applyTransform(el.transform);
    }
    viewRect.width = width;
    viewRect.height = height;
    return !tmpRect$1.intersect(viewRect);
}

function isClipPathChanged(clipPaths, prevClipPaths) {
    if (clipPaths == prevClipPaths) { // Can both be null or undefined
        return false;
    }

    if (!clipPaths || !prevClipPaths || (clipPaths.length !== prevClipPaths.length)) {
        return true;
    }
    for (var i = 0; i < clipPaths.length; i++) {
        if (clipPaths[i] !== prevClipPaths[i]) {
            return true;
        }
    }
}

function doClip(clipPaths, ctx) {
    for (var i = 0; i < clipPaths.length; i++) {
        var clipPath = clipPaths[i];

        clipPath.setTransform(ctx);
        ctx.beginPath();
        clipPath.buildPath(ctx, clipPath.shape);
        ctx.clip();
        // Transform back
        clipPath.restoreTransform(ctx);
    }
}

function createRoot(width, height) {
    var domRoot = document.createElement('div');

    // domRoot.onselectstart = returnFalse; // 避免页面选中的尴尬
    domRoot.style.cssText = [
        'position:relative',
        'overflow:hidden',
        'width:' + width + 'px',
        'height:' + height + 'px',
        'padding:0',
        'margin:0',
        'border-width:0'
    ].join(';') + ';';

    return domRoot;
}

/**
 * @alias module:zrender/Painter
 * @constructor
 * @param {HTMLElement} root 绘图容器
 * @param {module:zrender/Storage} storage
 * @param {Object} opts
 */
var Painter = function (root, storage, opts) {

    this.type = 'canvas';

    // In node environment using node-canvas
    var singleCanvas = !root.nodeName // In node ?
        || root.nodeName.toUpperCase() === 'CANVAS';

    this._opts = opts = extend$1({}, opts || {});

    /**
     * @type {number}
     */
    this.dpr = opts.devicePixelRatio || devicePixelRatio;
    /**
     * @type {boolean}
     * @private
     */
    this._singleCanvas = singleCanvas;
    /**
     * 绘图容器
     * @type {HTMLElement}
     */
    this.root = root;

    var rootStyle = root.style;

    if (rootStyle) {
        rootStyle['-webkit-tap-highlight-color'] = 'transparent';
        rootStyle['-webkit-user-select'] =
        rootStyle['user-select'] =
        rootStyle['-webkit-touch-callout'] = 'none';

        root.innerHTML = '';
    }

    /**
     * @type {module:zrender/Storage}
     */
    this.storage = storage;

    /**
     * @type {Array.<number>}
     * @private
     */
    var zlevelList = this._zlevelList = [];

    /**
     * @type {Object.<string, module:zrender/Layer>}
     * @private
     */
    var layers = this._layers = {};

    /**
     * @type {Object.<string, Object>}
     * @private
     */
    this._layerConfig = {};

    if (!singleCanvas) {
        this._width = this._getSize(0);
        this._height = this._getSize(1);

        var domRoot = this._domRoot = createRoot(
            this._width, this._height
        );
        root.appendChild(domRoot);
    }
    else {
        if (opts.width != null) {
            root.width = opts.width;
        }
        if (opts.height != null) {
            root.height = opts.height;
        }
        // Use canvas width and height directly
        var width = root.width;
        var height = root.height;
        this._width = width;
        this._height = height;

        // Create layer if only one given canvas
        // Device pixel ratio is fixed to 1 because given canvas has its specified width and height
        var mainLayer = new Layer(root, this, 1);
        mainLayer.initContext();
        // FIXME Use canvas width and height
        // mainLayer.resize(width, height);
        layers[0] = mainLayer;
        zlevelList.push(0);

        this._domRoot = root;
    }

    // Layers for progressive rendering
    this._progressiveLayers = [];

    /**
     * @type {module:zrender/Layer}
     * @private
     */
    this._hoverlayer;

    this._hoverElements = [];
};

Painter.prototype = {

    constructor: Painter,

    getType: function () {
        return 'canvas';
    },

    /**
     * If painter use a single canvas
     * @return {boolean}
     */
    isSingleCanvas: function () {
        return this._singleCanvas;
    },
    /**
     * @return {HTMLDivElement}
     */
    getViewportRoot: function () {
        return this._domRoot;
    },

    getViewportRootOffset: function () {
        var viewportRoot = this.getViewportRoot();
        if (viewportRoot) {
            return {
                offsetLeft: viewportRoot.offsetLeft || 0,
                offsetTop: viewportRoot.offsetTop || 0
            };
        }
    },

    /**
     * 刷新
     * @param {boolean} [paintAll=false] 强制绘制所有displayable
     */
    refresh: function (paintAll) {
        var this$1 = this;


        var list = this.storage.getDisplayList(true);

        var zlevelList = this._zlevelList;

        this._paintList(list, paintAll);

        // Paint custum layers
        for (var i = 0; i < zlevelList.length; i++) {
            var z = zlevelList[i];
            var layer = this$1._layers[z];
            if (!layer.__builtin__ && layer.refresh) {
                layer.refresh();
            }
        }

        this.refreshHover();

        if (this._progressiveLayers.length) {
            this._startProgessive();
        }

        return this;
    },

    addHover: function (el, hoverStyle) {
        if (el.__hoverMir) {
            return;
        }
        var elMirror = new el.constructor({
            style: el.style,
            shape: el.shape
        });
        elMirror.__from = el;
        el.__hoverMir = elMirror;
        elMirror.setStyle(hoverStyle);
        this._hoverElements.push(elMirror);
    },

    removeHover: function (el) {
        var elMirror = el.__hoverMir;
        var hoverElements = this._hoverElements;
        var idx = indexOf$1(hoverElements, elMirror);
        if (idx >= 0) {
            hoverElements.splice(idx, 1);
        }
        el.__hoverMir = null;
    },

    clearHover: function (el) {
        var hoverElements = this._hoverElements;
        for (var i = 0; i < hoverElements.length; i++) {
            var from = hoverElements[i].__from;
            if (from) {
                from.__hoverMir = null;
            }
        }
        hoverElements.length = 0;
    },

    refreshHover: function () {
        var this$1 = this;

        var hoverElements = this._hoverElements;
        var len = hoverElements.length;
        var hoverLayer = this._hoverlayer;
        hoverLayer && hoverLayer.clear();

        if (!len) {
            return;
        }
        sort(hoverElements, this.storage.displayableSortFunc);

        // Use a extream large zlevel
        // FIXME?
        if (!hoverLayer) {
            hoverLayer = this._hoverlayer = this.getLayer(1e5);
        }

        var scope = {};
        hoverLayer.ctx.save();
        for (var i = 0; i < len;) {
            var el = hoverElements[i];
            var originalEl = el.__from;
            // Original el is removed
            // PENDING
            if (!(originalEl && originalEl.__zr)) {
                hoverElements.splice(i, 1);
                originalEl.__hoverMir = null;
                len--;
                continue;
            }
            i++;

            // Use transform
            // FIXME style and shape ?
            if (!originalEl.invisible) {
                el.transform = originalEl.transform;
                el.invTransform = originalEl.invTransform;
                el.__clipPaths = originalEl.__clipPaths;
                // el.
                this$1._doPaintEl(el, hoverLayer, true, scope);
            }
        }
        hoverLayer.ctx.restore();
    },

    _startProgessive: function () {
        var self = this;

        if (!self._furtherProgressive) {
            return;
        }

        // Use a token to stop progress steps triggered by
        // previous zr.refresh calling.
        var token = self._progressiveToken = +new Date();

        self._progress++;
        requestAnimationFrame(step);

        function step() {
            // In case refreshed or disposed
            if (token === self._progressiveToken && self.storage) {

                self._doPaintList(self.storage.getDisplayList());

                if (self._furtherProgressive) {
                    self._progress++;
                    requestAnimationFrame(step);
                }
                else {
                    self._progressiveToken = -1;
                }
            }
        }
    },

    _clearProgressive: function () {
        this._progressiveToken = -1;
        this._progress = 0;
        each$1(this._progressiveLayers, function (layer) {
            layer.__dirty && layer.clear();
        });
    },

    _paintList: function (list, paintAll) {

        if (paintAll == null) {
            paintAll = false;
        }

        this._updateLayerStatus(list);

        this._clearProgressive();

        this.eachBuiltinLayer(preProcessLayer);

        this._doPaintList(list, paintAll);

        this.eachBuiltinLayer(postProcessLayer);
    },

    _doPaintList: function (list, paintAll) {
        var this$1 = this;

        var currentLayer;
        var currentZLevel;
        var ctx;

        // var invTransform = [];
        var scope;

        var progressiveLayerIdx = 0;
        var currentProgressiveLayer;

        var width = this._width;
        var height = this._height;
        var layerProgress;
        var frame = this._progress;
        function flushProgressiveLayer(layer) {
            var dpr = ctx.dpr || 1;
            ctx.save();
            ctx.globalAlpha = 1;
            ctx.shadowBlur = 0;
            // Avoid layer don't clear in next progressive frame
            currentLayer.__dirty = true;
            ctx.setTransform(1, 0, 0, 1, 0, 0);
            ctx.drawImage(layer.dom, 0, 0, width * dpr, height * dpr);
            ctx.restore();
        }

        for (var i = 0, l = list.length; i < l; i++) {
            var el = list[i];
            var elZLevel = this$1._singleCanvas ? 0 : el.zlevel;

            var elFrame = el.__frame;

            // Flush at current context
            // PENDING
            if (elFrame < 0 && currentProgressiveLayer) {
                flushProgressiveLayer(currentProgressiveLayer);
                currentProgressiveLayer = null;
            }

            // Change draw layer
            if (currentZLevel !== elZLevel) {
                if (ctx) {
                    ctx.restore();
                }

                // Reset scope
                scope = {};

                // Only 0 zlevel if only has one canvas
                currentZLevel = elZLevel;
                currentLayer = this$1.getLayer(currentZLevel);

                if (!currentLayer.__builtin__) {
                    log$1(
                        'ZLevel ' + currentZLevel
                        + ' has been used by unkown layer ' + currentLayer.id
                    );
                }

                ctx = currentLayer.ctx;
                ctx.save();

                // Reset the count
                currentLayer.__unusedCount = 0;

                if (currentLayer.__dirty || paintAll) {
                    currentLayer.clear();
                }
            }

            if (!(currentLayer.__dirty || paintAll)) {
                continue;
            }

            if (elFrame >= 0) {
                // Progressive layer changed
                if (!currentProgressiveLayer) {
                    currentProgressiveLayer = this$1._progressiveLayers[
                        Math.min(progressiveLayerIdx++, MAX_PROGRESSIVE_LAYER_NUMBER - 1)
                    ];

                    currentProgressiveLayer.ctx.save();
                    currentProgressiveLayer.renderScope = {};

                    if (currentProgressiveLayer
                        && (currentProgressiveLayer.__progress > currentProgressiveLayer.__maxProgress)
                    ) {
                        // flushProgressiveLayer(currentProgressiveLayer);
                        // Quick jump all progressive elements
                        // All progressive element are not dirty, jump over and flush directly
                        i = currentProgressiveLayer.__nextIdxNotProg - 1;
                        // currentProgressiveLayer = null;
                        continue;
                    }

                    layerProgress = currentProgressiveLayer.__progress;

                    if (!currentProgressiveLayer.__dirty) {
                        // Keep rendering
                        frame = layerProgress;
                    }

                    currentProgressiveLayer.__progress = frame + 1;
                }

                if (elFrame === frame) {
                    this$1._doPaintEl(el, currentProgressiveLayer, true, currentProgressiveLayer.renderScope);
                }
            }
            else {
                this$1._doPaintEl(el, currentLayer, paintAll, scope);
            }

            el.__dirty = false;
        }

        if (currentProgressiveLayer) {
            flushProgressiveLayer(currentProgressiveLayer);
        }

        // Restore the lastLayer ctx
        ctx && ctx.restore();
        // If still has clipping state
        // if (scope.prevElClipPaths) {
        //     ctx.restore();
        // }

        if (env$1.wxa) {
            // Flush for weixin application
            each$1(this._layers, function (layer) {
                if (layer && layer.ctx && layer.ctx.draw) {
                    layer.ctx.draw();
                }
            });
        }

        this._furtherProgressive = false;
        each$1(this._progressiveLayers, function (layer) {
            if (layer.__maxProgress >= layer.__progress) {
                this._furtherProgressive = true;
            }
        }, this);
    },

    _doPaintEl: function (el, currentLayer, forcePaint, scope) {
        var ctx = currentLayer.ctx;
        var m = el.transform;
        if (
            (currentLayer.__dirty || forcePaint)
            // Ignore invisible element
            && !el.invisible
            // Ignore transparent element
            && el.style.opacity !== 0
            // Ignore scale 0 element, in some environment like node-canvas
            // Draw a scale 0 element can cause all following draw wrong
            // And setTransform with scale 0 will cause set back transform failed.
            && !(m && !m[0] && !m[3])
            // Ignore culled element
            && !(el.culling && isDisplayableCulled(el, this._width, this._height))
        ) {

            var clipPaths = el.__clipPaths;

            // Optimize when clipping on group with several elements
            if (scope.prevClipLayer !== currentLayer
                || isClipPathChanged(clipPaths, scope.prevElClipPaths)
            ) {
                // If has previous clipping state, restore from it
                if (scope.prevElClipPaths) {
                    scope.prevClipLayer.ctx.restore();
                    scope.prevClipLayer = scope.prevElClipPaths = null;

                    // Reset prevEl since context has been restored
                    scope.prevEl = null;
                }
                // New clipping state
                if (clipPaths) {
                    ctx.save();
                    doClip(clipPaths, ctx);
                    scope.prevClipLayer = currentLayer;
                    scope.prevElClipPaths = clipPaths;
                }
            }
            el.beforeBrush && el.beforeBrush(ctx);

            el.brush(ctx, scope.prevEl || null);
            scope.prevEl = el;

            el.afterBrush && el.afterBrush(ctx);
        }
    },

    /**
     * 获取 zlevel 所在层，如果不存在则会创建一个新的层
     * @param {number} zlevel
     * @return {module:zrender/Layer}
     */
    getLayer: function (zlevel) {
        if (this._singleCanvas) {
            return this._layers[0];
        }

        var layer = this._layers[zlevel];
        if (!layer) {
            // Create a new layer
            layer = new Layer('zr_' + zlevel, this, this.dpr);
            layer.__builtin__ = true;

            if (this._layerConfig[zlevel]) {
                merge$1(layer, this._layerConfig[zlevel], true);
            }

            this.insertLayer(zlevel, layer);

            // Context is created after dom inserted to document
            // Or excanvas will get 0px clientWidth and clientHeight
            layer.initContext();
        }

        return layer;
    },

    insertLayer: function (zlevel, layer) {

        var layersMap = this._layers;
        var zlevelList = this._zlevelList;
        var len = zlevelList.length;
        var prevLayer = null;
        var i = -1;
        var domRoot = this._domRoot;

        if (layersMap[zlevel]) {
            log$1('ZLevel ' + zlevel + ' has been used already');
            return;
        }
        // Check if is a valid layer
        if (!isLayerValid(layer)) {
            log$1('Layer of zlevel ' + zlevel + ' is not valid');
            return;
        }

        if (len > 0 && zlevel > zlevelList[0]) {
            for (i = 0; i < len - 1; i++) {
                if (
                    zlevelList[i] < zlevel
                    && zlevelList[i + 1] > zlevel
                ) {
                    break;
                }
            }
            prevLayer = layersMap[zlevelList[i]];
        }
        zlevelList.splice(i + 1, 0, zlevel);

        layersMap[zlevel] = layer;

        // Vitual layer will not directly show on the screen.
        // (It can be a WebGL layer and assigned to a ZImage element)
        // But it still under management of zrender.
        if (!layer.virtual) {
            if (prevLayer) {
                var prevDom = prevLayer.dom;
                if (prevDom.nextSibling) {
                    domRoot.insertBefore(
                        layer.dom,
                        prevDom.nextSibling
                    );
                }
                else {
                    domRoot.appendChild(layer.dom);
                }
            }
            else {
                if (domRoot.firstChild) {
                    domRoot.insertBefore(layer.dom, domRoot.firstChild);
                }
                else {
                    domRoot.appendChild(layer.dom);
                }
            }
        }
    },

    // Iterate each layer
    eachLayer: function (cb, context) {
        var this$1 = this;

        var zlevelList = this._zlevelList;
        var z;
        var i;
        for (i = 0; i < zlevelList.length; i++) {
            z = zlevelList[i];
            cb.call(context, this$1._layers[z], z);
        }
    },

    // Iterate each buildin layer
    eachBuiltinLayer: function (cb, context) {
        var this$1 = this;

        var zlevelList = this._zlevelList;
        var layer;
        var z;
        var i;
        for (i = 0; i < zlevelList.length; i++) {
            z = zlevelList[i];
            layer = this$1._layers[z];
            if (layer.__builtin__) {
                cb.call(context, layer, z);
            }
        }
    },

    // Iterate each other layer except buildin layer
    eachOtherLayer: function (cb, context) {
        var this$1 = this;

        var zlevelList = this._zlevelList;
        var layer;
        var z;
        var i;
        for (i = 0; i < zlevelList.length; i++) {
            z = zlevelList[i];
            layer = this$1._layers[z];
            if (!layer.__builtin__) {
                cb.call(context, layer, z);
            }
        }
    },

    /**
     * 获取所有已创建的层
     * @param {Array.<module:zrender/Layer>} [prevLayer]
     */
    getLayers: function () {
        return this._layers;
    },

    _updateLayerStatus: function (list) {
        var this$1 = this;


        var layers = this._layers;
        var progressiveLayers = this._progressiveLayers;

        var elCountsLastFrame = {};
        var progressiveElCountsLastFrame = {};

        this.eachBuiltinLayer(function (layer, z) {
            elCountsLastFrame[z] = layer.elCount;
            layer.elCount = 0;
            layer.__dirty = false;
        });

        each$1(progressiveLayers, function (layer, idx) {
            progressiveElCountsLastFrame[idx] = layer.elCount;
            layer.elCount = 0;
            layer.__dirty = false;
        });

        var progressiveLayerCount = 0;
        var currentProgressiveLayer;
        var lastProgressiveKey;
        var frameCount = 0;
        for (var i = 0, l = list.length; i < l; i++) {
            var el = list[i];
            var zlevel = this$1._singleCanvas ? 0 : el.zlevel;
            var layer = layers[zlevel];
            var elProgress = el.progressive;
            if (layer) {
                layer.elCount++;
                layer.__dirty = layer.__dirty || el.__dirty;
            }

            /////// Update progressive
            if (elProgress >= 0) {
                // Fix wrong progressive sequence problem.
                if (lastProgressiveKey !== elProgress) {
                    lastProgressiveKey = elProgress;
                    frameCount++;
                }
                var elFrame = el.__frame = frameCount - 1;
                if (!currentProgressiveLayer) {
                    var idx = Math.min(progressiveLayerCount, MAX_PROGRESSIVE_LAYER_NUMBER - 1);
                    currentProgressiveLayer = progressiveLayers[idx];
                    if (!currentProgressiveLayer) {
                        currentProgressiveLayer = progressiveLayers[idx] = new Layer(
                            'progressive', this$1, this$1.dpr
                        );
                        currentProgressiveLayer.initContext();
                    }
                    currentProgressiveLayer.__maxProgress = 0;
                }
                currentProgressiveLayer.__dirty = currentProgressiveLayer.__dirty || el.__dirty;
                currentProgressiveLayer.elCount++;

                currentProgressiveLayer.__maxProgress = Math.max(
                    currentProgressiveLayer.__maxProgress, elFrame
                );

                if (currentProgressiveLayer.__maxProgress >= currentProgressiveLayer.__progress) {
                    // Should keep rendering this  layer because progressive rendering is not finished yet
                    layer.__dirty = true;
                }
            }
            else {
                el.__frame = -1;

                if (currentProgressiveLayer) {
                    currentProgressiveLayer.__nextIdxNotProg = i;
                    progressiveLayerCount++;
                    currentProgressiveLayer = null;
                }
            }
        }

        if (currentProgressiveLayer) {
            progressiveLayerCount++;
            currentProgressiveLayer.__nextIdxNotProg = i;
        }

        // 层中的元素数量有发生变化
        this.eachBuiltinLayer(function (layer, z) {
            if (elCountsLastFrame[z] !== layer.elCount) {
                layer.__dirty = true;
            }
        });

        progressiveLayers.length = Math.min(progressiveLayerCount, MAX_PROGRESSIVE_LAYER_NUMBER);
        each$1(progressiveLayers, function (layer, idx) {
            if (progressiveElCountsLastFrame[idx] !== layer.elCount) {
                el.__dirty = true;
            }
            if (layer.__dirty) {
                layer.__progress = 0;
            }
        });
    },

    /**
     * 清除hover层外所有内容
     */
    clear: function () {
        this.eachBuiltinLayer(this._clearLayer);
        return this;
    },

    _clearLayer: function (layer) {
        layer.clear();
    },

    /**
     * 修改指定zlevel的绘制参数
     *
     * @param {string} zlevel
     * @param {Object} config 配置对象
     * @param {string} [config.clearColor=0] 每次清空画布的颜色
     * @param {string} [config.motionBlur=false] 是否开启动态模糊
     * @param {number} [config.lastFrameAlpha=0.7]
     *                 在开启动态模糊的时候使用，与上一帧混合的alpha值，值越大尾迹越明显
     */
    configLayer: function (zlevel, config) {
        if (config) {
            var layerConfig = this._layerConfig;
            if (!layerConfig[zlevel]) {
                layerConfig[zlevel] = config;
            }
            else {
                merge$1(layerConfig[zlevel], config, true);
            }

            var layer = this._layers[zlevel];

            if (layer) {
                merge$1(layer, layerConfig[zlevel], true);
            }
        }
    },

    /**
     * 删除指定层
     * @param {number} zlevel 层所在的zlevel
     */
    delLayer: function (zlevel) {
        var layers = this._layers;
        var zlevelList = this._zlevelList;
        var layer = layers[zlevel];
        if (!layer) {
            return;
        }
        layer.dom.parentNode.removeChild(layer.dom);
        delete layers[zlevel];

        zlevelList.splice(indexOf$1(zlevelList, zlevel), 1);
    },

    /**
     * 区域大小变化后重绘
     */
    resize: function (width, height) {
        var this$1 = this;

        if (!this._domRoot.style) { // Maybe in node or worker
            if (width == null || height == null) {
                return;
            }
            this._width = width;
            this._height = height;

            this.getLayer(0).resize(width, height);
        }
        else {
            var domRoot = this._domRoot;
            // FIXME Why ?
            domRoot.style.display = 'none';

            // Save input w/h
            var opts = this._opts;
            width != null && (opts.width = width);
            height != null && (opts.height = height);

            width = this._getSize(0);
            height = this._getSize(1);

            domRoot.style.display = '';

            // 优化没有实际改变的resize
            if (this._width != width || height != this._height) {
                domRoot.style.width = width + 'px';
                domRoot.style.height = height + 'px';

                for (var id in this$1._layers) {
                    if (this$1._layers.hasOwnProperty(id)) {
                        this$1._layers[id].resize(width, height);
                    }
                }
                each$1(this._progressiveLayers, function (layer) {
                    layer.resize(width, height);
                });

                this.refresh(true);
            }

            this._width = width;
            this._height = height;

        }
        return this;
    },

    /**
     * 清除单独的一个层
     * @param {number} zlevel
     */
    clearLayer: function (zlevel) {
        var layer = this._layers[zlevel];
        if (layer) {
            layer.clear();
        }
    },

    /**
     * 释放
     */
    dispose: function () {
        this.root.innerHTML = '';

        this.root =
        this.storage =

        this._domRoot =
        this._layers = null;
    },

    /**
     * Get canvas which has all thing rendered
     * @param {Object} opts
     * @param {string} [opts.backgroundColor]
     * @param {number} [opts.pixelRatio]
     */
    getRenderedCanvas: function (opts) {
        var this$1 = this;

        opts = opts || {};
        if (this._singleCanvas) {
            return this._layers[0].dom;
        }

        var imageLayer = new Layer('image', this, opts.pixelRatio || this.dpr);
        imageLayer.initContext();

        imageLayer.clearColor = opts.backgroundColor;
        imageLayer.clear();

        var displayList = this.storage.getDisplayList(true);

        var scope = {};
        var zlevel;

        var self = this;
        function findAndDrawOtherLayer(smaller, larger) {
            var zlevelList = self._zlevelList;
            if (smaller == null) {
                smaller = -Infinity;
            }
            var intermediateLayer;
            for (var i = 0; i < zlevelList.length; i++) {
                var z = zlevelList[i];
                var layer = self._layers[z];
                if (!layer.__builtin__ && z > smaller && z < larger) {
                    intermediateLayer = layer;
                    break;
                }
            }
            if (intermediateLayer && intermediateLayer.renderToCanvas) {
                imageLayer.ctx.save();
                intermediateLayer.renderToCanvas(imageLayer.ctx);
                imageLayer.ctx.restore();
            }
        }
        for (var i = 0; i < displayList.length; i++) {
            var el = displayList[i];

            if (el.zlevel !== zlevel) {
                findAndDrawOtherLayer(zlevel, el.zlevel);
                zlevel = el.zlevel;
            }
            this$1._doPaintEl(el, imageLayer, true, scope);
        }

        findAndDrawOtherLayer(zlevel, Infinity);

        return imageLayer.dom;
    },
    /**
     * 获取绘图区域宽度
     */
    getWidth: function () {
        return this._width;
    },

    /**
     * 获取绘图区域高度
     */
    getHeight: function () {
        return this._height;
    },

    _getSize: function (whIdx) {
        var opts = this._opts;
        var wh = ['width', 'height'][whIdx];
        var cwh = ['clientWidth', 'clientHeight'][whIdx];
        var plt = ['paddingLeft', 'paddingTop'][whIdx];
        var prb = ['paddingRight', 'paddingBottom'][whIdx];

        if (opts[wh] != null && opts[wh] !== 'auto') {
            return parseFloat(opts[wh]);
        }

        var root = this.root;
        // IE8 does not support getComputedStyle, but it use VML.
        var stl = document.defaultView.getComputedStyle(root);

        return (
            (root[cwh] || parseInt10(stl[wh]) || parseInt10(root.style[wh]))
            - (parseInt10(stl[plt]) || 0)
            - (parseInt10(stl[prb]) || 0)
        ) | 0;
    },

    pathToImage: function (path, dpr) {
        dpr = dpr || this.dpr;

        var canvas = document.createElement('canvas');
        var ctx = canvas.getContext('2d');
        var rect = path.getBoundingRect();
        var style = path.style;
        var shadowBlurSize = style.shadowBlur * ctx.dpr;
        var shadowOffsetX = style.shadowOffsetX * ctx.dpr;
        var shadowOffsetY = style.shadowOffsetY * ctx.dpr;
        var lineWidth = style.hasStroke() ? style.lineWidth : 0;

        var leftMargin = Math.max(lineWidth / 2, -shadowOffsetX + shadowBlurSize);
        var rightMargin = Math.max(lineWidth / 2, shadowOffsetX + shadowBlurSize);
        var topMargin = Math.max(lineWidth / 2, -shadowOffsetY + shadowBlurSize);
        var bottomMargin = Math.max(lineWidth / 2, shadowOffsetY + shadowBlurSize);
        var width = rect.width + leftMargin + rightMargin;
        var height = rect.height + topMargin + bottomMargin;

        canvas.width = width * dpr;
        canvas.height = height * dpr;

        ctx.scale(dpr, dpr);
        ctx.clearRect(0, 0, width, height);
        ctx.dpr = dpr;

        var pathTransform = {
            position: path.position,
            rotation: path.rotation,
            scale: path.scale
        };
        path.position = [leftMargin - rect.x, topMargin - rect.y];
        path.rotation = 0;
        path.scale = [1, 1];
        path.updateTransform();
        if (path) {
            path.brush(ctx);
        }

        var ImageShape = ZImage;
        var imgShape = new ImageShape({
            style: {
                x: 0,
                y: 0,
                image: canvas
            }
        });

        if (pathTransform.position != null) {
            imgShape.position = path.position = pathTransform.position;
        }

        if (pathTransform.rotation != null) {
            imgShape.rotation = path.rotation = pathTransform.rotation;
        }

        if (pathTransform.scale != null) {
            imgShape.scale = path.scale = pathTransform.scale;
        }

        return imgShape;
    }
};

/**
 * 事件辅助类
 * @module zrender/core/event
 * @author Kener (@Kener-林峰, kener.linfeng@gmail.com)
 */

var isDomLevel2 = (typeof window !== 'undefined') && !!window.addEventListener;

var MOUSE_EVENT_REG = /^(?:mouse|pointer|contextmenu|drag|drop)|click/;

function getBoundingClientRect(el) {
    // BlackBerry 5, iOS 3 (original iPhone) don't have getBoundingRect
    return el.getBoundingClientRect ? el.getBoundingClientRect() : {left: 0, top: 0};
}

// `calculate` is optional, default false
function clientToLocal(el, e, out, calculate) {
    out = out || {};

    // According to the W3C Working Draft, offsetX and offsetY should be relative
    // to the padding edge of the target element. The only browser using this convention
    // is IE. Webkit uses the border edge, Opera uses the content edge, and FireFox does
    // not support the properties.
    // (see http://www.jacklmoore.com/notes/mouse-position/)
    // In zr painter.dom, padding edge equals to border edge.

    // FIXME
    // When mousemove event triggered on ec tooltip, target is not zr painter.dom, and
    // offsetX/Y is relative to e.target, where the calculation of zrX/Y via offsetX/Y
    // is too complex. So css-transfrom dont support in this case temporarily.
    if (calculate || !env$1.canvasSupported) {
        defaultGetZrXY(el, e, out);
    }
    // Caution: In FireFox, layerX/layerY Mouse position relative to the closest positioned
    // ancestor element, so we should make sure el is positioned (e.g., not position:static).
    // BTW1, Webkit don't return the same results as FF in non-simple cases (like add
    // zoom-factor, overflow / opacity layers, transforms ...)
    // BTW2, (ev.offsetY || ev.pageY - $(ev.target).offset().top) is not correct in preserve-3d.
    // <https://bugs.jquery.com/ticket/8523#comment:14>
    // BTW3, In ff, offsetX/offsetY is always 0.
    else if (env$1.browser.firefox && e.layerX != null && e.layerX !== e.offsetX) {
        out.zrX = e.layerX;
        out.zrY = e.layerY;
    }
    // For IE6+, chrome, safari, opera. (When will ff support offsetX?)
    else if (e.offsetX != null) {
        out.zrX = e.offsetX;
        out.zrY = e.offsetY;
    }
    // For some other device, e.g., IOS safari.
    else {
        defaultGetZrXY(el, e, out);
    }

    return out;
}

function defaultGetZrXY(el, e, out) {
    // This well-known method below does not support css transform.
    var box = getBoundingClientRect(el);
    out.zrX = e.clientX - box.left;
    out.zrY = e.clientY - box.top;
}

/**
 * 如果存在第三方嵌入的一些dom触发的事件，或touch事件，需要转换一下事件坐标.
 * `calculate` is optional, default false.
 */
function normalizeEvent(el, e, calculate) {

    e = e || window.event;

    if (e.zrX != null) {
        return e;
    }

    var eventType = e.type;
    var isTouch = eventType && eventType.indexOf('touch') >= 0;

    if (!isTouch) {
        clientToLocal(el, e, e, calculate);
        e.zrDelta = (e.wheelDelta) ? e.wheelDelta / 120 : -(e.detail || 0) / 3;
    }
    else {
        var touch = eventType != 'touchend'
            ? e.targetTouches[0]
            : e.changedTouches[0];
        touch && clientToLocal(el, touch, e, calculate);
    }

    // Add which for click: 1 === left; 2 === middle; 3 === right; otherwise: 0;
    // See jQuery: https://github.com/jquery/jquery/blob/master/src/event.js
    // If e.which has been defined, if may be readonly,
    // see: https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent/which
    var button = e.button;
    if (e.which == null && button !== undefined && MOUSE_EVENT_REG.test(e.type)) {
        e.which = (button & 1 ? 1 : (button & 2 ? 3 : (button & 4 ? 2 : 0)));
    }

    return e;
}

function addEventListener(el, name, handler) {
    if (isDomLevel2) {
        el.addEventListener(name, handler);
    }
    else {
        el.attachEvent('on' + name, handler);
    }
}

function removeEventListener(el, name, handler) {
    if (isDomLevel2) {
        el.removeEventListener(name, handler);
    }
    else {
        el.detachEvent('on' + name, handler);
    }
}

/**
 * 动画主类, 调度和管理所有动画控制器
 *
 * @module zrender/animation/Animation
 * @author pissang(https://github.com/pissang)
 */

/**
 * @typedef {Object} IZRenderStage
 * @property {Function} update
 */

/**
 * @alias module:zrender/animation/Animation
 * @constructor
 * @param {Object} [options]
 * @param {Function} [options.onframe]
 * @param {IZRenderStage} [options.stage]
 * @example
 *     var animation = new Animation();
 *     var obj = {
 *         x: 100,
 *         y: 100
 *     };
 *     animation.animate(node.position)
 *         .when(1000, {
 *             x: 500,
 *             y: 500
 *         })
 *         .when(2000, {
 *             x: 100,
 *             y: 100
 *         })
 *         .start('spline');
 */
var Animation = function (options) {

    options = options || {};

    this.stage = options.stage || {};

    this.onframe = options.onframe || function() {};

    // private properties
    this._clips = [];

    this._running = false;

    this._time;

    this._pausedTime;

    this._pauseStart;

    this._paused = false;

    Eventful.call(this);
};

Animation.prototype = {

    constructor: Animation,
    /**
     * 添加 clip
     * @param {module:zrender/animation/Clip} clip
     */
    addClip: function (clip) {
        this._clips.push(clip);
    },
    /**
     * 添加 animator
     * @param {module:zrender/animation/Animator} animator
     */
    addAnimator: function (animator) {
        var this$1 = this;

        animator.animation = this;
        var clips = animator.getClips();
        for (var i = 0; i < clips.length; i++) {
            this$1.addClip(clips[i]);
        }
    },
    /**
     * 删除动画片段
     * @param {module:zrender/animation/Clip} clip
     */
    removeClip: function(clip) {
        var idx = indexOf$1(this._clips, clip);
        if (idx >= 0) {
            this._clips.splice(idx, 1);
        }
    },

    /**
     * 删除动画片段
     * @param {module:zrender/animation/Animator} animator
     */
    removeAnimator: function (animator) {
        var this$1 = this;

        var clips = animator.getClips();
        for (var i = 0; i < clips.length; i++) {
            this$1.removeClip(clips[i]);
        }
        animator.animation = null;
    },

    _update: function() {

        var time = new Date().getTime() - this._pausedTime;
        var delta = time - this._time;
        var clips = this._clips;
        var len = clips.length;

        var deferredEvents = [];
        var deferredClips = [];
        for (var i = 0; i < len; i++) {
            var clip = clips[i];
            var e = clip.step(time, delta);
            // Throw out the events need to be called after
            // stage.update, like destroy
            if (e) {
                deferredEvents.push(e);
                deferredClips.push(clip);
            }
        }

        // Remove the finished clip
        for (var i = 0; i < len;) {
            if (clips[i]._needsRemove) {
                clips[i] = clips[len - 1];
                clips.pop();
                len--;
            }
            else {
                i++;
            }
        }

        len = deferredEvents.length;
        for (var i = 0; i < len; i++) {
            deferredClips[i].fire(deferredEvents[i]);
        }

        this._time = time;

        this.onframe(delta);

        this.trigger('frame', delta);

        if (this.stage.update) {
            this.stage.update();
        }
    },

    _startLoop: function () {
        var self = this;

        this._running = true;

        function step() {
            if (self._running) {

                requestAnimationFrame(step);

                !self._paused && self._update();
            }
        }

        requestAnimationFrame(step);
    },

    /**
     * 开始运行动画
     */
    start: function () {

        this._time = new Date().getTime();
        this._pausedTime = 0;

        this._startLoop();
    },
    /**
     * 停止运行动画
     */
    stop: function () {
        this._running = false;
    },

    /**
     * Pause
     */
    pause: function () {
        if (!this._paused) {
            this._pauseStart = new Date().getTime();
            this._paused = true;
        }
    },

    /**
     * Resume
     */
    resume: function () {
        if (this._paused) {
            this._pausedTime += (new Date().getTime()) - this._pauseStart;
            this._paused = false;
        }
    },

    /**
     * 清除所有动画片段
     */
    clear: function () {
        this._clips = [];
    },
    /**
     * 对一个目标创建一个animator对象，可以指定目标中的属性使用动画
     * @param  {Object} target
     * @param  {Object} options
     * @param  {boolean} [options.loop=false] 是否循环播放动画
     * @param  {Function} [options.getter=null]
     *         如果指定getter函数，会通过getter函数取属性值
     * @param  {Function} [options.setter=null]
     *         如果指定setter函数，会通过setter函数设置属性值
     * @return {module:zrender/animation/Animation~Animator}
     */
    // TODO Gap
    animate: function (target, options) {
        options = options || {};

        var animator = new Animator(
            target,
            options.loop,
            options.getter,
            options.setter
        );

        this.addAnimator(animator);

        return animator;
    }
};

mixin(Animation, Eventful);

/**
 * Only implements needed gestures for mobile.
 */

var GestureMgr = function () {

    /**
     * @private
     * @type {Array.<Object>}
     */
    this._track = [];
};

GestureMgr.prototype = {

    constructor: GestureMgr,

    recognize: function (event, target, root) {
        this._doTrack(event, target, root);
        return this._recognize(event);
    },

    clear: function () {
        this._track.length = 0;
        return this;
    },

    _doTrack: function (event, target, root) {
        var touches = event.touches;

        if (!touches) {
            return;
        }

        var trackItem = {
            points: [],
            touches: [],
            target: target,
            event: event
        };

        for (var i = 0, len = touches.length; i < len; i++) {
            var touch = touches[i];
            var pos = clientToLocal(root, touch, {});
            trackItem.points.push([pos.zrX, pos.zrY]);
            trackItem.touches.push(touch);
        }

        this._track.push(trackItem);
    },

    _recognize: function (event) {
        var this$1 = this;

        for (var eventName in recognizers) {
            if (recognizers.hasOwnProperty(eventName)) {
                var gestureInfo = recognizers[eventName](this$1._track, event);
                if (gestureInfo) {
                    return gestureInfo;
                }
            }
        }
    }
};

function dist$1(pointPair) {
    var dx = pointPair[1][0] - pointPair[0][0];
    var dy = pointPair[1][1] - pointPair[0][1];

    return Math.sqrt(dx * dx + dy * dy);
}

function center$2(pointPair) {
    return [
        (pointPair[0][0] + pointPair[1][0]) / 2,
        (pointPair[0][1] + pointPair[1][1]) / 2
    ];
}

var recognizers = {

    pinch: function (track, event) {
        var trackLen = track.length;

        if (!trackLen) {
            return;
        }

        var pinchEnd = (track[trackLen - 1] || {}).points;
        var pinchPre = (track[trackLen - 2] || {}).points || pinchEnd;

        if (pinchPre
            && pinchPre.length > 1
            && pinchEnd
            && pinchEnd.length > 1
        ) {
            var pinchScale = dist$1(pinchEnd) / dist$1(pinchPre);
            !isFinite(pinchScale) && (pinchScale = 1);

            event.pinchScale = pinchScale;

            var pinchCenter = center$2(pinchEnd);
            event.pinchX = pinchCenter[0];
            event.pinchY = pinchCenter[1];

            return {
                type: 'pinch',
                target: track[0].target,
                event: event
            };
        }
    }

    // Only pinch currently.
};

var TOUCH_CLICK_DELAY = 300;

var mouseHandlerNames = [
    'click', 'dblclick', 'mousewheel', 'mouseout',
    'mouseup', 'mousedown', 'mousemove', 'contextmenu'
];

var touchHandlerNames = [
    'touchstart', 'touchend', 'touchmove'
];

var pointerEventNames = {
    pointerdown: 1, pointerup: 1, pointermove: 1, pointerout: 1
};

var pointerHandlerNames = map$1(mouseHandlerNames, function (name) {
    var nm = name.replace('mouse', 'pointer');
    return pointerEventNames[nm] ? nm : name;
});

function eventNameFix(name) {
    return (name === 'mousewheel' && env$1.browser.firefox) ? 'DOMMouseScroll' : name;
}

function processGesture(proxy, event, stage) {
    var gestureMgr = proxy._gestureMgr;

    stage === 'start' && gestureMgr.clear();

    var gestureInfo = gestureMgr.recognize(
        event,
        proxy.handler.findHover(event.zrX, event.zrY, null).target,
        proxy.dom
    );

    stage === 'end' && gestureMgr.clear();

    // Do not do any preventDefault here. Upper application do that if necessary.
    if (gestureInfo) {
        var type = gestureInfo.type;
        event.gestureEvent = type;

        proxy.handler.dispatchToElement({target: gestureInfo.target}, type, gestureInfo.event);
    }
}

// function onMSGestureChange(proxy, event) {
//     if (event.translationX || event.translationY) {
//         // mousemove is carried by MSGesture to reduce the sensitivity.
//         proxy.handler.dispatchToElement(event.target, 'mousemove', event);
//     }
//     if (event.scale !== 1) {
//         event.pinchX = event.offsetX;
//         event.pinchY = event.offsetY;
//         event.pinchScale = event.scale;
//         proxy.handler.dispatchToElement(event.target, 'pinch', event);
//     }
// }

/**
 * Prevent mouse event from being dispatched after Touch Events action
 * @see <https://github.com/deltakosh/handjs/blob/master/src/hand.base.js>
 * 1. Mobile browsers dispatch mouse events 300ms after touchend.
 * 2. Chrome for Android dispatch mousedown for long-touch about 650ms
 * Result: Blocking Mouse Events for 700ms.
 */
function setTouchTimer(instance) {
    instance._touching = true;
    clearTimeout(instance._touchTimer);
    instance._touchTimer = setTimeout(function () {
        instance._touching = false;
    }, 700);
}


var domHandlers = {
    /**
     * Mouse move handler
     * @inner
     * @param {Event} event
     */
    mousemove: function (event) {
        event = normalizeEvent(this.dom, event);

        this.trigger('mousemove', event);
    },

    /**
     * Mouse out handler
     * @inner
     * @param {Event} event
     */
    mouseout: function (event) {
        var this$1 = this;

        event = normalizeEvent(this.dom, event);

        var element = event.toElement || event.relatedTarget;
        if (element != this.dom) {
            while (element && element.nodeType != 9) {
                // 忽略包含在root中的dom引起的mouseOut
                if (element === this$1.dom) {
                    return;
                }

                element = element.parentNode;
            }
        }

        this.trigger('mouseout', event);
    },

    /**
     * Touch开始响应函数
     * @inner
     * @param {Event} event
     */
    touchstart: function (event) {
        // Default mouse behaviour should not be disabled here.
        // For example, page may needs to be slided.
        event = normalizeEvent(this.dom, event);

        // Mark touch, which is useful in distinguish touch and
        // mouse event in upper applicatoin.
        event.zrByTouch = true;

        this._lastTouchMoment = new Date();

        processGesture(this, event, 'start');

        // In touch device, trigger `mousemove`(`mouseover`) should
        // be triggered, and must before `mousedown` triggered.
        domHandlers.mousemove.call(this, event);

        domHandlers.mousedown.call(this, event);

        setTouchTimer(this);
    },

    /**
     * Touch移动响应函数
     * @inner
     * @param {Event} event
     */
    touchmove: function (event) {

        event = normalizeEvent(this.dom, event);

        // Mark touch, which is useful in distinguish touch and
        // mouse event in upper applicatoin.
        event.zrByTouch = true;

        processGesture(this, event, 'change');

        // Mouse move should always be triggered no matter whether
        // there is gestrue event, because mouse move and pinch may
        // be used at the same time.
        domHandlers.mousemove.call(this, event);

        setTouchTimer(this);
    },

    /**
     * Touch结束响应函数
     * @inner
     * @param {Event} event
     */
    touchend: function (event) {

        event = normalizeEvent(this.dom, event);

        // Mark touch, which is useful in distinguish touch and
        // mouse event in upper applicatoin.
        event.zrByTouch = true;

        processGesture(this, event, 'end');

        domHandlers.mouseup.call(this, event);

        // Do not trigger `mouseout` here, in spite of `mousemove`(`mouseover`) is
        // triggered in `touchstart`. This seems to be illogical, but by this mechanism,
        // we can conveniently implement "hover style" in both PC and touch device just
        // by listening to `mouseover` to add "hover style" and listening to `mouseout`
        // to remove "hover style" on an element, without any additional code for
        // compatibility. (`mouseout` will not be triggered in `touchend`, so "hover
        // style" will remain for user view)

        // click event should always be triggered no matter whether
        // there is gestrue event. System click can not be prevented.
        if (+new Date() - this._lastTouchMoment < TOUCH_CLICK_DELAY) {
            domHandlers.click.call(this, event);
        }

        setTouchTimer(this);
    },

    pointerdown: function (event) {
        domHandlers.mousedown.call(this, event);

        // if (useMSGuesture(this, event)) {
        //     this._msGesture.addPointer(event.pointerId);
        // }
    },

    pointermove: function (event) {
        // FIXME
        // pointermove is so sensitive that it always triggered when
        // tap(click) on touch screen, which affect some judgement in
        // upper application. So, we dont support mousemove on MS touch
        // device yet.
        if (!isPointerFromTouch(event)) {
            domHandlers.mousemove.call(this, event);
        }
    },

    pointerup: function (event) {
        domHandlers.mouseup.call(this, event);
    },

    pointerout: function (event) {
        // pointerout will be triggered when tap on touch screen
        // (IE11+/Edge on MS Surface) after click event triggered,
        // which is inconsistent with the mousout behavior we defined
        // in touchend. So we unify them.
        // (check domHandlers.touchend for detailed explanation)
        if (!isPointerFromTouch(event)) {
            domHandlers.mouseout.call(this, event);
        }
    }
};

function isPointerFromTouch(event) {
    var pointerType = event.pointerType;
    return pointerType === 'pen' || pointerType === 'touch';
}

// function useMSGuesture(handlerProxy, event) {
//     return isPointerFromTouch(event) && !!handlerProxy._msGesture;
// }

// Common handlers
each$1(['click', 'mousedown', 'mouseup', 'mousewheel', 'dblclick', 'contextmenu'], function (name) {
    domHandlers[name] = function (event) {
        event = normalizeEvent(this.dom, event);
        this.trigger(name, event);
    };
});

/**
 * 为控制类实例初始化dom 事件处理函数
 *
 * @inner
 * @param {module:zrender/Handler} instance 控制类实例
 */
function initDomHandler(instance) {
    each$1(touchHandlerNames, function (name) {
        instance._handlers[name] = bind(domHandlers[name], instance);
    });

    each$1(pointerHandlerNames, function (name) {
        instance._handlers[name] = bind(domHandlers[name], instance);
    });

    each$1(mouseHandlerNames, function (name) {
        instance._handlers[name] = makeMouseHandler(domHandlers[name], instance);
    });

    function makeMouseHandler(fn, instance) {
        return function () {
            if (instance._touching) {
                return;
            }
            return fn.apply(instance, arguments);
        };
    }
}


function HandlerDomProxy(dom) {
    Eventful.call(this);

    this.dom = dom;

    /**
     * @private
     * @type {boolean}
     */
    this._touching = false;

    /**
     * @private
     * @type {number}
     */
    this._touchTimer;

    /**
     * @private
     * @type {module:zrender/core/GestureMgr}
     */
    this._gestureMgr = new GestureMgr();

    this._handlers = {};

    initDomHandler(this);

    if (env$1.pointerEventsSupported) { // Only IE11+/Edge
        // 1. On devices that both enable touch and mouse (e.g., MS Surface and lenovo X240),
        // IE11+/Edge do not trigger touch event, but trigger pointer event and mouse event
        // at the same time.
        // 2. On MS Surface, it probablely only trigger mousedown but no mouseup when tap on
        // screen, which do not occurs in pointer event.
        // So we use pointer event to both detect touch gesture and mouse behavior.
        mountHandlers(pointerHandlerNames, this);

        // FIXME
        // Note: MS Gesture require CSS touch-action set. But touch-action is not reliable,
        // which does not prevent defuault behavior occasionally (which may cause view port
        // zoomed in but use can not zoom it back). And event.preventDefault() does not work.
        // So we have to not to use MSGesture and not to support touchmove and pinch on MS
        // touch screen. And we only support click behavior on MS touch screen now.

        // MS Gesture Event is only supported on IE11+/Edge and on Windows 8+.
        // We dont support touch on IE on win7.
        // See <https://msdn.microsoft.com/en-us/library/dn433243(v=vs.85).aspx>
        // if (typeof MSGesture === 'function') {
        //     (this._msGesture = new MSGesture()).target = dom; // jshint ignore:line
        //     dom.addEventListener('MSGestureChange', onMSGestureChange);
        // }
    }
    else {
        if (env$1.touchEventsSupported) {
            mountHandlers(touchHandlerNames, this);
            // Handler of 'mouseout' event is needed in touch mode, which will be mounted below.
            // addEventListener(root, 'mouseout', this._mouseoutHandler);
        }

        // 1. Considering some devices that both enable touch and mouse event (like on MS Surface
        // and lenovo X240, @see #2350), we make mouse event be always listened, otherwise
        // mouse event can not be handle in those devices.
        // 2. On MS Surface, Chrome will trigger both touch event and mouse event. How to prevent
        // mouseevent after touch event triggered, see `setTouchTimer`.
        mountHandlers(mouseHandlerNames, this);
    }

    function mountHandlers(handlerNames, instance) {
        each$1(handlerNames, function (name) {
            addEventListener(dom, eventNameFix(name), instance._handlers[name]);
        }, instance);
    }
}

var handlerDomProxyProto = HandlerDomProxy.prototype;
handlerDomProxyProto.dispose = function () {
    var this$1 = this;

    var handlerNames = mouseHandlerNames.concat(touchHandlerNames);

    for (var i = 0; i < handlerNames.length; i++) {
        var name = handlerNames[i];
        removeEventListener(this$1.dom, eventNameFix(name), this$1._handlers[name]);
    }
};

handlerDomProxyProto.setCursor = function (cursorStyle) {
    this.dom.style.cursor = cursorStyle || 'default';
};

mixin(HandlerDomProxy, Eventful);

/*!
* ZRender, a high performance 2d drawing library.
*
* Copyright (c) 2013, Baidu Inc.
* All rights reserved.
*
* LICENSE
* https://github.com/ecomfe/zrender/blob/master/LICENSE.txt
*/

var useVML = !env$1.canvasSupported;

var painterCtors = {
    canvas: Painter
};

var instances = {};    // ZRender实例map索引

/**
 * Initializing a zrender instance
 * @param {HTMLElement} dom
 * @param {Object} opts
 * @param {string} [opts.renderer='canvas'] 'canvas' or 'svg'
 * @param {number} [opts.devicePixelRatio]
 * @param {number|string} [opts.width] Can be 'auto' (the same as null/undefined)
 * @param {number|string} [opts.height] Can be 'auto' (the same as null/undefined)
 * @return {module:zrender/ZRender}
 */
function init$1(dom, opts) {
    var zr = new ZRender(guid(), dom, opts);
    instances[zr.id] = zr;
    return zr;
}

function delInstance(id) {
    delete instances[id];
}

/**
 * @module zrender/ZRender
 */
/**
 * @constructor
 * @alias module:zrender/ZRender
 * @param {string} id
 * @param {HTMLElement} dom
 * @param {Object} opts
 * @param {string} [opts.renderer='canvas'] 'canvas' or 'svg'
 * @param {number} [opts.devicePixelRatio]
 * @param {number} [opts.width] Can be 'auto' (the same as null/undefined)
 * @param {number} [opts.height] Can be 'auto' (the same as null/undefined)
 */
var ZRender = function (id, dom, opts) {

    opts = opts || {};

    /**
     * @type {HTMLDomElement}
     */
    this.dom = dom;

    /**
     * @type {string}
     */
    this.id = id;

    var self = this;
    var storage = new Storage();

    var rendererType = opts.renderer;
    // TODO WebGL
    if (useVML) {
        if (!painterCtors.vml) {
            throw new Error('You need to require \'zrender/vml/vml\' to support IE8');
        }
        rendererType = 'vml';
    }
    else if (!rendererType || !painterCtors[rendererType]) {
        rendererType = 'canvas';
    }
    var painter = new painterCtors[rendererType](dom, storage, opts);

    this.storage = storage;
    this.painter = painter;

    var handerProxy = (!env$1.node && !env$1.worker) ? new HandlerDomProxy(painter.getViewportRoot()) : null;
    this.handler = new Handler(storage, painter, handerProxy, painter.root);

    /**
     * @type {module:zrender/animation/Animation}
     */
    this.animation = new Animation({
        stage: {
            update: bind(this.flush, this)
        }
    });
    this.animation.start();

    /**
     * @type {boolean}
     * @private
     */
    this._needsRefresh;

    // 修改 storage.delFromStorage, 每次删除元素之前删除动画
    // FIXME 有点ugly
    var oldDelFromStorage = storage.delFromStorage;
    var oldAddToStorage = storage.addToStorage;

    storage.delFromStorage = function (el) {
        oldDelFromStorage.call(storage, el);

        el && el.removeSelfFromZr(self);
    };

    storage.addToStorage = function (el) {
        oldAddToStorage.call(storage, el);

        el.addSelfToZr(self);
    };
};

ZRender.prototype = {

    constructor: ZRender,
    /**
     * 获取实例唯一标识
     * @return {string}
     */
    getId: function () {
        return this.id;
    },

    /**
     * 添加元素
     * @param  {module:zrender/Element} el
     */
    add: function (el) {
        this.storage.addRoot(el);
        this._needsRefresh = true;
    },

    /**
     * 删除元素
     * @param  {module:zrender/Element} el
     */
    remove: function (el) {
        this.storage.delRoot(el);
        this._needsRefresh = true;
    },

    /**
     * Change configuration of layer
     * @param {string} zLevel
     * @param {Object} config
     * @param {string} [config.clearColor=0] Clear color
     * @param {string} [config.motionBlur=false] If enable motion blur
     * @param {number} [config.lastFrameAlpha=0.7] Motion blur factor. Larger value cause longer trailer
    */
    configLayer: function (zLevel, config) {
        this.painter.configLayer(zLevel, config);
        this._needsRefresh = true;
    },

    /**
     * Repaint the canvas immediately
     */
    refreshImmediately: function () {
        // var start = new Date();
        // Clear needsRefresh ahead to avoid something wrong happens in refresh
        // Or it will cause zrender refreshes again and again.
        this._needsRefresh = false;
        this.painter.refresh();
        /**
         * Avoid trigger zr.refresh in Element#beforeUpdate hook
         */
        this._needsRefresh = false;
        // var end = new Date();

        // var log = document.getElementById('log');
        // if (log) {
        //     log.innerHTML = log.innerHTML + '<br>' + (end - start);
        // }
    },

    /**
     * Mark and repaint the canvas in the next frame of browser
     */
    refresh: function() {
        this._needsRefresh = true;
    },

    /**
     * Perform all refresh
     */
    flush: function () {
        if (this._needsRefresh) {
            this.refreshImmediately();
        }
        if (this._needsRefreshHover) {
            this.refreshHoverImmediately();
        }
    },

    /**
     * Add element to hover layer
     * @param  {module:zrender/Element} el
     * @param {Object} style
     */
    addHover: function (el, style) {
        if (this.painter.addHover) {
            this.painter.addHover(el, style);
            this.refreshHover();
        }
    },

    /**
     * Add element from hover layer
     * @param  {module:zrender/Element} el
     */
    removeHover: function (el) {
        if (this.painter.removeHover) {
            this.painter.removeHover(el);
            this.refreshHover();
        }
    },

    /**
     * Clear all hover elements in hover layer
     * @param  {module:zrender/Element} el
     */
    clearHover: function () {
        if (this.painter.clearHover) {
            this.painter.clearHover();
            this.refreshHover();
        }
    },

    /**
     * Refresh hover in next frame
     */
    refreshHover: function () {
        this._needsRefreshHover = true;
    },

    /**
     * Refresh hover immediately
     */
    refreshHoverImmediately: function () {
        this._needsRefreshHover = false;
        this.painter.refreshHover && this.painter.refreshHover();
    },

    /**
     * Resize the canvas.
     * Should be invoked when container size is changed
     * @param {Object} [opts]
     * @param {number|string} [opts.width] Can be 'auto' (the same as null/undefined)
     * @param {number|string} [opts.height] Can be 'auto' (the same as null/undefined)
     */
    resize: function(opts) {
        opts = opts || {};
        this.painter.resize(opts.width, opts.height);
        this.handler.resize();
    },

    /**
     * Stop and clear all animation immediately
     */
    clearAnimation: function () {
        this.animation.clear();
    },

    /**
     * Get container width
     */
    getWidth: function() {
        return this.painter.getWidth();
    },

    /**
     * Get container height
     */
    getHeight: function() {
        return this.painter.getHeight();
    },

    /**
     * Export the canvas as Base64 URL
     * @param {string} type
     * @param {string} [backgroundColor='#fff']
     * @return {string} Base64 URL
     */
    // toDataURL: function(type, backgroundColor) {
    //     return this.painter.getRenderedCanvas({
    //         backgroundColor: backgroundColor
    //     }).toDataURL(type);
    // },

    /**
     * Converting a path to image.
     * It has much better performance of drawing image rather than drawing a vector path.
     * @param {module:zrender/graphic/Path} e
     * @param {number} width
     * @param {number} height
     */
    pathToImage: function(e, dpr) {
        return this.painter.pathToImage(e, dpr);
    },

    /**
     * Set default cursor
     * @param {string} [cursorStyle='default'] 例如 crosshair
     */
    setCursorStyle: function (cursorStyle) {
        this.handler.setCursorStyle(cursorStyle);
    },

    /**
     * Find hovered element
     * @param {number} x
     * @param {number} y
     * @return {Object} {target, topTarget}
     */
    findHover: function (x, y) {
        return this.handler.findHover(x, y);
    },

    /**
     * Bind event
     *
     * @param {string} eventName Event name
     * @param {Function} eventHandler Handler function
     * @param {Object} [context] Context object
     */
    on: function(eventName, eventHandler, context) {
        this.handler.on(eventName, eventHandler, context);
    },

    /**
     * Unbind event
     * @param {string} eventName Event name
     * @param {Function} [eventHandler] Handler function
     */
    off: function(eventName, eventHandler) {
        this.handler.off(eventName, eventHandler);
    },

    /**
     * Trigger event manually
     *
     * @param {string} eventName Event name
     * @param {event=} event Event object
     */
    trigger: function (eventName, event) {
        this.handler.trigger(eventName, event);
    },


    /**
     * Clear all objects and the canvas.
     */
    clear: function () {
        this.storage.delRoot();
        this.painter.clear();
    },

    /**
     * Dispose self.
     */
    dispose: function () {
        this.animation.stop();

        this.clear();
        this.storage.dispose();
        this.painter.dispose();
        this.handler.dispose();

        this.animation =
        this.storage =
        this.painter =
        this.handler = null;

        delInstance(this.id);
    }
};

var EVENTS = [
    'rotate',
    'touchStart',
    'multipointStart',
    'multipointEnd',
    'pinch',
    'swipe',
    'tap',
    'doubleTap',
    'longTap',
    'singleTap',
    'pressMove',
    'pan',
    'panEnd',
    'twoFingerPressMove',
    'touchMove',
    'touchEnd' ];


function getLen(v) {
    return Math.sqrt(v.x * v.x + v.y * v.y);
}

function dot$1(v1, v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

function getAngle(v1, v2) {
    var mr = getLen(v1) * getLen(v2);
    if (mr === 0) { return 0; }
    var r = dot$1(v1, v2) / mr;
    if (r > 1) { r = 1; }
    return Math.acos(r);
}

function cross$3(v1, v2) {
    return v1.x * v2.y - v2.x * v1.y;
}

function getRotateAngle(v1, v2) {
    var angle = getAngle(v1, v2);
    if (cross$3(v1, v2) > 0) {
        angle *= -1;
    }

    return angle * 180 / Math.PI;
}

// make a custom GestureMgr to replace origin one in HandlerProxy
var GestureMgr$1 = function (handler) {
    this.handler = handler;

    this.preV = { x: null, y: null };
    this.pinchStartLen = null;
    this.zoom = 1;
    this.isDoubleTap = false;

    this.delta = null;
    this.last = null;
    this.now = null;
    this.tapTimeout = null;
    this.singleTapTimeout = null;
    this.longTapTimeout = null;
    this.swipeTimeout = null;
    this.x1 = this.x2 = this.y1 = this.y2 = null;
    this.preTapPosition = { x: null, y: null };

    // most events should be fired on the first touched element
    this.startElement = null;
    // `move` should be fired on the current hovered element
    this.currentElement = null;
};

GestureMgr$1.prototype.dispatch = function (e) {
    var target = this.handler.findHover(e.zrX, e.zrY, null).target;
    this.recognize(e, target);
    return !!e.preventDefault
};

GestureMgr$1.prototype.clear = function () {
};

GestureMgr$1.prototype.recognize = function (event, target) {
    var type = event.type;

    if (this.currentElement !== target) {
        this.touchEnd(event, this.currentElement);
        this.currentElement = target;
    }

    if (type === 'touchstart') {
        this.startElement = target;
        this.start(event);
    }
    else if (type === 'touchmove') {
        this.move(event);
    }
    else if (type === 'touchend') {
        this.end(event);
        console.log('touchend');
    }
    else {
        throw 'unrecognized type:' + type
    }
};

GestureMgr$1.prototype.start = function (evt) {
    if (!evt.touches) { return; }
    this.now = Date.now();
    this.x1 = evt.touches[0].pageX;
    this.y1 = evt.touches[0].pageY;
    this.delta = this.now - (this.last || this.now);
    // #event
    this.touchStart(evt, this.startElement);
    if (this.preTapPosition.x !== null) {
        this.isDoubleTap = (this.delta > 0 && this.delta <= 250 && Math.abs(this.preTapPosition.x - this.x1) < 30 && Math.abs(this.preTapPosition.y - this.y1) < 30);
        if (this.isDoubleTap) { clearTimeout(self.singleTapTimeout); }
    }
    this.preTapPosition.x = this.x1;
    this.preTapPosition.y = this.y1;
    this.last = this.now;
    var preV = this.preV,
        len = evt.touches.length;
    if (len > 1) {
        this._cancelLongTap();
        this._cancelSingleTap();
        var v = { x: evt.touches[1].pageX - this.x1, y: evt.touches[1].pageY - this.y1 };
        preV.x = v.x;
        preV.y = v.y;
        this.pinchStartLen = getLen(preV);
        // #event
        this.multipointStart(evt, this.startElement);
    }
    this._preventTap = false;
    this.longTapTimeout = setTimeout(function () {
        // #event
        this.longTap(evt, this.startElement);
        this._preventTap = true;
    }.bind(this), 750);
};

GestureMgr$1.prototype.move = function (evt) {
    if (!evt.touches) { return; }
    var preV = this.preV,
        len = evt.touches.length,
        currentX = evt.touches[0].pageX,
        currentY = evt.touches[0].pageY;
    this.isDoubleTap = false;
    if (len > 1) {
        var sCurrentX = evt.touches[1].pageX,
            sCurrentY = evt.touches[1].pageY;
        var v = { x: evt.touches[1].pageX - currentX, y: evt.touches[1].pageY - currentY };

        if (preV.x !== null) {
            if (this.pinchStartLen > 0) {
                evt.zoom = getLen(v) / this.pinchStartLen;
                // #event
                this.pinch(evt, this.startElement);
            }

            evt.angle = getRotateAngle(v, preV);
            // #event
            this.rotate(evt, this.startElement);
        }
        preV.x = v.x;
        preV.y = v.y;

        if (this.x2 !== null && this.sx2 !== null) {
            evt.deltaX = (currentX - this.x2 + sCurrentX - this.sx2) / 2;
            evt.deltaY = (currentY - this.y2 + sCurrentY - this.sy2) / 2;
        } else {
            evt.deltaX = 0;
            evt.deltaY = 0;
        }
        // #event
        this.twoFingerPressMove(evt, this.startElement);

        this.sx2 = sCurrentX;
        this.sy2 = sCurrentY;
    } else {
        if (this.x2 !== null) {
            evt.deltaX = currentX - this.x2;
            evt.deltaY = currentY - this.y2;

            //move事件中添加对当前触摸点到初始触摸点的判断，
            //如果曾经大于过某个距离(比如10),就认为是移动到某个地方又移回来，应该不再触发tap事件才对。
            var movedX = Math.abs(this.x1 - this.x2),
                movedY = Math.abs(this.y1 - this.y2);

            if(movedX > 10 || movedY > 10){
                this._preventTap = true;
            }

        } else {
            evt.deltaX = 0;
            evt.deltaY = 0;
        }

        // #event
        this.pressMove(evt, this.startElement);
        this.pan(evt, this.startElement);
    }

    // #event
    this.touchMove(evt, this.currentElement);

    this._cancelLongTap();
    this.x2 = currentX;
    this.y2 = currentY;
};

GestureMgr$1.prototype.end = function (evt) {
    if (!evt.changedTouches) { return; }
    this._cancelLongTap();
    var self = this;
    if (evt.touches.length < 2) {
        // #event
        this.multipointEnd(evt, this.startElement);
        this.sx2 = this.sy2 = null;
    }

    //swipe
    if ((this.x2 && Math.abs(this.x1 - this.x2) > 30) ||
        (this.y2 && Math.abs(this.y1 - this.y2) > 30)) {
        evt.direction = this._swipeDirection(this.x1, this.x2, this.y1, this.y2);
        this.swipeTimeout = setTimeout(function () {
            // #event
            self.swipe(evt, self.startElement);

        }, 0);
    } else {
        this.tapTimeout = setTimeout(function () {
            if(!self._preventTap){
                // #event
                self.tap(evt, self.startElement);
            }
            // trigger double tap immediately
            if (self.isDoubleTap) {
                // #event
                self.doubleTap(evt, self.startElement);
                self.isDoubleTap = false;
            }
        }, 0);

        if (!self.isDoubleTap) {
            self.singleTapTimeout = setTimeout(function () {
                // #event
                self.singleTap(evt, self.startElement);
            }, 250);
        }
    }

    // #event
    this.touchEnd(evt, this.startElement);
    this.panEnd(evt, this.startElement);
    this.touchEnd(evt, this.currentElement);

    this.preV.x = 0;
    this.preV.y = 0;
    this.zoom = 1;
    this.pinchStartLen = null;
    this.x1 = this.x2 = this.y1 = this.y2 = null;
};

GestureMgr$1.prototype._cancelLongTap = function () {
    clearTimeout(this.longTapTimeout);
};

GestureMgr$1.prototype._cancelSingleTap = function () {
    clearTimeout(this.singleTapTimeout);
};

GestureMgr$1.prototype._swipeDirection = function (x1, x2, y1, y2) {
    return Math.abs(x1 - x2) >= Math.abs(y1 - y2) ? (x1 - x2 > 0 ? 'Left' : 'Right') : (y1 - y2 > 0 ? 'Up' : 'Down')
};

GestureMgr$1.prototype.dispose = function () {
    if(this.singleTapTimeout) { clearTimeout(this.singleTapTimeout); }
    if(this.tapTimeout) { clearTimeout(this.tapTimeout); }
    if(this.longTapTimeout) { clearTimeout(this.longTapTimeout); }
    if(this.swipeTimeout) { clearTimeout(this.swipeTimeout); }

    this.preV = this.pinchStartLen = this.zoom = this.isDoubleTap = this.delta = this.last = this.now = this.tapTimeout = this.singleTapTimeout = this.longTapTimeout = this.swipeTimeout = this.x1 = this.x2 = this.y1 = this.y2 = this.preTapPosition = this.rotate = this.touchStart = this.multipointStart = this.multipointEnd = this.pinch = this.swipe = this.tap = this.doubleTap = this.longTap = this.singleTap = this.pressMove = this.touchMove = this.touchEnd = this.touchCancel = this.twoFingerPressMove = null;

    this.startElement = this.currentElement = null;

    return null;
};

EVENTS.forEach(function (name) {
    var type = name.toLowerCase();
    GestureMgr$1.prototype[name] = function (event, target) {
        event.gestureEvent = type;
        this.handler.dispatchToElement({ target: target }, type, event);
    };
});

function GestureTouch (handler, touch) {

    // off useless drag events
    handler.off();

    if (touch === 'native') {
        // ios native event api test

        handler.setHandlerProxy(null);

        var mgr = new GestureMgr$1(handler);

        handler.proxy = mgr;

        mgr.handler = handler;

        return mgr

    } else {

        /** ***************************
         * Off normal mouse events
         * This way, we don't need to silent all mouse events manually
         * ****************************
         */
        handler.proxy.off();

        var mgr$1 = new GestureMgr$1(handler);

        // hack
        // process gestures by our manager.
        handler.proxy._gestureMgr = mgr$1;

        var originDispose = handler.proxy.dispose;
        handler.proxy.dispose = function () {
            originDispose.call(this);
            mgr$1.dispose();
        };

        return mgr$1;
    }
}

var param$1 = function (target) { return ({target: target}); };

function GestureMouse (handler) {
    var panningTarget = null;
    var docHandlerProxy = new HandlerDomProxy(document);
    docHandlerProxy.handler = handler; // prevent error if touch event fired

    handler.on('mousedown', panStart);
    // listen `mousemove`, `mouseup` on document after mousedown

    function panStart (e) {
        panningTarget = e.target;
        if (panningTarget) {
            // console.warn('panstart', e);

            this._x = e.clientX;
            this._y = e.clientY;

            // subscribe to document events
            docHandlerProxy.on('mousemove', pan, handler);
            docHandlerProxy.on('mouseup', panEnd, handler);

            this.dispatchToElement(param$1(panningTarget), 'panstart', e.event);
        }
    }

    // `e` comes from `document`, it's a native event obj
    function pan (e) {
        if (!panningTarget) {
            return;
        }

        panningTarget.panning = true;

        var x = e.clientX;
        var y = e.clientY;

        var dx = x - this._x;
        var dy = y - this._y;
        this._x = x;
        this._y = y;

        e.deltaX = dx;
        e.deltaY = dy;

        this.dispatchToElement(param$1(panningTarget), 'pan', e);
    }

    // native event obj
    function panEnd (e) {
        // console.warn(e);
        docHandlerProxy.off('mousemove', pan);
        docHandlerProxy.off('mouseup', panEnd);

        if (panningTarget) {
            panningTarget.panning = false;
            this.dispatchToElement(param$1(panningTarget), 'panend', e);
            panningTarget = null;
        }
    }

    var originDispose = handler.proxy.dispose;
    handler.proxy.dispose = function () {
        originDispose.call(this);
        handler.off('mousedown', panStart);
        docHandlerProxy.dispose();
    };
}

/**
 * attach gesture add-on
 * @param {ZRender} zr          - zrender instance
 * @param {boolean|string} touch       - is touch env
 * @returns {object} proxy      - handler proxy
 * @constructor
 */
function Gesture (zr, touch) {
    if (touch) {
        return GestureTouch(zr.handler, touch);
    } else {
        return GestureMouse(zr.handler);
    }
}

// https://github.com/developit/preact/tree/821c793bcff199999e88167da4f84c38f2fd5d73

var group = graphic.group;

var instances$1 = {};

{
    window.__van_zr_instances = instances$1;
}

/** Render JSX into a `parent` Element.
 *	@param {VNode} vnode		A (JSX) VNode to render
 *	@param {Element} dom		DOM element to render into
 *	@param {Object} [options]       - Additional options
 *	@param {ZRender} [options.zr]   - ZRender instance. Re-use existed zr instance.
 *  @param {boolean|string} [options.touch]- touch env
 *	@public
 */
function render(vnode, dom, options) {
    if ( options === void 0 ) options = {};

    var zr = options.zr || init$1(dom);

    instances$1[zr.getId()] = zr;

    // 并没有研究多一层group会不会对性能有影响
    // 这里的主要目的是利用已有的parent/children操作
    var parent = group();
    zr.add(parent);

    // custom properties
    zr.__parent = parent;

    // PENDING, return ret or zr
    var ret = diff(null, vnode, {}, false, parent, false);

    zr.__ret = ret;

    // attach custom
    //
    Gesture(zr, options.touch);

    return zr
}

render.initZr = init$1;

function getInterpolator(attr) {
  if (attr === 'transform') {
    return interpolateTransformSvg;
  }

  return interpolate$1;
}

function getTween(nameSpace, attr, value1) {
  return function tween() {
    var this$1 = this;
    var obj;

    var value0 = nameSpace ? this.state[nameSpace][attr] : this.state[attr];

    if (value0 === value1) {
      return null;
    }

    var i = getInterpolator(attr)(value0, value1);

    var stateTween;

    if (nameSpace === null) {
      stateTween = function (t) {
        this$1.setState(function () {
          return ( obj = {}, obj[attr] = i(t), obj);
        });
      };
    } else {
      stateTween = function (t) {
        this$1.setState(function (state) {
          // return { [nameSpace]: { ...state[nameSpace], [attr]: i(t) } };
            var r = {};
            r[nameSpace] = assign({}, state[nameSpace]);
            r[nameSpace][attr] = i(t);
            return r
        });
      };
    }

    return stateTween;
  };
}

function tween (nameSpace, attr, value) {
  return getTween.call(this, nameSpace, attr, value);
}

var CREATED$1 = 0;
var SCHEDULED$1 = 1;
var STARTING$1 = 2;
var STARTED$1 = 3;
var RUNNING$1 = 4;
var ENDING$1 = 5;
var ENDED$1 = 6;

function schedule$1 (node, stateKey, id, timing, tweens, events) {
  if ( events === void 0 ) events = {};

  var schedules = node.TRANSITION_SCHEDULES;
  if (!schedules) {
    node.TRANSITION_SCHEDULES = {}; // eslint-disable-line no-param-reassign
  } else if (id in schedules) {
    return;
  }

  var config = assign({}, {stateKey: stateKey, events: events, tweens: tweens}, timing, {timer: null, state: CREATED$1});
  create$4(node, id, config);
}

function create$4(node, id, config) {
  var schedules = node.TRANSITION_SCHEDULES;

  // Initialize the transition timer when the transition is created.
  // Note the actual delay is not known until the first callback!
  var transition = assign({}, config);
  var n = transition.tweens.length;
  var tweens = new Array(n);

  schedules[id] = transition;
  transition.timer = timer(schedule, 0, transition.time);

  function schedule(elapsed) {
    transition.state = SCHEDULED$1;
    transition.timer.restart(start, transition.delay, transition.time);

    // If the elapsed delay is less than our first sleep, start immediately.
    if (transition.delay <= elapsed) {
      start(elapsed - transition.delay);
    }
  }

  function start(elapsed) {
    var this$1 = this;
 // eslint-disable-line consistent-return
    // If the state is not SCHEDULED, then we previously errored on start.
    if (transition.state !== SCHEDULED$1) { return stop(); }

    for (var sid in schedules) { // eslint-disable-line
      var s = schedules[sid];

      if (s.stateKey !== transition.stateKey) {
        continue; // eslint-disable-line no-continue
      }

      // While this element already has a starting transition during this frame,
      // defer starting an interrupting transition until that transition has a
      // chance to tick (and possibly end); see d3/d3-transition#54!
      if (s.state === STARTED$1) { return timeout$1(start); }

      // 1. Interrupt the active transition, if any. dispatch the interrupt event.
      // 2. Cancel any pre-empted transitions. No interrupt event is dispatched
      // because the cancelled transitions never started. Note that this also
      // removes this transition from the pending list!

      if (s.state === RUNNING$1) {
        s.state = ENDED$1;
        s.timer.stop();
        if (s.events.interrupt && typeof s.events.interrupt === 'function') {
          s.events.interrupt.call(this$1);
        }
        delete schedules[sid];
      } else if (+sid < id) {
        s.state = ENDED$1;
        s.timer.stop();
        delete schedules[sid];
      }
    }

    // Defer the first tick to end of the current frame; see d3/d3#1576.
    // Note the transition may be canceled after start and before the first tick!
    // Note this must be scheduled before the start event; see d3/d3-transition#16!
    // Assuming this is successful, subsequent callbacks go straight to tick.
    timeout$1(function () {
      if (transition.state === STARTED$1) {
        transition.state = RUNNING$1;
        transition.timer.restart(tick, transition.delay, transition.time);
        tick(elapsed);
      }
    });

    transition.state = STARTING$1;
    if (transition.events.start && typeof transition.events.start === 'function') {
      transition.events.start.call(node);
    }

    if (transition.state !== STARTING$1) { // interrupted
      return; // eslint-disable-line consistent-return
    }

    transition.state = STARTED$1;

    // Initialize the tween, deleting null tween.
    var j = -1;

    for (var i = 0; i < n; ++i) {
      var res = transition.tweens[i].call(node);

      if (res) {
        tweens[++j] = res;
      }
    }

    tweens.length = j + 1;
  }

  function tick(elapsed) {
    var t = 1;

    if (elapsed < transition.duration) {
      t = transition.ease.call(null, elapsed / transition.duration);
    } else {
      transition.timer.restart(stop);
      transition.state = ENDING$1;
    }

    var i = -1;

    while (++i < tweens.length) {
      tweens[i].call(null, t);
    }

    if (transition.state === ENDING$1) {
      if (transition.events.end && typeof transition.events.end === 'function') {
        transition.events.end.call(node);
      }
      stop();
    }
  }

  function stop() {
    transition.state = ENDED$1;
    transition.timer.stop();

    delete schedules[id];
    for (var i in schedules) { return; } // eslint-disable-line guard-for-in, no-restricted-syntax
    delete node.TRANSITION_SCHEDULES; // eslint-disable-line no-param-reassign
  }
}

function once(func) {
  var called = false;

  return function transitionEvent() {
    if (!called) {
      called = true;
      func.call(this);
    }
  };
}

var id$1 = 0;

function newId$1() {
  return ++id$1;
}

// from https://github.com/d3/d3-ease/blob/master/src/linear.js
function linear$3(t) {
  return +t;
}

var preset = {
  time: null,
  delay: 0,
  duration: 250,
  ease: linear$3,
};

function scheduleTransitions(config) {
  var this$1 = this;
  var obj, obj$1, obj$2;

  if ( config === void 0 ) config = {};
  var transitions = assign({}, config);

  var events = transitions.events || {};
  delete transitions.events;

  // each event handler should be called only once
  keys(events).forEach(function (d) {
    if (typeof events[d] !== 'function') {
      throw new Error('Event handlers must be a function');
    } else {
      events[d] = once(events[d]);
    }
  });

  var timing = transitions.timing || {};
  delete transitions.timing;

  keys(transitions).forEach(function (stateKey) {
    var tweens = [];

    if (
      typeof transitions[stateKey] === 'object' &&
      Array.isArray(transitions[stateKey]) === false
    ) {
      keys(transitions[stateKey]).forEach(function (attr) {
        var val = transitions[stateKey][attr];

        if (Array.isArray(val)) {
          if (val.length === 1) {
            tweens.push(tween.call(this$1, stateKey, attr, val[0]));
          } else {
            this$1.setState(function (state) {
              // return { [stateKey]: { ...state[stateKey], [attr]: val[0] } };
                var r = {};
                r[stateKey] = assign({}, state[stateKey]);
                r[stateKey][attr] = val[0];
                return r
            });

            tweens.push(tween.call(this$1, stateKey, attr, val[1]));
          }
        } else if (typeof val === 'function') {
          var getResonanceCustomTween = function () {
            var resonanceCustomTween = function (t) {
              this$1.setState(function (state) {
                // return { [stateKey]: { ...state[stateKey], [attr]: val(t) } };
                  var r = {};
                  r[stateKey] = assign({}, state[stateKey]);
                  r[stateKey][attr] = val(t);
                  return r
              });
            };

            return resonanceCustomTween;
          };

          tweens.push(getResonanceCustomTween);
        } else {
          this$1.setState(function (state) {
            // return { [stateKey]: { ...state[stateKey], [attr]: val } };
              var r = {};
              r[stateKey] = assign({}, state[stateKey]);
              r[stateKey][attr] = val;
              return r
          });
          // This assures any existing transitions are stopped
          tweens.push(tween.call(this$1, stateKey, attr, val));
        }
      });
    } else {
      var val = transitions[stateKey];

      if (Array.isArray(val)) {
        if (val.length === 1) {
          tweens.push(tween.call(this$1, null, stateKey, val[0]));
        } else {
          this$1.setState(function () {
            return ( obj = {}, obj[stateKey] = val[0], obj);
          });

          tweens.push(tween.call(this$1, null, stateKey, val[1]));
        }
      } else if (typeof val === 'function') {
        var getResonanceCustomTween = function () {
          var resonanceCustomTween = function (t) {
            this$1.setState(function () {
              return ( obj$1 = {}, obj$1[stateKey] = val(t), obj$1);
            });
          };

          return resonanceCustomTween;
        };

        tweens.push(getResonanceCustomTween);
      } else {
        this$1.setState(function () {
          return ( obj$2 = {}, obj$2[stateKey] = val, obj$2);
        });
        // This assures any existing transitions are stopped
        tweens.push(tween.call(this$1, null, stateKey, val));
      }
    }

    var timingConfig = assign({}, preset, timing, {time: now()});
    schedule$1(this$1, stateKey, newId$1(), timingConfig, tweens, events);
  });
}

function transition$1(config) {
  var this$1 = this;

  if (Array.isArray(config)) {
    config.forEach(function (c) {
      scheduleTransitions.call(this$1, c);
    });
  } else {
    scheduleTransitions.call(this, config);
  }
}

// @flow weak

function stop$1() {
  var ts = this.TRANSITION_SCHEDULES;

  if (ts) {
    keys(ts).forEach(function (s) {
      ts[s].timer.stop();
    });
  }
}

/* eslint-disable flowtype/require-valid-file-annotation */

var Animate$$1 = (function (Component$$1) {
  function Animate$$1 (props) {
      Component$$1.call(this, props);

      this.state = typeof this.props.start === 'function' ? this.props.start() : assign({}, this.props.start);

      this.interval = null;
      this.renderNull = true;

      this.checkTransitionStatus = this.checkTransitionStatus.bind(this);
  }

  if ( Component$$1 ) Animate$$1.__proto__ = Component$$1;
  Animate$$1.prototype = create( Component$$1 && Component$$1.prototype );
  Animate$$1.prototype.constructor = Animate$$1;

  Animate$$1.prototype.componentWillMount = function () {
    if (this.props.show === true) {
      this.renderNull = false;
    }
  };

  Animate$$1.prototype.componentDidMount = function () {
    var ref = this.props;
    var enter = ref.enter;
    var show = ref.show;

    if (enter && show === true) {
      transition$1.call(this, typeof enter === 'function' ? enter() : enter);
    }
  };

  Animate$$1.prototype.componentWillReceiveProps = function (next) {
    var this$1 = this;

    var show = next.show;
    var start = next.start;
    var enter = next.enter;
    var update = next.update;
    var leave = next.leave;

    if (this.props.show === false && this.renderNull === true && show === true) {
      this.renderNull = false;

      this.setState(function () { return (typeof start === 'function' ? start() : start); }, function () {
        if (enter) {
          transition$1.call(this$1, typeof enter === 'function' ? enter() : enter);
        }
      });
    } else if (this.props.show === true && show === false) {
      if (leave) {
        transition$1.call(this, typeof leave === 'function' ? leave() : leave);
        this.interval = interval$1(this.checkTransitionStatus);
      } else {
        this.renderNull = true;
        this.setState(function (prevState) { return prevState; }); // force render as null
      }
    } else if (show === true && update) {
      if (this.interval) {
        this.interval.stop();
      }

      transition$1.call(this, typeof update === 'function' ? update() : update);
    }
  };

  Animate$$1.prototype.componentWillUnmount = function () {
    if (this.interval) {
      this.interval.stop();
    }

    stop$1.call(this);
  };

  Animate$$1.prototype.checkTransitionStatus = function () {
    if (!this.TRANSITION_SCHEDULES) {
      this.interval.stop();

      if (this.props.show === false) {
        this.renderNull = true;
        this.setState(function (prevState) { return prevState; }); // force render as null
      }
    }
  };

  Animate$$1.prototype.render = function () {
    if (this.renderNull === true) {
      return null;
    }

    return this.props.children[0](this.state);
  };

  return Animate$$1;
}(Component));

Animate$$1.defaultProps = {
    show: true,
};

// @flow weak
/* eslint guard-for-in: "off", no-restricted-syntax: "off" */
// Node constructor used to manage transitions internally
// Basically a modified preact component (https://github.com/developit/preact)

function extend$3(obj, props) {
  for (var i in props) {
    obj[i] = props[i]; // eslint-disable-line no-param-reassign
  }

  return obj;
}

function Node$2(key, data, type, condition) {
  this.key = key;
  this.data = data;
  this.type = type;
  this.condition = condition;
  this.prevCondition = null;
  this.state = {};
}

extend$3(Node$2.prototype, {
  setState: function(state) {
    var s = this.state;
    extend$3(s, typeof state === 'function' ? state(s) : state);
  },
  updateData: function(data) {
    this.data = data;

    return this;
  },
  updateType: function(type) {
    this.type = type;

    return this;
  },
  updateCondition: function(condition) {
    this.prevCondition = this.condition;
    this.condition = condition;
  },

});

// @flow weak
/* eslint no-continue: "off" */
/* based on react-motion's mergeDiff (https://github.com/chenglou/react-motion) */

function mergeKeys(
  currNodeKeys,
  currKeyIndex,
  nextNodeKeys,
  nextKeyIndex
) {
  var allKeys = [];

  for (var i = 0; i < nextNodeKeys.length; i++) {
    allKeys[i] = nextNodeKeys[i];
  }

  for (var i$1 = 0; i$1 < currNodeKeys.length; i$1++) {
    if (nextKeyIndex[currNodeKeys[i$1]] === undefined) {
      allKeys.push(currNodeKeys[i$1]);
    }
  }

  return allKeys.sort(function (a, b) {
    var nextOrderA = nextKeyIndex[a];
    var nextOrderB = nextKeyIndex[b];
    var currOrderA = currKeyIndex[a];
    var currOrderB = currKeyIndex[b];

    if (nextOrderA != null && nextOrderB != null) {
      return nextKeyIndex[a] - nextKeyIndex[b];
    } else if (currOrderA != null && currOrderB != null) {
      return currKeyIndex[a] - currKeyIndex[b];
    } else if (nextOrderA != null) {
      for (var i = 0; i < nextNodeKeys.length; i++) {
        var pivot = nextNodeKeys[i];

        if (!currKeyIndex[pivot]) {
          continue;
        }

        if (
          nextOrderA < nextKeyIndex[pivot] &&
          currOrderB > currKeyIndex[pivot]
        ) {
          return -1;
        } else if (
          nextOrderA > nextKeyIndex[pivot] &&
          currOrderB < currKeyIndex[pivot]
        ) {
          return 1;
        }
      }

      return 1;
    }

    for (var i$1 = 0; i$1 < nextNodeKeys.length; i$1++) {
      var pivot$1 = nextNodeKeys[i$1];

      if (!currKeyIndex[pivot$1]) {
        continue;
      }

      if (
        nextOrderB < nextKeyIndex[pivot$1] &&
        currOrderA > currKeyIndex[pivot$1]
      ) {
        return 1;
      } else if (
        nextOrderB > nextKeyIndex[pivot$1] &&
        currOrderA < currKeyIndex[pivot$1]
      ) {
        return -1;
      }
    }

    return -1;
  });
}

// @flow weak

var ENTER = 'ENTER';
var UPDATE = 'UPDATE';
var LEAVE = 'LEAVE';

var NodeGroup$$1 = (function (Component$$1) {
  function NodeGroup$$1(props) {
    Component$$1.call(this, props);

    this.state = {
      nodes: [],
    };

    this.nodeHash = {};
    this.nodeKeys = [];
    this.interval = null;
    this.unmounting = false;

    this.animate = this.animate.bind(this);
  }

  if ( Component$$1 ) NodeGroup$$1.__proto__ = Component$$1;
  NodeGroup$$1.prototype = create( Component$$1 && Component$$1.prototype );
  NodeGroup$$1.prototype.constructor = NodeGroup$$1;

  NodeGroup$$1.prototype.componentDidMount = function () {
    this.updateNodes(this.props);
  };

  NodeGroup$$1.prototype.componentWillReceiveProps = function (next) {
    if (next.data !== this.props.data) {
      this.updateNodes(next);
    }
  };

  NodeGroup$$1.prototype.componentWillUnmount = function () {
    var this$1 = this;

    this.unmounting = true;

    if (this.interval) {
      this.interval.stop();
    }

    this.nodeKeys.forEach(function (key) {
      stop$1.call(this$1.nodeHash[key]);
    });
  };

  NodeGroup$$1.prototype.updateNodes = function (props) {
    var this$1 = this;

    var data = props.data;
    var keyAccessor = props.keyAccessor;
    var start = props.start;
    var enter = props.enter;
    var update = props.update;
    var leave = props.leave;

    var currKeyIndex = {};
    var currNodeKeys = this.nodeKeys;
    var currNodeKeysLength = this.nodeKeys.length;

    for (var i = 0; i < currNodeKeysLength; i++) {
      currKeyIndex[currNodeKeys[i]] = i;
    }

    var nextKeyIndex = {};
    var nextNodeKeys = [];

    for (var i$1 = 0; i$1 < data.length; i$1++) {
      var d = data[i$1];
      var k = keyAccessor(d, i$1);

      nextKeyIndex[k] = i$1;
      nextNodeKeys.push(k);

      if (currKeyIndex[k] === undefined) {
        this$1.nodeHash[k] = new Node$2(k, d, ENTER);
      }
    }

    for (var i$2 = 0; i$2 < currNodeKeysLength; i$2++) {
      var k$1 = currNodeKeys[i$2];
      var n = this$1.nodeHash[k$1];

      if (nextKeyIndex[k$1] !== undefined) {
        n.updateData(data[nextKeyIndex[k$1]]);
        n.updateType(UPDATE);
      } else {
        n.updateType(LEAVE);
      }
    }

    this.nodeKeys = mergeKeys(
      currNodeKeys,
      currKeyIndex,
      nextNodeKeys,
      nextKeyIndex
    );

    for (var i$3 = 0; i$3 < this.nodeKeys.length; i$3++) {
      var k$2 = this$1.nodeKeys[i$3];
      var n$1 = this$1.nodeHash[k$2];
      var d$1 = n$1.data;

      if (n$1.type === ENTER) {
        n$1.setState(start(d$1, nextKeyIndex[k$2]));
        transition$1.call(n$1, enter(d$1, nextKeyIndex[k$2]));
      } else if (n$1.type === LEAVE) {
        transition$1.call(n$1, leave(d$1, currKeyIndex[k$2]));
      } else {
        transition$1.call(n$1, update(d$1, nextKeyIndex[k$2]));
      }
    }

    if (!this.interval) {
      this.interval = interval$1(this.animate);
    } else {
      this.interval.restart(this.animate);
    }

    this.renderNodes();
  };

  NodeGroup$$1.prototype.animate = function (){
    var this$1 = this;

    if (this.unmounting) {
      return;
    }

    var pending = false;

    var nextNodeKeys = [];
    var length = this.nodeKeys.length;

    for (var i = 0; i < length; i++) {
      var k = this$1.nodeKeys[i];
      var n = this$1.nodeHash[k];

      if (n.TRANSITION_SCHEDULES) {
        pending = true;
      }

      if (n.type === LEAVE && !n.TRANSITION_SCHEDULES) {
        delete this$1.nodeHash[k];
      } else {
        nextNodeKeys.push(k);
      }
    }

    if (!pending) {
      this.interval.stop();
    }

    this.nodeKeys = nextNodeKeys;
    this.renderNodes();
  };

  NodeGroup$$1.prototype.renderNodes = function () {
    var this$1 = this;

    this.setState(function () { return ({
      nodes: this$1.nodeKeys.map(function (key) {
        return this$1.nodeHash[key];
      }),
    }); });
  };

  NodeGroup$$1.prototype.render = function () {
    return this.props.children[0](this.state.nodes);
  };

  return NodeGroup$$1;
}(Component));

NodeGroup$$1.defaultProps = {
    keyAccessor: function (d) { return d.id; },
    enter: function () {},
    update: function () {},
    leave: function () {},
};

/**
 * Created by eason on 15/6/24.
 */
//
//Copyright (c) 2014 Ga?tan Renaudeau
//
//Permission is hereby granted, free of charge, to any person
//obtaining a copy of this software and associated documentation
//files (the "Software"), to deal in the Software without
//restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the
//Software is furnished to do so, subject to the following
//conditions:
//
//    The above copyright notice and this permission notice shall be
//included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.



    var global$2 = window;

    // These values are established by empiricism with tests (tradeoff: performance VS precision)
    var NEWTON_ITERATIONS = 4;
    var NEWTON_MIN_SLOPE = 0.001;
    var SUBDIVISION_PRECISION = 0.0000001;
    var SUBDIVISION_MAX_ITERATIONS = 10;

    var kSplineTableSize = 11;
    var kSampleStepSize = 1.0 / (kSplineTableSize - 1.0);

    var float32ArraySupported = 'Float32Array' in global$2;

    function A$1 (aA1, aA2) { return 1.0 - 3.0 * aA2 + 3.0 * aA1; }
    function B$1 (aA1, aA2) { return 3.0 * aA2 - 6.0 * aA1; }
    function C$1 (aA1)      { return 3.0 * aA1; }

    // Returns x(t) given t, x1, and x2, or y(t) given t, y1, and y2.
    function calcBezier (aT, aA1, aA2) {
        return ((A$1(aA1, aA2)*aT + B$1(aA1, aA2))*aT + C$1(aA1))*aT;
    }

    // Returns dx/dt given t, x1, and x2, or dy/dt given t, y1, and y2.
    function getSlope (aT, aA1, aA2) {
        return 3.0 * A$1(aA1, aA2)*aT*aT + 2.0 * B$1(aA1, aA2) * aT + C$1(aA1);
    }

    function binarySubdivide (aX, aA, aB, mX1, mX2) {
        var currentX, currentT, i = 0;
        do {
            currentT = aA + (aB - aA) / 2.0;
            currentX = calcBezier(currentT, mX1, mX2) - aX;
            if (currentX > 0.0) {
                aB = currentT;
            } else {
                aA = currentT;
            }
        } while (Math.abs(currentX) > SUBDIVISION_PRECISION && ++i < SUBDIVISION_MAX_ITERATIONS);
        return currentT;
    }

    function BezierEasing (mX1, mY1, mX2, mY2) {
        var arguments$1 = arguments;

        // Validate arguments
        if (arguments.length !== 4) {
            throw new Error("BezierEasing requires 4 arguments.");
        }
        for (var i=0; i<4; ++i) {
            if (typeof arguments$1[i] !== "number" || isNaN(arguments$1[i]) || !isFinite(arguments$1[i])) {
                throw new Error("BezierEasing arguments should be integers.");
            }
        }
        if (mX1 < 0 || mX1 > 1 || mX2 < 0 || mX2 > 1) {
            throw new Error("BezierEasing x values must be in [0, 1] range.");
        }

        var mSampleValues = float32ArraySupported ? new Float32Array(kSplineTableSize) : [];

        function newtonRaphsonIterate (aX, aGuessT) {
            for (var i = 0; i < NEWTON_ITERATIONS; ++i) {
                var currentSlope = getSlope(aGuessT, mX1, mX2);
                if (currentSlope === 0.0) {return aGuessT;}
                var currentX = calcBezier(aGuessT, mX1, mX2) - aX;
                aGuessT -= currentX / currentSlope;
            }
            return aGuessT;
        }

        function calcSampleValues () {
            for (var i = 0; i < kSplineTableSize; ++i) {
                mSampleValues[i] = calcBezier(i * kSampleStepSize, mX1, mX2);
            }
        }

        function getTForX (aX) {
            var intervalStart = 0.0;
            var currentSample = 1;
            var lastSample = kSplineTableSize - 1;

            for (; currentSample != lastSample && mSampleValues[currentSample] <= aX; ++currentSample) {
                intervalStart += kSampleStepSize;
            }
            --currentSample;

            // Interpolate to provide an initial guess for t
            var dist = (aX - mSampleValues[currentSample]) / (mSampleValues[currentSample+1] - mSampleValues[currentSample]);
            var guessForT = intervalStart + dist * kSampleStepSize;

            var initialSlope = getSlope(guessForT, mX1, mX2);
            if (initialSlope >= NEWTON_MIN_SLOPE) {
                return newtonRaphsonIterate(aX, guessForT);
            } else if (initialSlope === 0.0) {
                return guessForT;
            } else {
                return binarySubdivide(aX, intervalStart, intervalStart + kSampleStepSize, mX1, mX2);
            }
        }

        var _precomputed = false;
        function precompute() {
            _precomputed = true;
            if (mX1 != mY1 || mX2 != mY2)
            {calcSampleValues();}
        }

        var f = function (aX) {
            if (!_precomputed) {precompute();}
            if (mX1 === mY1 && mX2 === mY2) {return aX;} // linear
            // Because JavaScript number are imprecise, we should guarantee the extremes are right.
            if (aX === 0) {return 0;}
            if (1 - aX  < 0.001) {return 1;}
            return calcBezier(getTForX(aX), mY1, mY2);
        };

        f.getControlPoints = function() { return [{ x: mX1, y: mY1 }, { x: mX2, y: mY2 }]; };

        var args = [mX1, mY1, mX2, mY2];
        var str = "BezierEasing("+args+")";
        f.toString = function () { return str; };

        //var css = "cubic-bezier("+args+")";
        //f.toCSS = function () {?return css; };

        return f;
    }

    // CSS mapping
    BezierEasing.css = {
        "ease":        BezierEasing(0.25, 0.1, 0.25, 1.0),
        "linear":      BezierEasing(0.00, 0.0, 1.00, 1.0),
        "ease-in":     BezierEasing(0.42, 0.0, 1.00, 1.0),
        "ease-out":    BezierEasing(0.00, 0.0, 0.58, 1.0),
        "ease-in-out": BezierEasing(0.42, 0.0, 0.58, 1.0),
        "swing":       BezierEasing(0.02, 0.01,0.47, 1.0),
        "ease-in-back":BezierEasing(0.6,-0.28,0.735,0.045),
        "ease-out-back":BezierEasing(0.175, 0.885, 0.32, 1.275),
        "ease-in-quart":BezierEasing(0.895, 0.03, 0.685, 0.22),
        "ease-out-quart":BezierEasing(0.165, 0.84, 0.44, 1),
        "ease-in-quint":BezierEasing(0.755, 0.05, 0.855, 0.06),
        "ease-out-quint":BezierEasing(0.23, 1, 0.32, 1),
        "ease-in-quad":BezierEasing(0.55,0.085,0.68,0.53),
        "ease-out-quad":BezierEasing(0.25,0.46,0.45,0.94),
        "ease-in-out-quad": BezierEasing(0.455, 0.03, 0.515, 0.955),
        "ease-out-cubic":BezierEasing(0.215,0.61,0.355,1)
    };

/**
 * Created by Jeffrey on 2018/1/3.
 */

// [100, ) e.g. Legend
var StaticLayerZ = 100;
// data update
var ENTER$1 = 'ENTER';
var UPDATE$1 = 'UPDATE';
var LEAVE$1 = 'LEAVE';
// element condition
var FADE = 'FADE';
var HOVER = 'HOVER';

var css = BezierEasing.css;
var Ease = {
    Swing: css.swing,
    Ease: css.ease,
    A: BezierEasing(0.18, 0.89, 0.32, 1.6),
    B: BezierEasing(0.02, 0.01, 0.47, 1),
    Default: css.ease,
    EaseOutCubic: easing.cubicOut,
    EaseOutBounce: easing.bounceOut,
    EaseInQuint: easing.quinticIn,
    EaseOutQuint: easing.quinticOut,
    EaseInBack: easing.backIn,
    EaseOutRevise: BezierEasing(0, 0, 0.16, 1.03),
    EaseOutBack: easing.backOut,
    EaseInOutExpo: easing.exponentialInOut,
    // for line marker animation
    EaseOutBackAcute: BezierEasing(.35, .81, .21, 2),
};

var NodeGroup$$2 = (function (Component$$1) {
    function NodeGroup (props) {
        Component$$1.call(this, props);

        this.state = {
            nodes: [],
        };

        // todo,
        this.elements = {};

        this.nodeHash = {};
        this.nodeKeys = [];
        this.interval = null;
        this.unmounting = false;
        this.cleaning = false;

        this.clean = this.clean.bind(this);
        this.deferredClean = this.deferredClean.bind(this);

        this.updateData(props);
    }

    if ( Component$$1 ) NodeGroup.__proto__ = Component$$1;
    NodeGroup.prototype = create( Component$$1 && Component$$1.prototype );
    NodeGroup.prototype.constructor = NodeGroup;

    NodeGroup.prototype.componentWillReceiveProps = function (next) {
        // if (next.data !== this.props.data) {
        if (next.process === 'SHOW' || next.process === 'HIDE') {
            this.updateData(next);
        } else {
            this.updateNodes(next);
        }
    };

    NodeGroup.prototype.updateData = function (props) {
        var this$1 = this;

        var data = props.data;
        var keyAccessor = props.keyAccessor;
        var vanishChecker = props.vanishChecker;
        var fadeChecker = props.fadeChecker;
        var highlights = props.highlights;

        var currKeyIndex = {};
        var currNodeKeys = this.nodeKeys;
        var currNodeKeysLength = this.nodeKeys.length;

        for (var i = 0; i < currNodeKeysLength; i++) {
            currKeyIndex[currNodeKeys[i]] = i;
        }

        var nextKeyIndex = {};
        var nextNodeKeys = [];

        var isVanish = vanishChecker;
        var isFade = fadeChecker;

        for (var i$1 = 0; i$1 < data.length; i$1++) {
            var d = data[i$1];
            var k = keyAccessor(d, i$1);

            if (!isVanish(d)) {
                nextKeyIndex[k] = i$1;
                nextNodeKeys.push(k);

                if (currKeyIndex[k] === undefined) {
                    this$1.nodeHash[k] = new Node$2(k, d, ENTER$1, isFade(d) ? FADE : null);
                    // fill in start state
                    // this.nodeHash[k].setState(start(d, nextKeyIndex[k]));
                }
            }
        }

        for (var i$2 = 0; i$2 < currNodeKeysLength; i$2++) {
            var k$1 = currNodeKeys[i$2];
            var n = this$1.nodeHash[k$1];
            var d$1 = data[nextKeyIndex[k$1]];

            if (nextKeyIndex[k$1] !== undefined) {
                n.updateData(d$1);
                n.updateType(UPDATE$1);

                if (isFade(d$1)) {
                    n.updateCondition(FADE);
                }
                else if (highlights[k$1]) {
                    n.updateCondition(HOVER);
                } else {
                    n.updateCondition(null);
                }
            } else {
                n.updateType(LEAVE$1);
            }
        }

        this.nodeKeys = mergeKeys(
            currNodeKeys,
            currKeyIndex,
            nextNodeKeys,
            nextKeyIndex
        );

        // for (let i = 0; i < this.nodeKeys.length; i++) {
        //
        //     const k = this.nodeKeys[i];
        //     const n = this.nodeHash[k];
        //     const d = n.data;
        //
        //     if (n.type === ENTER) {
        //         // n.setState(start(d, nextKeyIndex[k]));
        //         // transition.call(n, enter(d, nextKeyIndex[k]));
        //         // n.setState(enter(d, nextKeyIndex[k]));
        //     } else if (n.type === LEAVE) {
        //         // transition.call(n, leave(d, currKeyIndex[k]));
        //         // n.setState(leave(d, currKeyIndex[k]));
        //     } else {
        //         // transition.call(n, update(d, nextKeyIndex[k]));
        //         // n.setState(update(d, nextKeyIndex[k]));
        //     }
        // }

        this.renderNodes();
    };

    NodeGroup.prototype.updateNodes = function (props) {
        var this$1 = this;

        // data no change
        // filter(fade) or highlights change
        var highlights = props.highlights;
        var fadeChecker = props.fadeChecker;

        forEach(this.nodeKeys, function (k, i) {
            var n = this$1.nodeHash[k];
            var d = n.data;

            if (n.type === ENTER$1) {
                n.updateType(UPDATE$1);
            }

            if (fadeChecker(d)) {
                n.updateCondition(FADE);
            }
            else if (highlights[k]) {
                n.updateCondition(HOVER);
            }
            else {
                n.updateCondition(null);
            }

        });
    };

    NodeGroup.prototype.clean = function (element) {
        delete this.nodeHash[element.id];

        if (this.cleaning) {
            return
        }
        this.cleaning = true;
        defer(this.deferredClean);
    };
    NodeGroup.prototype.deferredClean = function () {
        this.nodeKeys = keys(this.nodeHash);
        this.renderNodes();

        this.cleaning = false;
    };

    NodeGroup.prototype.renderNodes = function () {
        var this$1 = this;

        // sync animate result and vnode
        this.setState({
            nodes: this.nodeKeys.map(function (key) {
                return this$1.nodeHash[key];
            }),
        });
    };

    NodeGroup.prototype.render = function () {
        // todo,
        // 1. Render `group` here, render fn returns one level children
        // 2. Can we remove state.nodes? Use elements directly.
        return this.props.children[0](this.state.nodes, this.clean);
    };

    return NodeGroup;
}(Component));

NodeGroup$$2.defaultProps = {
    keyAccessor: function (d) { return d['..id']; },
};

/**
 * Created by Jeffrey on 2018/5/31.
 */

// set a flag
var nativeStop = function (e) {
    e = e.event || e;
    e._preventDefault = true;
};

// export const stop = typeof window !== 'undefined' && window.Event ? domStop : nativeStop;
var stop$2 = nativeStop;

var clamp$1 = clamp;

var VERTICAL   = 'vertical';

var THUMB_THICKNESS = 6;

var THUMB_RADIUS = 3;

var COLOR_BASE = '#3d4d66';
var THUMB_COLOR    = modifyAlpha(COLOR_BASE, 0.1),
      TRACK_COLOR    = modifyAlpha(COLOR_BASE, 0.03),
      ON_THUMB_COLOR = modifyAlpha(COLOR_BASE, 0.3),
      ON_TRACK_COLOR = modifyAlpha(COLOR_BASE, 0.05);

var MIN_THUMB_LENGTH = 20;

var ScrollBar = (function (Component$$1) {
    function ScrollBar (props) {
        Component$$1.call(this, props);

        this.state = {
            trackOn: false,
            thumbOn: false,
            panning: false,
        };

        this.startPos = {};

        autoBind(this);
    }

    if ( Component$$1 ) ScrollBar.__proto__ = Component$$1;
    ScrollBar.prototype = create( Component$$1 && Component$$1.prototype );
    ScrollBar.prototype.constructor = ScrollBar;

    ScrollBar.prototype.onTrackMouseOver = function () {
        this.setState({ trackOn: true });
    };

    ScrollBar.prototype.onTrackMouseOut = function () {
        this.setState({ trackOn: false });
    };

    ScrollBar.prototype.onThumbMouseOver = function (e) {
        this.setState({ thumbOn: true });
    };

    ScrollBar.prototype.onThumbMouseOut = function () {
        this.setState({ thumbOn: false });
    };

    ScrollBar.prototype.onThumbPanStart = function (e) {
        var ref = this.props;
        var onThumbPanStart = ref.onThumbPanStart;
        var pct = ref.pct;
        this.startPos = { x: e.offsetX, y: e.offsetY, pct: pct };

        this.setState({ panning: true });

        onThumbPanStart && onThumbPanStart();
    };

    ScrollBar.prototype.onThumbPan = function (e) {
        var ref = this.props;
        var direction = ref.direction; if ( direction === void 0 ) direction = VERTICAL;
        var length = ref.length;
        var realLength = ref.realLength;
        var onThumbPan = ref.onThumbPan;
        var ref$1 = this.startPos;
        var x = ref$1.x;
        var y = ref$1.y;
        var pct = ref$1.pct;
        var d = direction === VERTICAL
            ? e.offsetY - y
            : e.offsetX - x;
        var nextPct = clamp$1(
            pct + d / getThumbMovableLength(length, realLength),
            0,
            1
        );

        onThumbPan && onThumbPan(nextPct);
    };

    ScrollBar.prototype.onThumbPanEnd = function () {
        var ref = this.props;
        var onThumbPanEnd = ref.onThumbPanEnd;

        this.setState({ panning: false });

        onThumbPanEnd && onThumbPanEnd();
    };

    ScrollBar.prototype.render = function () {
        var ref = this.props;
        var silent = ref.silent;
        var direction = ref.direction; if ( direction === void 0 ) direction = VERTICAL;
        var x = ref.x; if ( x === void 0 ) x = 0;
        var y = ref.y; if ( y === void 0 ) y = 0;
        var length = ref.length; if ( length === void 0 ) length = 0;
        var realLength = ref.realLength; if ( realLength === void 0 ) realLength = 0;
        var pct = ref.pct; if ( pct === void 0 ) pct = 0;

        var ref$1 = this.state;
        var trackOn = ref$1.trackOn;
        var thumbOn = ref$1.thumbOn;
        var panning = ref$1.panning;

        var visible = this.props.visible;
        // visible when default or panning
        visible = visible == null ? true : (visible || panning);

        var isVertical  = direction === VERTICAL,
              thumbLength = getThumbLength(length, realLength),
              thumbPos    = getThumbPosByPct(length, realLength, pct);

        return h('group', {
                silent: silent
            },
            // track
            h('rect', {
                invisible  : !visible,
                zlevel     : StaticLayerZ,
                cursor     : null,
                shape      : {
                    x: x,
                    y: y,
                    width : isVertical ? THUMB_THICKNESS : length,
                    height: isVertical ? length : THUMB_THICKNESS,
                    r     : THUMB_RADIUS
                },
                style      : {
                    fill: trackOn ? ON_TRACK_COLOR : TRACK_COLOR
                },
                onmouseover: this.onTrackMouseOver,
                onmouseout : this.onTrackMouseOut
            }),
            // thumb
            h('rect', {
                invisible  : !visible,
                zlevel     : StaticLayerZ,
                pannable   : true,
                shape      : {
                    x     : isVertical ? x : thumbPos,
                    y     : isVertical ? thumbPos : y,
                    width : isVertical ? THUMB_THICKNESS : thumbLength,
                    height: isVertical ? thumbLength : THUMB_THICKNESS,
                    r     : THUMB_RADIUS
                },
                style      : {
                    fill: thumbOn || panning ? ON_THUMB_COLOR : THUMB_COLOR
                },
                onmouseover: this.onThumbMouseOver,
                onmouseout : this.onThumbMouseOut,
                onpanstart : this.onThumbPanStart,
                onpan      : this.onThumbPan,
                onpanend   : this.onThumbPanEnd,
            })
        )
    };

    return ScrollBar;
}(Component));

/**
 * calc thumb length, min = 20
 * @param length
 * @param realLength
 * @returns {number}
 */
function getThumbLength (length, realLength) {
    var l = length / realLength * length;
    return Math.max(l, MIN_THUMB_LENGTH);
}

/**
 * length that thumb can drag
 * @param length
 * @param realLength
 * @returns {number}
 */
function getThumbMovableLength (length, realLength) {
    return length - getThumbLength(length, realLength)
}

/**
 * get thumb's position according to percent
 * @param length
 * @param realLength
 * @param pct
 * @returns {number}
 */
function getThumbPosByPct (length, realLength, pct) {
    return getThumbMovableLength(length, realLength) * clamp$1(pct, 0, 1)
}

/**
 * Created by Jeffrey on 2018/1/8.
 */

var PIXEL_STEP = 10;

function normalizeWheel (e) {
    e.vanDeltaY = (e.wheelDelta) ? e.wheelDelta / 120 : -(e.detail || 0) / 3;
    if (e.wheelDeltaY) {
        e.vanDeltaY = e.wheelDeltaY / 120;
    }

    e.vanDeltaX = 0;
    if (e.wheelDeltaX) {
        e.vanDeltaX = e.wheelDeltaX / 120;
    }

    e.vanDeltaY *= PIXEL_STEP;
    e.vanDeltaX *= PIXEL_STEP;
    return e
}

var needScroll = function (length, realLength) { return length < realLength; };

var getOffsetByPct = function (length, realLength, pct) { return (length - realLength) * pct; };

var getPctByOffset = function (length, realLength, offset) { return offset / (length - realLength) || 0; };

var Background = (function (PureComponent) {
    function Background () {
        PureComponent.apply(this, arguments);
    }

    if ( PureComponent ) Background.__proto__ = PureComponent;
    Background.prototype = create( PureComponent && PureComponent.prototype );
    Background.prototype.constructor = Background;

    Background.prototype.render = function () {
        var ref = this.props;
        var width = ref.width;
        var height = ref.height;
        var borderColor = ref.borderColor;
        return h('rect', {
            cursor: null,
            shape: { x: 0, y: 0, width: width, height: height },
            style: { fill: 'transparent', stroke: borderColor }
        });
    };

    return Background;
}(PureComponent$$1));

var ControlledScrollArea = function (ref) {
    var touch = ref.touch;
    var x = ref.x; if ( x === void 0 ) x = 0;
    var y = ref.y; if ( y === void 0 ) y = 0;
    var width = ref.width;
    var height = ref.height;
    var realWidth = ref.realWidth; if ( realWidth === void 0 ) realWidth = width;
    var realHeight = ref.realHeight; if ( realHeight === void 0 ) realHeight = height;
    var paddingLeft = ref.paddingLeft; if ( paddingLeft === void 0 ) paddingLeft = 0;
    var paddingTop = ref.paddingTop; if ( paddingTop === void 0 ) paddingTop = 0;
    var paddingRight = ref.paddingRight; if ( paddingRight === void 0 ) paddingRight = 0;
    var paddingBottom = ref.paddingBottom; if ( paddingBottom === void 0 ) paddingBottom = 0;
    var pctX = ref.pctX; if ( pctX === void 0 ) pctX = 0;
    var pctY = ref.pctY; if ( pctY === void 0 ) pctY = 0;
    var borderColor = ref.borderColor;
    var visible = ref.visible;
    var handleEnter = ref.handleEnter;
    var handleLeave = ref.handleLeave;
    var handleMousewheel = ref.handleMousewheel;
    var handleThumbPanV = ref.handleThumbPanV;
    var handleThumbPanH = ref.handleThumbPanH;
    var handleTouchStart = ref.handleTouchStart;
    var handleTouchMove = ref.handleTouchMove;
    var handleTouchEnd = ref.handleTouchEnd;
    var children = ref.children;


    var innerWidth = width - paddingLeft - paddingRight;
    var innerHeight = height - paddingTop - paddingBottom;

    var needScrollH = needScroll(innerWidth, realWidth);
    var needScrollV = needScroll(innerHeight, realHeight);

    // give a simple group if no scroll
    if (!(needScrollH || needScrollV)) {
        return h('group', {
                position: [x, y]
            },
            h(Background, { width: width, height: height, borderColor: borderColor }),

            // todo clip not remove ????
            h("group",
                {
                    clip: {
                        type: "rect",
                        shape: {
                            x: 0,
                            y: 0,
                            width: innerWidth,
                            height: innerHeight
                        }
                    }
                },
                children
            )
        )
    }

    var offsetX = getOffsetByPct(innerWidth, realWidth, pctX) || 0;
    var offsetY = getOffsetByPct(innerHeight, realHeight, pctY) || 0;

    // move wrapper to scroll
    var wrapper =
        h('group', {
            // pure: true,
            clip: {
                type: 'rect',
                shape: {
                    x: paddingLeft - offsetX,
                    y: paddingTop - offsetY,
                    width: innerWidth,
                    height: innerHeight
                }
            },
            position: [
                paddingLeft + offsetX,
                paddingTop + offsetY
            ]
        },
            // h('rect', {
            //     silent: true,
            //     shape: {
            //         x: paddingLeft,
            //         y: paddingTop,
            //         width: innerWidth,
            //         height: innerHeight
            //     },
            //     style: {
            //         fill: 'rgba(0, 200, 200, 0.5)'
            //     }
            // }),
            children
        );

    return h('group', {
        // pure: true,  // pure will cause position not update
        position: [x, y], // outer leftTop
        onmousewheel: handleMousewheel,
        onmouseover: handleEnter,
        onmouseout: handleLeave,

        ontouchstart: handleTouchStart,
        onpan: handleTouchMove,
        onpanend: handleTouchEnd
    },
        h(Background, { width: width, height: height, borderColor: borderColor }),

        // main area
        wrapper,

        needScrollV && h(ScrollBar, {
            silent: touch,
            direction: 'vertical',
            x: width - THUMB_THICKNESS,
            y: paddingTop,
            length: innerHeight,
            realLength: realHeight,
            pct: pctY,
            visible: visible,
            onThumbPan: handleThumbPanV
        }),

        needScrollH && h(ScrollBar, {
            silent: touch,
            direction: 'horizontal',
            x: paddingLeft,
            y: height - THUMB_THICKNESS,
            length: innerWidth,
            realLength: realWidth,
            pct: pctX,
            visible: visible,
            onThumbPan: handleThumbPanH
        })
    );
};

ControlledScrollArea.propTypes = {
    touch: propTypes.bool,
    x: propTypes.number,
    y: propTypes.number,
    width: propTypes.number.isRequired,
    height: propTypes.number.isRequired,
    realWidth: propTypes.number,
    realHeight: propTypes.number,
    paddingLeft: propTypes.number,
    paddingTop: propTypes.number,
    paddingRight: propTypes.number,
    paddingBottom: propTypes.number,
    pctX: propTypes.number,
    pctY: propTypes.number,
    borderColor: propTypes.string,
    visible: propTypes.bool,
    handleEnter: propTypes.func,
    handleLeave: propTypes.func,
    handleMousewheel: propTypes.func,
    handleThumbPanV: propTypes.func,
    handleThumbPanH: propTypes.func,
    handleTouchMove: propTypes.func,
    handleTouchEnd: propTypes.func,
    children: propTypes.array,
};

var ScrollArea = (function (PureComponent) {
    function ScrollArea (props) {
        PureComponent.call(this, props);

        this.state = {
            visible: false,
            pctX: 0,
            pctY: 0
        };

        this.moved = false;

        autoBind(this);
    }

    if ( PureComponent ) ScrollArea.__proto__ = PureComponent;
    ScrollArea.prototype = create( PureComponent && PureComponent.prototype );
    ScrollArea.prototype.constructor = ScrollArea;

    ScrollArea.prototype.onHover = function (enterOrLeave) {
        this.setState({
            visible: enterOrLeave
        });
    };

    ScrollArea.prototype.onEnter = function () {
        this.onHover(true);
    };

    ScrollArea.prototype.onLeave = function () {
        this.onHover(false);
    };

    ScrollArea.prototype.onMove = function (e, d) {

        var ref = d || e;
        var deltaX = ref.deltaX;
        var deltaY = ref.deltaY;

        var ref$1 = this.props;
        var width = ref$1.width;
        var realWidth = ref$1.realWidth;
        var height = ref$1.height;
        var realHeight = ref$1.realHeight;
        var ref$2 = this.state;
        var pctX = ref$2.pctX;
        var pctY = ref$2.pctY;

        var nextPctX = pctX + getPctByOffset(width, realWidth, deltaX);
        var nextPctY = pctY + getPctByOffset(height, realHeight, deltaY);

        var nextState = {};
        if (0 <= nextPctX && nextPctX <= 1) {
            nextState.pctX = nextPctX;
        }
        if (0 <= nextPctY && nextPctY <= 1) {
            nextState.pctY = nextPctY;
        }

        if (nextState.pctX || nextState.pctY) {
            // stop(e);
            nextState.visible = true;
            this.setState(nextState);
        } else {
            // console.log('default', nextPctX, nextPctY);
            // default scroll
            // if (this.moved) {
            //     stop(e);
            // }
        }

        if (deltaX || deltaY) {
            this.moved = true;
        }
    };

    ScrollArea.prototype.onMousewheel = function (e) {
        normalizeWheel(e.event);
        this.onMove(e.event, { deltaX: e.event.vanDeltaX, deltaY: e.event.vanDeltaY });
    };

    ScrollArea.prototype.onTouchStart = function (e) {
        // todo, handle conflict
        stop$2(e);
        // this.onMove(e.event, { deltaX: 0, deltaY: 0 });
    };

    ScrollArea.prototype.onTouchMove = function (e) {
        this.onMove(e.event);
    };

    ScrollArea.prototype.onTouchEnd = function () {
        this.moved = false;
        this.onHover(false);
    };

    ScrollArea.prototype.onThumbPanV = function (pctY) {
        this.setState({ pctY: pctY });
    };

    ScrollArea.prototype.onThumbPanH = function (pctX) {
        this.setState({ pctX: pctX });
    };

    ScrollArea.prototype.render = function (ref) {
        var touch = ref.touch;

        return h(ControlledScrollArea, assign({}, this.props,
            this.state,
            {handleEnter: this.onEnter,
            handleLeave: this.onLeave,
            handleMousewheel: this.onMousewheel,
            handleThumbPanV: this.onThumbPanV,
            handleThumbPanH: this.onThumbPanH,
            handleTouchStart: this.onTouchStart,
            handleTouchMove: this.onTouchMove,
            handleTouchEnd: this.onTouchEnd}))
    };

    return ScrollArea;
}(PureComponent$$1));

ScrollArea.THUMB_THICKNESS = THUMB_THICKNESS;

/**
 * Created by Jeffrey on 2017/11/13.
 */

// import '../src/visible/debug';

// const chartDom = $$('#app')[0];

var genData = function (n) { return Array.from({ length: n }, function () { return Math.random(); }); };

var App = (function (Component$$1) {
    function App (props, context) {
        Component$$1.call(this, props, context);

        this.state = {
            color: null,
            offsetX: null,
            offsetY: null,
            type: null,
            gestureEvent: null,
            size: 10,
        };

        autoBind(this);
    }

    if ( Component$$1 ) App.__proto__ = Component$$1;
    App.prototype = create( Component$$1 && Component$$1.prototype );
    App.prototype.constructor = App;

    App.prototype.onevent = function (e) {
        this.setState(assign({}, e));
    };

    App.prototype.ontouchstart = function (e) {
        this.setState(assign({}, {color: '#00aeff'},
            e));
    };

    App.prototype.ontouchmove = function (e) {
        this.setState(assign({}, {color: '#eec519'},
            e));
    };

    App.prototype.ontouchend = function (e) {
        this.setState(assign({}, {color: null},
            e));
    };

    App.prototype.onpinch = function (e) {
        console.log(e);
        this.setState(assign({}, {size: this.state.size * e.event.zoom},
            e));
    };

    App.prototype.render = function (ref) {
        var data = ref.data;
        var height = ref.height; if ( height === void 0 ) height = 400;
        var width = ref.width; if ( width === void 0 ) width = 300;
        var barWidth = ref.barWidth; if ( barWidth === void 0 ) barWidth = 20;
        var realHeight = ref.realHeight; if ( realHeight === void 0 ) realHeight = 600;
        var indicatorSize = ref.indicatorSize; if ( indicatorSize === void 0 ) indicatorSize = 40;


        var ref$1 = this.state;
        var color = ref$1.color;
        var x = ref$1.offsetX;
        var y = ref$1.offsetY;
        var type = ref$1.type;
        var gestureEvent = ref$1.gestureEvent;
        var size = ref$1.size;

        return h('group', {
            ontouchstart: this.ontouchstart,
            ontouchmove: this.ontouchmove,
            ontouchend: this.ontouchend,

            onrotate: this.onevent,
            onmultipointstart: this.onevent,
            onmultipointend: this.onevent,
            onswipe: this.onevent,
            ontap: this.onevent,
            ondoubletap: this.onevent,
            onlongtap: this.onevent,
            onsingletap: this.onevent,
            onpressmove: this.onevent,
            onpan: this.onevent,
            onpanend: this.onevent,
            ontwofingerpressmove: this.onevent,

            onpinch: this.onpinch,
        },
            h('rect', {
                shape: {
                    x: width / 2,
                    y: height / 2,
                    width: size,
                    height: size
                },
                style: { fill: '#0b9' }
            }),

            // event state indicator
            h('rect', {
                shape: {
                    x: x - indicatorSize / 2,
                    y: y - indicatorSize / 2,
                    width: indicatorSize,
                    height: indicatorSize
                },
                style: { fill: color }
            }),

            h('rect', {
                silent: true,
                position: [10, 10],
                z: 10,
                style: {
                    text: (gestureEvent + ": " + x + ", " + y),
                    fontSize: 12,
                    textAlign: 'left',
                }
            }),

            h(ScrollArea, {
                x: 10,
                y: 10,
                width: width - 20,
                height: height - 20,
                realHeight: realHeight,
                borderColor: '#ccc'
            }, data.map(function (d$$1, i) { return h('rect', {
                shape: {
                    x: 0,
                    y: i * (realHeight / 20),
                    height: barWidth,
                    width: d$$1 * 100
                },
                style: { fill: '#0bd' }
            }); }))
        )
    };

    return App;
}(Component));

// const zr = render(h(App, {
//     data: genData(20)
// }), chartDom, {
//     touch: 'native'
// });
//
// window.dispatchEvent = function dispatchEvent (e) {
//     return zr.handler.proxy.dispatch(e);
// };

function VanMobileExp (dom, props) {
    var zr = render(h(App, assign({}, {data: genData(20)},
        props)), dom, {
        touch: 'native'
    });

    return {
        zr: zr,
        dispatchEvent: function (e) {
            return zr.handler.proxy.dispatch(e);
        }
    }
}

VanMobileExp.init = function () {
    var dom = $$('#app')[0];
    dom.style.cssText = "\n    width: " + (400) + "px;\n    height: " + (1000) + "px;\n    border: 1px solid #ccc;\n    ";
    var chart = VanMobileExp(dom);
    window.dispatchEvent = function (type, x, y) {
        return chart.dispatchEvent({
            type: type,
            zrX: x,
            zrY: y,
            touches: [{pageX: x, pageY: y}]
        });
    };
};

return VanMobileExp;

})));
//# sourceMappingURL=mobile-test.js.map

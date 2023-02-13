var fs = require("fs");

/** Custom Error: RuntimeError */
function RuntimeError(message) {
  this.name = "RuntimeError";
  this.message = message || "Error running the current script";
  this.stack = new Error().stack;
}

var _f = function () {};
_f.prototype = Error.prototype;
RuntimeError.prototype = new _f();
RuntimeError.prototype.constructor = RuntimeError;
/** */

if (!fs.existsSync("./gui.dll")) {
  throw new RuntimeError('Cannot find dll library file "gui.dll"');
}

var nativeBinding = require("./CrashpadGui.node");

/**
 * Initialize the gui app
 * @returns {boolean} - indicate whether the gui app is initialized
 */
var InitializeApp = function () {
  return nativeBinding.InitializeApp();
};

/**
 * Register your callback function to the button click event, must call before app is initialized
 * @param {function} callback - the callback function be called after the button is clicked
 */
var RegisterButtonCallback = function (callback) {
  nativeBinding.RegisterButtonCallback(callback);
};

var interval = null;
/**
 * Render the gui app frame, must call after the app is initialized
 * @param {function} onClose - callback function to call when the gui app is closing
 */
var RenderFrame = function (onClose) {
  if (!interval) {
    interval = setInterval(function () {
      var message = nativeBinding.RenderFrame();
      if (message != -1 && message == 161) {
        clearInterval(interval);
        nativeBinding.ReleaseBtnCallback();
        typeof onClose == "function" && onClose();
      }
    }, 10);

    setTimeout(function () {
      nativeBinding.ShowAndUpdateWindow();
    }, 20);
  }
};

var ReleaseBtnCallback = function () {
  nativeBinding.ReleaseBtnCallback();
};

module.exports = {
  InitializeApp: InitializeApp,
  RenderFrame: RenderFrame,
  RegisterButtonCallback: RegisterButtonCallback,
};

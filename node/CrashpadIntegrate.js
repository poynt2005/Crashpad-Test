var path = require("path");
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

/**
 * Class which setup a crashpad handler
 * @param {string} executablePath - path to crashpad_handler.exe
 * @param {string} dumpPath - directory path to dump file stored
 */
function CrashpadIntegrate(executablePath, dumpPath) {
  if (!fs.existsSync("integrateCrashpad.dll")) {
    throw new RuntimeError(
      'Cannot find dll library file "integrateCrashpad.dll"'
    );
  }

  var nativeBinding = require("./CrashpadIntegrate.node");
  this.getNativeBinding = function () {
    var allowedCaller = Object.keys(this.__proto__).map(
      function (fnName) {
        return this.__proto__[fnName];
      }.bind(this)
    );
    allowedCaller.push(this.__proto__.constructor);

    if (allowedCaller.indexOf(arguments.callee.caller) < 0) {
      return null;
    }
    return nativeBinding;
  }.bind(this);

  if (CrashpadIntegrate.__instance) {
    return CrashpadIntegrate.__instance;
  }

  this.executablePath = executablePath;
  this.dumpPath = dumpPath;

  if (!this.executablePath) {
    this.executablePath = path.resolve("./crashpad/bin/crashpad_handler.exe");
  }

  if (!this.dumpPath) {
    this.dumpPath = path.resolve("dump");

    if (!fs.existsSync(this.dumpPath)) {
      fs.mkdirSync(this.dumpPath);
    }
  }

  this.getNativeBinding().NewIntegrateCrashpadInstance(
    this.executablePath,
    this.dumpPath
  );

  this.isInitialized = false;
  this.isStart = false;

  CrashpadIntegrate.__instance = this;
}

/**
 * SetProductName - Set your product name, default: UnknownWinProduct
 * @param {string} name -  your product name
 */
CrashpadIntegrate.prototype.SetProductName = function (name) {
  this.getNativeBinding().SetPropertyValue("productName", name);
};

/**
 * Set whether the product is in development, default: false
 * @param {boolean} isDev -  in development
 */
CrashpadIntegrate.prototype.SetIsDevlopement = function (isDev) {
  this.getNativeBinding().SetPropertyValue("isDevlopement", !!isDev);
};

/**
 * Set your product version, default 1.0.0
 * @param {string} version - product version
 */
CrashpadIntegrate.prototype.SetVersion = function (version) {
  this.getNativeBinding().SetPropertyValue("version", version);
};

/**
 * Set your product process type, default WindowsExecutableFile
 * @param {string} version - product process type
 */
CrashpadIntegrate.prototype.SetProcessType = function (type) {
  this.getNativeBinding().SetPropertyValue("processType", type);
};

/**
 * Set the dump file server host that you want to upload to
 * @param {string} url - dump file server host
 */
CrashpadIntegrate.prototype.SetServerURL = function (url) {
  this.getNativeBinding().SetPropertyValue("serverURL", url);
};

/**
 * Set upload to server or not, default false
 * @param {*} isUploadToServer - upload to server or not
 */
CrashpadIntegrate.prototype.SetIsUploadToServer = function (isUploadToServer) {
  this.getNativeBinding().SetPropertyValue(
    "isUploadToServer",
    !!isUploadToServer
  );
};

/**
 * Add crash handler start argument
 * @param {string} arg - crash handler argument
 */
CrashpadIntegrate.prototype.AddCrashpadArgs = function (arg) {
  this.getNativeBinding().AddCrashpadArgs(arg);
};

/**
 * Initialize Crashpad, call this method after you set all arguments
 */
CrashpadIntegrate.prototype.InitializeCrashpad = function () {
  this.isInitialized = this.getNativeBinding().InitializeCrashpad();
};

/**
 * Start Crashpad, call this method after you Initialized Crashpad
 */
CrashpadIntegrate.prototype.Start = function () {
  this.isStart = this.getNativeBinding().StartCrashpad();
};

/**
 * Make a c++ crash manually
 */
CrashpadIntegrate.prototype.Crash = function () {
  this.getNativeBinding().MakeCrash();
};

module.exports = CrashpadIntegrate;

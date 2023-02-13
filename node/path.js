var path = require("path");
var crashpadLib = path
  .resolve("../integrateCrashpad.lib")
  .replace(/\\/gm, "\\\\");

var crashpadIncl = path.resolve("../crashpad/include").replace(/\\/gm, "\\\\");

var miniChromiumIncl = path
  .resolve("../crashpad/include/mini_chromium")
  .replace(/\\/gm, "\\\\");

module.exports = {
  crashpadLib: crashpadLib,
  crashpadIncl: crashpadIncl,
  miniChromiumIncl: miniChromiumIncl,
};

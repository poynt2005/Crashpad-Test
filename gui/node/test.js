var gui = require("./CrashpadGui");

var counter = 0;
gui.RegisterButtonCallback(function () {
  console.log(++counter);
});
gui.InitializeApp();
gui.RenderFrame();

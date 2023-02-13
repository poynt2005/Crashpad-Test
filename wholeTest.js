var Crashpad = require("./CrashpadIntegrate");
var gui = require("./CrashpadGui");

var cp = new Crashpad("../crashpad/bin/crashpad_handler.exe", "./dump");
cp.SetServerURL("http://127.0.0.1:7000/crashdump");
cp.SetIsUploadToServer(true);
cp.SetProductName("crashGUITest");
cp.InitializeCrashpad();
cp.Start();

gui.RegisterButtonCallback(function () {
  cp.Crash();
});
gui.InitializeApp();
gui.RenderFrame();

var Crashpad = require("./CrashpadIntegrate");

var cp = new Crashpad("../crashpad/bin/crashpad_handler.exe", "./dump");

cp.SetServerURL("http://127.0.0.1:7000/crashdump");
cp.SetIsUploadToServer(true);
cp.SetProductName("my_test_js");
cp.InitializeCrashpad();
cp.Start();

console.log("Crashpad initialized");

// cp.Crash();

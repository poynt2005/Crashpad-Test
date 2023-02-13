{
  "targets": [
    {
      "target_name": "CrashpadIntegrate",
      "cflags!": [ "-fno-exceptions"],
      "cflags_cc!": [ "-fno-exceptions"],
      "sources": [ "NodeBinding.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(node -p \"require('./path.js').crashpadIncl\")",
        "<!@(node -p \"require('./path.js').miniChromiumIncl\")",
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'libraries': ["<!@(node -p \"require('./path.js').crashpadLib\")"]
    }
  ]
}
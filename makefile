cc = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe"
ldflags = /LIBPATH:./ /LIBPATH:./crashpad/lib_md /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\lib\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.20348.0\ucrt\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.20348.0\um\x64"
cflags = /I./crashpad/include /I./crashpad./include./mini_chromium /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\shared" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\winrt" /I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\include"
libs = base.lib client.lib common.lib util.lib advapi32.lib Rpcrt4.lib infoUtility.lib

liber = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\lib.exe"

dll:
	$(cc) /DBUILDCPAPI /MD /std:c++17 $(cflags) /c integrateCrashpad.cc /Fo:integrateCrashpad.obj
	$(cc) /LD integrateCrashpad.obj /link $(ldflags) $(libs) /OUT:integrateCrashpad.dll /IMPLIB:integrateCrashpad.lib
	-del integrateCrashpad.obj integrateCrashpad.exp

test:
	$(cc) /MD /std:c++17 $(cflags) /c test.cc /Fo:test.obj
	$(cc) test.obj /link $(ldflags) $(libs) integrateCrashpad.lib /OUT:test.exe
	-del test.obj test.exp

util:
	$(cc) /MD /std:c++17 $(cflags) /c infoUtility.cc /Fo:infoUtility.obj
	$(liber) infoUtility.obj
	-del infoUtility.obj

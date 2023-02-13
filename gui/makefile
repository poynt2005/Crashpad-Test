cc = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe"
ldflags = /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\lib\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.20348.0\ucrt\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.20348.0\um\x64"
cflags = /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\shared" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\winrt" /I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\include"
libs = 	User32.lib Gdi32.lib

dll:
	$(cc) $(cflags) /DBUILDGUIAPI /std:c++17 /c gui.cc /Fo:gui.obj
	$(cc) /LD gui.obj /link $(ldflags) $(libs) /OUT:gui.dll /IMPLIB:gui.lib
	-del gui.obj gui.exp

test:
	$(cc) $(cflags) /c test.cc /Fo:test.obj
	$(cc) test.obj /link $(ldflags) /LIBPATH:./ $(libs) gui.lib /OUT:test.exe
	-del test.obj
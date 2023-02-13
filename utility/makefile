cc = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe"
ldflags = /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\lib\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.20348.0\ucrt\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.20348.0\um\x64"
cflags = /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\shared" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\winrt" /I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\include"
libs = Urlmon.lib 

pythoncflags = /I"C:\Users\SteveChung\AppData\Local\Programs\Python\Python310\include"
pythonldflags = /LIBPATH:"C:\Users\SteveChung\AppData\Local\Programs\Python\Python310\libs"

build: 
	$(cc) $(cflags) /std:c++17 /c $(project).cc /Fo:$(project).obj
	$(cc) $(project).obj /link $(ldflags) $(libs) /OUT:$(project).exe
	-del $(project).obj

pybuild: 
	$(cc) $(cflags) $(pythoncflags) /std:c++17 /c $(project).cc /Fo:$(project).obj
	$(cc) $(project).obj /link $(ldflags) $(pythonldflags) $(libs) python310.lib /OUT:$(project).exe
	-del $(project).obj


	
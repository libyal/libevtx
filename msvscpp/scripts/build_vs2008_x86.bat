@echo off
set python_lib_dir=C:\Python27\Lib\site-packages
if exist C:\Program Files\Microsoft Visual Studio 9.0\VC\bin C:\Program Files\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat
if exist C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat
msbuild msvscpp\libevtx.sln /p:Configuration=Release;Platform=Win32
if exist %python_lib_dir% (
	copy /y msvscpp\Release\libevtx.dll %python_lib_dir%
	copy /y msvscpp\Release\pyevtx.pyd %python_lib_dir%
)

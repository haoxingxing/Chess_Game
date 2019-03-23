@echo off
title qmake and nmake build prompt
cd server
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC
set QTDIR=D:\Software\Qt\5.12.0\msvc_static\
set PATH=%VCINSTALLDIR%\bin;%QTDIR%\bin;C:\Program Files\7-Zip;%PATH%
call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" amd64
qmake server.pro -spec win32-msvc "CONFIG+=debug" "CONFIG+=qml_debug"
nmake
cd debug
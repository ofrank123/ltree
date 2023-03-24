@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
if not exist "out" mkdir "out\"
pushd out\
cl /Zi /EHsc /I "..\extern\include" ..\ltree.cpp SDL2.lib /link /libpath:..\extern\lib
popd

@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
if not exist "out" mkdir "out\"
pushd out\
cl /Zi /EHsc /std:c++20 /I "..\extern\include" ..\ltree.cpp SDL2.lib SDL2_ttf.lib /link /libpath:..\extern\lib
popd

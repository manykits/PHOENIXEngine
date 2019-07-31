@echo off

: dir
cd /d %cd%
del "SampleToLua.cpp"
: tolua
tolua++.exe -n Sample -o SampleToLua.cpp PX2ToScript.pkg

copy SampleToLua.cpp ..\\

pause
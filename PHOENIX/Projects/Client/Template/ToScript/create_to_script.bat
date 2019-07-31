@echo off

: dir
cd /d %cd%
del "ProjectToLua.cpp"
: tolua
tolua++.exe -n Project -o ProjectToLua.cpp PX2ToScript.pkg

copy ProjectToLua.cpp ..\\

pause
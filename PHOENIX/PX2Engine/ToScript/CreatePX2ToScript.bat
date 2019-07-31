@echo off

: dir
cd /d %cd%
del "PX2ToLua.cpp"
: tolua
tolua++.exe -n PX2 -o PX2ToLua.cpp PX2ToScript.pkg

..\\..\\..\\Bin\\PkgParser.exe PX2ToScript.pkg pkg.cpp
xcopy /DY pkg.xml ..\\..\\..\\Bin\\Data\\engine\\pkg

pause
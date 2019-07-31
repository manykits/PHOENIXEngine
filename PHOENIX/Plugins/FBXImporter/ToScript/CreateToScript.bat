@echo off

: dir
cd /d %cd%
del "FBXImporterToLua.cpp"
: tolua
tolua++.exe -n FBXImporter -o FBXImporterToLua.cpp PX2ToScript.pkg

copy FBXImporterToLua.cpp ..\\

pause
msbuild PHOENIX.sln /t:Clean
MSBuild.exe PHOENIX.sln /p:Configuration=Release /p:Platform="Win32"
MSBuild.exe PHOENIX.sln /p:Configuration=OpenGLRelease /p:Platform="Win32"
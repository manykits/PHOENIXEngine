@echo off

:: SVN
cd ..
svn update

:: Achieve
cd ./Achieve

::PHONEIXEngine Dir
rd PHOENIXEngine /s /q
mkdir PHOENIXEngine

:: Engine
XCOPY ..\Bin\PX2EngineOpenGL.dll PHOENIXEngine\ /s

::locales
XCOPY ..\Bin\locales\* PHOENIXEngine\locales\ /s

::plugins
XCOPY ..\Bin\plugins\* PHOENIXEngine\plugins\ /s

::PluginsCommon
XCOPY ..\Bin\PluginsCommon\* PHOENIXEngine\PluginsCommon\ /s

::PluginsTool
XCOPY ..\Bin\PluginsTool\Max2012\Win32\Release\* PHOENIXEngine\PluginsTool\Max2012\Win32\Release\ /s

:: dll
XCOPY ..\Bin\fmod.dll PHOENIXEngine\ /s

XCOPY ..\Bin\astra.dll PHOENIXEngine\ /s
XCOPY ..\Bin\astra.toml PHOENIXEngine\ /s

XCOPY ..\Bin\astra_core.dll PHOENIXEngine\ /s
XCOPY ..\Bin\astra_core_api.dll PHOENIXEngine\ /s
XCOPY ..\Bin\AstraDotNet.dll PHOENIXEngine\ /s
XCOPY ..\Bin\avcodec-53.dll PHOENIXEngine\ /s
XCOPY ..\Bin\avformat-53.dll PHOENIXEngine\ /s
XCOPY ..\Bin\avutil-51.dll PHOENIXEngine\ /s

XCOPY ..\Bin\awesomium.dll PHOENIXEngine\ /s
XCOPY ..\Bin\awesomium_pak_utility.exe PHOENIXEngine\ /s
XCOPY ..\Bin\awesomium_process.exe PHOENIXEngine\ /s

XCOPY ..\Bin\cef.pak PHOENIXEngine\ /s
XCOPY ..\Bin\cef_100_percent.pak PHOENIXEngine\ /s
XCOPY ..\Bin\cef_200_percent.pak PHOENIXEngine\ /s
XCOPY ..\Bin\cef_extensions.pak PHOENIXEngine\ /s
XCOPY ..\Bin\chrome_elf.dll PHOENIXEngine\ /s
XCOPY ..\Bin\d3dcompiler_43.dll PHOENIXEngine\ /s
XCOPY ..\Bin\d3dcompiler_47.dll PHOENIXEngine\ /s
XCOPY ..\Bin\devtools_resources.pak PHOENIXEngine\ /s
XCOPY ..\Bin\icudt.dll PHOENIXEngine\ /s
XCOPY ..\Bin\icudtl.dll PHOENIXEngine\ /s
XCOPY ..\Bin\Inconsolata.dll PHOENIXEngine\ /s
XCOPY ..\Bin\libcef.dll PHOENIXEngine\ /s
XCOPY ..\Bin\libEGL.dll PHOENIXEngine\ /s
XCOPY ..\Bin\libfbxsdk.dll PHOENIXEngine\ /s
XCOPY ..\Bin\libGLESv2.dll PHOENIXEngine\ /s
XCOPY ..\Bin\libvlc.dll PHOENIXEngine\ /s
XCOPY ..\Bin\libvlccore.dll PHOENIXEngine\ /s
XCOPY ..\Bin\msvcp120.dll PHOENIXEngine\ /s
XCOPY ..\Bin\msvcr120.dll PHOENIXEngine\ /s
XCOPY ..\Bin\natives_blob.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_core340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_core340d.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_highgui340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_imgcodecs340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_imgproc340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_objdetect340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_photo340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_video340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\opencv_videoio340.bin PHOENIXEngine\ /s
XCOPY ..\Bin\OpenNI.ini PHOENIXEngine\ /s
XCOPY ..\Bin\OpenNI2.dll PHOENIXEngine\ /s

XCOPY ..\Bin\sfml-graphics-2.dll PHOENIXEngine\ /s
XCOPY ..\Bin\sfml-system-2.dll PHOENIXEngine\ /s
XCOPY ..\Bin\sfml-window-2.dll PHOENIXEngine\ /s

XCOPY ..\Bin\snapshot_blob.bin PHOENIXEngine\ /s

XCOPY ..\Bin\vlcvideo.sdp PHOENIXEngine\ /s

XCOPY ..\Bin\xinput9_1_0.dll PHOENIXEngine\ /s

:: appplayer
XCOPY ..\Bin\AppPlayerOpenGL.exe PHOENIXEngine\ /s

:: editor
::XCOPY ..\Bin\NIRVANAwxOpenGL.exe PHOENIXEngine\ /s
::XCOPY ..\Bin\NIRVANAwx.exe PHOENIXEngine\ /s
::XCOPY ..\Bin\PX2Editor.dll PHOENIXEngine\ /s
::XCOPY ..\Bin\PX2EditorOpenGL.dll PHOENIXEngine\ /s
::XCOPY ..\Bin\NIRVANAwx.layout.1.0.xml PHOENIXEngine\ /s

:: tools
XCOPY ..\Bin\PkgParser.exe PHOENIXEngine\ /s
XCOPY ..\Bin\FxCompiler.exe PHOENIXEngine\ /s

:: data
XCOPY ..\Bin\Data\boost.list PHOENIXEngine\Data\ /s
XCOPY ..\Bin\Data\boost.lua PHOENIXEngine\Data\ /s
XCOPY ..\Bin\Data\boost.xml PHOENIXEngine\Data\ /s
XCOPY ..\Bin\Data\engine\* PHOENIXEngine\Data\engine\ /s
XCOPY ..\Bin\Data\engine_mtls\* PHOENIXEngine\Data\engine_mtls\ /s
XCOPY ..\Bin\Data\EntryBoard\* PHOENIXEngine\Data\EntryBoard\ /s
XCOPY ..\Bin\Data\ZERONE\* PHOENIXEngine\Data\ZERONE\ /s
XCOPY ..\Bin\Data\SampleLua\* PHOENIXEngine\Data\SampleLua\ /s
XCOPY ..\Bin\Data\LPlugin\* PHOENIXEngine\Data\LPlugin\ /s

:: data editor
::XCOPY ..\Bin\DataNIRVANAwx\* PHOENIXEngine\DataNIRVANAwx\ /s

:: launcher
XCOPY ..\Bin\LauncherOpenGL.exe PHOENIXEngine\ /s
XCOPY ..\Bin\Write_Launcher\* PHOENIXEngine\Write_Launcher\ /s

:: updater
XCOPY ..\Bin\PX2EngineOpenGL.dll PHOENIXEngine\Updater\ /s
XCOPY ..\Bin\Updater\UpdaterOpenGL.exe PHOENIXEngine\Updater\ /s

XCOPY ..\Bin\fmod.dll PHOENIXEngine\Updater\ /s
XCOPY ..\Bin\awesomium.dll PHOENIXEngine\Updater\ /s
XCOPY ..\Bin\xinput9_1_0.dll PHOENIXEngine\Updater\ /s
XCOPY ..\Bin\libvlc.dll PHOENIXEngine\Updater\ /s
XCOPY ..\Bin\libvlccore.dll PHOENIXEngine\Updater\ /s

:: gen infos
PHOENIXEngine\Updater\UpdaterOpenGL.exe genfilelist PHOENIXEngine updatefilelist.xml
PHOENIXEngine\Updater\UpdaterOpenGL.exe genfilelist PHOENIXEngine/Updater updatefilelist.xml

:: set fDate=%date:~0,4%%date:~5,2%%date:~8,2%
:: set bTime=%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
::.\7z.exe a -t7z .\PHOENIXEngine_%fDate%_%bTime%.7z  .\PHOENIXEngine\* -r
:: .\7z.exe a -t7z .\PHOENIXEngine.7z  .\PHOENIXEngine\* -r

pause
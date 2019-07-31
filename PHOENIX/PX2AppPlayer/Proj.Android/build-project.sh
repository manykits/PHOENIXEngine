#!/usr/bin/env bash
# build.sh

# set params
NDK_ROOT_LOCAL=/cygdrive/D/Android/android-ndk-r8e
ENGINE_ROOT_LOCAL=/cygdrive/F/GitHub/PHOENIXEngine/PHOENIX
APPPLAY_MYAPP_BIN_ROOT_LOCAL=/cygdrive/F/GitHub/PHOENIXEngine/Bin
APPPLAY_MYAPP_DATAFROM=Data

if [ $NDK_ROOT"xyz" != "xyz" ]; then
	echo "use global definition of NDK_ROOT: $NDK_ROOT"
	NDK_ROOT_LOCAL=$NDK_ROOT
fi

if [ $PX2_ROOT"xyz" != "xyz" ]; then
	echo "use global definition of PX2_ROOT: $PX2_ROOT"
	ENGINE_ROOT_LOCAL=$PX2_ROOT
fi

if [ $APPPLAY_MYAPP_BIN_ROOT"xyz" != "xyz" ]; then
	echo "use global definition of APPPLAY_MYAPP_BIN_ROOT: $APPPLAY_MYAPP_BIN_ROOT"
	APPPLAY_MYAPP_BIN_ROOT_LOCAL=$APPPLAY_MYAPP_BIN_ROOT
fi

#project params
PLUGIN_NAME=None

#appplay params
APPPLAY_MYAPP_ANDROID_ROOT=$ENGINE_ROOT_LOCAL/PX2AppPlayer/Proj.Android

#clear and make dir
if [ -d $APPPLAY_MYAPP_ANDROID_ROOT/assets ]; then
	rm -rf $APPPLAY_MYAPP_ANDROID_ROOT/assets
fi
mkdir $APPPLAY_MYAPP_ANDROID_ROOT/assets
mkdir $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data
mkdir $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/engine
mkdir $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/engine_mtls

if [ $PLUGIN_NAME"xyz" != "Nonexyz" ]; then
	mkdir $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/$PLUGIN_NAME
fi

# copy resources

# boost.lua boost.xml version.xml versionList.dat
cp -rf $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/boost.xml $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data
cp -rf $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/boost.lua $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data
cp -rf $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/boost.list $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data
cp -rf $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/project.list $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data
cp -rf $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/plugin.list $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data

# engine
for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/engine/*
do
if [ -d $file ]; then
cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/engine
fi
if [ -f $file ]; then
cp $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/engine
fi
done

# engine appicon
for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/engine/appinfo/appicon/*
do
if [ -d $file ]; then
cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/res
fi
if [ -f $file ]; then
cp $file $APPPLAY_MYAPP_ANDROID_ROOT/res
fi
done

# engine values
for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/engine/appinfo/values/*
do
if [ -d $file ]; then
cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/res/values
fi
if [ -f $file ]; then
cp $file $APPPLAY_MYAPP_ANDROID_ROOT/res/values
fi
done

# engine_mtls
for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/engine_mtls/*
do
if [ -d $file ]; then
cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/engine_mtls
fi
if [ -f $file ]; then
cp $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/engine_mtls
fi
done

# plugins
cat $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/plugin.list | while read line
do
	CATA_NAME=$line
	if [ $CATA_NAME"xyz" != "xyz" ]; then
		mkdir $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/$CATA_NAME
	fi	
done
cat $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/plugin.list | while read line
do
	CATA_NAME=$line
	echo $CATA_NAME
	if [ $CATA_NAME"xyz" != "xyz" ]; then
		for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/$CATA_NAME/*
		do
			if [ -d $file ]; then
				cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/$CATA_NAME
			fi
			if [ -f $file ]; then
				cp $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/$CATA_NAME
			fi
		done
	fi
done

# projects
cat $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/project.list | while read line
do
	CATA_NAME=$line
	if [ $CATA_NAME"xyz" != "xyz" ]; then
		mkdir $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/$CATA_NAME
	fi
done
cat $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/project.list | while read line
do
	CATA_NAME=$line
	echo $CATA_NAME
	if [ $CATA_NAME"xyz" != "xyz" ]; then
		for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/$CATA_NAME/*
		do
			if [ -d $file ]; then
				cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/$CATA_NAME
			fi
			if [ -f $file ]; then
				cp $file $APPPLAY_MYAPP_ANDROID_ROOT/assets/Data/$CATA_NAME
			fi
		done
	fi
done

# boost appicon
echo "Copy boost project icons begin"
cat $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/boost.list | while read line
do
	CATA_NAME=$line
	echo $CATA_NAME
	
	if [ $CATA_NAME"xyz" != "xyz" ]; then	
		for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/$CATA_NAME/appinfo/appicon/*
		do					
			if [ -d $file ]; then
				cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/res
			fi
			if [ -f $file ]; then
				cp $file $APPPLAY_MYAPP_ANDROID_ROOT/res
			fi
		done
	fi
done
echo "Copy boost project icons end"

cat $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/boost.list | while read line
do
	CATA_NAME=$line
	echo $CATA_NAME
	
	if [ $CATA_NAME"xyz" != "xyz" ]; then	
		for file in $APPPLAY_MYAPP_BIN_ROOT_LOCAL/$APPPLAY_MYAPP_DATAFROM/$CATA_NAME/appinfo/values/*
		do					
			if [ -d $file ]; then
				cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/res/values
			fi
			if [ -f $file ]; then
				cp $file $APPPLAY_MYAPP_ANDROID_ROOT/res/values
			fi
		done
	fi
done
echo "Copy boost project values end"

chmod -R 777 $APPPLAY_MYAPP_ANDROID_ROOT/res
chmod -R 777 $APPPLAY_MYAPP_ANDROID_ROOT/assets
chmod -R 777 $APPPLAY_MYAPP_ANDROID_ROOT/obj/local/armeabi

# build
echo "start build"

export NDK_MODULE_PATH=$ENGINE_ROOT_LOCAL\
:$ENGINE_ROOT_LOCAL/PX2Engine/\
:$ENGINE_ROOT_LOCAL/ThirdPartiesLibs/\
:$APPPLAY_MYAPP_ANDROID_ROOT/\
:$ENGINE_ROOT_LOCAL/Projects/\
:$ENGINE_ROOT_LOCAL/Projects/Client/\

$NDK_ROOT_LOCAL/ndk-build -C $APPPLAY_MYAPP_ANDROID_ROOT

echo "end build"

echo "begin copy so"

# vlc
for file in $ENGINE_ROOT_LOCAL/ThirdPartiesLibs/vlc/libandroid/armeabi/*
do
if [ -d $file ]; then
cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
fi
if [ -f $file ]; then
cp $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
fi
done

# BaiDu
#for file in $ENGINE_ROOT_LOCAL/ThirdPartiesLibs/AI/BaiDu/speech/libs/armeabi/*
#do
#if [ -d $file ]; then
#cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
#fi
#if [ -f $file ]; then
#cp $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
#fi
#done

#for file in $ENGINE_ROOT_LOCAL/ThirdPartiesLibs/AI/BaiDu/tts/libs/armeabi/*
#do
#if [ -d $file ]; then
#cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
#fi
#if [ -f $file ]; then
#cp $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
#fi
#done

# Tuling
for file in $ENGINE_ROOT_LOCAL/ThirdPartiesLibs/AI/Tuling/libs/armeabi/*
do
if [ -d $file ]; then
cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
fi
if [ -f $file ]; then
cp $file $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi
fi
done

# HardCamera
#for file in $ENGINE_ROOT_LOCAL/ThirdPartiesLibs/HardCamera/android/libs/*
#do
#if [ -d $file ]; then
#cp -rf $file $APPPLAY_MYAPP_ANDROID_ROOT/libs
#fi
#if [ -f $file ]; then
#cp $file $APPPLAY_MYAPP_ANDROID_ROOT/libs
#fi
#done

chmod -R 777 $APPPLAY_MYAPP_ANDROID_ROOT/libs
chmod -R 777 $APPPLAY_MYAPP_ANDROID_ROOT/libs/armeabi

echo "end copy so"
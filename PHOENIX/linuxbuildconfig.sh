# install opengl
apt-get install freeglut3-dev

# cp fmod
cp -fr ./ThirdPartiesLibs/FMOD /usr/local/lib/
chmod 777 /usr/local/lib/FMOD

if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
    cp -f /usr/local/lib/FMOD/linux/lib/x86_64/libfmod.so.9 /usr/local/lib/
else
    cp -f /usr/local/lib/FMOD/linux/lib/x86/libfmod.so.9 /usr/local/lib/
fi

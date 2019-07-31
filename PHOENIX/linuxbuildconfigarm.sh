# install opengl
apt-get install libgles2-mesa-dev

# cp fmod
cp -fr ./ThirdPartiesLibs/FMOD /usr/local/lib/
chmod 777 /usr/local/lib/FMOD

# cp fmod so
cp -f /usr/local/lib/FMOD/linux/lib/armhf/libfmod.so.9 /usr/local/lib/
@echo off

..\\..\\..\\Tools\\protoc -I=./ --cpp_out=./ ./PX2Slam.proto

pause
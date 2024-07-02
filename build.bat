@echo off
REM 获取当前脚本的目录
set "CURRENT_DIR=%~dp0"

REM 复制文件到当前目录
copy "D:\Desktop\SnowElves\cmake-build-release\SnowElves.exe" "%CURRENT_DIR%"
copy "D:\Desktop\SnowElves\cmake-build-release\RESOURCE.rcc" "%CURRENT_DIR%"


E:\Qt\6.7.2\mingw_64\bin\windeployqt6.exe SnowElves.exe

pause
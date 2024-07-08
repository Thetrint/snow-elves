@echo off
REM 获取当前脚本的目录
set "CURRENT_DIR=%~dp0"

REM 删除所有文件除了Update.bat和build.bat
for %%i in ("%CURRENT_DIR%*") do (
    if not "%%~nxi" == "Update.bat" if not "%%~nxi" == "build.bat" if not "%%~nxi" == ".gitignore" (
        del "%%i"
    )
)

REM 删除所有文件夹及其内容
for /d %%d in ("%CURRENT_DIR%*") do (
    rd /s /q "%%d"
)


REM 复制文件到当前目录
copy "E:\Desktop\SnowElvesScript\cmake-build-release\SnowElvesScript.exe" "%CURRENT_DIR%"
copy "E:\Desktop\SnowElvesScript\cmake-build-release\RESOURCE.rcc" "%CURRENT_DIR%"
copy "E:\Desktop\SnowElvesScript\cmake-build-release\libcpr.dll" "%CURRENT_DIR%"
copy "E:\Desktop\SnowElvesScript\cmake-build-release\libcurl-d.dll" "%CURRENT_DIR%"

REM 使用 windeployqt6.exe 处理 SnowElves.exe
D:\Qt\6.7.2\mingw_64\bin\windeployqt6.exe SnowElvesScript.exe


REM 获取 SnowElves.exe 的依赖项
REM 将 dumpbin 的路径替换为你的实际路径
set "DUMPBIN_PATH="D:\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\dumpbin.exe""
set "EXE_PATH=%CURRENT_DIR%\SnowElvesScript.exe"
set "DLL_PATH=%CURRENT_DIR%"

%DUMPBIN_PATH% /dependents %EXE_PATH% | findstr "\.dll$" > .\_dependent_dll_log.txt

echo 解析完毕，总计
type .\_dependent_dll_log.txt | find /v /c ""

type .\_dependent_dll_log.txt

REM 循环处理依赖 DLL 列表
for /f %%i in (.\_dependent_dll_log.txt) do (
    REM 检查是否能找到 DLL 文件
    if exist "%CURRENT_DIR%\%%i" (
        echo 可以找到%%i
    ) else (
        REM 找不到时记录到临时文件
        echo 找不到%%i，记录于 .\temp.txt
        echo %%i>>.\temp.txt
    )
)


REM 从临时文件中读取 DLL 文件名，并查找路径复制到当前目录
for /f %%j in (.\temp.txt) do (
    REM 使用 WHERE 命令查找 DLL 文件的完整路径
    for /f "delims=" %%p in ('WHERE "%%j"') do (
        set "dll_path=%%p"
        
        REM 检查找到的路径是否为空
        if not "!dll_path!"=="" (
            echo 开始复制，复制对象：%%p
            copy "%%p" "%CURRENT_DIR%"
        ) else (
            echo 找不到文件: %%j
        )
    )
)

REM 清理临时文件
del .\_dependent_dll_log.txt
del .\temp.txt

git add .
git commit -m "update"
git push

echo 复制工作结束

exit

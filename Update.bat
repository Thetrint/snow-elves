@echo off
setlocal

ping 127.0.0.1

rem 接收第一个参数
set PARAM1=%1

echo %3 > version.txt

rem 根据参数执行相应操作
if "%PARAM1%"=="Incremental" (
    echo Executing Incremental Update

    rem 创建临时批处理文件
    (
        echo @echo off
        rem 设置文件名
        echo set "file_name=SnowElvesScript.exe"
        echo echo File name is %%file_name%%

        rem 构建完整的源文件路径和目标文件路径
        echo set "source=%temp_dir%\%%file_name%%"
        echo set "destination=%CD%\%%file_name%%"
        echo echo Source path is %%source%%
        echo echo Destination path is %%destination%%

        rem 执行复制操作
        echo copy /Y "%%source%%" "%%destination%%"
        echo echo Copy completed with errorlevel %%errorlevel%%

        rem 删除源文件
        echo del "%%source%%"
        echo echo Source file removed with errorlevel %%errorlevel%%

        rem 启动程序
        echo start "" ".\SnowElvesScript.exe"

        echo exit
        echo del "%%~f0"
    ) > "%CD%\temp_update.bat"

    rem 启动临时批处理文件并关闭当前窗口
    start "" "%CD%\temp_update.bat"
    endlocal
    exit

) else if "%PARAM1%"=="Full" (
    echo Executing Full Update

    (
        echo @echo off
        echo powershell -Command "Expand-Archive -Path '%%TEMP%%\ScriptApp.zip' -DestinationPath '%%TEMP%%\FullUpdate' -Force"
        echo xcopy /Y %%TEMP%%\FullUpdate\* %%CD%%\ /E /H /C /I /exclude:%~nx0
        echo rmdir /S /Q %%TEMP%%\FullUpdate
        echo start "" ".\SnowElvesScript.exe"
        echo exit
        echo del "%%~f0"
    ) > "%CD%\temp_update.bat"

    rem 启动临时批处理文件并关闭当前窗口
    start "" "%CD%\temp_update.bat"
    endlocal
    exit
)

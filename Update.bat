@echo off
setlocal
echo Current directory: %cd%
ping 127.0.0.1

rem ���յ�һ������
set PARAM1=%1
set PARAM3=%3

echo %PARAM3% > version.txt

rem ���ݲ���ִ����Ӧ����
if "%PARAM1%"=="Incremental" (
    echo Executing Incremental Update
    del "SnowElvesScriptTemp.exe"
    ren "SnowElvesScript.exe" "SnowElvesScriptTemp.exe"
    del "SnowElvesScript.exe"
    ren "SnowElvesScriptUpdate.exe" "SnowElvesScript.exe"

    del resources\images.dat
    ren "resources\imagesUpdate.dat" "images.dat"

    start "" ".\SnowElvesScript.exe"
    exit

) else if "%PARAM1%"=="Full" (
    echo Executing Full Update

    powershell -Command "Expand-Archive -Path 'ScriptApp.zip' -DestinationPath 'FullUpdate' -Force"
    xcopy /Y "FullUpdate\*" "%CD%\" /E /H /C /I /exclude:%~nx0
    rmdir /S /Q "FullUpdate"
    del "ScriptApp.zip"
    start "" ".\SnowElvesScript.exe"
    exit
)

endlocal
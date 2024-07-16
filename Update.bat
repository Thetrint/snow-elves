@echo off
setlocal

ping 127.0.0.1

rem ���յ�һ������
set PARAM1=%1
set PARAM3=%3

echo %PARAM3% > version.txt

rem ���ݲ���ִ����Ӧ����
if "%PARAM1%"=="Incremental" (
    echo Executing Incremental Update
    ren "SnowElvesScript.exe" "SnowElvesScriptTemp.exe"
    ren "SnowElvesScriptUpdate.exe" "SnowElvesScript.exe"

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

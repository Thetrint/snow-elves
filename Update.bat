@echo off
setlocal

ping 127.0.0.1

rem ���յ�һ������
set PARAM1=%1

echo %3 > version.txt

rem ���ݲ���ִ����Ӧ����
if "%PARAM1%"=="Incremental" (
    echo Executing Incremental Update

    rem ������ʱ�������ļ�
    (
        echo @echo off
        rem �����ļ���
        echo set "file_name=SnowElvesScript.exe"
        echo echo File name is %%file_name%%

        rem ����������Դ�ļ�·����Ŀ���ļ�·��
        echo set "source=%temp_dir%\%%file_name%%"
        echo set "destination=%CD%\%%file_name%%"
        echo echo Source path is %%source%%
        echo echo Destination path is %%destination%%

        rem ִ�и��Ʋ���
        echo copy /Y "%%source%%" "%%destination%%"
        echo echo Copy completed with errorlevel %%errorlevel%%

        rem ɾ��Դ�ļ�
        echo del "%%source%%"
        echo echo Source file removed with errorlevel %%errorlevel%%

        rem ��������
        echo start "" ".\SnowElvesScript.exe"

        echo exit
        echo del "%%~f0"
    ) > "%CD%\temp_update.bat"

    rem ������ʱ�������ļ����رյ�ǰ����
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

    rem ������ʱ�������ļ����رյ�ǰ����
    start "" "%CD%\temp_update.bat"
    endlocal
    exit
)

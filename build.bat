@echo off
REM ��ȡ��ǰ�ű���Ŀ¼
set "CURRENT_DIR=%~dp0"

REM ɾ�������ļ�����Update.bat��build.bat
for %%i in ("%CURRENT_DIR%*") do (
    if not "%%~nxi" == "Update.bat" if not "%%~nxi" == "build.bat" if not "%%~nxi" == ".gitignore" (
        del "%%i"
    )
)

REM ɾ�������ļ��м�������
for /d %%d in ("%CURRENT_DIR%*") do (
    rd /s /q "%%d"
)


REM �����ļ�����ǰĿ¼
copy "E:\Desktop\SnowElvesScript\cmake-build-release\SnowElvesScript.exe" "%CURRENT_DIR%"
copy "E:\Desktop\SnowElvesScript\cmake-build-release\RESOURCE.rcc" "%CURRENT_DIR%"
copy "E:\Desktop\SnowElvesScript\cmake-build-release\libcpr.dll" "%CURRENT_DIR%"
copy "E:\Desktop\SnowElvesScript\cmake-build-release\libcurl-d.dll" "%CURRENT_DIR%"

REM ʹ�� windeployqt6.exe ���� SnowElves.exe
D:\Qt\6.7.2\mingw_64\bin\windeployqt6.exe SnowElvesScript.exe


REM ��ȡ SnowElves.exe ��������
REM �� dumpbin ��·���滻Ϊ���ʵ��·��
set "DUMPBIN_PATH="D:\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\dumpbin.exe""
set "EXE_PATH=%CURRENT_DIR%\SnowElvesScript.exe"
set "DLL_PATH=%CURRENT_DIR%"

%DUMPBIN_PATH% /dependents %EXE_PATH% | findstr "\.dll$" > .\_dependent_dll_log.txt

echo ������ϣ��ܼ�
type .\_dependent_dll_log.txt | find /v /c ""

type .\_dependent_dll_log.txt

REM ѭ���������� DLL �б�
for /f %%i in (.\_dependent_dll_log.txt) do (
    REM ����Ƿ����ҵ� DLL �ļ�
    if exist "%CURRENT_DIR%\%%i" (
        echo �����ҵ�%%i
    ) else (
        REM �Ҳ���ʱ��¼����ʱ�ļ�
        echo �Ҳ���%%i����¼�� .\temp.txt
        echo %%i>>.\temp.txt
    )
)


REM ����ʱ�ļ��ж�ȡ DLL �ļ�����������·�����Ƶ���ǰĿ¼
for /f %%j in (.\temp.txt) do (
    REM ʹ�� WHERE ������� DLL �ļ�������·��
    for /f "delims=" %%p in ('WHERE "%%j"') do (
        set "dll_path=%%p"
        
        REM ����ҵ���·���Ƿ�Ϊ��
        if not "!dll_path!"=="" (
            echo ��ʼ���ƣ����ƶ���%%p
            copy "%%p" "%CURRENT_DIR%"
        ) else (
            echo �Ҳ����ļ�: %%j
        )
    )
)

REM ������ʱ�ļ�
del .\_dependent_dll_log.txt
del .\temp.txt

git add .
git commit -m "update"
git push

echo ���ƹ�������

exit

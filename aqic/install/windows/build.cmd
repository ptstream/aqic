@echo off

rem Product version.
set APPVERSION=1.0.2

rem Qt installation folder where find bin (Change to your Qt folder).
set QT=D:\Qt\QT5152
set QTMAIN=%QT%\5.15.2
set QTDIR=%QTMAIN%\mingw81_64
set QTSSL=%QT%\Tools\OpenSSL\Win_x64\bin
set MAINTARGET=build-aqic-Desktop_Qt_5_15_2_MinGW_64_bit-Release
set CURRENTPATH=%PATH%
set PATH=%PATH%;%QT%\Tools\mingw810_64\bin

rem The QtUPnP folder (Change to your QtUPnP folder).
set DEVELDIR=C:\Users\%USERNAME%\Documents\Development

rem Product name.
set PRODUCT=aqic

set TARGET=%PRODUCT%

rem Plateform
set PLATEFORM=x64

rem Compilation results.
set BINARIES=%DEVELDIR%\%MAINTARGET%\%PRODUCT%

if not exist %TARGET% goto nodel
echo ************* Remove old folder
echo %TARGET%
rmdir %TARGET% /S /Q

:nodel

md %TARGET%

echo ************* Binary files
echo "%BINARIES%\release\%PRODUCT%.exe"
copy %BINARIES%\release\%PRODUCT%.exe %TARGET%

echo ************* Icon file
copy ..\..\icons\%PRODUCT%_64.png %TARGET%\%PRODUCT%.png

echo ************* config folder
md %TARGET%\config 
xcopy ..\..\config\*.* %TARGET%\config  /Y /I /S
rem del %TARGET%\config\*.geojson
copy %BINARIES%\release\config\towns.bin %TARGET%

echo ************* Translation
xcopy ..\..\..\languages\*.qm "%TARGET%\config" /Y /I

echo ************* C++ mingw files
copy "%QTDIR%\bin\libgcc_s_seh-1.dll" %TARGET%
copy "%QTDIR%\bin\libstdc++-6.dll" %TARGET%
copy "%QTDIR%\bin\libwinpthread-1.dll" %TARGET%

echo ************* OPENSSL dlls
copy "%QTSSL%\libssl-1_1-x64.dll" %TARGET%
copy "%QTSSL%\libcrypto-1_1-x64.dll" %TARGET%

%QTDIR%\bin\windeployqt.exe --qmldir ..\..\ %TARGET%

echo ************* Remove unused files added by windeployqt.exe
rd /S /Q %TARGET%\sqldrivers
rd /S /Q %TARGET%\virtualkeyboard
rd /S /Q %TARGET%\qmltooling
rd /S /Q %TARGET%\platforminputcontexts
rd /S /Q %TARGET%\QtGraphicalEffects

rem goto end
echo Change version to %APPVERSION%
call chver "aqic.iss" "0.0.0" %APPVERSION%
set INNOSETUP=C:\Program Files (x86)\Inno Setup 6
"%INNOSETUP%\Compil32" /cc aqic.iss
echo Restore dummy version to 0.0.0
call chver "aqic.iss" %APPVERSION% "0.0.0"
echo Cleanup
rmdir %TARGET% /S /Q
set PATH=%CURRENTPATH%

:end


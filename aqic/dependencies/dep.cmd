@echo off
echo "Copy dependencies"
if not exist %2\config md %2\config
copy %1\..\languages\*.qm %2\config /Y
xcopy %1\config\*.* %2\config /Y /S
rem del %2\config\*.geojson

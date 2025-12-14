@echo off
set zip=%cd%\writelog_windows_http_piper.zip
if exist "%zip%" del %zip%
pushd install
7z a %zip% *
popd

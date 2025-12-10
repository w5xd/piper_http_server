@echo off
set zip=writelog_piper_scripts_unzip_me_first.zip
if exist "%zip%" del %zip%
pushd wsl_deployment
7z a ../%zip% *
popd
@echo off
if not exist WindowsChangeCase\Release\changeCase.exe (
	echo Build the WindowsChangeCase project first!
	goto :eof
)
set zip=%cd%\writelog_piper_scripts_unzip_me_first.zip
if exist "%zip%" del %zip%
pushd WindowsChangeCase\Release
7z a %zip% *.exe
popd
pushd wsl_deployment
7z a %zip% *
popd

@echo off
echo This command removes the u_piper2 distribution from WSL.
echo The piper_http_server cannot be run again until you repeat
echo run_once_to_setup.bat
set /p yesno=Do you wish to proceed? [Y/N]
if /i "%yesno%" NEQ "Y" goto end
:confirm
wsl --unregister u_piper2
:end

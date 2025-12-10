@ECHO OFF
rem This script copies the contents of the local Windows folder named linuxScripts into
rem the home directory of the "writelog" user of the WSL distribution "u_piper2"
rem
rem Construction wsl command requires converting our (Windows) current directory path to linux
set "dirpath=%cd%"
For %%A in ("%dirpath%") do (
    set dr=%%~dA
    set setwsl=%%~pA%%~nA/
    )
rem The "drive" variable has a trailing : take it off
set dronly=%dr:~0,1%
rem this is completely stupid, but changes the drive letter to lower case
for /f "tokens=*" %%i in ('changeCase -s "%dronly%"') do set drlc=%%i
rem construct complete (linux name...)
set linux=/mnt/%drlc%%setwsl%
rem except that it now needs forward slash
set "linux=%linux:\=/%"
rem copy contents of local linuxScripts to ~ directory in linux, setting them to unix format and set executable attribute as we go
wsl -d u_piper2 -u writelog --cd "~" -e bash -c ^
 "find \""%linux%/linuxScripts/\"" -type f | xargs -I {} sh -c 'cp {} . ; basename {}' | xargs -I X sh -c 'sed s/\\r$// X >X.tmp ; mv X.tmp X; chmod +x X'"
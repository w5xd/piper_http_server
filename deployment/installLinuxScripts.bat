@ECHO OFF
rem construction wsl command requires converting our (Windows) current directory path to linux
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
rem copy contents of local linuxScripts to ~ directory in linux, setting them to executable as we go
rem wsl -d u_piper2 -u writelog --cd "~" -e bash -c "find \""%linux%/linuxScripts/\"" -type f | xargs -I {} sh -c 'cp {} . ; basename {}' | xargs chmod +x"
wsl -d u_piper2 -u writelog --cd "~" -e bash -c "find \""%linux%/linuxScripts/\"" -type f | xargs -I {} sh -c 'cp {} . ; basename {}' | xargs -I X sh -c 'sed s/\\r$// X >X.tmp ; mv X.tmp X; chmod +x X'"
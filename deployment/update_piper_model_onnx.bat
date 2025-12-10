@echo off
set /p modelUrl=Enter the URL of the voice model file you want WriteLog to use:
echo you entered %modelUrl%
wsl -d u_piper2 -u writelog --cd "~" -e bash -c "./changeModel %modelUrl%"

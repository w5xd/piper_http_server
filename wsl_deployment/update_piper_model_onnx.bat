@echo off
set /p modelUrl=Enter the URL of the voice model .onnx file you want WriteLog to use:
echo you entered %modelUrl%
echo This script assumes appending .json works like on huggingface.com
wsl -d u_piper2 -u writelog --cd "~" -e bash -c "./changeModel %modelUrl%"

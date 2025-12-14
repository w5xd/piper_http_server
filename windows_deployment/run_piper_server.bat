@echo off
rem This script runs the piper_cpp_http_server in the bin directory using THE onnx model 
rem that must already be in the model directory. (If there is more than one, the script picks one!)
setlocal EnableExtensions
setlocal EnableDelayedExpansion 
for %%f in ("model\*.onnx") do (
	set onnx=%%~ff
	set json=!onnx!.json
	echo onnx !onnx!
	echo json !json!
	if not exist "!json!"  (
			echo THE .json file, !json!, must be colocated with onnx!
			goto :eof
	)
	rem infinite loop the server
	:rerun_on_quit 
	rem the -DIAG makes it print to its console. you may remove it.
	.\bin\piper_cpp_http_server.exe !onnx! %cd%\espeak-ng-data -DIAG -a localhost
	goto rerun_on_quit
)
echo THE ONNX file and its json file must be in the model directory here: %cd%\model
pause
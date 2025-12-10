(c) 2025 by Wayne E. Wright, W5XD. WriteLog Contesting Software, LLC
This software is licensed under the GPL

The files in this archive invoke WSL (windows subsystem for linux) to (first) create a
wsl "distribution named u_piper2. (wsl --distribution u_piper2)  And then use it for 
text-to-speech. WSL is a Microsoft product ONLY available on Windows 10 and later.

Start with:
	run_once_to_setup.bat
We must be co-located with the (very large) u_piper2.tar.gz
file (which is why this zip file is named "writelog_piper_scripts_unzip_me_first.zip")
The u_piper2.tar.gz must be placed here in this same directory. If you downloaded that .gz file
elsewhere, then move it here now.

Next run the script:
	installLinuxScripts.bat
It also should only be run once, unless you are unstructed to run it again.

Now run:
	update_piper_model_onnx.bat
it will prompt you for the URL of a .onnx model file. You have to figure out where to
get one of those. Enter the full URL. Repeat this script as desired for other models.
The installed linux scripts arrange for any previously installed model to be deleted.
The piper_http_server can only use one model at a time.

Every time you reboot your PC and want to run WriteLog with text-to-speech, you'll need
to run this one again:
	run_piper_http_server.bat
This server script should never exit. If it does, it is because of an error. WriteLog
text-to-speech won't work until this script is restarted.
	
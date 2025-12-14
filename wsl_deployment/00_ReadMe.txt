(c) 2025 by Wayne E. Wright, W5XD. WriteLog Contesting Software, LLC
This software is licensed under the GPL. See https://github.com/w5xd/piper_http_server
VERSION 1.0

The files in this archive invoke WSL (windows subsystem for linux) to (first) create a
wsl "distribution" named u_piper2. (wsl --distribution u_piper2)  Below, it is used for 
text-to-speech. WSL is a Microsoft product ONLY available on Windows 10 and later.

PREREQUISITE:
The Windows Subsystem For Linux, WSL, is not installed in Windows by default. 
You can start the default install from here. It will require a reboot.
	0_install_WSL.bat

The remainder of these scripts can (and should) be run by the same Windows user
as you run WriteLog.

Start with:
	1_run_once_to_setup.bat
We must be co-located with the (very large) u_piper2.tar.gz
file (which is why this zip file is named "writelog_piper_scripts_unzip_me_first.zip".)
The u_piper2.tar.gz must be placed here in this same directory. If you downloaded that .gz file
elsewhere, then move it here now.

Next run the script:
	2_installLinuxScripts.bat
It also should only be run once, unless you are instructed to run it again. (Only
happens if writelog_piper_scripts_unzip_me_first.zip needs to be changed.)

Now run:
	3_update_piper_model_onnx.bat
it will prompt you for the URL of a .onnx model file. You have to figure out where to
get one of those. Enter the full URL. Repeat this script as desired for other models.
The installed linux scripts arrange for any previously installed model to be deleted.
The piper_http_server can only use one model at a time.

Every time you reboot your PC and want to run WriteLog with text-to-speech, you'll need
to run this one again:
	4_run_piper_http_server.bat
This server script will never exit because, even if the http server crashes, the script
restarts it.

To remove the WSL u_piper2 deployment:
	99_remove_u_piper2.bat
Undoes what was done in installLinuxScripts
	
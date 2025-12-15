(c) 2025 by Wayne E. Wright, W5XD. WriteLog Contesting Software, LLC
This software is licensed under the GPL. see https://github.com/w5xd/piper_http_server
The piper1-gpl Text-To-Speech package is here: https://github.com/OHF-Voice/piper1-gpl
VERSION 1.2

This http server is designed by the team at WriteLog Contesting Software, LLC.
Its http interface is compatible with what WriteLog version 12.93 and later expects when you
configure WriteLog to use Text-to-Voice. This server and its source code are
under the GPL license and may be used by anyone under the terms of that license.
The text-to-speech capability that this server uses is from 
https://github.com/OHF-Voice/piper1-gpl

Prerequisites
1. This library only runs on 64 bit Windows. It has been tested on Win10 and Win11,
but it likely works on 64 bit Win7 as well. Installation: simply unzip
it, which you already did in order to read this. The .onnx file you'll eventually
need (see below) is 100MB or so, which means you'll need that much more room on the
disk where you unzip this.

2. Microsoft VC runtime install
The piper libraries here require the Microsoft runtime for X64 installed:
The direct link to the required installer: https://aka.ms/vc14/vc_redist.x64.exe
Learn more:
https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-supported-redistributable-version

3. A piper voice model is required. There are a number of them available for free download here:
https://huggingface.co/rhasspy/piper-voices/tree/main/en
The "GB" and "US" refer to "Great Britain" and "United States" respectively.
You must download both the .onnx file (the huge one) and its accompanying .onnx.json (small)
This server requires you maintain the naming convention that the json file is the same
as the .onnx, but with extension .onnx.json.

Place BOTH the .onnx and .onnx.json in the \model subfolder right here where this ReadMe is.
Hint: some browsers are hard to convince to download the json file instead of displaying it.
Right click the Raw button on huggingface.com and use Save-Link-As... See JsonSaveAs.png
file://JsonSaveAs.png

4. run_piper_server.bat
It will pick up only one .onnx file in the model directory. Its up to you make sure 
there is only one there. The server runs forever. If it should crash, the run_piper_server 
loops back and starts it again. You have kill it. Either close its Command window, or 
type CTRL+C at it.

5. Training piper to use your own voice is a large project of its own. The WriteLog
team is not involved in creating voice models. It can be done.

Version History
VERSION 1.2
Use Release build of libpiper (from here: https://github.com/OHF-Voice/piper1-gpl)

VERSION 1.1 
Support command line option to listen only on localhost (avoids firewall warning.)

VERSION 1.0
First version on Windows X64
@rem run this script to create the linux system -d u_piper2. If need remove u_piper2? remove_u_piper2.bat
@if exist u_piper2.tar.gz (
wsl --update
wsl --import u_piper2 .\u_piper2 u_piper2.tar.gz
) else @(
	@echo file u_piper2.tar.gz must be downloaded to here first: %cd%u_piper2.tar.gz
)
@pause

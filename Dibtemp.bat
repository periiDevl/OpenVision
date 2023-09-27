@echo off
mkdir DEBUG_BUILD 2>nul /Q
echo D | xcopy /s /e /q /y Assets DEBUG_BUILD\Assets 2>nul
echo. > DEBUG_BUILD\ov.ov
copy /Y Vision_engine.exe DEBUG_BUILD\Ovruntime.exe 2>nul
copy /Y OpenVisionIcon.png DEBUG_BUILD\OpenVisionIcon.png 2>nul
copy /Y Scene.ov DEBUG_BUILD\Scene.ov 2>nul
copy /Y runtimeconfig.ov DEBUG_BUILD\runtimeconfig.ov 2>nul
copy /Y SETTINGS.ov DEBUG_BUILD\SETTINGS.ov 2>nul
copy /Y SCRIPTS.ov DEBUG_BUILD\SCRIPTS.ov 2>nul
copy /Y DynaLL\x64\Debug\DynaLL.dll DEBUG_BUILD\DynaLL.dll 2>nul
cd /D DEBUG_BUILD
start /B Ovruntime.exe
exit

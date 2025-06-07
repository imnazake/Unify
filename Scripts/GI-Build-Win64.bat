@echo off

set "PLUGIN_NAME=GameplayInteraction"
set "PLUGIN_PATH=D:\Projects\Unify-GC-UE5.6-Dev\Plugins\GameplayInteraction"
set "UAT_PATH=D:\Epic Games\UE_5.6\Engine\Build\BatchFiles\RunUAT.bat"

set "PACKAGE_FOLDER_NAME=GI_Distribution_Win64_UE5.6"
set "PACKAGE_PATH=C:\Users\Nazake\Desktop\Output"

"%UAT_PATH%" BuildPlugin -Plugin="%PLUGIN_PATH%\%PLUGIN_NAME%.uplugin" -Package="%PACKAGE_PATH%\%PACKAGE_FOLDER_NAME%" -Rocket -VS2022

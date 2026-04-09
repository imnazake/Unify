@echo off

set "PLUGIN_NAME=GameplayInteraction"
set "PLUGIN_PATH=D:\Projects\Unify-GI-UE5.5-Dev\Plugins\GameplayInteraction"
set "UAT_PATH=D:\Epic Games\UE_5.5\Engine\Build\BatchFiles\RunUAT.bat"

set "PACKAGE_FOLDER_NAME=GI_Distribution_Win64_UE5.5"
set "PACKAGE_PATH=C:\Users\Nazake\Desktop\Output"

"%UAT_PATH%" BuildPlugin -Plugin="%PLUGIN_PATH%\%PLUGIN_NAME%.uplugin" -Package="%PACKAGE_PATH%\%PACKAGE_FOLDER_NAME%" -Rocket -VS2022

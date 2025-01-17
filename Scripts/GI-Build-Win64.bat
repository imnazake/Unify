@echo off

set "PLUGIN_NAME=GameplayInteraction"
set "PLUGIN_PATH=C:\Users\Nazake\Documents\Unreal Projects\Demos\Unify_5.4\Plugins\GameplayInteraction"
set "UAT_PATH=D:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat"

set "PACKAGE_FOLDER_NAME=GI_Distribution_Win64_UE5.4"
set "PACKAGE_PATH=C:\Users\Nazake\Desktop\Output"

"%UAT_PATH%" BuildPlugin -Plugin="%PLUGIN_PATH%\%PLUGIN_NAME%.uplugin" -Package="%PACKAGE_PATH%\%PACKAGE_FOLDER_NAME%" -Rocket -VS2022

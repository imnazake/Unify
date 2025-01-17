@echo off

set "PLUGIN_NAME=GameplayContainers"
set "PLUGIN_PATH=C:\Users\Nazake\Documents\Unreal Projects\Demos\Unify_5.5\Plugins\GameplayContainers"
set "UAT_PATH=D:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles\RunUAT.bat"

set "PACKAGE_FOLDER_NAME=GC_Distribution_Win64_UE5.5"
set "PACKAGE_PATH=C:\Users\Nazake\Desktop\Output"

"%UAT_PATH%" BuildPlugin -Plugin="%PLUGIN_PATH%\%PLUGIN_NAME%.uplugin" -Package="%PACKAGE_PATH%\%PACKAGE_FOLDER_NAME%" -Rocket -VS2022

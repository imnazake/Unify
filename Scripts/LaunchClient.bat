@echo off

set PROJECT_NAME=Unify
set PROJECT_PATH="C:\Users\Nazake\Desktop\Unify"
set ENGINE_PATH="D:\Epic Games\UE_5.1\Engine\Binaries\Win64\UnrealEditor.exe"

%ENGINE_PATH% %PROJECT_PATH%\%PROJECT_NAME%.uproject 127.0.0.1 -game -log

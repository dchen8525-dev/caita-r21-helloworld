@echo off
setlocal

set "CATSTART=C:\Program Files (x86)\Dassault Systemes\B21\intel_a\code\bin\CATSTART.exe"
set "CATENV=C:\ProgramData\DassaultSystemes\CATEnv"

if not exist "%CATSTART%" (
  echo ERROR: CATIA V5R21 CATSTART.exe was not found.
  exit /b 2
)

"%CATSTART%" -env "CATIA_P3.V5R21.B21" -direnv "%CATENV%" -run "cmd.exe /c call %~dp0_launch_runtime.bat"
exit /b %errorlevel%

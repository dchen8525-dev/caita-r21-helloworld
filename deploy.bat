@echo off
setlocal
set "ACTION=%~1"
if not defined ACTION set "ACTION=Install"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0deploy.ps1" -Action "%ACTION%"
exit /b %errorlevel%

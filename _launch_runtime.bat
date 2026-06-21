@echo off
setlocal
cd /d "%~dp0"

set "RUNTIME=%~dp0win_b64"
set "CATIA_HOME=C:\Program Files\Dassault Systemes\B21\win_b64"
set "CNEXT=%CATIA_HOME%\code\bin\CNEXT.exe"

if not exist "%RUNTIME%\code\bin\HelloWorldModule.dll" (
  echo ERROR: Plugin runtime is missing. Run build.bat first.
  exit /b 2
)
if not exist "%CNEXT%" (
  echo ERROR: CATIA V5R21 CNEXT.exe was not found.
  exit /b 2
)

rem Keep the official RADE runtime-view discovery used by mkrun.
set "MKMK_LST_RTV=%~dp0;C:\PROGRA~1\DASSAU~1\B21;"
set "MKMK_LST_RTV_OS=%RUNTIME%;%CATIA_HOME%;"
set "MKMK_RTV_OS=%RUNTIME%"
set "IT_DATA=%~dp0"

rem CATSTART has loaded CATIA_P3; explicitly keep plugin resources first.
set "PATH=%RUNTIME%\code\bin;%PATH%"
set "CATDLLPath=%RUNTIME%\code\bin;%CATDLLPath%"
set "CATICPath=%RUNTIME%\code\productIC;%CATICPath%"
set "CATDictionaryPath=%RUNTIME%\code\dictionary;%CATDictionaryPath%"
set "CATMsgCatalogPath=%RUNTIME%\resources\msgcatalog;%CATMsgCatalogPath%"

"%CNEXT%"
exit /b %errorlevel%

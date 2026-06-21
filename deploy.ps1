param(
    [ValidateSet("Install", "Uninstall")]
    [string]$Action = "Install",
    [string]$EnvironmentName = "CATIA_P3.V5R21.B21"
)

$ErrorActionPreference = "Stop"
$workspace = $PSScriptRoot
$runtime = Join-Path $workspace "win_b64"
$statePath = Join-Path $workspace ".deploy-state.json"
$environmentDirectory = "C:\ProgramData\DassaultSystemes\CATEnv"
$environmentFile = Join-Path $environmentDirectory ($EnvironmentName + ".txt")
$changeEnvironment = "C:\Program Files\Dassault Systemes\B21\win_b64\code\bin\chcatenv.exe"
$oldRuntime = "C:\ProgramData\DassaultSystemes\CATPlugins\HelloWorld"
$logPath = Join-Path $env:LOCALAPPDATA "DassaultSystemes\CATTemp\HelloWorldPlugin.log"
$systemDictionary = "C:\Program Files\Dassault Systemes\B21\win_b64\code\dictionary\CATIAApplicationFrame.dic"
$systemDictionaryBackup = Join-Path $workspace ".system-dictionary-backup"
$addinDictionaryLines = @(
    "HelloWorldGeneralWksAddin CATIWorkbenchAddin libHelloWorldModule",
    "HelloWorldGeneralWksAddin CATIAfrGeneralWksAddin libHelloWorldModule"
)

function Assert-Administrator {
    $identity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($identity)
    if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
        throw "Run PowerShell as Administrator before changing the CATIA environment."
    }
}

function Get-EnvironmentValue([string]$Name) {
    $line = Get-Content -LiteralPath $environmentFile |
            Where-Object { $_ -match ('^' + [Regex]::Escape($Name) + '=') } |
            Select-Object -First 1
    if ($null -eq $line) { return "" }
    return $line.Substring($Name.Length + 1)
}

function Remove-OldPluginPaths([string]$Value) {
    return (($Value -split ';' | Where-Object {
        $_ -and $_ -notmatch '\\CATPlugins\\HelloWorld(\\|$)' -and
        $_ -notlike "$workspace*"
    }) -join ';')
}

function Set-EnvironmentValue([string]$Name, [string]$Value) {
    & $changeEnvironment -e $EnvironmentName -d $environmentDirectory `
                         -var ($Name + "=" + $Value)
    if ($LASTEXITCODE) { throw "chcatenv failed while setting $Name." }
}

function Prepend([string]$Path, [string]$Existing) {
    if ([string]::IsNullOrEmpty($Existing)) { return $Path }
    return $Path + ";" + $Existing
}

Assert-Administrator
if (@(Get-Process CNEXT -ErrorAction SilentlyContinue).Count) {
    throw "Close CATIA before changing its runtime environment."
}
foreach ($required in @($environmentFile, $changeEnvironment, $systemDictionary)) {
    if (-not (Test-Path $required)) { throw "Required path is missing: $required" }
}

$variables = @("CATDLLPath", "CATICPath", "CATDictionaryPath", "CATMsgCatalogPath")

if ($Action -eq "Install") {
    foreach ($required in @(
        (Join-Path $runtime "code\bin\HelloWorldModule.dll"),
        (Join-Path $runtime "code\productIC\HelloWorldFrameIC.xml"),
        (Join-Path $runtime "code\dictionary\HelloWorldFrame.dico"),
        (Join-Path $runtime "resources\msgcatalog\HelloWorldHeader.CATNls")
    )) {
        if (-not (Test-Path $required -PathType Leaf)) {
            throw "RADE runtime output is missing: $required. Run build.bat first."
        }
    }

    $state = [ordered]@{}
    foreach ($name in $variables) {
        $state[$name] = Remove-OldPluginPaths (Get-EnvironmentValue $name)
    }
    $state | ConvertTo-Json | Set-Content -LiteralPath $statePath -Encoding UTF8

    Set-EnvironmentValue "CATDLLPath" `
        (Prepend (Join-Path $runtime "code\bin") $state["CATDLLPath"])
    Set-EnvironmentValue "CATICPath" `
        (Prepend (Join-Path $runtime "code\productIC") $state["CATICPath"])
    Set-EnvironmentValue "CATDictionaryPath" `
        (Prepend (Join-Path $runtime "code\dictionary") $state["CATDictionaryPath"])
    Set-EnvironmentValue "CATMsgCatalogPath" `
        (Prepend (Join-Path $runtime "resources\msgcatalog") $state["CATMsgCatalogPath"])

    if (-not (Test-Path $systemDictionaryBackup -PathType Leaf)) {
        Copy-Item -LiteralPath $systemDictionary -Destination $systemDictionaryBackup
    }
    $dictionaryText = Get-Content -LiteralPath $systemDictionary -Raw
    $missingLines = @($addinDictionaryLines | Where-Object { $dictionaryText -notmatch [Regex]::Escape($_) })
    if ($missingLines.Count) {
        [IO.File]::AppendAllText($systemDictionary,
            "`r`n" + ($missingLines -join "`r`n") + "`r`n",
            [Text.Encoding]::ASCII)
    }

    Remove-Item -LiteralPath $oldRuntime -Recurse -Force -ErrorAction SilentlyContinue
    Remove-Item -LiteralPath $logPath -Force -ErrorAction SilentlyContinue
    Write-Host "Installed official RADE runtime view: $runtime"
    Write-Host "Diagnostic log: $logPath"
}
else {
    if (-not (Test-Path $statePath -PathType Leaf)) {
        throw "Deployment state is missing: $statePath"
    }
    $state = Get-Content -LiteralPath $statePath -Raw | ConvertFrom-Json
    foreach ($name in $variables) {
        Set-EnvironmentValue $name $state.$name
    }
    if (Test-Path $systemDictionaryBackup -PathType Leaf) {
        Copy-Item -LiteralPath $systemDictionaryBackup -Destination $systemDictionary -Force
        Remove-Item -LiteralPath $systemDictionaryBackup -Force
    }
    Remove-Item -LiteralPath $statePath -Force
    Write-Host "CATIA environment restored."
}

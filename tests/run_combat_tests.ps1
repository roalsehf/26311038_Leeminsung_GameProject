param(
    [ValidateSet('x64', 'x86')]
    [string]$Architecture = 'x64'
)

$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$installation = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $installation) {
    throw 'Visual Studio C++ build tools are required.'
}

$developerCommand = Join-Path $installation 'Common7\Tools\VsDevCmd.bat'
$outputDirectory = Join-Path $repoRoot "obj\CombatTests\$Architecture"
New-Item -ItemType Directory -Path $outputDirectory -Force | Out-Null

$command = 'call "{0}" -arch={1} -host_arch=x64 >nul && cl /nologo /std:c++17 /EHsc /W4 /WX /I26311038_Leeminsung_GameProject tests\CombatStateTests.cpp 26311038_Leeminsung_GameProject\CombatState.cpp 26311038_Leeminsung_GameProject\Player.cpp /Fe:obj\CombatTests\{1}\CombatStateTests.exe /Fo:obj\CombatTests\{1}\ && obj\CombatTests\{1}\CombatStateTests.exe' -f $developerCommand, $Architecture
Push-Location $repoRoot
try {
    & $env:ComSpec /d /c $command
    if ($LASTEXITCODE -ne 0) {
        throw "Combat tests failed with exit code $LASTEXITCODE."
    }
}
finally {
    Pop-Location
}

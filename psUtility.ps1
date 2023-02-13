function runUtility {
    param (
        [string]$MakeTarget,
        [string]$Executable
    )

    Set-Location -Path .\\utility

    $ProjectName = $Executable.Split('.')[0]
    & 'mingw32-make' $MakeTarget "project=$ProjectName"

    $Dest = Join-Path '..\\' $Executable

    Copy-Item $Executable -Destination $Dest

    Remove-Item $Executable

    Set-Location -Path ..\\

    $ExecutablePath =  Resolve-Path $Executable

    & $ExecutablePath

    Remove-Item $Executable
}

Write-Host "Execute Order: C -> E -> B"

Write-Host "Enter What Utility"
Write-Host "  A: Copy Source File"
Write-Host "  B: Make Whole Test"
Write-Host "  C: Fetch Crashpad"
Write-Host "  D: Clear Files"
Write-Host "  E: Build Envirnoment"

$UtilityNO = Read-Host

if ($UtilityNO.ToLower() -eq "a") {
    runUtility "build" "makeNodeSrc.exe"
}
elseif ($UtilityNO.ToLower() -eq "b") {
    runUtility "build" "makeWholeTest.exe"
}
elseif ($UtilityNO.ToLower() -eq "c") {
    runUtility "pybuild" "getCrashpad.exe"
}
elseif ($UtilityNO.ToLower() -eq "d") {
    runUtility "pybuild" "clearAll.exe"
}
elseif ($UtilityNO.ToLower() -eq "e") {
    runUtility "build" "buildEnv.exe"
}



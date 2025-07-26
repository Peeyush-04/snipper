$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$CurrentPath = [Environment]::GetEnvironmentVariable("Path", "User")

if ($CurrentPath -notlike "*$ScriptDir*") {
    $NewPath = "$CurrentPath;$ScriptDir"
    [Environment]::SetEnvironmentVariable("Path", $NewPath, "User")
    Write-Host "Snipper path added to user PATH"
    Write-Host "Please restart your terminal to apply changes"
} else {
    Write-Host "Snipper script path already in PATH"
}

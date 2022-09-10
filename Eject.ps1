Write-Host "Ejecting!"
$Injectable = Resolve-Path ".\BTDB2_Injectable.dll"
& ".\Injector.exe" eject "$Injectable"
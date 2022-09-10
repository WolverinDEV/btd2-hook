Write-Host "Injecting!"
$Injectable = Resolve-Path ".\BTDB2_Injectable.dll"
& ".\Injector.exe" inject "$Injectable"
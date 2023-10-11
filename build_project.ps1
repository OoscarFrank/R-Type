Invoke-Expression "git submodule update --init --recursive"

Set-Location "server"
.\build.ps1
Set-Location ".."

Set-Location "client"
.\build.ps1
Set-Location ".."

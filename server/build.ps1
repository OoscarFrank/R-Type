Invoke-Expression "git submodule update --init --recursive"

if (Test-Path -Path "build") {
    Remove-Item -Path "build" -Recurse -Force
    Write-Output "Le dossier 'build' et son contenu ont été supprimés."
} else {
    Write-Output "Le dossier 'build' n'existe pas, aucune action requise."
}
New-Item -ItemType Directory -Name "build"
Set-Location "build"
Invoke-Expression "cmake .."
Invoke-Expression "msbuild .\r-type_server.vcxproj /p:Configuration=Release"
Invoke-Expression "msbuild .\PACKAGE.vcxproj /p:Configuration=Release"
Set-Location ".."

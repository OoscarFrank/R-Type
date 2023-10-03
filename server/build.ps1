Remove-Item -Path "build" -Recurse -Force
New-Item -ItemType Directory -Name "build"
Set-Location "build"
Invoke-Expression "cmake .."
Invoke-Expression "msbuild .\r-type_server.vcxproj /p:Configuration=Release"
Invoke-Expression "msbuild .\PACKAGE.vcxproj /p:Configuration=Release"
Set-Location ".."

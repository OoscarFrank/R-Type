Remove-Item -Path "build" -Recurse -Force
New-Item -ItemType Directory -Name "build"
Set-Location "build"
Invoke-Expression "cmake .."
Set-Location ".."
Invoke-Expression "msbuild .\build\r-type_server.vcxproj /p:Configuration=Release"
Invoke-Expression "msbuild .\build\PACKAGE.vcxproj /p:Configuration=Release"

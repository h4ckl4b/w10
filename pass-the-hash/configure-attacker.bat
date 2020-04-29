
REM Que el antimalware no moleste
powershell -Command Add-MpPreference -ExclusionPath "C:\Tools"

REM Habilitar las conexiones de carpetas compartidas y pings.
netsh advfirewall firewall set rule group="File and Printer Sharing" new enable=Yes

pause

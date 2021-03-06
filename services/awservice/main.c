 /**************************************************************
Based on http://oghazal.blogspot.com/2012/04/writing-windows-services-in-c.html
**************************************************************/

#include <windows.h>
#include <stdio.h>

typedef int (__cdecl *ProcAddress)(int); 

int current = 0;

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

DWORD ServiceInitialization(DWORD argc, LPTSTR * argv, DWORD * specificError);

void WINAPI ServiceMain(DWORD argc, LPTSTR * argv);
void WINAPI ServiceCtrlHandler(DWORD opcode);
void WorkHard(int *current);

 int main() {

   HINSTANCE awlib = LoadLibrary("awlib.dll");
   ProcAddress Inc = (ProcAddress) GetProcAddress(awlib, "Inc"); 

   SERVICE_TABLE_ENTRY DispatchTable[] = {
     {
       "awservice",
       ServiceMain
     },
     {
       NULL,
       NULL
     }
   };

   if (!StartServiceCtrlDispatcher(DispatchTable))
     printf("StartServiceCtrlDispatcher() failed, error: %ld.\n", GetLastError());
   else
     printf("StartServiceCtrlDispatcher() looks OK.\n");

   return 0;
 }

 // Stub initialization function...
 DWORD ServiceInitialization(DWORD argc, LPTSTR * argv, DWORD * specificError) {
   // These statments have no effect!
   * argv;
   argc;
   specificError;

   return 0;
 }

 void WINAPI ServiceMain(DWORD argc, LPTSTR * argv) {

   DWORD status;
   DWORD specificError;

   // Type of service, application or driver...
   ServiceStatus.dwServiceType = SERVICE_WIN32;

   // The service is starting...
   ServiceStatus.dwCurrentState = SERVICE_START_PENDING;

   // The service can be stopped & can be paused and continued.
   ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
   ServiceStatus.dwWin32ExitCode = 0;
   ServiceStatus.dwServiceSpecificExitCode = 0;
   ServiceStatus.dwCheckPoint = 0;
   ServiceStatus.dwWaitHint = 0;

   ServiceStatusHandle = RegisterServiceCtrlHandler("MyService", ServiceCtrlHandler);

   if (ServiceStatusHandle == (SERVICE_STATUS_HANDLE) 0) {

     printf("RegisterServiceCtrlHandler() failed, error: %ld.\n", GetLastError());
     return;
   } else
     printf("RegisterServiceCtrlHandler() looks OK.\n");

   // Initialization code goes here...return the status...
   status = ServiceInitialization(argc, argv, & specificError);

   // Handle error condition
   if (status != NO_ERROR) {

     // The service is not running...
     ServiceStatus.dwCurrentState = SERVICE_STOPPED;
     ServiceStatus.dwCheckPoint = 0;
     ServiceStatus.dwWaitHint = 0;
     ServiceStatus.dwWin32ExitCode = status;
     ServiceStatus.dwServiceSpecificExitCode = specificError;
     SetServiceStatus(ServiceStatusHandle, & ServiceStatus);
     return;
   }

   // Initialization complete - report running status.
   ServiceStatus.dwCurrentState = SERVICE_RUNNING;
   ServiceStatus.dwCheckPoint = 0;
   ServiceStatus.dwWaitHint = 0;

   if (!SetServiceStatus(ServiceStatusHandle, & ServiceStatus)) {
     status = GetLastError();
     printf("SetServiceStatus() error: %ld\n", status);
   } else
     printf("SetServiceStatus() looks OK.\n");

   // This is where the service does its work...

   while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
     WorkHard(&current);
     Sleep(5000);
   }
   return;
 }

 // Handler function - receives Opcode, calls SetServiceStatus()
 void WINAPI ServiceCtrlHandler(DWORD Opcode) {

   DWORD status;

   switch (Opcode) {

   case SERVICE_CONTROL_PAUSE:

     // Do whatever it takes to pause here...
     ServiceStatus.dwCurrentState = SERVICE_PAUSED;
     break;

   case SERVICE_CONTROL_CONTINUE:

     // Do whatever it takes to continue here...
     ServiceStatus.dwCurrentState = SERVICE_RUNNING;
     break;

   case SERVICE_CONTROL_STOP:

     // Do whatever it takes to stop here...
     ServiceStatus.dwWin32ExitCode = 0;
     ServiceStatus.dwCurrentState = SERVICE_STOPPED;
     ServiceStatus.dwCheckPoint = 0;
     ServiceStatus.dwWaitHint = 0;

     if (!SetServiceStatus(ServiceStatusHandle, & ServiceStatus)) {

       status = GetLastError();
       printf("[MY_SERVICE] SetServiceStatus() error: %ld\n", status);
     }

     printf("Leaving MyService.\n");
     return;

   case SERVICE_CONTROL_INTERROGATE:

     // Fall through to send current status.
     break;

   default:
     // else
     printf("Unrecognized opcode %ld.\n", Opcode);
   }

   // Send current status.
   if (!SetServiceStatus(ServiceStatusHandle, & ServiceStatus)) {

     status = GetLastError();
     printf("SetServiceStatus error %ld.\n", status);
     return;
   } else
     printf("SetServiceStatus() is OK.\n");

   return;
 }

 // do the heavy work
 void WorkHard(int *current) {

   FILE * fp;

   (*current)++;

   fp = fopen("awservice.txt", "a");
   fprintf(fp, "Current: %d\n", *current);
   fclose(fp);

   return;
 }

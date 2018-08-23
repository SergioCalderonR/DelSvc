#include <Windows.h>
#include <wchar.h>

int wmain(int argc, wchar_t * argv[])
{
	if (argc != 2)
	{
		fwprintf(stderr, L"\nUsage: %s [ServiceName]\n", argv[0]);
		return 1;		
	}

	// OpenSCManagerW
	SC_HANDLE hServiceCM;
	LPCWSTR databaseName = SERVICES_ACTIVE_DATABASE;	// Local database
	DWORD scmDesiredAccess = SC_MANAGER_ALL_ACCESS;	   //  Full access

	hServiceCM = OpenSCManagerW(NULL,
		                        databaseName,
		                        scmDesiredAccess);

	if (hServiceCM == NULL)
	{
		fwprintf(stderr, L"\nService Control Manager was not opened. Code: %u\n", GetLastError());
		return;
	}

	// Service Control Manager was opened! 
	// Let's open the service itselft

	// OpenServiceW
	SC_HANDLE hOpenService;
	DWORD serviceDesiredAccess = SERVICE_ALL_ACCESS;

	hOpenService = OpenServiceW(hServiceCM,
								argv[1],
							    serviceDesiredAccess);

	if (hOpenService == NULL)
	{
		fwprintf(stderr, L"\nCould not open the service, code: %lu\n", GetLastError());
		CloseServiceHandle(hServiceCM);
		return;
		
	}

	// Service was opened! 
	// Let's stop it befor deleting 

	// ControlService
	DWORD controlCode = SERVICE_CONTROL_STOP;
	SERVICE_STATUS serviceStatus;

	if (!ControlService(hOpenService, controlCode, &serviceStatus))
	{
		fwprintf(stderr, L"\nService could not be stopped, code: %u\n", GetLastError());
		CloseServiceHandle(hOpenService);
		CloseServiceHandle(hServiceCM);
		return;

	}

	wprintf(L"\nService has been stopped.\n");


	if (!DeleteService(hOpenService))
	{
		fwprintf(stderr, L"\nService was not deleted, code: %u\n", GetLastError());
		
		
	}
	else
	{
		// Print it to user
		fwprintf(stderr, L"Service was deleted successfully!\n");

	}

	CloseServiceHandle(hOpenService);
	CloseServiceHandle(hServiceCM);	


	return 0;
}
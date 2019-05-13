#include "stdafx.h"
#include "WinSystemService.h"
#include <windows.h>


WinSystemService::WinSystemService(IProgramService* p, char* name, const char* displayName, const char* description)
	: svc_name_(name)
	, svc_displayName_(displayName)
	, svc_description_(description)
{
	program_ = p;
}

WinSystemService::~WinSystemService()
{
}


bool WinSystemService::Control(int argc, char * argv[])
{
	if (!strcmp(argv[1], "run")) {
		return service_run();
	}
	if (!strcmp(argv[1], "install")) {	
		return service_install();
	}
	if (!strcmp(argv[1], "uninstall")) {
		return service_uninstall();
	}
	return false;
}

bool WinSystemService::service_install()
{
	char exe_path[MAX_PATH + 5];
	GetModuleFileName(NULL, exe_path, MAX_PATH);
	strcat(exe_path, " run");

	SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!sc_manager) {
		return false;
	}
	SC_HANDLE svc_handle = CreateService(sc_manager, svc_name_.c_str(), svc_displayName_.c_str(),
		SERVICE_ALL_ACCESS,//SERVICE_START | SERVICE_STOP | SERVICE_CHANGE_CONFIG,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		exe_path, NULL, NULL, NULL, NULL, NULL);
	if (!svc_handle) {
		CloseServiceHandle(sc_manager);
		return false;
	}
	SERVICE_DESCRIPTION svc_desc;
	svc_desc.lpDescription = (LPSTR)svc_description_.c_str();
	ChangeServiceConfig2(svc_handle, SERVICE_CONFIG_DESCRIPTION, &svc_desc);
	SERVICE_FAILURE_ACTIONS actions = { 0 };
	SC_ACTION sca[3];
	for (int i = 0; i < 3; i++) {
		sca[i].Delay = 5000;
		sca[i].Type = SC_ACTION_RESTART;
	}
	actions.cActions = 3;
	actions.lpsaActions = sca;
	actions.lpCommand = nullptr;
	ChangeServiceConfig2(svc_handle, SERVICE_CONFIG_FAILURE_ACTIONS, &actions);
	CloseServiceHandle(svc_handle);
	CloseServiceHandle(sc_manager);
	return true;
}

bool WinSystemService::service_uninstall()
{
	SC_HANDLE sc_manager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, /*SC_MANAGER_ALL_ACCESS*/SC_MANAGER_CONNECT);
	if (!sc_manager) {
		return false;
	}
	SC_HANDLE svc_handle = OpenService(sc_manager, svc_name_.c_str(), /*SERVICE_ALL_ACCESS*/SERVICE_QUERY_STATUS | DELETE);
	if (!svc_handle){
		CloseServiceHandle(sc_manager);
		return false;
	}
	SERVICE_STATUS status;
	if (QueryServiceStatus(svc_handle, &status)){
		if (status.dwCurrentState == SERVICE_STOPPED){
			DeleteService(svc_handle);
		}
	}
	CloseServiceHandle(svc_handle);
	CloseServiceHandle(sc_manager);
	return true;
}

bool WinSystemService::service_run()
{
	SERVICE_TABLE_ENTRY ste[] =
	{
		{ (LPSTR)svc_name_.c_str(), service_main },
		{ NULL, NULL }
	};
	return StartServiceCtrlDispatcher(ste) == TRUE;
}

void __stdcall WinSystemService::service_main(DWORD dwArgc, LPSTR *lpszArgv)
{
	service_handle_ = RegisterServiceCtrlHandler(lpszArgv[0], service_handler);
	if (!service_handle_) {
		return;
	}
	service_status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	service_status_.dwCurrentState = SERVICE_RUNNING;
	service_status_.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
	service_status_.dwWin32ExitCode = NO_ERROR;
	service_status_.dwCheckPoint = 0;
	SetServiceStatus(service_handle_, &service_status_);

	// 程序执行
	if (program_) program_->run();

	service_status_.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(service_handle_, &service_status_);
}

void __stdcall WinSystemService::service_handler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_CONTINUE:
		/* Continue from Paused state. */
		break;
	case SERVICE_CONTROL_PAUSE:
		/* Pause service. */
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		/* System is shutting down. */
	case SERVICE_CONTROL_STOP:
		/* Service should stop. */
		service_status_.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(service_handle_, &service_status_);
		// 正在停止服务
		if (program_) program_->stop();
		break;
	}
}

IProgramService* WinSystemService::program_ = nullptr;

SERVICE_STATUS_HANDLE WinSystemService::service_handle_;

SERVICE_STATUS WinSystemService::service_status_;

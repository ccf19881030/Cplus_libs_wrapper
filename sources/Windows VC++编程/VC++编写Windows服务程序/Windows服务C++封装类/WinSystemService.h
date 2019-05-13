#pragma once
#include "IProgramService.h"

class WinSystemService
{
public:
	WinSystemService(IProgramService* p, char* name, const char* displayName, const char* description);
	~WinSystemService();

	bool Control(int argc, char* argv[]);

protected:
	bool service_install();

	bool service_uninstall();

	bool service_run();

	static void __stdcall service_main(DWORD dwArgc, LPSTR *lpszArgv);

	static void __stdcall service_handler(DWORD fdwControl);

private:
	std::string svc_name_;				// 服务名称
	std::string svc_displayName_;		// 服务显示名称
	std::string svc_description_;		// 描述

	static IProgramService* program_;

	static SERVICE_STATUS_HANDLE service_handle_;
	static SERVICE_STATUS service_status_;
};


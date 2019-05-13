#include <Windows.h>
#include <Tlhelp32.h>
#include <iostream>

using namespace std;

// 判断指定进程名的进程是否存在，若返回0则表示该进程不存在，非0表示该进程存在
DWORD GetProcessidFromName(LPCTSTR name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);

	return id;
}

// 判断指定进程名的进程是否存在
bool isProcessNameRunning(std::string name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return false;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, name.c_str()) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);

	if (id == 0)
	{
		return false;
	}
	else {
		return true;
	}
}

int main(int argc, char *argv[])
{
	char strProcessName[1024] = {0};
	while (true)
	{
		std::cout << "请输入进程名：";
		std::cin >> strProcessName;

		if (strcmp(strProcessName, "quit") == 0)
		{
			break;
		}

		if (isProcessNameRunning(strProcessName))
		{
			std::cout << "进程" << "存在！" << std::endl;
		}
		else {
			std::cout << "进程" << "不存在！" << std::endl;
		}
	}
	

	system("pause");

	return 0;
}

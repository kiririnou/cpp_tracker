#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include "utils.h"

#include <windows.h>
#include <chrono>
#include <sstream>

namespace system_info {

    std::string GetWindowName()
	{
		auto hwnd = GetForegroundWindow();
		int buf_length = GetWindowTextLength(hwnd) + 1;
		LPWSTR title = new WCHAR[buf_length];
		GetWindowTextW(hwnd, title, buf_length);

		std::string str = utils::ConvertUTF16ToUTF8(title);
		delete[] title;

		return str;
	}

	std::string GetUsername()
	{
		const int SIZE = 255;
		LPDWORD len = new DWORD[SIZE];
		LPWSTR uname = new WCHAR[SIZE];
		GetUserNameW(uname, len);

		std::string str = utils::ConvertUTF16ToUTF8(uname);
		delete[] len;
		delete[] uname;
		return str;
	}

	std::string GetHWID()
	{
		HKEY hKey = 0;
		LPWSTR buff = new WCHAR[255];
		DWORD dwType;
		DWORD dwBufSize = 255;
		const char* subkey = "Software\\Microsoft\\Cryptography";
		std::wstring strValueOfKey = L"bad";

		if (RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hKey) == ERROR_SUCCESS)
		{
			dwType = REG_SZ;
			if (RegQueryValueExW(hKey, L"MachineGuid", 0, &dwType, (LPBYTE)buff, &dwBufSize) == ERROR_SUCCESS)
			{
				strValueOfKey = buff;
			}
		}
		delete[]  buff;
		return utils::ConvertUTF16ToUTF8(strValueOfKey);
	}

	std::string GetNow()
	{
		using namespace std::chrono;

		auto time = system_clock::to_time_t(system_clock::now());
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

		return oss.str();
	}

	#define SystemProcessorPerformanceInformation 0x8
	#define SystemBasicInformation    0x0
	int GetCPUData(int sec)
	{
		typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
		{
			LARGE_INTEGER IdleTime;
			LARGE_INTEGER KernelTime;
			LARGE_INTEGER UserTime;
			LARGE_INTEGER Reserved1[2];
			ULONG Reserved2;
		} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;
		typedef struct _SYSTEM_BASIC_INFORMATION {
			ULONG Reserved;
			ULONG TimerResolution;
			ULONG PageSize;
			ULONG NumberOfPhysicalPages;
			ULONG LowestPhysicalPageNumber;
			ULONG HighestPhysicalPageNumber;
			ULONG AllocationGranularity;
			ULONG_PTR MinimumUserModeAddress;
			ULONG_PTR MaximumUserModeAddress;
			KAFFINITY ActiveProcessorsAffinityMask;
			CCHAR NumberOfProcessors;
		} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;
		//	SYSTEM_INFO  sysinf;

		int nWaitSec = sec;
		if (nWaitSec <= 0)
			throw std::runtime_error("Waiting interval in seconds should be positive integer");
		typedef DWORD(WINAPI * PNTQUERYSYSYTEMINFORMATION)(DWORD info_class, void *out, DWORD size, DWORD *out_size);
		PNTQUERYSYSYTEMINFORMATION pNtQuerySystemInformation = NULL;

		pNtQuerySystemInformation = (PNTQUERYSYSYTEMINFORMATION)GetProcAddress(GetModuleHandle("NTDLL.DLL"), "NtQuerySystemInformation");
		SYSTEM_BASIC_INFORMATION sbi;
		SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION * spi;

		DWORD returnlength;
		DWORD status = pNtQuerySystemInformation(SystemBasicInformation, &sbi,
			sizeof(SYSTEM_BASIC_INFORMATION), &returnlength);

		spi = new SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION[sbi.NumberOfProcessors];

		memset(spi, 0, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*sbi.NumberOfProcessors);

		status = pNtQuerySystemInformation(SystemProcessorPerformanceInformation, spi,
			(sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*sbi.NumberOfProcessors), &returnlength);
		int numberOfCores = returnlength / sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION);

		static ULARGE_INTEGER  ul_sys_idle_old[32];
		static ULARGE_INTEGER  ul_sys_kernel_old[32];
		static ULARGE_INTEGER  ul_sys_user_old[32];

		float          usage = 0;
		float          usageAccum = 0;

		Sleep(nWaitSec * 1000);
		status = pNtQuerySystemInformation(SystemProcessorPerformanceInformation, spi,
			(sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*numberOfCores), &returnlength);
		usageAccum = 0;
		for (int ii = 0; ii < numberOfCores; ii++)
		{
			ULARGE_INTEGER        ul_sys_idle;
			ULARGE_INTEGER        ul_sys_kernel;
			ULARGE_INTEGER        ul_sys_user;

			ul_sys_idle.QuadPart = spi[ii].IdleTime.QuadPart;
			ul_sys_kernel.QuadPart = spi[ii].KernelTime.QuadPart;
			ul_sys_user.QuadPart = spi[ii].UserTime.QuadPart;

			ULONGLONG kernelTime = (ul_sys_kernel.QuadPart - ul_sys_kernel_old[ii].QuadPart);
			ULONGLONG usertime = (ul_sys_user.QuadPart - ul_sys_user_old[ii].QuadPart);
			ULONGLONG idletime = (ul_sys_idle.QuadPart - ul_sys_idle_old[ii].QuadPart);

			ULONGLONG proctime = kernelTime + usertime - idletime;

			ULONGLONG totaltime = kernelTime + usertime;

			usage = (float)(proctime * 100) / totaltime;
			usageAccum += usage;
		}
		usageAccum /= numberOfCores;
		delete[] spi;

		return static_cast<int>(usageAccum);
	}

	int GetUsedMemory()
	{
		MEMORYSTATUSEX memStatusEx;
		memStatusEx.dwLength = sizeof(memStatusEx);
		GlobalMemoryStatusEx(&memStatusEx);

		// need convert in to int (uint)
		auto mem = ((memStatusEx.ullTotalPhys - memStatusEx.ullAvailPhys) * 100 / memStatusEx.ullTotalPhys);
		// std::wstring wstr = std::to_wstring(mem);
		return static_cast<int>(mem);
	}

}

#endif
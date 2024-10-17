#pragma once
#include <iostream> 
#include <Windows.h>
#include <TlHelp32.h>
#include "memory.h"

//driver communication 
namespace driver {

	namespace codes {
		constexpr ULONG attach =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9600, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

		constexpr ULONG read =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9601, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

		constexpr ULONG write =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9602, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	}

	struct Request
	{
		HANDLE proccess_id;

		PVOID target;
		PVOID buffer;

		SIZE_T size;
		SIZE_T return_size;
	};

	inline bool attach_to_process(HANDLE driver_handle, const DWORD pid) {
		Request r;

		r.proccess_id = reinterpret_cast<HANDLE>(pid);

		return DeviceIoControl(driver_handle, codes::attach, &r, sizeof(r),
			&r, sizeof(r), nullptr, nullptr);
	}

	template <class T>
	T read_memory(HANDLE driver_handle, const std::uintptr_t addr) {
		T temp = {};

		Request r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = &temp;
		r.size = sizeof(T);

		DeviceIoControl(driver_handle, codes::read, &r, sizeof(r),
			&r, sizeof(r), nullptr, nullptr);

		return temp;
	}

	inline void read_virtual_memory(HANDLE driver_handle, const std::uintptr_t addr, PVOID buf, SIZE_T size)  { 
		Request r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = buf;
		r.size = size;

		DeviceIoControl(driver_handle, codes::read, &r, sizeof(r),
			&r, sizeof(r), nullptr, nullptr);
	}

	template <class T>
	void write_memory(HANDLE driver_handle, const std::uintptr_t addr, const T& value) {
		Request r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = (PVOID)&value;
		r.size = sizeof(T);

		DeviceIoControl(driver_handle, codes::write, &r, sizeof(r),
			&r, sizeof(r), nullptr, nullptr);
	}

}

//user-mode memory segment 
static DWORD get_process_id(const wchar_t* process_name)
{

	DWORD process_id = 0;

	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snap_shot == INVALID_HANDLE_VALUE)
		return process_id;

	PROCESSENTRY32W entry = { };
	entry.dwSize = sizeof(decltype(entry));
	if (Process32FirstW(snap_shot, &entry) == TRUE) {

		if (_wcsicmp(process_name, entry.szExeFile) == 0)
			process_id = entry.th32ProcessID;
		else {
			while (Process32NextW(snap_shot, &entry) == TRUE)
			{
				if (_wcsicmp(process_name, entry.szExeFile) == 0) {
					process_id = entry.th32ProcessID;
					break;
				}

			}
		}

	}

	CloseHandle(snap_shot);

	return process_id;
}

inline std::uintptr_t get_module_address(const DWORD pid, const wchar_t* module_name)
{
	uintptr_t moduleBaseAddress = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (snapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(snapshot, &moduleEntry)) {
			do {
				if (!_wcsicmp(moduleEntry.szModule, module_name)) {
					moduleBaseAddress = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(snapshot, &moduleEntry));
		}
		CloseHandle(snapshot);
	}
	return moduleBaseAddress;
}

inline std::string read_str(std::uintptr_t dst)
{
	char buf[256];

	driver::read_virtual_memory(driver_handle, dst, &buf, sizeof(buf));

	return buf;
}


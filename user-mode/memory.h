#pragma once
#include <iostream> 
#include <Windows.h>
#include <TlHelp32.h>

inline DWORD pid;
inline HANDLE driver_handle;
inline uintptr_t client;
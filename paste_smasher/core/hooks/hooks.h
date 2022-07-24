#pragma once

#define console_log 1
#define file_log 1
#define dump_memory 1

#include "../../dependencies/minhook/minhook.h"
#include "../utils/utils.h"
#include <fstream>

namespace data
{
	inline int alloc_count;
	inline int write_count;
}

namespace hooks
{
	namespace write_virtual_memory
	{
		using fn = NTSTATUS(__stdcall*)(HANDLE, PVOID, PVOID, ULONG, PULONG);
		NTSTATUS __stdcall hook(HANDLE proc, PVOID addr, PVOID buffer, ULONG num_write, PULONG num_written);
	}

	namespace allocate_virtual_ex
	{
		using fn = LPVOID(__stdcall*)(HANDLE, LPVOID, SIZE_T, DWORD, DWORD);
		LPVOID __stdcall hook(HANDLE proc, PVOID addr, SIZE_T size, DWORD alloc, DWORD prot);
	}

	bool init();
}
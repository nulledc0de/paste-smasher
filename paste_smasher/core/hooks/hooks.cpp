#include "hooks.h"

hooks::write_virtual_memory::fn o_wvm = nullptr;
hooks::allocate_virtual_ex::fn o_avex = nullptr;

bool hooks::init()
{
	const auto ntdll = GetModuleHandleA(E("ntdll.dll"));

	if (!ntdll)
		return false;

	const auto kernel32 = GetModuleHandleA("kernel32.dll");

	if (!kernel32)
		return false;

	const auto syscall_addr = reinterpret_cast<void*>(GetProcAddress(ntdll, E("NtWriteVirtualMemory")));

	if (!syscall_addr)
		return false;

	const auto alloc_addr = reinterpret_cast<void*>(GetProcAddress(kernel32, E("VirtualAllocEx")));

	if (!alloc_addr)
		return false;

	if (MH_Initialize() != MH_OK)
		return false;

	if (MH_CreateHook(syscall_addr, &hooks::write_virtual_memory::hook, (LPVOID*)&o_wvm) != MH_OK)
		return false;

	if (MH_CreateHook(alloc_addr, &hooks::allocate_virtual_ex::hook, (LPVOID*)&o_avex) != MH_OK)
		return false;

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		return false;

	if (console_log)
		printf(E("[paste-smasher] setup hooks\n"));

	if (file_log)
		utils::log_to_file(E("[paste-smasher] setup hooks\n"));

	return true;
}

NTSTATUS __stdcall hooks::write_virtual_memory::hook(HANDLE proc, PVOID addr, PVOID buffer, ULONG size, PULONG written)
{
	data::write_count++;

	if (console_log)
		printf(E("[paste-smasher] nt_write_virtual_memory: [handle: %p, addr: %p, buffer: %p, size: %d, idx: %d, alloc_idx: %d]\n"), proc, addr, buffer, size, data::write_count, data::alloc_count);

	if (file_log)
		utils::log_to_file(E("[paste-smasher] nt_write_virtual_memory: [handle: %p, addr: %p, buffer: %p, size: %d, idx: %d, alloc_idx: %d]\n"), proc, addr, buffer, size, data::write_count, data::alloc_count);

	if (dump_memory)
	{
		char file_name_buf[512];

		sprintf_s(file_name_buf, E("dumps/dmp-a%i-%p-w%i.bin"), data::alloc_count, (void*)addr, data::write_count);

		const auto file_handle = CreateFileA(file_name_buf, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_NEW, 0, 0);

		if (file_handle != INVALID_HANDLE_VALUE)
		{
			DWORD ret;

			const auto write_status = WriteFile(file_handle, buffer, size, &ret, 0);

			CloseHandle(file_handle);
		}

	}

	return o_wvm(proc, addr, buffer, size, written);
}

PVOID __stdcall hooks::allocate_virtual_ex::hook(HANDLE proc, LPVOID addr, SIZE_T size, DWORD type, DWORD prot)
{
	data::alloc_count++;

	const auto allocated_memory = o_avex(proc, addr, size, type, prot);

	if (console_log)
		printf(E("[paste-smasher] virtual_alloc_ex: [in_addr: %p, ret_addr: %p, alloc_size: %d, type: %d, prot: %d, idx: %d]\n"), addr, allocated_memory, (int)size, type, prot, data::alloc_count);

	if (file_log)
		utils::log_to_file(E("[paste-smasher] virtual_alloc_ex: [in_addr: %p, ret_addr: %p, alloc_size: %d, type: %d, prot: %d, idx: %d]\n"), addr, allocated_memory, (int)size, type, prot, data::alloc_count);

	return allocated_memory;
}
#include <windows.h>

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}
	return TRUE;
}

SIZE_T ScanSegments(HANDLE proc)
{
	MEMORY_BASIC_INFORMATION meminfo;
	LPCVOID addr = 0;

	if (!proc)
		return 0;

	while (1)
	{
		if (VirtualQueryEx(proc, addr, &meminfo, sizeof(meminfo)) == 0)
			break;
		
		if ((meminfo.State == MEM_COMMIT) && (meminfo.Type & MEM_IMAGE) &&
			(meminfo.Protect == PAGE_READWRITE) && (meminfo.RegionSize == 0x1000))
		{
			return (SIZE_T)meminfo.BaseAddress;
		}
		addr = (unsigned char*)meminfo.BaseAddress + meminfo.RegionSize;
	}
	return 0;
}

WORD ReadWord(HANDLE hProc, DWORD_PTR address)
{
	WORD result = 0;

	if (ReadProcessMemory(hProc, (void*)address, &result, sizeof(result), NULL) == 0)
	{
		printf("Failed to read memory: %u\n", GetLastError());
	}
	return result;
}

void WriteWord(HANDLE hProc, DWORD_PTR address, WORD value)
{
	if (WriteProcessMemory(hProc, (void*)address, &value, sizeof(value), NULL) == 0)
		printf("Failed to write memory: %u\n", GetLastError());
}

int main()
{
	Sleep(4000);

	HANDLE hProc = GetCurrentProcess();

	HANDLE hToken = NULL;
	if (!OpenProcessToken(hProc, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		printf("Failed to open access token\n");
		return 1;
	}

	if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
	{
		printf("Failed to set debug privilege\n");
		return 1;
	}

	HWND wnd = GetForegroundWindow();
	DWORD pid = 0;
	if (!GetWindowThreadProcessId(wnd, &pid))
	{
		printf("Error of the pid detection\n");
		return 1;
	}

	HANDLE hTargetProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hTargetProc)
	{
		printf("Failed to open process: %u\n", GetLastError());
		return 1;
	}

	SIZE_T lifeAddress = ScanSegments(hTargetProc);

	printf("lifeAddress = 0x%x\n", lifeAddress);
	
	ULONG hp = 0;
	while (1)
	{
		hp = ReadWord(hTargetProc, lifeAddress);
		printf("life = %lu\n", hp);

		if (hp < 10)
			WriteWord(hTargetProc, lifeAddress, 20);

		Sleep(1000);
	}
	return 0;
}


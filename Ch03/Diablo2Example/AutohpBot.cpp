#include <windows.h>
#include <stdio.h>

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

SIZE_T IsArrayMatch(HANDLE proc, SIZE_T address, SIZE_T segmentSize, BYTE array[], SIZE_T arraySize)
{
	BYTE* procArray = new BYTE[segmentSize];

	if (ReadProcessMemory(proc, (void*)address, procArray, segmentSize, NULL) == 0)
	{
		printf("Failed to read memory: %u\n", GetLastError());
		return 0;
	}
	
	for (SIZE_T i = 0; i < segmentSize; ++i)
	{
		if ((array[0] == procArray[i]) && ((i + arraySize) < segmentSize))
		{
			if (!memcmp(array, procArray + i, arraySize))
			{
				delete[] procArray;
				return address + i;
			}
		}
	}

	delete[] procArray;
	return 0;
}

SIZE_T ScanSegments(HANDLE proc, BYTE array[], SIZE_T size)
{
	MEMORY_BASIC_INFORMATION meminfo;
	LPCVOID addr = 0;
	SIZE_T result = 0;

	if (!proc)
		return 0;

	while (1)
	{
		if (VirtualQueryEx(proc, addr, &meminfo, sizeof(meminfo)) == 0)
			break;

		if ((meminfo.State & MEM_COMMIT) && (meminfo.Type & MEM_PRIVATE) && 
			(meminfo.Protect & PAGE_READWRITE) && !(meminfo.Protect & PAGE_GUARD))
		{
			result = IsArrayMatch(proc, (SIZE_T)meminfo.BaseAddress, 
				meminfo.RegionSize, array, size);

			if (result != 0)
				return result;
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

	BYTE array[] = { 0, 0, 0, 0, 0x04, 0, 0, 0, 0x03, 0, 0x28, 0x0F, 0, 0x4B, 0x61, 0x69,
		0x6E, 0, 0, 0 };

	SIZE_T objectAddress = ScanSegments(hTargetProc, array, sizeof(array));
	SIZE_T hpAddress = objectAddress + 0x490;
	
	ULONG hp = 0;
	while (1)
	{
		hp = ReadWord(hTargetProc, hpAddress);
		printf("HP = %lu\n", hp);

		if (hp < 100)
			PostMessage(wnd, WM_KEYDOWN, 0x31, 0x1);

		Sleep(2000);
	}
    return 0;
}

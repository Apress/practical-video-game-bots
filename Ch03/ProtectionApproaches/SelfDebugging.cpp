#include <stdint.h>
#include <windows.h>
#include <string>

using namespace std;

static const uint16_t MAX_LIFE = 20;
static uint16_t gLife = MAX_LIFE;

void DebugSelf()
{
	wstring cmdChild(GetCommandLine());
	cmdChild.append(L" x");

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	GetStartupInfo(&si);

	CreateProcess(NULL, (LPWSTR)cmdChild.c_str(), NULL, NULL, FALSE,
		DEBUG_PROCESS | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	DEBUG_EVENT de;
	ZeroMemory(&de, sizeof(DEBUG_EVENT));

	for (;;)
	{
		if (!WaitForDebugEvent(&de, INFINITE))
			return;

		ContinueDebugEvent(de.dwProcessId,
			de.dwThreadId,
			DBG_CONTINUE);
	}
}

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		DebugSelf();
	}

	SHORT result = 0;

	while (gLife > 0)
	{
		result = GetAsyncKeyState(0x31);
		if (result != 0xFFFF8001)
			--gLife;
		else
			++gLife;
			
		printf("life = %u\n", gLife);
		Sleep(1000);
	}

	printf("stop\n");
	return 0;
}

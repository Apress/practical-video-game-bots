#include <stdint.h>
#include <windows.h>

static const uint16_t MAX_LIFE = 20;
static uint16_t gLife = MAX_LIFE;

BOOL IsDebug()
{
	__try
	{
		CloseHandle((HANDLE)0x12345);
	}
	__except (GetExceptionCode() == EXCEPTION_INVALID_HANDLE ?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		return TRUE;
	}
	return FALSE;
}

int main()
{
	SHORT result = 0;

	while (gLife > 0)
	{
		if (IsDebuggerPresent())
		{
			printf("debugger detected!\n");
			exit(EXIT_FAILURE);
		}

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


#include <stdint.h>
#include <windows.h>

static const uint16_t MAX_LIFE = 20;
static uint16_t gLife = MAX_LIFE;

#define CheckDebug() \
bool isDebugger = true; \
__try \
{ \
	__asm int 3 \
} \
__except (GetExceptionCode() == EXCEPTION_BREAKPOINT ? \
		  EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) \
{ \
	isDebugger = false; \
} \
if (isDebugger) \
{ \
	printf("debugger detected!\n"); \
	exit(EXIT_FAILURE); \
}

int main()
{
	SHORT result = 0;

	while (gLife > 0)
	{
		CheckDebug();

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


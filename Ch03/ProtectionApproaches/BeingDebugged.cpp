#include <stdint.h>
#include <windows.h>

static const uint16_t MAX_LIFE = 20;
static uint16_t gLife = MAX_LIFE;

#define CheckDebug() \
int isDebugger = 0; \
{ \
__asm mov eax, dword ptr fs : [18h] \
__asm mov eax, dword ptr ds : [eax + 30h] \
__asm movzx eax, byte ptr ds : [eax + 2h] \
__asm mov isDebugger, eax \
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


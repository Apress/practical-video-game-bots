#include <stdint.h>
#include <windows.h>

using namespace std;

inline uint16_t maskValue(uint16_t value)
{
	static const uint16_t MASK = 0xAAAA;
	return (value ^ MASK);
}

static const uint16_t MAX_LIFE = 20;
static uint16_t gLife = maskValue(MAX_LIFE);

int main(int argc, char* argv[])
{
	SHORT result = 0;

	while (maskValue(gLife) > 0)
	{
		result = GetAsyncKeyState(0x31);
		if (result != 0xFFFF8001)
			gLife = maskValue(maskValue(gLife) - 1);
		else
			gLife = maskValue(maskValue(gLife) + 1);

		printf("life = %u\n", maskValue(gLife));
		Sleep(1000);
	}

	printf("stop\n");

	return 0;
}

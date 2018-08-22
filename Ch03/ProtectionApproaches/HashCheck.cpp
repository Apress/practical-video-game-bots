#include <stdint.h>
#include <windows.h>
#include <functional>

using namespace std;

static const uint16_t MAX_LIFE = 20;
static uint16_t gLife = MAX_LIFE;

std::hash<uint16_t> hashFunc;
static size_t gLifeHash = hashFunc(gLife);

void UpdateHash()
{
	gLifeHash = hashFunc(gLife);
}

__forceinline void CheckHash()
{
	if (gLifeHash != hashFunc(gLife))
	{
		printf("unauthorized modification detected!\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[])
{
	SHORT result = 0;

	while (gLife > 0)
	{
		result = GetAsyncKeyState(0x31);

		CheckHash();

		if (result != 0xFFFF8001)
			--gLife;
		else
			++gLife;

		UpdateHash();
		
		printf("life = %u\n", gLife);

		Sleep(1000);
	}

	printf("stop\n");

	return 0;
}

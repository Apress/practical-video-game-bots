#include <windows.h>

int main()
{
	SIZE_T segmentSize = 260;

	void* segmentAddress = ::VirtualAllocEx(GetCurrentProcess(), NULL, segmentSize,
		MEM_COMMIT, PAGE_READWRITE);

	printf("segmentAddress = %p\n", segmentAddress);

	while (1)
	{
		Sleep(1000);
	}
    return 0;
}


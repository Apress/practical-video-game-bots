#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

typedef LPCTSTR(CALLBACK* LPFNDLLFUNC1)(void);

HINSTANCE hDLL;               // Handle to DLL
LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
LPCTSTR uReturnVal;

int main()
{
	hDLL = LoadLibraryA("FastFind");
	if (hDLL != NULL)
	{
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,
			"FFVersion");
		if (!lpfnDllFunc1)
		{
			// handle the error
			FreeLibrary(hDLL);
			cout << "error" << endl;
			return 1;
		}
		else
		{
			// call the function
			uReturnVal = lpfnDllFunc1();
			cout << "version = " << uReturnVal << endl;
		}
	}
    return 0;
}
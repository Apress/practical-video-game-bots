#include <windows.h>
#include <tlhelp32.h>

void ListProcessHeaps(DWORD pid)
{
    HEAPLIST32 hl;

    HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, pid);

    hl.dwSize = sizeof(HEAPLIST32);

    if (hHeapSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot failed (%d)\n", GetLastError());
        return;
    }

    if (Heap32ListFirst(hHeapSnap, &hl))
    {
        do
        {
            printf("\nHeap ID: 0x%lx\n", hl.th32HeapID);
            printf("\Flags: 0x%lx\n", hl.dwFlags);
        } while (Heap32ListNext(hHeapSnap, &hl));
    }
    else
        printf("Cannot list first heap (%d)\n", GetLastError());

    CloseHandle(hHeapSnap);
}

int main()
{
    DWORD pid = 6712;

    ListProcessHeaps(pid);

    return 0;
}

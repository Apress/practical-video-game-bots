#include <windows.h>
#include <winternl.h>

#pragma comment(lib,"ntdll.lib")

typedef struct _CLIENT_ID {
    DWORD UniqueProcess;
    DWORD UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _THREAD_BASIC_INFORMATION {
    typedef PVOID KPRIORITY;
    NTSTATUS ExitStatus;
    PVOID TebBaseAddress;
    CLIENT_ID ClientId;
    KAFFINITY AffinityMask;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef enum _THREADINFOCLASS2
{
    ThreadBasicInformation,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair_Reusable,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    _ThreadIsIoPending,
    ThreadHideFromDebugger,
    ThreadBreakOnTermination,
    MaxThreadInfoClass
}   THREADINFOCLASS2;

PTEB GetTeb1()
{
#if defined(_M_X64)
    PTEB pTeb = reinterpret_cast<PTEB>(__readgsqword(0x30));
#else
    PTEB pTeb = reinterpret_cast<PTEB>(__readfsdword(0x18));
#endif
    return pTeb;
}

PTEB GetTeb2()
{
#if defined(_M_X64) // x64
    PTEB pTeb = reinterpret_cast<PTEB>(__readgsqword(reinterpret_cast<DWORD>(&static_cast<PNT_TIB>(nullptr)->Self)));
#else // x86
    PTEB pTeb = reinterpret_cast<PTEB>(__readfsdword(reinterpret_cast<DWORD>(&static_cast<PNT_TIB>(nullptr)->Self)));
#endif
    return pTeb;
}

PTEB GetTeb3()
{
    THREAD_BASIC_INFORMATION threadInfo;
    if (NtQueryInformationThread(GetCurrentThread(), (THREADINFOCLASS)ThreadBasicInformation,
        &threadInfo, sizeof(threadInfo), NULL))
    {
        printf("NtQueryInformationThread return error\n");
        return NULL;
    }
    return reinterpret_cast<PTEB>(threadInfo.TebBaseAddress);
}

PTEB GetTeb4()
{
    PTEB pTeb;

    __asm {
        mov EAX, FS:[0x18]
        mov pTeb, EAX
    }
    return pTeb;
}

int main()
{
    {
        PTEB pTeb = GetTeb1();
        PPEB pPeb = pTeb->ProcessEnvironmentBlock;

        printf("GetTeb1() - PEB = %p TEB = %p\n", pPeb, pTeb);
    }
    {
    PTEB pTeb = GetTeb2();
    PPEB pPeb = pTeb->ProcessEnvironmentBlock;

    printf("GetTeb2() - PEB = %p TEB = %p\n", pPeb, pTeb);
    }
    {
        PTEB pTeb = GetTeb3();
        PPEB pPeb = pTeb->ProcessEnvironmentBlock;

        printf("GetTeb3() - PEB = %p TEB = %p\n", pPeb, pTeb);
    }
    {
        PTEB pTeb = GetTeb4();
        PPEB pPeb = pTeb->ProcessEnvironmentBlock;

        printf("GetTeb4() - PEB = %p TEB = %p\n", pPeb, pTeb);
    }
    {
        PTEB pTeb = NtCurrentTeb();
        PPEB pPeb = pTeb->ProcessEnvironmentBlock;

        printf("NtCurrentTeb() - PEB = %p TEB = %p\n", pPeb, pTeb);
    }
    return 0;
}


#include "stdafx.h"
#include <Windows.h>
// one can also use Winternl.h if needed
//#include <Winternl.h> // for UNICODE_STRING and SYSTEM_INFORMATION_CLASS
#include <stdio.h>
#include <tchar.h>

#define STATUS_SUCCESS               ((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH  ((NTSTATUS)0xC0000004L)

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemProcessInformation = 5
} SYSTEM_INFORMATION_CLASS;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;

typedef LONG KPRIORITY; // Thread priority

typedef struct _SYSTEM_PROCESS_INFORMATION_DETAILD {
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER WorkingSetPrivateSize;
	ULONG HardFaultCount;
	ULONG NumberOfThreadsHighWatermark;
	ULONGLONG CycleTime;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR UniqueProcessKey;
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION_DETAILD, *PSYSTEM_PROCESS_INFORMATION_DETAILD;


typedef NTSTATUS(WINAPI *PFN_NT_QUERY_SYSTEM_INFORMATION)(
    IN       SYSTEM_INFORMATION_CLASS SystemInformationClass,
    IN OUT   PVOID SystemInformation,
    IN       ULONG SystemInformationLength,
    OUT OPTIONAL  PULONG ReturnLength
    );

int ProcessInformation()
{
    size_t bufferSize = 102400;
    PSYSTEM_PROCESS_INFORMATION_DETAILD pspid =
        (PSYSTEM_PROCESS_INFORMATION_DETAILD)malloc(bufferSize);
    ULONG ReturnLength;
    PFN_NT_QUERY_SYSTEM_INFORMATION pfnNtQuerySystemInformation = (PFN_NT_QUERY_SYSTEM_INFORMATION)
        GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtQuerySystemInformation");
    NTSTATUS status;

    while (TRUE) {
        status = pfnNtQuerySystemInformation(SystemProcessInformation, (PVOID)pspid,
            bufferSize, &ReturnLength);
        if (status == STATUS_SUCCESS)
            break;
        else if (status != STATUS_INFO_LENGTH_MISMATCH) { // 0xC0000004L
            _tprintf(TEXT("ERROR 0x%X\n"), status);
            return 1;   // error
        }

        bufferSize *= 2;
        pspid = (PSYSTEM_PROCESS_INFORMATION_DETAILD)realloc((PVOID)pspid, bufferSize);
    }

    for (;;
        pspid = (PSYSTEM_PROCESS_INFORMATION_DETAILD)(pspid->NextEntryOffset + (PBYTE)pspid)) {

		_tprintf(TEXT("ProcessId: %d, ImageFileName: %ls, PrivatePageCount: %d, VirtualSize: %d\n"), 
			pspid->UniqueProcessId,
            (pspid->ImageName.Length && pspid->ImageName.Buffer) ? pspid->ImageName.Buffer : L"",
			pspid->PrivatePageCount,
			pspid->VirtualSize);

        if (pspid->NextEntryOffset == 0) break;
    }

    return 0;
}
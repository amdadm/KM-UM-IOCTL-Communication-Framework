#pragma once

#include <ntdef.h>
#include <intrin.h>
#include <ntifs.h>
#include <ntddk.h>

typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

extern "C" {
	NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(IN HANDLE ProcessId, OUT PEPROCESS* Process);
	NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(IN PEPROCESS Process);
	NTKERNELAPI NTSTATUS ZwQuerySystemInformation(DWORD32 systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, PULONG returnLength);
	NTKERNELAPI NTSTATUS IoCreateDriver(IN PUNICODE_STRING DriverName, OPTIONAL IN PDRIVER_INITIALIZE InitializationFunction);
}

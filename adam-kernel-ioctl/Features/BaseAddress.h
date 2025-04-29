#pragma once 

#include "../Includes/Structs.h"

uintptr_t GetBaseAddress(int ProcessPid) {

    PEPROCESS TargetProcess = nullptr;

    NTSTATUS  Status = PsLookupProcessByProcessId(HANDLE(ProcessPid), &TargetProcess);
    if (!NT_SUCCESS(Status))
        return 0;

    const auto BaseAddress = uintptr_t(PsGetProcessSectionBaseAddress(TargetProcess));
    ObDereferenceObject(TargetProcess);

    return BaseAddress;
}
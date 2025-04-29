#pragma once 

#include "../../Includes/Structs.h"
#include "../../Includes/Definitions.h"

#include "../../Features/BaseAddress.h"

NTSTATUS IoDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

    NTSTATUS Status;
    ULONG InfoSize = 0;
    PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
    ULONG ControlCode = Stack->Parameters.DeviceIoControl.IoControlCode;
    ULONG Size = Stack->Parameters.DeviceIoControl.InputBufferLength;

    switch (ControlCode) {


        // Base Address
    case IOCTL_GET_BASE_ADDRESS: {

        auto ModuleInfo = (PGetBaseAddressRequest)Irp->AssociatedIrp.SystemBuffer;

        ULONGLONG handle = GetBaseAddress(ModuleInfo->ProcessId);
        ModuleInfo->BaseAddress = handle;

        Status = STATUS_SUCCESS;

        InfoSize = sizeof(GetBaseAddressRequest);

    } break;

    // ...

    default:
        Status = STATUS_INVALID_PARAMETER;
        InfoSize = 0;
        break;
    }

    Irp->IoStatus.Status = Status;
    Irp->IoStatus.Information = InfoSize;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Status;
}
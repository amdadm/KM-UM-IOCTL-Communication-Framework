#pragma once

/*===============================================================================================
  Project         : KM-UM-IOCTL-Communication-Framework

  Original Author : amdadm
  GitHub          : https://github.com/amdadm
  Created         : 2025-04-28

  Description     : Framework for establishing communication between Kernel Mode (KM) and User Mode (UM) components through IOCTL.

  License         : MIT License
===============================================================================================*/

#include "../Includes/Structs.h"

#include "IoControl/IoDeviceControl.h"

#define DeviceNameBuffer L"\\Device\\IoctlCommunicationDriver"
#define DeviceSymLinkBuffer L"\\DosDevices\\IoctlCommunicationDriver"
#define DriverNameBuffer  L"\\Driver\\IoctlCommunicationDriver"

PDEVICE_OBJECT DeviceObject;
UNICODE_STRING DeviceName, DeviceSymLink;

namespace DriverUtils {

    NTSTATUS UnsupportedDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

        UNREFERENCED_PARAMETER(DeviceObject);
        Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return Irp->IoStatus.Status;
    }

    NTSTATUS CreateDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

        Irp->IoStatus.Status = STATUS_SUCCESS;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    }

    void DriverUnload(PDRIVER_OBJECT DriverObject) {

        NTSTATUS Status = { };

        Status = IoDeleteSymbolicLink(&DeviceSymLink);
        if (!NT_SUCCESS(Status))
            return;

        IoDeleteDevice(DriverObject->DeviceObject);
    }
}

NTSTATUS Entry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

    NTSTATUS Status = STATUS_UNSUCCESSFUL;

    RtlInitUnicodeString(&DeviceName, DeviceNameBuffer);
    RtlInitUnicodeString(&DeviceSymLink, DeviceSymLinkBuffer);

    Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    Status = IoCreateSymbolicLink(&DeviceSymLink, &DeviceName);
    if (!NT_SUCCESS(Status)) {
        IoDeleteDevice(DeviceObject);
        return Status;
    }

    for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++) {
        DriverObject->MajorFunction[i] = &DriverUtils::UnsupportedDispatch;
    }

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoDeviceControl;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverUtils::CreateDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverUtils::CreateDispatch;
    DriverObject->DriverUnload = DriverUtils::DriverUnload;

    DeviceObject->Flags |= DO_DIRECT_IO;
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    DriverObject->DriverSection = NULL;

    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    UNICODE_STRING DriverName;
    RtlInitUnicodeString(&DriverName, DriverNameBuffer);

    return IoCreateDriver(&DriverName, &Entry);
}

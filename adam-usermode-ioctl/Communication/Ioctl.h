#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <emmintrin.h>
#include <intrin.h>

const char* DeviceNameBuffer = "\\\\.\\IoctlCommunicationDriver";

#define IOCTL_GET_BASE_ADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x12345, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

HANDLE DriverHandle = nullptr;
uint64_t TargetPid = 0;
uintptr_t BaseAddress = 0;

typedef struct _GetBaseAddressRequest {
    ULONG       ProcessId;
    ULONGLONG   BaseAddress;
} GetBaseAddressRequest, * PGetBaseAddressRequest;

DWORD GetProcessID(const std::string& processName) {

    DWORD pid = 0;
    PROCESSENTRY32 processEntry = {};
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (_stricmp(processEntry.szExeFile, processName.c_str()) == 0) {
                pid = processEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return pid;
}

uint64_t GetBaseAddress() {

    if (!DriverHandle || DriverHandle == INVALID_HANDLE_VALUE)
        return 0;

    GetBaseAddressRequest request = {};
    request.ProcessId = TargetPid;

    if (!DeviceIoControl(
        DriverHandle,
        IOCTL_GET_BASE_ADDRESS,
        &request,
        sizeof(request),
        &request,
        sizeof(request),
        nullptr,
        nullptr
    )) {
        return 0;
    }

    return request.BaseAddress;
}

bool InitializeCommunication() {

    DriverHandle = CreateFileA(
        DeviceNameBuffer,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    return (DriverHandle && DriverHandle != INVALID_HANDLE_VALUE);
}

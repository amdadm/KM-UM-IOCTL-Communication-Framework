#pragma once

#define IOCTL_GET_BASE_ADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x12345, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _GetBaseAddressRequest
{
	ULONG       ProcessId;
	ULONGLONG   BaseAddress;
} GetBaseAddressRequest, * PGetBaseAddressRequest;

#ifndef SLSUSBINTERFACE_H
#define SLSUSBINTERFACE_H

#include "windows.h"

#ifdef SLSUSB_MULT_EP_EXPORTS
#define SLSUSB_API __declspec(dllexport)
#else
#define SLSUSB_API __declspec(dllimport)
#endif


typedef PVOID	SLS_HANDLE;
typedef ULONG	SLS_STATUS;

struct SLS_OVERLAPPED:public OVERLAPPED
{

   BOOL bFlagReadWrite;

};

struct SLS_DEVICE_STATUS
{
	BOOL bFlagDeviceStatus;
};

typedef struct SLS_OVERLAPPED SLS_OVERLAPPED;
typedef struct SLS_OVERLAPPED *PSLS_OVERLAPPED;
//
// Device status
//
enum {
    SLS_OK,
    SLS_INVALID_HANDLE,
    SLS_DEVICE_NOT_FOUND,
    SLS_DEVICE_NOT_OPENED,
    SLS_IO_ERROR,
    SLS_INSUFFICIENT_RESOURCES,
    SLS_INVALID_PARAMETER,


    SLS_DEVICE_NOT_OPENED_FOR_WRITE,
    SLS_FAILED_TO_WRITE_DEVICE,
    SLS_INVALID_ARGS,
	SLS_NOT_SUPPORTED,
	SLS_OTHER_ERROR
};


#define SLS_SUCCESS(status) ((status) == SLS_OK)

//
// SLS_OpenEx Flags
//

#define SLS_OPEN_BY_SERIAL_NUMBER    1
#define SLS_OPEN_BY_DESCRIPTION      2

//
// SLS_ListDevices Flags (used in conjunction with SLS_OpenEx Flags
//
#define SLS_DEVICE_INTERFACE            0xA0000000
#define SLS_LIST_NUMBER_ONLY			0x80000000
#define SLS_LIST_BY_INDEX		    	0x40000000
#define SLS_LIST_ALL					0x20000000

#define SLS_LIST_MASK (SLS_LIST_NUMBER_ONLY|SLS_LIST_BY_INDEX|SLS_LIST_ALL)


#ifdef __cplusplus
extern "C" {
#endif


SLSUSB_API
SLS_STATUS WINAPI SLS_ListDevices(
	PVOID pArg1,
	PVOID pArg2,
	DWORD dwFlags
	);

SLSUSB_API
SLS_HANDLE WINAPI SLS_W32_CreateFile(
	LPCSTR					lpszName,
	DWORD					dwAccess,
	DWORD					dwShareMode,
	LPSECURITY_ATTRIBUTES	lpSecurityAttributes,
	DWORD					dwCreate,
	DWORD					dwAttrsAndFlags,
	HANDLE					hTemplate,
	WORD					interface_ep
	);
SLSUSB_API
BOOL WINAPI SLS_W32_ReadFile(
    SLS_HANDLE SLSHandle,
    LPVOID lpBuffer,
    DWORD nBufferSize,
    LPDWORD lpBytesReturned,
	PSLS_OVERLAPPED lpOverlapped

    );

SLSUSB_API
BOOL WINAPI SLS_W32_WriteFile(
    SLS_HANDLE SLSHandle,
    LPVOID lpBuffer,
    DWORD nBufferSize,
    LPDWORD lpBytesWritten,
	PSLS_OVERLAPPED lpOverlapped

    );

SLSUSB_API
BOOL WINAPI SLS_W32_CloseDevcie(
    SLS_HANDLE SLSHandle

	);

SLSUSB_API
BOOL WINAPI SLS_W32_ClosePipeHandle(
    SLS_HANDLE SLSHandle


	);

SLSUSB_API
SLS_STATUS WINAPI SLS_ResetDevice(
    SLS_HANDLE SLSHandle
	);

SLSUSB_API
BOOL WINAPI SLS_W32_GetOverlappedResult(
    SLS_HANDLE SLSHandle,
	PSLS_OVERLAPPED lpOverlapped,
    LPDWORD lpdwBytesTransferred,
	BOOL bWait

    );

SLSUSB_API
BOOL WINAPI SLS_W32_CancelIo(
    SLS_HANDLE SLSHandle

    );

SLSUSB_API
SLS_STATUS WINAPI SLS_W32_SendVendorRequest(

	HANDLE					hDevice,					 
	LPCSTR					inbuffer,
	UINT					inbufSize,
	LPCSTR					outbuffer,
	UINT					outbufSize,
	PULONG					dwBytesReturned
	);

SLSUSB_API
SLS_STATUS WINAPI SLS_W32_SetInterface(

	HANDLE					hDevice,					 
	LPCSTR					inbuffer,
	UINT					inbufSize,
	LPCSTR					outbuffer,
	UINT					outbufSize,
	PULONG					dwBytesReturned
	
	);
#ifdef __cplusplus
}
#endif


#endif


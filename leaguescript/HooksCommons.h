#pragma once

#include <Windows.h>
#include <vector>

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING;

typedef UNICODE_STRING* PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;

typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;

struct VECTORED_HANDLER_ENTRY
{
	VECTORED_HANDLER_ENTRY* next;
	VECTORED_HANDLER_ENTRY* previous;
	void* refs;
	void* unk;
	PVECTORED_EXCEPTION_HANDLER handler;
};

struct _VECTORED_HANDLER_LIST
{
	void* mutex_exception;
	VECTORED_HANDLER_ENTRY* first_exception_handler;
	VECTORED_HANDLER_ENTRY* last_exception_handler;
	void* mutex_continue;
	VECTORED_HANDLER_ENTRY* first_continue_handler;
	VECTORED_HANDLER_ENTRY* last_continue_handler;
};;

#ifndef InitializeObjectAttributes
#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }
#endif


struct HookDetails {
	DWORD hookAddress;
	DWORD addressToHook;
};

struct HookEntries
{
	std::vector<HookDetails> hookDetails;
	DWORD addressToHookOldProtect;
	DWORD addressToHookMbiStart;
	DWORD addressToHookMbiEnd;
	DWORD addressToHookMbiSize;
	DWORD allocatedAddressStart;
	DWORD allocatedAddressEnd;
	DWORD allocatedAddressSize;
	DWORD addressToHookoffsetFromStart;
};

namespace Process
{
	std::vector<PVECTORED_EXCEPTION_HANDLER>	GetAllHandlers();
	void										RemoveAllHandlers();
	void										ReAddAllHandlers(std::vector<PVECTORED_EXCEPTION_HANDLER> handlerList);
}
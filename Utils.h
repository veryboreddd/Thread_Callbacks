#pragma once

#include <ntifs.h>
#include <windef.h>

/*
	Function prototypes
*/


BOOLEAN Compare_bytes(IN PBYTE base, IN PBYTE pattern);

PBYTE Find_Pattern(IN PBYTE base, IN SIZE_T size, IN PBYTE pattern);

PVOID Get_Rel_Address(IN PVOID base, IN ULONG offset, IN ULONG size);
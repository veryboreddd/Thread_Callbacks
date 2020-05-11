#include "Utils.h"

BOOLEAN Compare_bytes(IN PBYTE base, IN PBYTE pattern)
{
	for (; *pattern; pattern++, base++)
	{
		if (*pattern == 0xFF)
			continue;

		if (*base != *pattern)
		{
			return FALSE;
		}
	}

	return TRUE;
}

PBYTE Find_Pattern(IN PBYTE base, IN SIZE_T size, IN PBYTE pattern)
{
	PBYTE end_address = base + size;

	for (; base < end_address; base++)
	{
		if (*base != *pattern)
			continue;

		if (Compare_bytes(base, pattern))
		{
			return base;
		}
	}

	return NULL;
}

PVOID Get_Rel_Address(IN PVOID base, IN ULONG offset, IN ULONG size)
{
	ULONG_PTR instr = (ULONG_PTR)base;
	LONG rel_off = *(PLONG)(instr + offset);
	PVOID addr = (PVOID)(instr + size + rel_off);

	return addr;
}
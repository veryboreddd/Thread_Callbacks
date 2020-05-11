#include "ThreadHook.h"

#define MAX_CALLBACKS 10

/*
	PATTERNS:
*/

BYTE Psp_Create_Thread_Notify_Routine_Pattern[14] = { 0x48,0x8D,0xFF,0xFF,0xFF,0xFF,0xFF,0x48,0x8D,0x2C,0xF9,0x48,0x8B,0xCD };

/*
	Query the thread callback routines and hook ours at index 0
*/

NTSTATUS Get_Thread_Callbacks(IN PULONG_PTR PspCreateThreadNotifyRoutine)
{
	/*
		CASE: PspCreateThreadNotifyRoutine is NULL
	*/

	if (PspCreateThreadNotifyRoutine == NULL)
	{
		return STATUS_INVALID_PARAMETER;
	}

	/*
		LOCALS:

	*/

	PEX_CALLBACK ex_callback;
	EX_FAST_REF callback_ref;
	PEX_CALLBACK_ROUTINE_BLOCK callback_routine_block;

	ULONG index = 0;

	for (; index < MAX_CALLBACKS; index++)
	{
		/*
			CASE: The next entry is empty we've reached the end of the list.
		*/

		if (PspCreateThreadNotifyRoutine[index] == 0)
		{
			break;
		}

		/*
			Get the callback entry
		*/

		ex_callback = (PEX_CALLBACK)&PspCreateThreadNotifyRoutine[index];

		callback_ref = ex_callback->RoutineBlock;

		/*
			Ignore the reference count.
		*/

		callback_routine_block = (PEX_CALLBACK_ROUTINE_BLOCK)(callback_ref.Value & 0xFFFFFFFFFFFFFFF0);

		/*
			Hook the first callback routine
		*/

		if (index == 0)
		{
			original_callback_routine_block = callback_routine_block;

			original_callback = (PEX_CALLBACK_FUNCTION)InterlockedExchangePointer((PVOID*)&callback_routine_block->Function, (PVOID)Our_Notify_Routine);

			o_Notify_Routine = (t_Notify_Routine)original_callback;
		}
	}

	return STATUS_SUCCESS;
}

/*
	Our hooked thread notify routine
*/

VOID Our_Notify_Routine(HANDLE ProcessId, HANDLE ThreadId, BOOLEAN Create)
{
	DbgPrintEx(0, 0, "Hooked \nProcess %x \nThread %x \nCreated: %x \n", ProcessId, ThreadId, Create);

	o_Notify_Routine(ProcessId, ThreadId, Create);
}

/*
	Obtain the address of PspCreateThreadNotifyRoutine
*/

PULONG_PTR Get_PspCreateThreadNotifyRoutine(VOID)
{
	/*
		LOCALS:
	*/

	PULONG_PTR PspCreateThreadNotifyRoutine = NULL;
	PBYTE found_pattern;

	/*
		Search for our pattern inside of the routine
		SIZE: 0x100 due to it's location inside our function.
	*/

	found_pattern = Find_Pattern((PBYTE)PsRemoveCreateThreadNotifyRoutine, 0x100, Psp_Create_Thread_Notify_Routine_Pattern);

	/*
		CASE: Our pattern is NULL
	*/

	if (found_pattern == NULL)
	{
		return NULL;
	}

	/*
		CASE: Unable to calculate the relative address.
	*/

	if (PspCreateThreadNotifyRoutine == NULL)
	{
		return NULL;
	}

	DbgPrintEx(0, 0, "%llx %llx \n", PspCreateThreadNotifyRoutine, found_pattern);


	return PspCreateThreadNotifyRoutine;
}
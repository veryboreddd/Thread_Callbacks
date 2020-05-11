#include <ntifs.h>
#include <windef.h>

#include "ThreadHook.h"

NTSTATUS DriverEntry(IN PDRIVER_OBJECT driver_object, IN PUNICODE_STRING reg_path);
VOID Saucy_Unload(IN PDRIVER_OBJECT driver_object);


NTSTATUS DriverEntry(IN PDRIVER_OBJECT driver_object, PUNICODE_STRING reg_path)
{
	UNREFERENCED_PARAMETER(reg_path);

	/*
		CASE: Driver object is NULL.
	*/

	if (driver_object == NULL)
	{
		return STATUS_ENTRYPOINT_NOT_FOUND;
	}

	/*
		Set our unload routine
	*/

	driver_object->DriverUnload = Saucy_Unload;

	/*
		LOCALS: (FOR TESTING)
	*/

	PULONG_PTR PspCreateThreadNotifyRoutine = Get_PspCreateThreadNotifyRoutine();

	/*
		CASE: We couldn't found our address
	*/

	if (PspCreateThreadNotifyRoutine == NULL)
	{
		return STATUS_INVALID_ADDRESS;
	}

	/*
		CASE: We fail to query or hook the callback routine.
	*/

	if (!NT_SUCCESS(Get_Thread_Callbacks(PspCreateThreadNotifyRoutine)))
	{
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}

VOID Saucy_Unload(IN PDRIVER_OBJECT driver_object)
{
	UNREFERENCED_PARAMETER(driver_object);

	/*
		Revert the callback routine back to the original.
	*/

	original_callback_routine_block->Function = original_callback;
}
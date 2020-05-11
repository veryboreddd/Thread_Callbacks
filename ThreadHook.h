#pragma once

#include <ntifs.h>
#include <windef.h>
#include "Natives.h"
#include "Utils.h"

/*
	TYPEDEF:
*/

typedef VOID(*t_Notify_Routine)(HANDLE ProcessId, HANDLE ThreadId, BOOLEAN Create);
t_Notify_Routine o_Notify_Routine;

PEX_CALLBACK_ROUTINE_BLOCK original_callback_routine_block;
PEX_CALLBACK_FUNCTION original_callback;

/*
	PROTO:
*/

VOID Our_Notify_Routine(IN HANDLE ProcessId, IN HANDLE ThreadId, IN BOOLEAN Create);

NTSTATUS Get_Thread_Callbacks(IN PULONG_PTR PspCreateThreadNotifyRoutine);

PULONG_PTR Get_PspCreateThreadNotifyRoutine(VOID);

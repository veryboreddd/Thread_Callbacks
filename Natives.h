#pragma onceobreg
#pragma warning(disable : 4201)
#pragma warning(disable : 4214)

typedef struct _EX_FAST_REF
{
	union
	{
		PVOID Object;
		ULONG RefCnt : 3;
		ULONG64 Value;
	};
} EX_FAST_REF, * PEX_FAST_REF;

typedef struct _EX_CALLBACK
{
	EX_FAST_REF RoutineBlock;
} EX_CALLBACK, * PEX_CALLBACK;

typedef struct _EX_CALLBACK_ROUTINE_BLOCK
{
	EX_RUNDOWN_REF RundownProtect;
	PEX_CALLBACK_FUNCTION Function;
	PVOID Context;
} EX_CALLBACK_ROUTINE_BLOCK, * PEX_CALLBACK_ROUTINE_BLOCK;

#if defined WIN32

long HookMethod(long object, long argument, ...);

long _stdcall HookMethod_0()
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, NULL);
}

long _stdcall HookMethod_1(long argument)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument);
}

long _stdcall HookMethod_2(long argument, long argument2)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2);
}

long _stdcall HookMethod_3(long argument, long argument2, long argument3)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3);
}

long _stdcall HookMethod_4(long argument, long argument2, long argument3, long argument4)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4);
}

long _stdcall HookMethod_5(long argument, long argument2, long argument3, long argument4, long argument5)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5);
}

long _stdcall HookMethod_6(long argument, long argument2, long argument3, long argument4, long argument5, long argument6)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5, argument6);
}

long _stdcall HookMethod_7(long argument, long argument2, long argument3, long argument4, long argument5, long argument6, long argument7)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5, argument6, argument7);
}

long _stdcall HookMethod_8(long argument, long argument2, long argument3, long argument4, long argument5, long argument6, long argument7, long argument8)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5, argument6, argument7, argument8);
}

long _stdcall HookMethod_9(long argument, long argument2, long argument3, long argument4, long argument5, long argument6, long argument7, long argument8, long argument9)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5, argument6, argument7, argument8, argument9);
}

long _stdcall HookMethod_10(long argument, long argument2, long argument3, long argument4, long argument5, long argument6, long argument7, long argument8, long argument9, long argument10)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5, argument6, argument7, argument8, argument9, argument10);
}

long _stdcall HookMethod_11(long argument, long argument2, long argument3, long argument4, long argument5, long argument6, long argument7, long argument8, long argument9, long argument10, long argument11)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5, argument6, argument7, argument8, argument9, argument10, argument11);
}

long _stdcall HookMethod_12(long argument, long argument2, long argument3, long argument4, long argument5, long argument6, long argument7, long argument8, long argument9, long argument10, long argument11, long argument12)
{
	long object;
	_asm mov object, ecx;
	return HookMethod(object, argument, argument2, argument3, argument4, argument5, argument6, argument7, argument8, argument9, argument10, argument11, argument12);
}

void* HooksMethod[] =
{
	(void*)&HookMethod_0,
	(void*)&HookMethod_1,
	(void*)&HookMethod_2,
	(void*)&HookMethod_3,
	(void*)&HookMethod_4,
	(void*)&HookMethod_5,
	(void*)&HookMethod_6,
	(void*)&HookMethod_7,
	(void*)&HookMethod_8,
	(void*)&HookMethod_9,
	(void*)&HookMethod_10,
	(void*)&HookMethod_11,
	(void*)&HookMethod_12
};

#endif


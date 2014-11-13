
#include <function.h>
#include <configManager.h>
#include <cbase.h>
#include <am-string.h>

#include <global.h>

#include <functionMethodHooks.h>

unsigned char PreJumpBase[] ={ 0x66, 0xc7, 0x5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0, 0xe9, 0x00, 0x00, 0x00, 0x00 };
unsigned int PreJumpSize = sizeof PreJumpBase;

unsigned int PreJumpIDAddressPosition = 3;
unsigned int PreJumpIDValuePosition = 7;
unsigned int PreJumpIDHookAddressPosition = 10;

unsigned char *PreJumps[0xFFFF];

unsigned short int ID;

long Hook(long argument, ...)
{
	Function* function = Global::FunctionManagerObj->getFunction(ID);
	long* argumentsHolder = function->getHookArgumentsHolder();

	va_list arguments;
	va_start(arguments, argument);

	for (size_t i = 0; i < function->getArgumentsCount(); ++i)
	{
		argumentsHolder[i] = argument;
		argument = va_arg(arguments, long);
	}

	va_end(arguments);

	return function->hook();
}

#if defined WIN32

long HookMethod(long object, long argument, ...)
{
	Function* function = Global::FunctionManagerObj->getFunction(ID);
	long* argumentsHolder = function->getHookArgumentsHolder();

	argumentsHolder[0] = (long)object;

	va_list arguments;
	va_start(arguments, argument);

	for (unsigned int i=1; i < function->getArgumentsCount(); ++i)
	{
		argumentsHolder[i] = argument;
		argument = va_arg(arguments, long);
	}

	va_end(arguments);

	return function->hook();
}

#endif

unsigned char Function::patch[] ={ 0xE9, 0x00, 0x00, 0x00, 0x00 };
int Function::patchSize = sizeof(patch);
unsigned short int* Function::patchFunctionIDAddress = (unsigned short int*) &patch[2];
long* Function::patchFunctionJumpAddress = (long*)&patch[1];

Function::Function(void* address, TypeHandler** argumentsHandlers, unsigned int argumentsCount, TypeHandler* returnHandler, ke::AString library, bool isMethod)
{
	this->address = address;
	this->isMethod = isMethod;

	this->argumentsHandlers = argumentsHandlers;
	this->argumentsCount = argumentsCount;
	this->returnHandler = returnHandler;

	this->library = library;

	ifReturnByRefParamsCount = returnHandler ? returnHandler->getIfByRefParamsCount() : 0;

	normalArguments = new long[argumentsCount];

	espDislocation = sizeof(long) * argumentsCount;
	espDislocationAddHook = espDislocation + sizeof(long) + sizeof(char*) + sizeof(AMX*);
	espDislocationCallForward = espDislocation + sizeof(long);

#if defined WIN32
	passObject = (isMethod) ? &Function::passObjectMethod : &Function::passObjectNonMethod;

	if (isMethod)
	{
		espDislocationCall = 0;
	}
	else
	{
		espDislocationCall = espDislocation;
	}
#endif

	argumentsByRef = new TypeHandler*[argumentsCount];
	argumentsByRefPosition = new unsigned int[argumentsCount];
	argumentsByRefCount = 0;

	parameters = new ForwardParam[argumentsCount];
	argumentsToAmx = new cell[argumentsCount];

	for (size_t i = 0; i < argumentsCount; ++i)
	{
		parameters[i] = argumentsHandlers[i]->getParamType();

		if (argumentsHandlers[i]->isPassedByRef())
		{
			argumentsByRef[argumentsByRefCount] = argumentsHandlers[i];
			argumentsByRefPosition[argumentsByRefCount] = i;
			argumentsByRefCount++;
		}
	}

	originalBytes = (unsigned char*) new unsigned char[patchSize];
	patchedBytes = (unsigned char*) new unsigned char[patchSize];

	argumentsPassLoopStart = argumentsCount - 1;

#if defined WIN32
	argumentsPassLoopEnd = (int)isMethod;
#else
	argumentsPassLoopEnd = 0;
#endif

	isPatched = false;

	this->currentHookID[OrpheuHookPre] = this->currentHookID[OrpheuHookPost] = 0;

	hooks[OrpheuHookPre] = new HooksDataMap;
	hooks[OrpheuHookPost] = new HooksDataMap;

	hooks[OrpheuHookPre]->init();
	hooks[OrpheuHookPost]->init();

	shouldCallHooks = true;
}

void Function::setID(unsigned short int id)
{
	this->id = id;
	this->preparePatch();
}

void Function::preparePatch()
{
	unsigned char* preJump = PreJumps[id] = (unsigned char*)malloc(PreJumpSize);

	Memory::ChangeMemoryProtection(preJump, PreJumpSize, PAGE_EXECUTE_READWRITE);

	memcpy((void*)preJump, PreJumpBase, PreJumpSize);

	*((long*)(&preJump[PreJumpIDAddressPosition])) = (long)&ID;
	*((unsigned short int*)(&preJump[PreJumpIDValuePosition])) = id;

#if defined WIN32
	if (isMethod)
		*((long*)(&preJump[PreJumpIDHookAddressPosition])) = (char*)HooksMethod[argumentsCount - 1] - (char*)preJump - PreJumpSize;
	else
#endif
		*((long*)(&preJump[PreJumpIDHookAddressPosition])) = (char*)&Hook - (char*)preJump - PreJumpSize;

	patchedBytes[0] = 0xE9;
	*((long*)(&patchedBytes[1])) = (char*)preJump - (char*)address - 5;

	memcpy((void*)originalBytes, address, patchSize);
}

void Function::doPatch()
{
	if (!isPatched)
	{
		if (Memory::ChangeMemoryProtection(address, patchSize, PAGE_EXECUTE_READWRITE))
		{
			isPatched = true;
			memcpy(address, (void*)patchedBytes, patchSize);
		}
		else
		{
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString("Memory patching failed\n"));
		}
	}
}

void Function::undoPatch()
{
	if (isPatched)
	{
		if (Memory::ChangeMemoryProtection(address, patchSize, PAGE_EXECUTE_READWRITE))
		{
			isPatched = false;
			memcpy(address, (void*)originalBytes, this->patchSize);
		}
		else
		{
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString("Memory patching failed\n"));
		}
	}
}

cell Function::call(AMX* amx, cell* params)
{
	for (size_t i = 0; i < argumentsCount; ++i)
	{
		normalArguments[i] = (long)argumentsHandlers[i]->convertFromAmx(amx, params[i]);
	}

#if defined WIN32
	int end = (int)isMethod;

#else
	int end = 0;

#endif
	for (int i = argumentsCount - 1; i >= end; i--)
	{
#if defined WIN32
		long argument = normalArguments[i];
		__asm push argument;
#else
		asm volatile ("push %0"::"m"(normalArguments[i]));
#endif
	}

	long returnValue;

#if defined WIN32

	void* address = this->address;

	(this->*(passObject))();

	_asm
	{
		call address;
		mov returnValue, eax;
	};
#else
	asm ("call *%0"::"m"(address));
	asm ("mov %%eax , %0":"=r"(returnValue));
#endif

#if defined WIN32
	long espDislocation = this->espDislocationCall;
	_asm add esp, espDislocation
#else
	asm ("add %0, %%esp"::"m"(espDislocation));
#endif

	cell value = 0;

	if (returnHandler)
	{
		returnHandler->convertToAmx(value, returnValue, Call);
		returnHandler->convertToAmxByRef(amx, &params[argumentsCount], returnValue);
	}

	for (size_t i = 0; i < argumentsByRefCount; ++i)
	{
		unsigned int argumentPositionInFunction = argumentsByRefPosition[i];
		argumentsByRef[i]->convertToAmxByRef(amx, &params[argumentPositionInFunction], normalArguments[argumentPositionInFunction]);
	}

	return value;
}

unsigned int& Function::getIfReturnByRefParamsCount()
{
	return ifReturnByRefParamsCount;
}

unsigned int& Function::getArgumentsCount()
{
	return argumentsCount;
}

long Function::callOriginal()
{
	for (int i=argumentsPassLoopStart; i >= argumentsPassLoopEnd; i--)
	{
#if defined WIN32
		long argument = normalArguments[i];
		__asm push argument;
#else
		asm volatile ("push %0"::"m"(normalArguments[i]));
#endif
	}

	long returnValue;

#if defined WIN32

	void* address = this->address;

	(this->*(passObject))();

	_asm
	{
		call address;
		mov returnValue, eax;
	};

#else
	asm ("call *%0"::"m"(address));
	asm ("mov %%eax , %0":"=r"(returnValue));
#endif

#if defined WIN32
	long espDislocation = this->espDislocationCall;
	_asm add esp, espDislocation
#else
	asm ("add %0, %%esp"::"m"(espDislocation));
#endif

	return returnValue;
}

long Function::hook()
{
	hookPhase = OrpheuHookPre;

	hookReturnStatus = OrpheuIgnored;

	Function* functionBeingHookedBefore = Global::FunctionBeingHooked;
	Global::FunctionBeingHooked = this;

	hookReturnValue = 0;

	bool shouldCallHooks = this->shouldCallHooks;
	this->shouldCallHooks = true;

	convertArgumentsToAmx();

	if (shouldCallHooks)
		hookReturnStatus = callHooks(OrpheuHookPre);

	if (hookReturnStatus < OrpheuSupercede)
	{
		this->undoPatch();
		long returnValue = callOriginal();
		this->doPatch();

		hookPhase = OrpheuHookPost;

		if (hookReturnStatus != OrpheuOverride)
		{
			hookReturnValue = returnValue;

			if (returnHandler && !getIfReturnByRefParamsCount())
			{
				returnHandler->convertToAmx(hookReturnValueConverted, hookReturnValue, CallHooked);
			}
		}
	}

	if (shouldCallHooks)
		hookReturnStatus = callHooks(OrpheuHookPost);

	Global::FunctionBeingHooked = functionBeingHookedBefore;

	return hookReturnValue;
}

long Function::addHook(AMX* amx, const char* functionName, OrpheuHookPhase phase)
{
	doPatch();

	long hookID = this->currentHookID[OrpheuHookPre]++;
	long forward;

#ifdef WIN32

	unsigned int start = argumentsCount;
	long addressStart = (long)&parameters[0];
	long espDislocation = this->espDislocationAddHook;

	_asm
	{
		mov eax, addressStart
			mov ebx, start
			push FP_DONE
		loopStartAdd :
		cmp ebx, 0
			je loopEndAdd
			dec ebx
			mov  ecx, [eax + ebx * 4]
			push ecx
			jmp loopStartAdd
		loopEndAdd :
		push functionName
			push amx
			call MF_RegisterSPForwardByName
			mov forward, eax
			add esp, espDislocation
	}
#else
	asm volatile(
		"mov %1, %%eax;"
		"mov %2, %%ebx;"
		"pushl $-1;"
		"loopStart:"
		"cmp $0, %%ebx;"
		"je loopEnd;"
		"dec %%ebx;"
		"mov (%%eax,%%ebx,4),%%ecx;"
		"pushl %%ecx;"
		"jmp loopStart;"
		"loopEnd:"
		"pushl %3;"
		"pushl %4;"
		"call *%5;"
		"mov %%eax, %0;"
		"add %6, %%esp;"
		:"=r"(forward):"m"(parameters),"m"(argumentsCount),"r"(functionName),"r"(amx),"m"(MF_RegisterSPForwardByName),"m"(espDislocationAddHook):"eax","ebx","ecx");
#endif

	HooksDataMap::Insert i = hooks[phase]->findForAdd(hookID);

	if (!i.found())
	{
		if (hooks[phase]->add(i))
		{
			i->key = hookID;
		}
	}
	i->value = forward;

	return hookID;
}

void Function::removeAllHooks()
{
	for (int i=0; i <= 1; ++i)
	{
		for (Function::HooksDataMap::iterator iter = hooks[i]->iter(); !iter.empty(); iter.next())
		{
			MF_UnregisterSPForward(iter->value);

			iter.erase();
		}
	}

	undoPatch();
	this->currentHookID[0] = this->currentHookID[1] = 0;
}
void Function::removeHook(OrpheuHookPhase phase, long functionHookPhaseID)
{
	Function::HooksDataMap::Result r = hooks[phase]->find(functionHookPhaseID);

	if (r.found())
	{
		MF_UnregisterSPForward(r->value);

		hooks[phase]->remove(r);

		if (!hooks[phase]->elements())
		{
			this->currentHookID[phase] = 0;

			if (!hooks[!phase]->elements())
			{
				undoPatch();
			}
		}
	}
}

void Function::convertArgumentsToAmx()
{
	for (size_t i = 0; i < argumentsCount; ++i)
	{
		argumentsHandlers[i]->convertToAmx(argumentsToAmx[i], normalArguments[i], Pass);
	}
}

OrpheuHookReturn Function::callHooks(OrpheuHookPhase phase)
{
	OrpheuHookReturn hookReturnStatus = OrpheuIgnored;

	for (HooksDataMap::iterator iter = hooks[phase]->iter(); !iter.empty(); iter.next())
	{
		long forward = iter->value;

		OrpheuHookReturn newHookReturnStatus = callForward(forward);

		Global::FunctionBeingHooked = this;

		if (newHookReturnStatus > hookReturnStatus)
		{
			hookReturnStatus = newHookReturnStatus;

			if (hookReturnStatus == OrpheuSupercede)
			{
				break;
			}
		}
	}

	return hookReturnStatus;
}

OrpheuHookReturn Function::callForward(long forward)
{
	OrpheuHookReturn hookReturnStatus;

#if defined WIN32

	unsigned int start = argumentsCount;
	long addressStart = (long)&argumentsToAmx[0];
	long espDislocation = this->espDislocationCallForward;

	_asm
	{
		mov eax, addressStart
			mov ebx, start
		loopStart :
		cmp ebx, 0
			je loopEnd
			dec ebx
			mov  ecx, [eax + ebx * 4]
			push ecx
			jmp loopStart
		loopEnd :
		push forward
			call MF_ExecuteForward
			mov hookReturnStatus, eax
			add esp, espDislocation
	}

#else

	asm volatile (
		"mov %1, %%eax;"
		"mov %2, %%ebx;"
		"0:"
		"cmp $0, %%ebx;"
		"je 1f;"
		"dec %%ebx;"
		"mov (%%eax,%%ebx,4),%%ecx;"
		"pushl %%ecx;"
		"jmp 0b;"
		"1:"
		"pushl %3;"
		"call *%4;"
		"mov %%eax, %0;"
		"add %5, %%esp;"
		:"=r"(hookReturnStatus):"m"(argumentsToAmx),"m"(argumentsCount),"r"(forward),"m"(MF_ExecuteForward),"r"(espDislocationCallForward):"eax","ebx","ecx");
#endif

	return hookReturnStatus;
}

cell Function::getReturnToAmx(AMX* amx, cell* params)
{
	if (this->getIfReturnByRefParamsCount())
	{
		returnHandler->convertToAmxByRef(amx, params, hookReturnValue);
	}
	else
	{
		return hookReturnValueConverted;
	}

	return 0;
}

void Function::setReturnFromAmx(AMX* amx, cell* params)
{
	hookReturnValue = (long)returnHandler->convertFromAmx(amx, *params);
}

bool Function::setParamFromAmx(AMX* amx, cell* params)
{
	unsigned int argumentID = params[0] - 1;

	if (argumentID < argumentsCount)
	{
		normalArguments[argumentID] = (long)argumentsHandlers[argumentID]->convertFromAmx(amx, params[1]);

		return true;
	}

	return false;
}

TypeHandler* Function::getArgumentHandler(unsigned int argumentID)
{
	if (argumentID < argumentsCount)
	{
		return argumentsHandlers[argumentID];
	}

	return NULL;
}

long Function::getArgumentValue(unsigned int argumentID)
{
	if (argumentID < argumentsCount)
	{
		return normalArguments[argumentID];
	}

	return 0;
}

Function::~Function()
{
	delete[] originalBytes;
	delete[] patchedBytes;

	delete[] normalArguments;
}
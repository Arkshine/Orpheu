
#include <memoryUtil.h>

namespace Memory
{
	int ChangeMemoryProtection(void* function, unsigned int size, unsigned long newProtection)
	{
#ifdef __linux__
		void* alignedAddress = Align(function);
		return !mprotect(alignedAddress, sysconf(_SC_PAGESIZE), newProtection);
#else
		FlushInstructionCache(GetCurrentProcess(), function, size);

		static DWORD oldProtection;
		return VirtualProtect(function, size, newProtection, &oldProtection);
#endif
	}

	int ChangeMemoryProtection(void* address, unsigned int size, unsigned long newProtection, unsigned long & oldProtection)
	{
#ifdef __linux__
		void* alignedAddress = Align(address);

		oldProtection = newProtection;

		return !mprotect(alignedAddress, sysconf(_SC_PAGESIZE), newProtection);
#else
		FlushInstructionCache(GetCurrentProcess(), address, size);

		return VirtualProtect(address, size, newProtection, &oldProtection);
#endif
	}
}
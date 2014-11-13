
#include <librariesManager.h>

#include <sm_stringhashmap.h>

#if defined __linux__
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <link.h>
#include <limits.h>
#include <errno.h>

#ifndef uint32
#define uint32	unsigned int
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef PAGESIZE
#define PAGESIZE sysconf(_SC_PAGESIZE)
#endif

typedef int BOOL;
#else
#include <windows.h>
#include <psapi.h>
#endif

namespace LibrariesManager
{
	StringHashMap<LibraryInfo*>* LibraryNameToLibraryInfo = new StringHashMap < LibraryInfo* > ;

	bool hasLibrary(const char *libraryName)
	{
		return LibraryNameToLibraryInfo->retrieve(libraryName);
	}

#if defined __linux__

	char* BaseAddress;
	char* EndAddress;
	char* LibraryName;

	inline uint32 IAlign(uint32 address)
	{
		return (address & ~(PAGESIZE - 1));
	}

	inline uint32 IAlign2(uint32 address)
	{
		return (IAlign(address) + PAGESIZE);
	}

	static int dl_callback(struct dl_phdr_info *info, size_t size, void *data)
	{
		char* libraryName = (char*)data;

		if ((!libraryName) || strstr(info->dlpi_name, libraryName) > 0)
		{
			int i;
			BOOL ismain = FALSE;

			if (info->dlpi_addr == 0x00)
				ismain = TRUE;
			else
				BaseAddress = (char *)info->dlpi_addr;

			for (i = 0; i < info->dlpi_phnum; ++i)
			{
				if (info->dlpi_phdr[i].p_memsz && IAlign(info->dlpi_phdr[i].p_vaddr))
				{
					if (ismain && (uint32)BaseAddress > IAlign(info->dlpi_phdr[i].p_vaddr))
						BaseAddress = (char*)IAlign(info->dlpi_phdr[i].p_vaddr);

					if ((uint32)EndAddress < (info->dlpi_phdr[i].p_vaddr + info->dlpi_phdr[i].p_memsz))
						EndAddress = (char*)IAlign2((info->dlpi_phdr[i].p_vaddr + info->dlpi_phdr[i].p_memsz));
				}
			}

			EndAddress += info->dlpi_addr;

			return (int)BaseAddress;
		}

		return 0;
	}

	bool addLibrary(const char* libraryName, void* addressContained)
	{
		Dl_info info;

		if (addressContained && dladdr(addressContained, &info))
		{
			LibraryName = (char*)info.dli_fname;

			BaseAddress = (char *)0xffffffff;
			EndAddress = 0;

			if (dl_iterate_phdr(dl_callback, LibraryName))
			{
				LibraryInfo* libraryInfo = new LibraryInfo;

				libraryInfo->baseAddress = (void*)BaseAddress;
				libraryInfo->length = (long)EndAddress - (long)BaseAddress;
				libraryInfo->handle = dlopen(LibraryName, RTLD_NOW);

				LibraryNameToLibraryInfo->insert(libraryName, libraryInfo);

				return true;
			}
		}

		return false;
	}

#else

	bool addLibrary(const char* libraryName, void* addressContained)
	{
		HMODULE module;

		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)addressContained, &module))
		{
			HANDLE process =  GetCurrentProcess();
			_MODULEINFO moduleInfo;

			if (GetModuleInformation(process, module, &moduleInfo, sizeof moduleInfo))
			{
				CloseHandle(process);

				LibraryInfo* libraryInfo = new LibraryInfo;

				libraryInfo->baseAddress = (void*)moduleInfo.lpBaseOfDll;
				libraryInfo->length = moduleInfo.SizeOfImage;
				libraryInfo->handle = module;

				LibraryNameToLibraryInfo->insert(libraryName, libraryInfo);

				return true;
			}
		}

		return false;
	}
#endif

	void* findFunction(const char* libraryName, const char* functionName)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
#if defined __linux__

			return dlsym(libraryInfo->handle, functionName);
#else

			return GetProcAddress((HMODULE)libraryInfo->handle, functionName);
#endif
		}
		return NULL;
	}
	bool compareSignature(unsigned char* address, unsigned char* signature, SignatureEntryType* signatureData, unsigned int length)
	{
		if (length == 1)
		{
			switch (*signatureData)
			{
			case AnyByteOrNothing:
			case AnyByte:
			{
				return true;
			}
			case SpecificByte:
			{
				return *address == *signature;
			}
			}
		}
		else
		{
			switch (*signatureData)
			{
			case SpecificByte:
			{
				if (*address != *signature)
				{
					return false;
				}
				else
				{
					return compareSignature(address + 1, signature + 1, signatureData + 1, length - 1);
				}
			}
			case AnyByteOrNothing:
			{
				if (compareSignature(address, signature + 1, signatureData + 1, length - 1))
				{
					return true;
				}
			}
			case AnyByte:
			{
				return compareSignature(address + 1, signature + 1, signatureData + 1, length - 1);
			}
			}
		}

		return true;
	}
	void* findFunction(const char* libraryName, unsigned char* signature, SignatureEntryType* signatureData, unsigned int length)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
			for (size_t i = 0; i <= libraryInfo->length - length; ++i)
			{
				if (compareSignature((unsigned char *)libraryInfo->baseAddress + i, signature, signatureData, length))
				{
					return (void*)(((unsigned char*)libraryInfo->baseAddress) + i);
				}
			}
		}

		return NULL;
	}

	void* findMemory(const char* libraryName, unsigned char* signature, SignatureEntryType* signatureData, unsigned int length, long start)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
			for (unsigned int i = start - (unsigned int)libraryInfo->baseAddress; i <= libraryInfo->length - length; ++i)
			{
				if (compareSignature((unsigned char *)libraryInfo->baseAddress + i, signature, signatureData, length))
				{
					return (void*)(((unsigned char*)libraryInfo->baseAddress) + i);
				}
			}
		}

		return NULL;
	}
	bool libraryContainsAddress(const char* libraryName, long address)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
			return (address >= (long)libraryInfo->baseAddress) && (address <= ((long)libraryInfo->baseAddress + libraryInfo->length));
		}

		return false;
	}
	long getAddressOffset(long address, const char* libraryName)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
			return address - (long)libraryInfo->baseAddress;
		}

		return 0;
	}
	long getAddressWithOffset(long offset, const char* libraryName)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
			if ((offset >= 0) && (offset <= (libraryInfo->length)))
			{
				return (long)libraryInfo->baseAddress + offset;
			}
		}

		return 0;
	}
	long getLibraryAddress(const char* libraryName)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
			return (long)libraryInfo->baseAddress;
		}

		return 0;
	}
	LibraryInfo* getLibrary(const char* libraryName)
	{
		LibraryInfo* libraryInfo;

		if (LibraryNameToLibraryInfo->retrieve(libraryName, &libraryInfo))
		{
			return libraryInfo;
		}

		return NULL;
	}
}

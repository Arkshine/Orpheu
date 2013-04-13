
#include <librariesManager.h>

#include <sm_trie_tpl.h>

#if defined __linux__
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#else
#include <windows.h>
#include <psapi.h>
#endif

namespace LibrariesManager
{
	KTrie<LibraryInfo*>* LibraryNameToLibraryInfo = new KTrie<LibraryInfo*>;	

	bool hasLibrary(const char *libraryName)
	{
		return LibraryNameToLibraryInfo->retrieve(libraryName) != NULL;
	}

#if defined __linux__

	// Code derived from code from David Anderson
	long getLength(void *baseAddress)
	{
			pid_t pid = getpid();
			char file[255];
			char buffer[2048];
			snprintf(file, sizeof(file)-1, "/proc/%d/maps", pid);
			FILE *fp = fopen(file, "rt");
			if (fp)
			{
				long length = 0;

        		unsigned long *start = NULL;
        		unsigned long *end = NULL;

        		while (!feof(fp))
				{
					fgets(buffer, sizeof(buffer)-1, fp);			
#if defined AMD64
					sscanf(buffer, "%Lx-%Lx", (unsigned long *)&start, (unsigned long *)&end);
#else
					sscanf(buffer, "%lx-%lx", (unsigned long *)&start, (unsigned long *)&end);
#endif
					if(start == baseAddress)
					{
						length = (unsigned long)end  - (unsigned long)start;

						char ignore[100];
						int value;

						while(!feof(fp))
						{
							fgets(buffer, sizeof(buffer)-1, fp);
#if defined AMD64
							sscanf(buffer, "%Lx-%Lx %s %s %s %d", (unsigned long *)&start, (unsigned long *)&end, ignore, ignore, ignore, &value);
#else
	        					sscanf(buffer, "%lx-%lx %s %s %s %d", (unsigned long *)&start, (unsigned long *)&end, ignore, ignore ,ignore, &value);
#endif
							if(!value)
							{		
								break;
							}
							else
							{
								length += end  - start;
							}
						}
						
						break;
					}
        		}

				fclose(fp);

			return length;
		}

		return 0;
	}
	bool addLibrary(const char* libraryName,void* addressContained)
	{
		Dl_info info;

		if(dladdr(addressContained,&info))
		{
			LibraryInfo* libraryInfo = new LibraryInfo;

			libraryInfo->baseAddress = (void*) info.dli_fbase;
			libraryInfo->length = getLength((void*) info.dli_fbase);
			libraryInfo->handle = dlopen(info.dli_fname, RTLD_NOW);

			LibraryNameToLibraryInfo->insert(libraryName,libraryInfo);
			
			return true;
		}

		return false;
	}

#else

	bool addLibrary(const char* libraryName,void* addressContained)
	{
		HMODULE module;

		if(GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(LPCSTR)addressContained,&module))
		{
			HANDLE process =  GetCurrentProcess();
			_MODULEINFO moduleInfo;

			if(GetModuleInformation(process,module,&moduleInfo,sizeof moduleInfo))
			{
				CloseHandle(process);

				LibraryInfo* libraryInfo = new LibraryInfo;

				libraryInfo->baseAddress = (void*) moduleInfo.lpBaseOfDll;
				libraryInfo->length = moduleInfo.SizeOfImage;
				libraryInfo->handle = module;

				LibraryNameToLibraryInfo->insert(libraryName,libraryInfo);

				return true;
			}
		}

		return false;
	}
#endif

	void* findFunction(const char* libraryName, const char* functionName)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			LibraryInfo* libraryInfo = *libraryInfoPointer;

#if defined __linux__

			return dlsym(libraryInfo->handle,functionName);
#else

			return GetProcAddress((HMODULE)libraryInfo->handle,functionName);
#endif
		}
		return NULL;
	}
	bool compareSignature(unsigned char* address,unsigned char* signature,SignatureEntryType* signatureData,unsigned int length)	
	{
		if( length == 1 )
		{
			switch( *signatureData )
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
			switch( *signatureData )
			{
				case SpecificByte:
				{
					if(*address != *signature)
					{
						return false;
					}
					else
					{
						return compareSignature(address+1,signature+1,signatureData+1,length-1);	
					}
				}
				case AnyByteOrNothing:
				{
					if(compareSignature(address,signature+1,signatureData+1,length-1))
					{
						return true;
					}
				}
				case AnyByte:
				{
					return compareSignature(address+1,signature+1,signatureData+1,length-1);
				}
			}
		}

		return true;
	}
	void* findFunction(const char* libraryName,unsigned char* signature,SignatureEntryType* signatureData,unsigned int length)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			LibraryInfo* libraryInfo = *libraryInfoPointer;

			for(unsigned int i=0;i<= libraryInfo->length - length;i++)
			{
				if(compareSignature((unsigned char *)libraryInfo->baseAddress + i,signature,signatureData,length))
				{
					return (void*) (((unsigned char*)libraryInfo->baseAddress) + i);
				}
			}
		}

		return NULL;
	}

	void* findMemory(const char* libraryName,unsigned char* signature,SignatureEntryType* signatureData,unsigned int length,long start)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			LibraryInfo* libraryInfo = *libraryInfoPointer;

			for( unsigned int i = start - (unsigned int) libraryInfo->baseAddress ;i<= libraryInfo->length - length;i++)
			{
				if(compareSignature((unsigned char *)libraryInfo->baseAddress + i,signature,signatureData,length))
				{
					return (void*) (((unsigned char*)libraryInfo->baseAddress) + i);
				}
			}
		}

		return NULL;
	}
	bool libraryContainsAddress(const char* libraryName,long address)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			LibraryInfo* libraryInfo = *libraryInfoPointer;

			return (address >= (long)libraryInfo->baseAddress) && (address <= ((long)libraryInfo->baseAddress + libraryInfo->length));
		}

		return false;
	}
	long getAddressOffset(long address, const char* libraryName)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			LibraryInfo* libraryInfo = *libraryInfoPointer;
			return address - (long)libraryInfo->baseAddress;
		}

		return 0;
	}
	long getAddressWithOffset(long offset,const char* libraryName)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			LibraryInfo* libraryInfo = *libraryInfoPointer;

			if((offset >= 0) && (offset <= (libraryInfo->length)))
			{
				return (long)libraryInfo->baseAddress + offset;
			}
		}

		return 0;
	}
	long getLibraryAddress( const char* libraryName)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			LibraryInfo* libraryInfo = *libraryInfoPointer;
			return (long) libraryInfo->baseAddress;
		}

		return 0;
	}
	LibraryInfo* getLibrary(const char* libraryName)
	{
		LibraryInfo** libraryInfoPointer = LibraryNameToLibraryInfo->retrieve(libraryName);

		if(libraryInfoPointer)
		{
			return *libraryInfoPointer;
		}

		return NULL;
	}		
}


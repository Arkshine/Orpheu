
#include <structHandler.h>

#undef min
#undef max

#include <map>



class StructManager
{
	map<long, StructHandler*> structAddressToHandler;

public:

	void add(long structureAddress, StructHandler* structureHandler)
	{
		structAddressToHandler[structureAddress] = structureHandler;
	}
	long clone(long structureAddress, StructHandler* structureHandler)
	{
		long newStructureAddress = createStructure(structureHandler);

		add(newStructureAddress, structureHandler);

		memcpy((void*)newStructureAddress, (void*)structureAddress, structureHandler->getSize());

		return newStructureAddress;
	}
	StructHandler* getHandler(long structureAddress)
	{
		map<long, StructHandler*>::iterator iterator = structAddressToHandler.find(structureAddress);

		if (iterator != structAddressToHandler.end())
		{
			return (*iterator).second;
		}

		return NULL;
	}
	long createStructure(StructHandler* structureHandler)
	{
		long structureAddress = structureHandler->allocate();
		add(structureAddress, structureHandler);
		return structureAddress;
	}
};


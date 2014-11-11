
#include <structHandler.h>

#undef min
#undef max

class StructManager
{
	typedef ke::HashMap< long, StructHandler*, ke::IntegerPolicy<long> > StructTableMap;
	StructTableMap structAddressToHandler;

public:

	StructManager()
	{
		structAddressToHandler.init();
	}

	void add(long structureAddress, StructHandler* structureHandler)
	{
		StructTableMap::Insert i = structAddressToHandler.findForAdd(structureAddress);

		if (!i.found())
		{
			if (structAddressToHandler.add(i))
			{
				i->key = structureAddress;
			}
		}
		i->value = structureHandler;
	}

	long clone(long structureAddress, StructHandler* structureHandler)
	{
		long newStructureAddress = createStructure(structureHandler);

		add(newStructureAddress, structureHandler);

		memcpy((void*)newStructureAddress, (void*)structureAddress, structureHandler->getStructSize());

		return newStructureAddress;
	}

	StructHandler* getHandler(long structureAddress)
	{
		StructTableMap::Result r = structAddressToHandler.find(structureAddress);

		if (r.found())
		{
			return r->value;
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


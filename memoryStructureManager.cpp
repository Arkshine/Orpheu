
#include <memoryStructureManager.h>

MemoryStructure* MemoryStructManager::get(char* name)
{
	unsigned int id;

	if (memoryStructureNameToMemoryStructureID.retrieve(name, &id))
	{
		return this->memoryStructures.at(id);
	}

	return NULL;
}
void MemoryStructManager::add(MemoryStructure* memoryStructure)
{
	char* memoryStructureName = (char*)memoryStructure->name.chars();

	unsigned int id;

	if (memoryStructureNameToMemoryStructureID.retrieve(memoryStructureName, &id))
	{
		//delete memoryStructures->at(id);
		this->memoryStructures.at(id) = memoryStructure;
	}
	else
	{
		id = memoryStructures.length();
		memoryStructures.append(memoryStructure);
	}

	this->memoryStructureNameToMemoryStructureID.replace(memoryStructureName, id);
}


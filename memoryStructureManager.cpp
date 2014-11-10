
#include <memoryStructureManager.h>

MemoryStructure* MemoryStructManager::get(char* name)
{
	unsigned int* idPointer = this->memoryStructureNameToMemoryStructureID.retrieve(name);

	if(idPointer)
	{
		return this->memoryStructures.at(*idPointer);
	}

	return NULL;
}
void MemoryStructManager::add(MemoryStructure* memoryStructure)
{
	char* memoryStructureName = (char*) memoryStructure->name.chars();

	unsigned int* idPointer = this->memoryStructureNameToMemoryStructureID.retrieve(memoryStructureName);

	unsigned int id;

	if(idPointer)
	{
		id = *idPointer;
		//delete memoryStructures->at(id);
		this->memoryStructures.at(id) = memoryStructure;
	}
	else
	{
		id = memoryStructures.length();
		memoryStructures.append(memoryStructure);
	}

	this->memoryStructureNameToMemoryStructureID.replace(memoryStructureName,id);
}



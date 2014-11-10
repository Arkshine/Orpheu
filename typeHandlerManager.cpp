
#include "typeHandlerManager.h"

TypeHandlerManager::TypeHandlerManager(KTrie<ke::Vector<char*>*>* typeAliases)
{
	registeredTypeHandlers = new KTrie<TypeHandler*>();
	this->typeAliases = typeAliases;
}

void TypeHandlerManager::registerTypeHandler(const char* label,TypeHandler* handler)
{
	registeredTypeHandlers->insert(label,handler);

	ke::Vector<char*>** aliasesPointer = typeAliases->retrieve(label);

	if(aliasesPointer)
	{
		ke::Vector<char*>* aliases = *aliasesPointer;

		for(unsigned int i=0;i<aliases->length();i++)
		{
			registeredTypeHandlers->insert(aliases->at(i),handler);
		}
	}
}
bool TypeHandlerManager::typeHandlerExists(char* label)
{
	return registeredTypeHandlers->retrieve(label) != NULL;
}
TypeHandler* TypeHandlerManager::getTypeHandler(char* label)
{
	TypeHandler** handlerPointer = registeredTypeHandlers->retrieve(label);
	
	if(handlerPointer)
		return *handlerPointer;
	
	return NULL;
}


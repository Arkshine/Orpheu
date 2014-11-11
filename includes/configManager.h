
#ifndef _INCLUDE_CONFIG_MANAGER_
#define _INCLUDE_CONFIG_MANAGER_

#include <orpheu.h>
#include <am-vector.h>
#include <am-string.h>
#include <sm_stringhashmap.h>
#include <jansson.h>
#include <memoryUtil.h>

class ConfigManager
{
public:

	ConfigManager();
	void loadBaseData();
	void loadFunctions();
	void loadVirtualFunctions();
	void loadMemoryStructures();
	void parseMemoryObject(json_t *root);

	ke::Vector<ke::AString> ModuleConfig;

private:

	Folders orpheuPaths;

	void obtainPaths();
	StringHashMap<char*>* parseExternalLibrariesInfo();
	StringHashMap<ke::Vector<char*>*>* parseTypeAliasesInfo(StringHashMap<long>& typeNameToVirtualTableOffset);
	void parseFunctionsInfo();
	void parseVirtualFunctionsInfo();
	void parseModsInfo();
};

#endif



#ifndef _INCLUDE_CONFIG_MANAGER_
#define _INCLUDE_CONFIG_MANAGER_

#include <orpheu.h>
#include <am-vector.h>
#include <sm_stringhashmap.h>
#include <am-string.h>
#include "jansson.h"

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

size_t UTIL_Format(char *buffer, size_t maxlength, const char *fmt, ...);

#endif


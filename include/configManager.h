
#ifndef _INCLUDE_CONFIG_MANAGER_
#define _INCLUDE_CONFIG_MANAGER_

#include <orpheu.h>
#include <CVector.h>
#include "CString.h"
#include <sm_trie_tpl.h>
#include <am-string.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <json/reader.h>
#include "jansson.h"

#include <memory.h>



class ConfigManager
{
public:

	ConfigManager();
	void loadBaseData();
	void loadFunctions();
	void loadVirtualFunctions();
	void loadMemoryStructures();
	void parseMemoryObject(json_t *root);

	String ModuleConfig;

private:

	Folders orpheuPaths;

	void obtainPaths();
	KTrie<char*>* parseExternalLibrariesInfo();
	KTrie<CVector<char*>*>* parseTypeAliasesInfo(KTrie<long>& typeNameToVirtualTableOffset);
	void parseFunctionsInfo();
	void parseVirtualFunctionsInfo();
	void parseModsInfo();
};

size_t UTIL_Format(char *buffer, size_t maxlength, const char *fmt, ...);

#endif


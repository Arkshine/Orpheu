
#ifndef _INCLUDE_CONFIG_MANAGER_
#define _INCLUDE_CONFIG_MANAGER_

#include <orpheu.h>
#include <CVector.h>
#include "CString.h"
#include <sm_trie_tpl.h>
#include <string>
#include <jansson.h>
#include <memoryUtil.h>

using namespace std;

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

#endif


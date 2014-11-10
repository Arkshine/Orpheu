
#ifndef _INCLUDE_CONFIG_MANAGER_
#define _INCLUDE_CONFIG_MANAGER_

#include <orpheu.h>
#include <am-vector.h>
#include <am-string.h>
#include <sm_trie_tpl.h>
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

		ke::Vector<ke::AString> ModuleConfig;

	private:

		Folders orpheuPaths;

		void obtainPaths();
		KTrie<char*>* parseExternalLibrariesInfo();		
		KTrie<ke::Vector<char*>*>* parseTypeAliasesInfo(KTrie<long>& typeNameToVirtualTableOffset);
		void parseFunctionsInfo();
		void parseVirtualFunctionsInfo();
		void parseModsInfo();
};

#endif


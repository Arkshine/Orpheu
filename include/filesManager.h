
#ifndef _INCLUDE_FILES_MANAGER_
#define _INCLUDE_FILES_MANAGER_

#include <CVector.h>

#include <am-string.h>

namespace FilesManager
{
	CVector<ke::AString>* getFolders(const ke::AString& directory);
	CVector<ke::AString>* getFiles(const ke::AString& directory);
}

#endif

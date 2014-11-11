
#ifndef _INCLUDE_FILES_MANAGER_
#define _INCLUDE_FILES_MANAGER_

#include <am-vector.h>
#include <am-string.h>

namespace FilesManager
{
	ke::Vector<ke::AString>* getFolders(const char *directory);
	ke::Vector<ke::AString>* getFiles(const char *directory);
}

#endif

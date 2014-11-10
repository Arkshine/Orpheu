
#ifndef _INCLUDE_FILES_MANAGER_
#define _INCLUDE_FILES_MANAGER_

#include <am-vector.h>

#include <string>
using namespace std;

namespace FilesManager
{
	ke::Vector<string>* getFolders(const char *directory);
	ke::Vector<string>* getFiles(const char *directory);
}

#endif

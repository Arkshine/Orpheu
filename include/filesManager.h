
#ifndef _INCLUDE_FILES_MANAGER_
#define _INCLUDE_FILES_MANAGER_

#include <CVector.h>

#include <string>
using namespace std;

namespace FilesManager
{
	CVector<string>* getFolders(const char *directory);
	CVector<string>* getFiles(const char *directory);
}

#endif

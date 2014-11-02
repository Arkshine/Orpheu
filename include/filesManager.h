
#ifndef _INCLUDE_FILES_MANAGER_
#define _INCLUDE_FILES_MANAGER_

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::filesystem;

#include <CVector.h>

#include <am-string.h>
using namespace std;

namespace FilesManager
{
	CVector<ke::AString>* getFolders(const ke::AString& directory);
	CVector<ke::AString>* getFiles(const ke::AString& directory);
}

#endif


#ifndef _INCLUDE_FILES_MANAGER_
#define _INCLUDE_FILES_MANAGER_

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::filesystem; 

#include <CVector.h>

#include <string>
using namespace std;

namespace FilesManager
{
	CVector<string>* getFolders(const path& directory);
	CVector<string>* getFiles(const path& directory);
}

#endif

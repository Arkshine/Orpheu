
#include <filesManager.h>

namespace FilesManager
{
	CVector<string>* getFolders(const path& directory)
	{
		CVector<string>* folders = new CVector<string>();

		if(exists(directory))
		{
			directory_iterator end;
			for(directory_iterator iter(directory); iter != end ; ++iter)
			{
				if(is_directory(*iter))
				{
					folders->push_back(iter->path().filename().c_str());
				}
			}
		}

		return folders;
	}
	CVector<string>* getFiles(const path& directory)
	{
		CVector<string>* files = new CVector<string>();

		if(exists(directory))
		{
			directory_iterator end;
			for(directory_iterator iter(directory); iter != end ; ++iter)
			{
				if(is_regular_file(*iter))
				{
					files->push_back(iter->path().filename().c_str());
				}
			}

		}

		return files;
	}
}



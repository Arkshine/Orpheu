
#include <filesManager.h>
#include <configManager.h>

#if defined(WIN32)
#include <Windows.h>
#else
#include <sys/types.h>
#include <dirent.h>
#endif

namespace FilesManager
{
	bool dirExists(const char *dir)
	{
#if defined WIN32 || defined _WIN32
		DWORD attr = GetFileAttributes(dir);

		if (attr == INVALID_FILE_ATTRIBUTES)
			return false;

		if (attr & FILE_ATTRIBUTE_DIRECTORY)
			return true;

#else
		struct stat s;

		if (stat(dir, &s) != 0)
			return false;

		if (S_ISDIR(s.st_mode))
			return true;
#endif

		return false;
	}

	ke::Vector<ke::AString>* getFolders(const char* directory)
	{
		ke::Vector<ke::AString>* folders = new ke::Vector<ke::AString>();

		if (dirExists(directory))
		{
#if defined WIN32 || defined _WIN32
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s*", directory);

			WIN32_FIND_DATA fd;
			HANDLE hFile = FindFirstFile(path, &fd);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (fd.cFileName[0] == '.')
					{
						continue;
					}

					UTIL_Format(path, sizeof(path) - 1, "%s%s", directory, fd.cFileName);

					if (dirExists(path))
					{
						folders->append(ke::AString(fd.cFileName));
					}
				} while (FindNextFile(hFile, &fd));

				FindClose(hFile);
			}
#else
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s", directory);

			DIR *dp = opendir(path);

			if (dp != 0)
			{
				struct dirent *ep;

				while ((ep = readdir(dp)))
				{
					if (ep->d_name[0] == '.')
					{
						continue;
					}

					UTIL_Format(path, sizeof(path) - 1, "%s%s", directory, ep->d_name);

					if (dirExists(path))
					{
						folders->append(ke::AString(ep->d_name));
					}
				}

				closedir(dp);
			}
#endif
		}

		return folders;
	}

	ke::Vector<ke::AString>* getFiles(const char* directory)
	{
		ke::Vector<ke::AString>* files = new ke::Vector<ke::AString>();

		if (dirExists(directory))
		{
#if defined WIN32 || defined _WIN32
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s*", directory);

			WIN32_FIND_DATA fd;
			HANDLE hFile = FindFirstFile(path, &fd);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (fd.cFileName[0] == '.')
					{
						continue;
					}

					UTIL_Format(path, sizeof(path) - 1, "%s%s", directory, fd.cFileName);

					if (!dirExists(path))
					{
						files->append(ke::AString(fd.cFileName));
					}
				} while (FindNextFile(hFile, &fd));

				FindClose(hFile);
			}
#else
			char path[256];
			UTIL_Format(path, sizeof(path) - 1, "%s", directory);

			DIR *dp = opendir(path);

			if (dp != 0)
			{
				struct dirent *ep;

				while ((ep = readdir(dp)))
				{
					if (ep->d_name[0] == '.')
					{
						continue;
					}

					UTIL_Format(path, sizeof(path) - 1, "%s%s", directory, ep->d_name);

					if (!dirExists(path))
					{
						files->append(ke::AString(ep->d_name));
					}
				}

				closedir(dp);
			}
#endif
		}

		return files;
	}
}


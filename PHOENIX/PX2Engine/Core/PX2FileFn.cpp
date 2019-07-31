// PX2FileFn.cpp

#include "PX2FileFn.hpp"
#include "PX2Filename.hpp"
using namespace PX2;

#if defined (WIN32) || defined(_WIN32)

#include <windows.h>

static bool PX2CheckWin32Permission(const std::string& path, DWORD access)
{
	const DWORD dwAttr = ::GetFileAttributes(path.c_str());
	if (dwAttr == INVALID_FILE_ATTRIBUTES)
	{
		// file probably doesn't exist at all
		return false;
	}

	HANDLE h = ::CreateFile
		(
		path.c_str(),
		access,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		dwAttr & FILE_ATTRIBUTE_DIRECTORY
		? FILE_FLAG_BACKUP_SEMANTICS
		: 0,
		NULL
		);
	if (h != INVALID_HANDLE_VALUE)
		CloseHandle(h);

	return h != INVALID_HANDLE_VALUE;
}
#endif

//----------------------------------------------------------------------------
bool IsWritable(const std::string &path)
{
#if defined (WIN32) || defined(_WIN32)
	return PX2CheckWin32Permission(path, GENERIC_WRITE);
#endif
    
    return false;
}
//----------------------------------------------------------------------------
bool IsReadable(const std::string &path)
{
#if defined (WIN32) || defined(_WIN32)
	return PX2CheckWin32Permission(path, GENERIC_READ);
#endif
    
    return false;
}
//----------------------------------------------------------------------------
bool IsExecutable(const std::string &path)
{
#if defined (WIN32) || defined(_WIN32)
	return PX2CheckWin32Permission(path, GENERIC_EXECUTE);
#endif
    
    return false;
}
//----------------------------------------------------------------------------
inline bool IsCharSeparator(char c)
{
#if ( defined(__UNIX__) && !defined(__OS2__) )|| defined(__MAC__)
	return c == PX2_FILE_SEP_PATH;
#else
	return c == PX2_FILE_SEP_PATH_DOS || c == PX2_FILE_SEP_PATH_UNIX;
#endif
}
//----------------------------------------------------------------------------
bool IsEndsWithPathSeparator(const std::string& filename)
{
	if (filename.empty())
		return false;

	return IsCharSeparator(filename[filename.length() - 1]);
}
//----------------------------------------------------------------------------
bool IsDirExists(const std::string& pathName)
{
	return Filename::IsDirExists(pathName);
}
//----------------------------------------------------------------------------
bool IsFileExists(const std::string& filename)
{
	return Filename::IsFileExists(filename);
}
//----------------------------------------------------------------------------
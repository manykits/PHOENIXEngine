// PX2DirData.cpp

#include "PX2DirData.hpp"
#include "PX2Memory.hpp"
#include "PX2Log.hpp"
#include "PX2Dir.hpp"
#include "PX2Filename.hpp"

#if defined (WIN32) || defined (_WIN32)
#include <windows.h>
using namespace PX2;

//----------------------------------------------------------------------------
typedef WIN32_FIND_DATA FIND_STRUCT;
typedef HANDLE FIND_DATA;
typedef DWORD FIND_ATTR;
//----------------------------------------------------------------------------
inline bool IsFindDataOk(FIND_DATA fd)
{
	return fd != INVALID_HANDLE_VALUE;
}
//----------------------------------------------------------------------------
inline void FreeFindData(FIND_DATA fd)
{
	if (!::FindClose(fd))
	{
	}
}
const char *GetNameFromFindData(const FIND_STRUCT *finddata)
{
	return finddata->cFileName;
}
//----------------------------------------------------------------------------
inline bool CheckFoundMatch(const FIND_STRUCT* finddata,
	const std::string& filter)
{
	//if (filter.empty())
	//	return true;

	//std::string fn(GetNameFromFindData(finddata));

	//if (filter.find_first_not_of("*?.") == std::string::npos)
	//	return fn.Matches(filter);

	//return fn.MakeUpper().Matches(filter.Upper());

	return true;
}
//----------------------------------------------------------------------------
inline bool FindNext(FIND_DATA fd, const std::string& filter,
	FIND_STRUCT *finddata)
{
	for (;;)
	{
		if (!::FindNextFile(fd, finddata))
			return false;

		// If we did find something, check that it really matches.
		if (CheckFoundMatch(finddata, filter))
			return true;
	}
}
//----------------------------------------------------------------------------
inline FIND_DATA FindFirst(const std::string& spec, const std::string& filter,
	FIND_STRUCT *finddata)
{
	FIND_DATA fd = ::FindFirstFile(spec.c_str(), finddata);

	if (IsFindDataOk(fd) && !CheckFoundMatch(finddata, filter))
	{
		if (!FindNext(fd, filter, finddata))
		{
			FreeFindData(fd);

			return INVALID_HANDLE_VALUE;
		}
	}

	return fd;
}
//----------------------------------------------------------------------------
inline FIND_ATTR GetAttrFromFindData(FIND_STRUCT *finddata)
{
	return finddata->dwFileAttributes;
}
//----------------------------------------------------------------------------
inline bool IsDir(FIND_ATTR attr)
{
	return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
//----------------------------------------------------------------------------
inline bool IsHidden(FIND_ATTR attr)
{
	return (attr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) != 0;
}
//----------------------------------------------------------------------------
#elif defined (__LINUX__) || defined (__ANDROID__) || defined(__IOS__)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
//----------------------------------------------------------------------------
inline bool IsFindDataOk(void* fd)
{
	return fd != 0;
}
#endif
//----------------------------------------------------------------------------
PX2::DirData::DirData(const std::string& dirname) :
mDirname(dirname)
{
#if defined (_WIN32) || defined (WIN32)
	mFinddata = INVALID_HANDLE_VALUE;
#else
	mFinddata = 0;
	mFinddata = opendir(mDirname.c_str());
#endif
}
//----------------------------------------------------------------------------
PX2::DirData::~DirData()
{
	Close();
}
//----------------------------------------------------------------------------
void PX2::DirData::SetFileSpec(const std::string& filespec)
{
	mFilespec = filespec;
}
//----------------------------------------------------------------------------
void PX2::DirData::SetFlags(int flags)
{
	mFlags = flags;
}
//----------------------------------------------------------------------------
void PX2::DirData::Close()
{
#if defined (WIN32) || defined (_WIN32)
	if (IsFindDataOk(mFinddata))
	{
		FreeFindData(mFinddata);

		mFinddata = INVALID_HANDLE_VALUE;
	}
#elif defined (__LINUX__) || defined (__ANDROID__)
	DIR *dir = (DIR*)mFinddata;
	if (dir)
	{
		if (closedir(dir) != 0)
		{
		}
		mFinddata = 0;
	}
#endif
}
//----------------------------------------------------------------------------
void PX2::DirData::Rewind()
{
	Close();
}
//----------------------------------------------------------------------------
bool PX2::DirData::Read(std::string *filename)
{
#if defined (WIN32) || defined (_WIN32)

	bool first = false;

	WIN32_FIND_DATA finddata;
#define PTR_TO_FINDDATA (&finddata)

	if (!IsFindDataOk(mFinddata))
	{
		// open first
		std::string filespec = mDirname;
		//if (!EndsWithPathSeparator(filespec))
		{
			filespec += '\\';
		}
		if (mFilespec.empty())
			filespec += "*.*";
		else
			filespec += mFilespec;

		mFinddata = FindFirst(filespec, mFilespec, PTR_TO_FINDDATA);

		first = true;
	}

	if (!IsFindDataOk(mFinddata))
	{
#if defined (WIN32) || defined (_WIN32)
		DWORD err = ::GetLastError();

		if (err != ERROR_FILE_NOT_FOUND && err != ERROR_NO_MORE_FILES)
		{
			PX2_LOG_ERROR("Cannot enumerate files in directory %s",
				mDirname.c_str());
		}
#endif

		return false;
	}

	const char *name = 0;
	FIND_ATTR attr;

	for (;;)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			if (!FindNext(mFinddata, mFilespec, PTR_TO_FINDDATA))
			{
#if defined (WIN32) || defined (_WIN32)
				DWORD err = ::GetLastError();

				if (err != ERROR_NO_MORE_FILES)
				{
					PX2_LOG_ERROR("FindNext");
				}
#endif // __WIN32__
				//else: not an error, just no more (such) files

				return false;
			}
		}

		name = GetNameFromFindData(PTR_TO_FINDDATA);
		attr = GetAttrFromFindData(PTR_TO_FINDDATA);

		// don't return "." and ".." unless asked for
		if (name[0] == ('.') &&
			((name[1] == ('.') && name[2] == ('\0')) ||
			(name[1] == ('\0'))))
		{
			if (!(mFlags & DirP::DIR_DOTDOT))
				continue;
		}

		// check the type now
		if (!(mFlags & DirP::DIR_FILES) && !IsDir(attr))
		{
			// it's a file, but we don't want them
			continue;
		}
		else if (!(mFlags & DirP::DIR_DIRS) && IsDir(attr))
		{
			// it's a dir, and we don't want it
			continue;
		}

		// finally, check whether it's a hidden file
		if (!(mFlags & DirP::DIR_HIDDEN))
		{
			if (IsHidden(attr))
			{
				// it's a hidden file, skip it
				continue;
			}
		}

		*filename = name;

		break;
	}

#elif defined (__LINUX__) || defined (__ANDROID__)

dirent *de = NULL;    // just to silence compiler warnings
bool matches = false;

// speed up string concatenation in the loop a bit
std::string path = mDirname;
path += "/";
path.reserve(path.length() + 255);

std::string de_d_name;

while (!matches)
{
	de = readdir((DIR*)mFinddata);
	if (!de)
		return false;

	de_d_name = de->d_name;

	// don't return "." and ".." unless asked for
	if (de->d_name[0] == '.' &&
		((de->d_name[1] == '.' && de->d_name[2] == '\0') ||
		(de->d_name[1] == '\0')))
	{
		if (!(mFlags & DirP::DIR_DOTDOT))
			continue;

		// we found a valid match
		break;
	}

	std::string thePath = path + de_d_name;

	//// check the type now: notice that we may want to check the type of
	//// the path itself and not whatever it points to in case of a symlink
	if (mFlags & DirP::DIR_NO_FOLLOW)
	{
		//fn.DontFollowLink();
	}

	if (!(mFlags & DirP::DIR_FILES) && !Filename::IsDirExists(thePath))
	{
		// it's a file, but we don't want them
		continue;
	}
	else if (!(mFlags & DirP::DIR_DIRS) && Filename::IsDirExists(thePath))
	{
		// it's a dir, and we don't want it
		continue;
	}

	// finally, check the name
	if (mFilespec.empty())
	{
		matches = mFlags & DirP::DIR_HIDDEN ? true : de->d_name[0] != '.';
	}
	else
	{
		matches = true;
		// test against the pattern
		//matches = MatchWild(mFilespec, de_d_name,
		//	!(mFlags & DirP::DIR_HIDDEN));
	}
}

*filename = de_d_name;

#endif

	return true;
}
//----------------------------------------------------------------------------
std::string PX2::DirData::GetName()
{
	return mDirname;
}
//----------------------------------------------------------------------------
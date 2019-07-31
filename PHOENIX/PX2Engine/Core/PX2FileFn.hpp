// PX2FileFn.hpp

#ifndef PX2FILEFN_HPP
#define PX2FILEFN_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

#define PX2_FILE_SEP_EXT			'.'
#define PX2_FILE_SEP_DSK			':'

#define PX2_FILE_SEP_PATH_DOS		'\\'
#define PX2_FILE_SEP_PATH_UNIX		'/'
#define PX2_FILE_SEP_PATH_MAC		':'

#define PX2_PATH_SEP_DOS			";"
#define PX2_PATH_SEP_UNIX			":"
#define PX2_PATH_SEP_MAC			";"

#if defined (WIN32) || defined (_WIN32)
#define PX2_FILE_SEP_PATH     PX2_FILE_SEP_PATH_DOS
#define PX2_PATH_SEP          PX2_PATH_SEP_DOS
#else
#define PX2_FILE_SEP_PATH     PX2_FILE_SEP_PATH_UNIX
#define PX2_PATH_SEP          PX2_PATH_SEP_UNIX
#endif

}

PX2_ENGINE_ITEM bool IsWritable(const std::string& path);
PX2_ENGINE_ITEM bool IsReadable(const std::string& path);
PX2_ENGINE_ITEM bool IsExecutable(const std::string& path);

PX2_ENGINE_ITEM bool IsEndsWithPathSeparator(const std::string& filename);

PX2_ENGINE_ITEM bool IsDirExists(const std::string& pathName);
PX2_ENGINE_ITEM bool IsFileExists(const std::string& filename);

#endif
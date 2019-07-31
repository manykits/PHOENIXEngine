// PX2Filename.hpp

#ifndef PX2FILENAME_HPP
#define PX2FILENAME_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Filename
	{
	public:
		Filename();
		~Filename();

		static bool IsDirExists(const std::string &dirPath);
		static bool IsFileExists(const std::string &filePath);
	};

	// FileExists flags
	enum
	{
		PX2_FILE_EXISTS_REGULAR = 0x0001,	// check for existence of a regular file
		PX2_FILE_EXISTS_DIR = 0x0002,		// check for existence of a directory
		PX2_FILE_EXISTS_SYMLINK = 0x1004,	// check for existence of a symbolic link;
		// also sets wxFILE_EXISTS_NO_FOLLOW as
		// it would never be satisfied otherwise
		PX2_FILE_EXISTS_DEVICE = 0x0008,	// check for existence of a device
		PX2_FILE_EXISTS_FIFO = 0x0016,		// check for existence of a FIFO
		PX2_FILE_EXISTS_SOCKET = 0x0032,	// check for existence of a socket
		// gap for future types
		PX2_FILE_EXISTS_NO_FOLLOW = 0x1000, // don't dereference a contained symlink
		PX2_FILE_EXISTS_ANY = 0x1FFF		// check for existence of anything
	};


}

#endif
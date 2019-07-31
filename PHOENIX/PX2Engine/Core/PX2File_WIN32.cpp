// PX2File_WIN32.cpp

#if defined (WIN32) || defined(_WIN32)

#include "PX2File_WIN32.hpp"
#include "PX2Exception.hpp"
#include "PX2StringHelp.hpp"

#include <windows.h>

namespace PX2
{

	//----------------------------------------------------------------------------
	class FileHandle
	{
	public:
		FileHandle(const std::string& path, DWORD access, DWORD share, DWORD disp)
		{
			_h = CreateFileA(path.c_str(), access, share, 0, disp, 0, 0);
			if (_h == INVALID_HANDLE_VALUE)
			{
				FileImpl::handleLastErrorImpl(path);
			}
		}

		~FileHandle()
		{
			if (_h != INVALID_HANDLE_VALUE) CloseHandle(_h);
		}

		HANDLE get() const
		{
			return _h;
		}

	private:
		HANDLE _h;
	};
	//----------------------------------------------------------------------------
	FileImpl::FileImpl()
	{
	}
	//----------------------------------------------------------------------------
	FileImpl::FileImpl(const std::string& path) : _path(path)
	{
		std::string::size_type n = _path.size();
		if (n > 1 && (_path[n - 1] == '\\' || _path[n - 1] == '/') && !((n == 3 && _path[1] == ':')))
		{
			_path.resize(n - 1);
		}
	}
	//----------------------------------------------------------------------------
	FileImpl::~FileImpl()
	{
	}
	//----------------------------------------------------------------------------
	void FileImpl::swapImpl(FileImpl& file)
	{
		std::swap(_path, file._path);
	}
	//----------------------------------------------------------------------------
	void FileImpl::setPathImpl(const std::string& path)
	{
		_path = path;
		std::string::size_type n = _path.size();
		if (n > 1 && (_path[n - 1] == '\\' || _path[n - 1] == '/') && !((n == 3 && _path[1] == ':')))
		{
			_path.resize(n - 1);
		}
	}
	//----------------------------------------------------------------------------
	bool FileImpl::existsImpl() const
	{
		assert(!_path.empty());

		DWORD attr = GetFileAttributes(_path.c_str());
		if (attr == INVALID_FILE_ATTRIBUTES)
		{
			switch (GetLastError())
			{
			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
			case ERROR_NOT_READY:
			case ERROR_INVALID_DRIVE:
				return false;
			default:
				handleLastErrorImpl(_path);
			}
		}
		return true;
	}
	//----------------------------------------------------------------------------
	bool FileImpl::canReadImpl() const
	{
		assert(!_path.empty());

		DWORD attr = GetFileAttributes(_path.c_str());
		if (attr == INVALID_FILE_ATTRIBUTES)
		{
			switch (GetLastError())
			{
			case ERROR_ACCESS_DENIED:
				return false;
			default:
				handleLastErrorImpl(_path);
			}
		}
		return true;
	}
	//----------------------------------------------------------------------------
	bool FileImpl::canWriteImpl() const
	{
		assert(!_path.empty());

		DWORD attr = GetFileAttributes(_path.c_str());
		if (attr == INVALID_FILE_ATTRIBUTES)
			handleLastErrorImpl(_path);
		return (attr & FILE_ATTRIBUTE_READONLY) == 0;
	}
	//----------------------------------------------------------------------------
	bool FileImpl::canExecuteImpl() const
	{
		assert(false);
		return false;
	}
	//----------------------------------------------------------------------------
	bool FileImpl::isFileImpl() const
	{
		return !isDirectoryImpl() && !isDeviceImpl();
	}
	//----------------------------------------------------------------------------
	bool FileImpl::isDirectoryImpl() const
	{
		assert(!_path.empty());

		DWORD attr = GetFileAttributes(_path.c_str());
		if (attr == INVALID_FILE_ATTRIBUTES)
			handleLastErrorImpl(_path);
		return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}
	//----------------------------------------------------------------------------
	bool FileImpl::isLinkImpl() const
	{
		return false;
	}
	//----------------------------------------------------------------------------
	bool FileImpl::isDeviceImpl() const
	{
		return
			_path.compare(0, 4, "\\\\.\\") == 0 ||
			Icompare(_path, "CON") == 0 ||
			Icompare(_path, "PRN") == 0 ||
			Icompare(_path, "AUX") == 0 ||
			Icompare(_path, "NUL") == 0 ||
			((Icompare(_path, 0, 3, "LPT") == 0 || Icompare(_path, 0, 3, "COM") == 0) &&
			_path.size() == 4 &&
			_path[3] > 0x30 &&
			isdigit(_path[3])
			);
	}
	//----------------------------------------------------------------------------
	bool FileImpl::isHiddenImpl() const
	{
		assert(!_path.empty());

		DWORD attr = GetFileAttributes(_path.c_str());
		if (attr == INVALID_FILE_ATTRIBUTES)
			handleLastErrorImpl(_path);
		return (attr & FILE_ATTRIBUTE_HIDDEN) != 0;
	}
	//----------------------------------------------------------------------------
	Timestamp FileImpl::createdImpl() const
	{
		assert(!_path.empty());

		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (GetFileAttributesEx(_path.c_str(), GetFileExInfoStandard, &fad) == 0)
			handleLastErrorImpl(_path);
		return Timestamp::FromFileTimeNP(fad.ftCreationTime.dwLowDateTime,
			fad.ftCreationTime.dwHighDateTime);
	}
	//----------------------------------------------------------------------------
	Timestamp FileImpl::getLastModifiedImpl() const
	{
		assert(!_path.empty());

		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (GetFileAttributesEx(_path.c_str(), GetFileExInfoStandard, &fad) == 0)
			handleLastErrorImpl(_path);
		return Timestamp::FromFileTimeNP(fad.ftLastWriteTime.dwLowDateTime,
			fad.ftLastWriteTime.dwHighDateTime);
	}
	//----------------------------------------------------------------------------
	void FileImpl::setLastModifiedImpl(const Timestamp& ts)
	{
		assert(!_path.empty());

		uint32_t low;
		uint32_t high;
		ts.ToFileTimeNP(low, high);
		FILETIME ft;
		ft.dwLowDateTime = low;
		ft.dwHighDateTime = high;
		FileHandle fh(_path, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ |
			FILE_SHARE_WRITE, OPEN_EXISTING);
		if (SetFileTime(fh.get(), 0, &ft, &ft) == 0)
			handleLastErrorImpl(_path);
	}
	//----------------------------------------------------------------------------
	FileImpl::FileSizeImpl FileImpl::getSizeImpl() const
	{
		assert(!_path.empty());

		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (GetFileAttributesEx(_path.c_str(), GetFileExInfoStandard, &fad) == 0)
			handleLastErrorImpl(_path);
		LARGE_INTEGER li;
		li.LowPart = fad.nFileSizeLow;
		li.HighPart = fad.nFileSizeHigh;
		return li.QuadPart;
	}
	//----------------------------------------------------------------------------
	void FileImpl::setSizeImpl(FileSizeImpl size)
	{
		assert(!_path.empty());

		FileHandle fh(_path, GENERIC_WRITE, FILE_SHARE_READ |
			FILE_SHARE_WRITE, OPEN_EXISTING);
		LARGE_INTEGER li;
		li.QuadPart = size;
		if (SetFilePointer(fh.get(), li.LowPart, &li.HighPart, FILE_BEGIN)
			== INVALID_SET_FILE_POINTER)
			handleLastErrorImpl(_path);
		if (SetEndOfFile(fh.get()) == 0)
			handleLastErrorImpl(_path);
	}
	//----------------------------------------------------------------------------
	void FileImpl::setWriteableImpl(bool flag)
	{
		assert(!_path.empty());

		DWORD attr = GetFileAttributes(_path.c_str());
		if (attr == -1)
			handleLastErrorImpl(_path);
		if (flag)
			attr &= ~FILE_ATTRIBUTE_READONLY;
		else
			attr |= FILE_ATTRIBUTE_READONLY;
		if (SetFileAttributes(_path.c_str(), attr) == 0)
			handleLastErrorImpl(_path);
	}
	//----------------------------------------------------------------------------
	void FileImpl::setExecutableImpl(bool flag)
	{
		// not supported
	}
	//----------------------------------------------------------------------------
	void FileImpl::copyToImpl(const std::string& path) const
	{
		assert(!_path.empty());

		if (CopyFileA(_path.c_str(), path.c_str(), FALSE) == 0)
			handleLastErrorImpl(_path);
	}
	//----------------------------------------------------------------------------
	void FileImpl::renameToImpl(const std::string& path)
	{
		assert(!_path.empty());

		if (MoveFileExA(_path.c_str(), path.c_str(),
			MOVEFILE_REPLACE_EXISTING) == 0)
			handleLastErrorImpl(_path);
	}
	//----------------------------------------------------------------------------
	void FileImpl::removeImpl()
	{
		assert(!_path.empty());

		if (isDirectoryImpl())
		{
			if (RemoveDirectoryA(_path.c_str()) == 0)
				handleLastErrorImpl(_path);
		}
		else
		{
			if (DeleteFileA(_path.c_str()) == 0)
				handleLastErrorImpl(_path);
		}
	}
	//----------------------------------------------------------------------------
	bool FileImpl::createFileImpl()
	{
		assert(!_path.empty());

		HANDLE hFile = CreateFileA(_path.c_str(), GENERIC_WRITE, 0, 0,
			CREATE_NEW, 0, 0);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			return true;
		}
		else if (GetLastError() == ERROR_FILE_EXISTS)
			return false;
		else
			handleLastErrorImpl(_path);
		return false;
	}
	//----------------------------------------------------------------------------
	bool FileImpl::createDirectoryImpl()
	{
		assert(!_path.empty());

		if (existsImpl() && isDirectoryImpl())
			return false;
		if (CreateDirectoryA(_path.c_str(), 0) == 0)
			handleLastErrorImpl(_path);
		return true;
	}
	//----------------------------------------------------------------------------
	void FileImpl::handleLastErrorImpl(const std::string& path)
	{
		DWORD err = GetLastError();
		switch (err)
		{
		case ERROR_FILE_NOT_FOUND:
			throw FileNotFoundException(path, err);
		case ERROR_PATH_NOT_FOUND:
		case ERROR_BAD_NETPATH:
		case ERROR_CANT_RESOLVE_FILENAME:
		case ERROR_INVALID_DRIVE:
			throw PathNotFoundException(path, err);
		case ERROR_ACCESS_DENIED:
			throw FileAccessDeniedException(path, err);
		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS:
			throw FileExistsException(path, err);
		case ERROR_INVALID_NAME:
		case ERROR_DIRECTORY:
		case ERROR_FILENAME_EXCED_RANGE:
		case ERROR_BAD_PATHNAME:
			throw PathSyntaxException(path, err);
		case ERROR_FILE_READ_ONLY:
			throw FileReadOnlyException(path, err);
		case ERROR_CANNOT_MAKE:
			throw CreateFileException(path, err);
		case ERROR_DIR_NOT_EMPTY:
			throw FileException("directory not empty", path, err);
		case ERROR_WRITE_FAULT:
			throw WriteFileException(path, err);
		case ERROR_READ_FAULT:
			throw ReadFileException(path, err);
		case ERROR_SHARING_VIOLATION:
			throw FileException("sharing violation", path, err);
		case ERROR_LOCK_VIOLATION:
			throw FileException("lock violation", path, err);
		case ERROR_HANDLE_EOF:
			throw ReadFileException("EOF reached", path, err);
		case ERROR_HANDLE_DISK_FULL:
		case ERROR_DISK_FULL:
			throw WriteFileException("disk is full", path, err);
		case ERROR_NEGATIVE_SEEK:
			throw FileException("negative seek", path, err);
		default:
			throw FileException(path, err);
		}
	}
	//----------------------------------------------------------------------------
}

#endif
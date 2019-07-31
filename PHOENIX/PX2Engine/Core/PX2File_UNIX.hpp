// PX2File_UNIX.hpp

#ifndef PX2FILE_UNIX_HPP
#define PX2FILE_UNIX_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"

namespace PX2 
{
	
	class FileImpl
	{
	protected:
		typedef uint64_t FileSizeImpl;

		FileImpl();
		FileImpl(const std::string& path);
		virtual ~FileImpl();
		void swapImpl(FileImpl& file);
		void setPathImpl(const std::string& path);
		const std::string& getPathImpl() const;
		bool existsImpl() const;
		bool canReadImpl() const;
		bool canWriteImpl() const;
		bool canExecuteImpl() const;
		bool isFileImpl() const;
		bool isDirectoryImpl() const;
		bool isLinkImpl() const;
		bool isDeviceImpl() const;
		bool isHiddenImpl() const;
		Timestamp createdImpl() const;
		Timestamp getLastModifiedImpl() const;
		void setLastModifiedImpl(const Timestamp& ts);
		FileSizeImpl getSizeImpl() const;
		void setSizeImpl(FileSizeImpl size);
		void setWriteableImpl(bool flag = true);
		void setExecutableImpl(bool flag = true);
		void copyToImpl(const std::string& path) const;
		void renameToImpl(const std::string& path);
		void removeImpl();
		bool createFileImpl();
		bool createDirectoryImpl();
		static void handleLastErrorImpl(const std::string& path);

	private:
		std::string _path;

		friend class DirectoryIteratorImpl;
		friend class LinuxDirectoryWatcherStrategy;
		friend class BSDDirectoryWatcherStrategy;
	};

	inline const std::string& FileImpl::getPathImpl() const
	{
		return _path;
	}

}

#endif

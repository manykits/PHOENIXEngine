// PX2File.hpp

#ifndef PX2FILE_HPP
#define PX2FILE_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"

#if defined(_WIN32)
#include "PX2File_WIN32.hpp"
#else
#include "PX2File_UNIX.hpp"
#endif

namespace PX2
{

	class Path;


	class PX2_ENGINE_ITEM File : private FileImpl
	{
	public:
		typedef FileSizeImpl FileSize;

		File();
		File(const std::string& path);
		File(const char* path);
		File(const File& file);

		virtual ~File();

		File& operator = (const File& file);
		File& operator = (const std::string& path);
		File& operator = (const char* path);
		File& operator = (const Path& path);

		void swap(File& file);

		const std::string& path() const;
		bool exists() const;
		/// Returns true iff the file exists.

		bool canRead() const;
		bool canWrite() const;
		bool canExecute() const;
		bool isFile() const;
		bool isLink() const;
		bool isDirectory() const;
		bool isDevice() const;
		bool isHidden() const;
		Timestamp created() const;

		Timestamp getLastModified() const;
		File& setLastModified(const Timestamp& ts);

		FileSize getSize() const;
		File& setSize(FileSize size);

		File& setWriteable(bool flag = true);

		File& setReadOnly(bool flag = true);

		File& setExecutable(bool flag = true);

		void renameTo(const std::string& path);

		bool createFile();
		bool createDirectory();

		bool operator == (const File& file) const;
		bool operator != (const File& file) const;
		bool operator <  (const File& file) const;
		bool operator <= (const File& file) const;
		bool operator >  (const File& file) const;
		bool operator >= (const File& file) const;

		static void handleLastError(const std::string& path);
	};


	//
	// inlines
	//
	inline const std::string& File::path() const
	{
		return getPathImpl();
	}


	inline bool File::operator == (const File& file) const
	{
		return getPathImpl() == file.getPathImpl();
	}


	inline bool File::operator != (const File& file) const
	{
		return getPathImpl() != file.getPathImpl();
	}


	inline bool File::operator < (const File& file) const
	{
		return getPathImpl() < file.getPathImpl();
	}


	inline bool File::operator <= (const File& file) const
	{
		return getPathImpl() <= file.getPathImpl();
	}


	inline bool File::operator > (const File& file) const
	{
		return getPathImpl() > file.getPathImpl();
	}


	inline bool File::operator >= (const File& file) const
	{
		return getPathImpl() >= file.getPathImpl();
	}


	inline void swap(File& f1, File& f2)
	{
		f1.swap(f2);
	}


} // namespace Poco


#endif // Foundation_File_INCLUDED

// PX2File.cpp

#include "PX2File.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
File::File()
{
}

File::File(const std::string& path): FileImpl(path)
{
}
//----------------------------------------------------------------------------
File::File(const char* path): FileImpl(std::string(path))
{
}
//----------------------------------------------------------------------------
File::File(const File& file): FileImpl(file.getPathImpl())
{
}
//----------------------------------------------------------------------------
File::~File()
{
}
//----------------------------------------------------------------------------
File& File::operator = (const File& file)
{
	setPathImpl(file.getPathImpl());
	return *this;
}
//----------------------------------------------------------------------------
File& File::operator = (const std::string& path)
{
	setPathImpl(path);
	return *this;
}
//----------------------------------------------------------------------------
File& File::operator = (const char* path)
{
	assert (path);
	setPathImpl(path);
	return *this;
}
//----------------------------------------------------------------------------
void File::swap(File& file)
{
	swapImpl(file);
}
//----------------------------------------------------------------------------
bool File::exists() const
{
	return existsImpl();
}
//----------------------------------------------------------------------------	
bool File::canRead() const
{
	return canReadImpl();
}
//----------------------------------------------------------------------------	
bool File::canWrite() const
{
	return canWriteImpl();
}
//----------------------------------------------------------------------------
bool File::canExecute() const
{
	return canExecuteImpl();
}
//----------------------------------------------------------------------------
bool File::isFile() const
{
	return isFileImpl();
}
//----------------------------------------------------------------------------	
bool File::isDirectory() const
{
	return isDirectoryImpl();
}
//----------------------------------------------------------------------------
bool File::isLink() const
{
	return isLinkImpl();
}
//----------------------------------------------------------------------------
bool File::isDevice() const
{
	return isDeviceImpl();
}
//----------------------------------------------------------------------------
bool File::isHidden() const
{
	return isHiddenImpl();
}
//----------------------------------------------------------------------------
Timestamp File::created() const
{
	return createdImpl();
}
//----------------------------------------------------------------------------	
Timestamp File::getLastModified() const
{
	return getLastModifiedImpl();
}
//----------------------------------------------------------------------------	
File& File::setLastModified(const Timestamp& ts)
{
	setLastModifiedImpl(ts);
	return *this;
}
//----------------------------------------------------------------------------	
File::FileSize File::getSize() const
{
	return getSizeImpl();
}
//----------------------------------------------------------------------------	
File& File::setSize(FileSizeImpl size)
{
	setSizeImpl(size);
	return *this;
}
//----------------------------------------------------------------------------	
File& File::setWriteable(bool flag)
{
	setWriteableImpl(flag);
	return *this;
}
//----------------------------------------------------------------------------
File& File::setReadOnly(bool flag)
{
	setWriteableImpl(!flag);
	return *this;
}
//----------------------------------------------------------------------------
File& File::setExecutable(bool flag)
{
	setExecutableImpl(flag);
	return *this;
}
//----------------------------------------------------------------------------	
void File::renameTo(const std::string& path)
{
	renameToImpl(path);
	setPathImpl(path);
}
//----------------------------------------------------------------------------
bool File::createFile()
{
	return createFileImpl();
}
//----------------------------------------------------------------------------
bool File::createDirectory()
{
	return createDirectoryImpl();
}
//----------------------------------------------------------------------------
void File::handleLastError(const std::string& path)
{
	handleLastErrorImpl(path);
}
//----------------------------------------------------------------------------
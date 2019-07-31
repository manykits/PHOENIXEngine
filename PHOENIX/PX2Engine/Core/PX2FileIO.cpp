// PX2FileIO.cpp

#include "PX2FileIO.hpp"
#include "PX2Endian.hpp"
#include "PX2Memory.hpp"
#include "PX2Log.hpp"
#include <sys/stat.h>
using namespace PX2;

//----------------------------------------------------------------------------
FileIO::FileIO () :
mFile(0),
mMode(FM_NONE)
{
}
//----------------------------------------------------------------------------
FileIO::FileIO (const std::string& filename, int mode)
:
mFile(0),
mMode(FM_NONE)
{
	Open(filename, mode);
}
//----------------------------------------------------------------------------
FileIO::~FileIO ()
{
	if (mMode != FM_NONE)
	{
		Close();
	}
}
//----------------------------------------------------------------------------
bool FileIO::Open (const std::string& filename, int mode)
{
	if (mMode == FM_NONE)
	{
		if (mode == FM_READ || mode == FM_READ_AND_SWAP)
		{
			mFile = fopen(filename.c_str(), "rb");
		}
		else
		{
			mFile = fopen(filename.c_str(), "wb");
		}

		if (mFile)
		{
			mMode = mode;
			return true;
		}

		PX2_LOG_INFO("Failed to open file %s\n", filename.c_str());

		//assertion(false, "Failed to open file %s\n", filename.c_str());
	}
	else
	{
		assertion(false, "File %s is already open\n", filename.c_str());
	}

	return false;
}
//----------------------------------------------------------------------------
bool FileIO::Close ()
{
	mMode = FM_NONE;
	if (fclose(mFile) == 0)
	{
		return true;
	}

	assertion(false, "Failed to close file\n");
	return false;
}
//----------------------------------------------------------------------------
FileIO::operator bool () const
{
	return mMode != FM_NONE;
}
//----------------------------------------------------------------------------
int FileIO::Seek (long offset, SeekMode mode)
{
	return fseek(mFile, offset, (int)mode);
}
//----------------------------------------------------------------------------
bool FileIO::Read (size_t itemSize, void* datum)
{
#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if ((mMode != FM_READ && mMode != FM_READ_AND_SWAP) || !mFile || !datum
		||  (itemSize != 1 && itemSize != 2 && itemSize != 4 && itemSize != 8))
	{
		assertion(false, "Invalid FileIO::Read\n");
		return false;
	}
#endif

	int numRead = (int)fread(datum, itemSize, 1, mFile);

#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if (numRead != 1)
	{
		assertion(false, "Invalid FileIO::Read\n");
		return false;
	}
#else
	PX2_UNUSED(numRead);
#endif

	if (mMode == FM_READ_AND_SWAP && itemSize > 1)
	{
		Endian::Swap(itemSize, datum);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FileIO::Read (size_t itemSize, int numItems, void* data)
{
#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if ((mMode != FM_READ && mMode != FM_READ_AND_SWAP) || !mFile
		||  numItems <= 0 || !data
		||  (itemSize != 1 && itemSize != 2 && itemSize != 4 && itemSize != 8))
	{
		assertion(false, "Invalid FileIO::Read\n");
		return false;
	}
#endif

	int numRead = (int)fread(data, itemSize, numItems, mFile);

#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if (numRead != numItems)
	{
		assertion(false, "Invalid FileIO::Read\n");
		return false;
	}
#else
	PX2_UNUSED(numRead);
#endif

	if (mMode == FM_READ_AND_SWAP && itemSize > 1)
	{
		Endian::Swap(itemSize, numItems, data);
	}

	return true;
}
//----------------------------------------------------------------------------
bool FileIO::Write (size_t itemSize, const void* datum)
{
#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if ((mMode != FM_WRITE && mMode != FM_WRITE_AND_SWAP) || !mFile || !datum
		||  (itemSize != 1 && itemSize != 2 && itemSize != 4 && itemSize != 8))
	{
		assertion(false, "Invalid FileIO::Write\n");
		return false;
	}
#endif

	int numWritten;
	if (mMode == FM_WRITE_AND_SWAP && itemSize > 1)
	{
		if (itemSize == 2)
		{
			short tmpData = *(const short*)datum;
			Endian::Swap(itemSize, &tmpData);
			numWritten = (int)fwrite(&tmpData, itemSize, 1, mFile);
		}
		else if (itemSize == 4)
		{
			int tmpData = *(const int*)datum;
			Endian::Swap(itemSize, &tmpData);
			numWritten = (int)fwrite(&tmpData, itemSize, 1, mFile);
		}
		else if (itemSize == 8)
		{
			double tmpData = *(const double*)datum;
			Endian::Swap(itemSize, &tmpData);
			numWritten = (int)fwrite(&tmpData, itemSize, 1, mFile);
		}
		else
		{
			numWritten = 0;
#ifdef PX2_FILEIO_VALIDATE_OPERATION
			assertion(false, "Invalid FileIO::Write\n");
#endif
		}
	}
	else
	{
		numWritten = (int)fwrite(datum, itemSize, 1, mFile);
	}


#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if (numWritten != 1)
	{
		assertion(false, "Invalid FileIO::Write\n");
		return false;
	}
#else
	PX2_UNUSED(numWritten);
#endif

	return true;
}
//----------------------------------------------------------------------------
bool FileIO::Write (size_t itemSize, int numItems, const void* data)
{
#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if ((mMode != FM_WRITE && mMode != FM_WRITE_AND_SWAP) || !mFile
		||  numItems <= 0 || !data
		||  (itemSize != 1 && itemSize != 2 && itemSize != 4 && itemSize != 8))
	{
		assertion(false, "Invalid FileIO::Write\n");
		return false;
	}
#endif

	int numWritten;
	if (mMode == FM_WRITE_AND_SWAP && itemSize > 1)
	{
		numWritten = 0;
		if (itemSize == 2)
		{
			const short* tmpData = (const short*)data;
			for (int i = 0; i < numItems; ++i, ++tmpData)
			{
				short temp = *tmpData;
				Endian::Swap(itemSize, &temp);
				numWritten += (int)fwrite(&temp, itemSize, 1, mFile);
			}
		}
		else if (itemSize == 4)
		{
			const int* tmpData = (const int*)data;
			for (int i = 0; i < numItems; ++i, ++tmpData)
			{
				int temp = *tmpData;
				Endian::Swap(itemSize, &temp);
				numWritten += (int)fwrite(&temp, itemSize, 1, mFile);
			}
		}
		else if (itemSize == 8)
		{
			const double* tmpData = (const double*)data;
			for (int i = 0; i < numItems; ++i, ++tmpData)
			{
				double temp = *tmpData;
				Endian::Swap(itemSize, &temp);
				numWritten += (int)fwrite(&temp, itemSize, 1, mFile);
			}
		}
		else
		{
#ifdef PX2_FILEIO_VALIDATE_OPERATION
			assertion(false, "Invalid FileIO::Write\n");
#endif
		}
	}
	else
	{
		numWritten = (int)fwrite(data, itemSize, numItems, mFile);
	}


#ifdef PX2_FILEIO_VALIDATE_OPERATION
	if (numWritten != numItems)
	{
		assertion(false, "Invalid FileIO::Write\n");
		return false;
	}
#else
	PX2_UNUSED(numWritten);
#endif

	return true;
}
//----------------------------------------------------------------------------
bool FileIO::Load (const std::string& filename, bool binaryFile,
				   int& bufferSize, char*& buffer)
{
	struct stat statistics;
	if (stat(filename.c_str(), &statistics) != 0)
	{
		// 文件不存在
		PX2_LOG_ERROR("Failed to open file %s.", filename.c_str());
		//assertion(false, "Failed to open file %s.\n", filename.c_str());
		buffer = 0;
		bufferSize = 0;
		return false;
	}

	FILE* inFile;
	if (binaryFile)
	{
		inFile = fopen(filename.c_str(), "rb");
	}
	else
	{
		inFile = fopen(filename.c_str(), "rt");
	}
	if (!inFile)
	{
		// 文件不能以读取的方式打开
		assertion(false, "Failed to open file %s\n", filename.c_str());
		PX2_LOG_ERROR("Failed to open file %s\n", filename.c_str());
		buffer = 0;
		bufferSize = 0;
		return false;
	}

	bufferSize = (int)((int64_t)statistics.st_size);
	if (bufferSize > 0)
	{
		buffer = new1<char>(bufferSize);
		int numRead = (int)fread(buffer, sizeof(char), bufferSize, inFile);
		if (fclose(inFile) != 0 || numRead != bufferSize)
		{
			assertion(false, "Failed to read or close file %s\n",
				filename.c_str());
			PX2_LOG_ERROR("Failed to read or close file %s\n",
				filename.c_str());
			delete1(buffer);
			buffer = 0;
			bufferSize = 0;
			return false;
		}
	}
	else
	{
		if (fclose(inFile) != 0)
		{
			assertion(false, "Failed to read or close file %s\n",
				filename.c_str());
			PX2_LOG_ERROR("Failed to read or close file %s\n",
				filename.c_str());
			buffer = 0;
			bufferSize = 0;
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool FileIO::Save (const std::string& filename, bool binaryFile,
				   int bufferSize, const char* buffer)
{
	if (!buffer || bufferSize <= 0)
	{
		// 输入的buffer必须存在
		assertion(false, "Invalid inputs, file %s\n", filename.c_str());
		return false;
	}

	FILE* outFile;
	if (binaryFile)
	{
		outFile = fopen(filename.c_str(), "wb");
	}
	else
	{
		outFile = fopen(filename.c_str(), "wt");
	}
	if (!outFile)
	{
		// 文件不能以写的方式打开
		assertion(false, "Failed to open file %s\n", filename.c_str());
		return false;
	}

	int numWritten = (int)fwrite(buffer, sizeof(char), bufferSize, outFile);
	if (fclose(outFile) != 0 || numWritten != bufferSize)
	{
		assertion(false, "Failed to write or close file %s\n",
			filename.c_str());
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool FileIO::Append (const std::string& filename, bool binaryFile,
					 int bufferSize, const char* buffer)
{
	if (!buffer || bufferSize <= 0)
	{
		// 输入的buffer必须存在
		assertion(false, "Invalid inputs, file %s\n", filename.c_str());
		return false;
	}

	FILE* outFile;
	if (binaryFile)
	{
		outFile = fopen(filename.c_str(), "ab");
	}
	else
	{
		outFile = fopen(filename.c_str(), "at");
	}
	if (!outFile)
	{
		// 文件不能以appending的方式打开
		assertion(false, "Failed to open file %s\n", filename.c_str());
		return false;
	}

	int numWritten = (int)fwrite(buffer, sizeof(char), bufferSize, outFile);
	if (fclose(outFile) != 0 || numWritten != bufferSize)
	{
		assertion(false, "Failed to write or close file %s\n",
			filename.c_str());
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
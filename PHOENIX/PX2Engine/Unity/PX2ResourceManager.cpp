// PX2ResourceManager.cpp

#include "PX2ResourceManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2XMLData.hpp"
#include "PX2System.hpp"
#include "PX2Time.hpp"
#include "PX2Crypt.hpp"
#include "PX2MD5.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Log.hpp"
#include "PX2GraphicsRoot.hpp"

#include "ImageLibs/PNG/png.h"

#if defined (__IOS__)
#include <unistd.h>
#endif

#include "unzip.h"
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <io.h>
#include <sys/stat.h>
#elif defined __LINUX__
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include<unistd.h>
#elif defined __ANDROID__
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "AppPlayJNI.hpp"
#elif defined __IOS__
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

#if defined(_WIN32) || defined(WIN32)
#pragma warning(disable : 4312)
#endif

using namespace PX2;
using namespace std;

#define PX2_RGB_PREMULTIPLY_ALPHA(vr,vg,vb,va) \
	(unsigned)(((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1)) >> 8) | \
	((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
	((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1) >> 8) << 16) | \
	((unsigned)(unsigned char)(va) << 24))
typedef struct
{
	unsigned char* data;
	int size;
	int offset;
}tImageSource;

static void pngReadCallBack(png_structp png_ptr, png_bytep data, png_size_t length)
{
	tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

	if ((int)(isource->offset + length) <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += (int)length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}

//----------------------------------------------------------------------------
Object *ResManUserObjectLoadFun(const char *filename)
{
	return ResourceManager::GetSingleton().BlockLoad(filename);
}
//----------------------------------------------------------------------------
bool ResManUserBufferLoadFun(const char *filename, int &bufferSize,
	char* &buffer)
{
	return ResourceManager::GetSingleton().LoadBuffer(filename, bufferSize,
		buffer);
}
//----------------------------------------------------------------------------
Object *ResManUserObjectLoadFun1(const char *filename, bool vb, bool ib, bool mi)
{
	return ResourceManager::GetSingleton().BlockLoadShareCopy(filename, vb, ib, mi);
}
//----------------------------------------------------------------------------
bool ResManAddTexPackFun(const std::string &textPackPath)
{
	return ResourceManager::GetSingleton().AddTexPack(textPackPath);
}
//----------------------------------------------------------------------------
const TexPack &ResManGetTexPackFun(const std::string &textPackPath)
{
	return ResourceManager::GetSingleton().GetTexPack(textPackPath);
}
//----------------------------------------------------------------------------
const TexPackElement &ResManGetTexPackElementFun(const std::string &texPackPath,
	const std::string &eleName)
{
	return ResourceManager::GetSingleton().GetTexPackElement(texPackPath, eleName);
}
//----------------------------------------------------------------------------
std::string ResourceManager::mDataUpdateServerType;
std::string ResourceManager::msResPath;
std::string ResourceManager::mWriteablePath;
TexPack ResourceManager::msIvalidTexPack;
TexPackElement ResourceManager::msIvalidTexPackElement;
//----------------------------------------------------------------------------
ResourceManager::ResourceManager() :
mDDSKeepCompressed(true),
mLoadRecordMutex(0),
mLoadingDequeMutex(0),
mLoadingThread(0),
mQuitLoading(false),
mIsUseGarbageCollect(false),
mGarbageCollectTime(6.0f),
mGarbageUpdateTime(1.0f),
mResTableMutex(0),
mCurl(0),
mResourceUpdateCallback(0)
{
	mCurl = new0 CurlObj();
	mCurl->Initlize();
	mCurl->SetGetType(CurlObj::GT_FILE);

	mLoadRecordMutex = new0 Mutex();
	mLoadingDequeMutex = new0 Mutex();
	mResTableMutex = new0 Mutex();
	mTexPacksMutex = new0 Mutex();

	GraphicsRoot::SetUserLoadFun(ResManUserObjectLoadFun);
	GraphicsRoot::SetUserLoadFun1(ResManUserObjectLoadFun1);
	GraphicsRoot::SetBufferLoadFun(ResManUserBufferLoadFun);
	GraphicsRoot::SetAddTexPackFun(ResManAddTexPackFun);
	GraphicsRoot::SetGetTexPackFun(ResManGetTexPackFun);
	GraphicsRoot::SetGetTexPackElementFun(ResManGetTexPackElementFun);

	mLoadingDequeSemaphore = new0 Semaphore(0, 99999);
	mLoadingThread = new0 Thread("ResLoadThread");
	mLoadingThread->Start(*this);

#if defined WIN32
	if (msResPath.empty())
	{
		char full_path[_MAX_PATH + 1];
		GetModuleFileName(NULL, full_path, _MAX_PATH + 1);
		std::string ret((char*)full_path);
		ret = ret.substr(0, ret.rfind("\\") + 1);

		msResPath = StringHelp::StandardisePath(ret);
	}

	char full_path[_MAX_PATH + 1];
	GetModuleFileName(NULL, full_path, _MAX_PATH + 1);
	std::string ret((char*)full_path);
	std::string writeablePath = ret.substr(0, ret.rfind("\\") + 1);
	writeablePath = StringHelp::StandardiseFilename(mWriteablePath);

	if (writeablePath.empty())
	{
		writeablePath += "./";
	}

	SetWriteablePath(writeablePath);

#elif defined __ANDROID__
	// the path is: /data/data/ + package name
	std::string dir("/data/data/");
	std::string temp = GetPackageNameJNI();

	if (!temp.empty())
	{
		dir.append(temp).append("/");
		SetWriteablePath(dir);
	}
	else
	{
		SetWriteablePath("");
	}
#endif

	mDataUpdateFromPath = "Data/";
	mDataUpdateWritePath = "DataUpdate/";
}
//----------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	mQuitLoading = true;
	mLoadingDequeSemaphore->Set();

	if (mLoadingThread)
	{
		mLoadingThread->Join();
		delete0(mLoadingThread);
		mLoadingThread = 0;
	}

	delete0(mLoadingDequeSemaphore);

	if (mTexPacksMutex)
	{
		delete0(mTexPacksMutex);
	}

	if (mLoadRecordMutex)
	{
		delete0(mLoadRecordMutex);
	}

	if (mLoadingDequeMutex)
	{
		delete0(mLoadingDequeMutex);
	}

	if (mResTableMutex)
	{
		delete0(mResTableMutex);
	}

	if (mCurl)
	{
		mCurl->Terminate();
		delete0(mCurl);
	}

	mLocalDataFileTable.clear();
	mDataUpdateFileTable.clear();
}
//----------------------------------------------------------------------------
void ResourceManager::Update(double appSeconds, double elapsedSeconds)
{
	if (mIsUseGarbageCollect)
	{
		GarbageCollect(appSeconds, elapsedSeconds);
	}

	PX2_UNUSED(elapsedSeconds);
}
//----------------------------------------------------------------------------
void ResourceManager::Clear()
{
	mResTableMutex->Enter();
	mResTable.clear();
	mResTableMutex->Leave();

	mTexPacksMutex->Enter();
	mTexPacks.clear();
	mPackElements.clear();
	mTexPacksMutex->Leave();
}
//----------------------------------------------------------------------------
void ResourceManager::ClearRes(const std::string &resPath)
{
	if (resPath.empty()) return;

	mResTableMutex->Enter();

	auto it = mResTable.find(resPath.c_str());
	if (it != mResTable.end())
	{
		mResTable.erase(it);
	}

	mResTableMutex->Leave();
}
//----------------------------------------------------------------------------
Object *ResourceManager::BlockLoad(const std::string &filename)
{
	if (filename.empty())
	{
		assertion(false, "filename must not be null");
		return 0;
	}

	LoadRecord &loadRec = InsertRecord(filename.c_str(), false);

	_LoadTheRecord(loadRec);

	return loadRec.Obj;
}
//----------------------------------------------------------------------------
Object *ResourceManager::BlockLoadCopy(const std::string &filename)
{
	Object *obj = BlockLoad(filename);

	if (obj)
	{
		return obj->Copy("");
	}

	return 0;
}
//----------------------------------------------------------------------------
Object *ResourceManager::BlockLoadShareCopy(const std::string &filename, 
	bool vb, bool ib, bool mi)
{
	Object *obj = BlockLoad(filename);

	if (obj)
	{
		return ShareCopy(obj, vb, ib, mi);
	}

	return 0;
}
//----------------------------------------------------------------------------
bool ResourceManager::CreateFloder(const std::string &parentPath,
	const std::string &path)
{
	std::string tempPath = path;
	int index = 0;

	bool bIsTrue = true;
	while (bIsTrue)
	{
		int pos = (int)tempPath.find('/', index);
		if (-1 == pos)
		{
			break;
		}

		std::string dirPath = parentPath;

		dirPath.append(tempPath.substr(0, pos));

#if defined(_WIN32) || defined(WIN32)
		if ((GetFileAttributesA(dirPath.c_str())) == INVALID_FILE_ATTRIBUTES)
		{
			CreateDirectoryA(dirPath.c_str(), 0);
		}
#else
		if ("/data"!=dirPath && "/data/data"!=dirPath)
		{
			DIR *pDir = 0;
			//打开该路径
			pDir = opendir (dirPath.c_str());
			if (! pDir)
			{
				//创建该路径
				mkdir(dirPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
			}
			else closedir(pDir);
		}
#endif

		index = pos + 1;
	}

	return true;
}
//----------------------------------------------------------------------------
bool ResourceManager::IsFileFloderExist(const std::string &path)
{
	std::string dirPath = path;

	if (-1 != access(dirPath.c_str(), 0))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool ResourceManager::CopyTheFile(const std::string &srcFilename,
	const std::string &dstFilename)
{
#if defined(_WIN32) || defined(WIN32)
	return TRUE == CopyFile(srcFilename.c_str(), dstFilename.c_str(), true);
#else
	PX2_UNUSED(srcFilename);
	PX2_UNUSED(dstFilename);
	return false;
#endif
}
//----------------------------------------------------------------------------
bool ResourceManager::LoadBuffer(const std::string &filename, int &bufferSize,
	char* &buffer)
{
	if (filename.empty())
	{
		assertion(false, "filename must not be null");
		return false;
	}

	LoadRecord &loadRec = InsertRecord(filename.c_str(), true);

	_LoadTheRecord(loadRec);

	buffer = loadRec.Buffer;
	bufferSize = loadRec.BufferSize;

	if (buffer && 0 != bufferSize)
	{
		return true;
	}
	else
	{
		loadRec.Buffer = 0;
		loadRec.BufferSize = 0;

		assertion(false, "load Buffer failed: %s \n", filename.c_str());

		return false;
	}
}
//----------------------------------------------------------------------------
std::string ResourceManager::LoadBuffer(const std::string &filename, 
	bool fromPathRaw)
{
	std::string buf;

	if (!fromPathRaw)
	{
		int bufferSize = 0;
		char *buffer = 0;
		if (LoadBuffer(filename, bufferSize, buffer))
		{
			buf.resize(bufferSize);
			char *dst = (char*)buf.c_str();
			memcpy(dst, buffer, bufferSize);

			return buf;
		}
	}
	else
	{
		int bufferSize = 0;
		char *buffer = 0;
		FileIO::Load(filename, true, bufferSize, buffer);
		if (bufferSize > 0)
		{
			buf.resize(bufferSize);
			char *dst = (char*)buf.c_str();
			memcpy(dst, buffer, bufferSize);
			delete1(buffer);
		}
	}

	return buf;
}
//----------------------------------------------------------------------------
bool ResourceManager::LoadBuffer(const std::string &filename, std::string &buf)
{
	int bufferSize = 0;
	char *buffer = 0;
	if (LoadBuffer(filename, bufferSize, buffer))
	{
		buf.resize(bufferSize);
		char *dst = (char*)buf.c_str();
		memcpy(dst, buffer, bufferSize);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool ResourceManager::ReWriteFileToWriteablePath(const std::string &filename,
	const std::string &toDestPath)
{
	const std::string &wirteablePath = PX2_RM.GetWriteablePath();
	std::string bufStr;
	if (PX2_RM.LoadBuffer(filename, bufStr))
	{
		std::string destPath = wirteablePath + toDestPath;
		bool ret = FileIO::Save(destPath, true, (int)bufStr.length(),
			bufStr.c_str());

		PX2_RM.ClearRes(filename);

		return ret;
	}

	return false;
}
//----------------------------------------------------------------------------
Object *ResourceManager::ShareCopyVB_IB(Object *objFrom)
{
	if (!objFrom)
		return 0;

	Object *obj = objFrom->Copy("");

	if (obj)
	{
		Node *node = DynamicCast<Node>(obj);
		if (node)
		{
			ShareCopyProcess(node, (Node*)objFrom, true, true, false);
		}
		Renderable *renderable = DynamicCast<Renderable>(obj);
		if (renderable)
		{
			ShareCopyProcess(renderable, (Renderable*)objFrom, true, true,
				false);
		}
	}

	return obj;
}
//----------------------------------------------------------------------------
Object *ResourceManager::ShareCopyVB_IB_MI(Object *objFrom)
{
	if (!objFrom)
		return 0;

	Object *obj = objFrom->Copy("");

	if (obj)
	{
		Node *node = DynamicCast<Node>(obj);
		if (node)
		{
			ShareCopyProcess(node, (Node*)objFrom, true, true, true);
		}
		Renderable *renderable = DynamicCast<Renderable>(obj);
		if (renderable)
		{
			ShareCopyProcess(renderable, (Renderable*)objFrom, true, true,
				true);
		}
	}

	return obj;
}
//----------------------------------------------------------------------------
Object *ResourceManager::ShareCopyMI(Object *objFrom)
{
	if (!objFrom)
		return 0;

	Object *obj = objFrom->Copy("");

	if (obj)
	{
		Node *node = DynamicCast<Node>(obj);
		if (node)
		{
			ShareCopyProcess(node, (Node*)objFrom, false, false, true);
		}
		Renderable *renderable = DynamicCast<Renderable>(obj);
		if (renderable)
		{
			ShareCopyProcess(renderable, (Renderable*)objFrom, false, false,
				true);
		}
	}

	return obj;
}
//----------------------------------------------------------------------------
Object *ResourceManager::ShareCopy(Object *objFrom, bool vb, bool ib,
	bool mi)
{
	if (!objFrom)
		return 0;

	Object *obj = objFrom->Copy("");

	if (obj)
	{
		Node *node = DynamicCast<Node>(obj);
		if (node)
		{
			ShareCopyProcess(node, (Node*)objFrom, vb, ib, mi);
		}
		Renderable *renderable = DynamicCast<Renderable>(obj);
		if (renderable)
		{
			ShareCopyProcess(renderable, (Renderable*)objFrom, vb, ib,
				mi);
		}
	}

	return obj;
}
//----------------------------------------------------------------------------
void ResourceManager::Dump(const std::string &filename)
{
	ScopedCS dequeCS(mResTableMutex);

	FILE* outFile = 0;
	outFile = fopen(filename.c_str(), "wt");

	std::string format = "RCout: %6d ,";
	format += "RT: %2d , ";
	format += "BufferSize: %13d , ";
	format += "Filename: %s \n";

	auto it = mResTable.begin();
	for (; it != mResTable.end(); it++)
	{
		ScopedCS recordCS(mLoadRecordMutex);

		LoadRecord &record = it->second;

		if (LS_LOADED == record.State)
		{
			int refCount = -1;
			if (LoadRecord::RT_OBJECT == record.TheRecordType)
			{
				refCount = record.Obj.GetCount();
			}
			else if (LoadRecord::RT_BUFFER == record.TheRecordType)
			{
				refCount = record.Buffer.GetCount();
			}
			else
			{
				refCount = 1;
			}

			fprintf(outFile, format.c_str(), refCount, (int)record.TheRecordType, record.BufferSize, record.Filename.c_str());
		}
	}

	if (outFile)
	{
		fclose(outFile);
	}
}
//----------------------------------------------------------------------------
void ResourceManager::BeginDumpDiff()
{
	mBeginDumpDiffFiles.clear();

	ScopedCS dequeCS(mResTableMutex);

	auto it = mResTable.begin();
	for (; it != mResTable.end(); it++)
	{
		ScopedCS recordCS(mLoadRecordMutex);

		LoadRecord &record = it->second;
		mBeginDumpDiffFiles.push_back(record.Filename);
	}
}
//----------------------------------------------------------------------------
void ResourceManager::EndDumpDiff(const std::string &filename)
{
	ScopedCS dequeCS(mResTableMutex);

	FILE* outFile = 0;
	outFile = fopen(filename.c_str(), "wt");

	std::string format = "RCout: %6d ,";
	format += "RT: %2d , ";
	format += "BufferSize: %13d , ";
	format += "Filename: %s \n";

	auto it = mResTable.begin();
	for (; it != mResTable.end(); it++)
	{
		ScopedCS recordCS(mLoadRecordMutex);

		LoadRecord &record = it->second;

		if (LS_LOADED == record.State)
		{
			int refCount = -1;
			if (LoadRecord::RT_OBJECT == record.TheRecordType)
			{
				refCount = record.Obj.GetCount();
			}
			else if (LoadRecord::RT_BUFFER == record.TheRecordType)
			{
				refCount = record.Buffer.GetCount();
			}
			else
			{
				refCount = 1;
			}

			std::vector<std::string>::iterator it1 = std::find(
				mBeginDumpDiffFiles.begin(), mBeginDumpDiffFiles.end(),
				record.Filename);
			if (it1 == mBeginDumpDiffFiles.end())
			{
				fprintf(outFile, format.c_str(), refCount,
					(int)record.TheRecordType, record.BufferSize,
					record.Filename.c_str());
			}
		}
	}

	if (outFile)
	{
		fclose(outFile);
	}
}
//----------------------------------------------------------------------------
bool ResourceManager::IsTexPack(const std::string &texPackPath)
{
	int bufferSize = 0;
	char *buffer = 0;
	if (LoadBuffer(texPackPath, bufferSize, buffer))
	{
		std::string outPath;
		std::string outBaseName;
		std::string outExt;
		StringHelp::SplitFullFilename(texPackPath, outPath, outBaseName, outExt);

		if ("xml" == outExt || "XML" == outExt)
		{
			XMLData data;
			if (data.LoadBuffer(buffer, bufferSize))
			{
				XMLNode rootNode = data.GetRootNode();
				std::string name = rootNode.GetName();

				if ("TextureAtlas" == name)
				{
					return true;
				}
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool ResourceManager::AddTexPack(const std::string &texPackPath)
{
	mTexPacksMutex->Enter();

	bool isHasUpdate = IsHasUpdate(texPackPath);

	auto it = mTexPacks.find(texPackPath.c_str());
	if (it != mTexPacks.end())
	{
		if (isHasUpdate && !it->second.IsHasUpdate)
		{
			// 如果有跟新，且当前没更新过，就让过去，进行更新
		}
		else
		{
			mTexPacksMutex->Leave();
			return false;
		}
	}

	char *buffer = 0;
	int bufferSize = 0;
	if (LoadBuffer(texPackPath, bufferSize, buffer))
	{
		XMLData data;
		if (data.LoadBuffer(buffer, bufferSize))
		{
			std::string outPath;
			std::string outBaseName;
			std::string outExt;
			StringHelp::SplitFullFilename(texPackPath, outPath, outBaseName, outExt);

			XMLNode rootNode = data.GetRootNode();

			std::string imagePath = rootNode.AttributeToString("imagePath");
			int width = rootNode.AttributeToInt("width");
			int height = rootNode.AttributeToInt("height");
			TexPack pack;
			pack.ImageFullPath = outPath + imagePath;
			pack.ImagePath = imagePath;
			pack.Width = width;
			pack.Height = height;
			pack.IsHasUpdate = isHasUpdate;

			XMLNode childNode = rootNode.IterateChild();
			while (!childNode.IsNull())
			{
				std::string eleName;
				int x = 0;
				int y = 0;
				int w = 0;
				int h = 0;
				int oX = 0;
				int oY = 0;
				int oW = 0;
				int oH = 0;
				bool rolated = false;

				if (childNode.HasAttribute("n"))
					eleName = childNode.AttributeToString("n");
				if (childNode.HasAttribute("x"))
					x = childNode.AttributeToInt("x");
				if (childNode.HasAttribute("y"))
					y = childNode.AttributeToInt("y");
				if (childNode.HasAttribute("w"))
					w = childNode.AttributeToInt("w");
				if (childNode.HasAttribute("h"))
					h = childNode.AttributeToInt("h");
				if (childNode.HasAttribute("oX"))
					oX = childNode.AttributeToInt("oX");
				if (childNode.HasAttribute("oY"))
					oY = childNode.AttributeToInt("oY");
				if (childNode.HasAttribute("oW"))
					oW = childNode.AttributeToInt("oW");
				if (childNode.HasAttribute("oH"))
					oH = childNode.AttributeToInt("oH");
				if (childNode.HasAttribute("r"))
					rolated = (std::string(childNode.AttributeToString("r")) == "y");

				TexPackElement ele;
				ele.X = x;
				ele.Y = y;
				ele.W = w;
				ele.H = h;
				ele.OX = oX;
				ele.OY = oY;
				ele.OW = oW;
				ele.OH = oH;
				ele.Rolated = rolated;
				ele.TexWidth = width;
				ele.TexHeight = height;
				ele.ElementName = eleName;
				ele.ImagePathFull = outPath + imagePath;

				pack.Elements.push_back(ele);

				std::string allStr = texPackPath + eleName;

				mPackElements.insert(std::pair<FString, TexPackElement>(allStr.c_str(), ele));

				childNode = rootNode.IterateChild(childNode);
			}

			mTexPacks.insert(std::pair<FString, TexPack>(texPackPath.c_str(), pack));

			mTexPacksMutex->Leave();

			return true;
		}
	}

	mTexPacksMutex->Leave();

	return false;
}
//----------------------------------------------------------------------------
const TexPack &ResourceManager::GetTexPack(const std::string &texPackPath)
{
	mTexPacksMutex->Enter();

	auto it = mTexPacks.find(texPackPath.c_str());
	if (it != mTexPacks.end())
	{
		mTexPacksMutex->Leave();

		return it->second;
	}

	mTexPacksMutex->Leave();

	return msIvalidTexPack;
}
//----------------------------------------------------------------------------
const TexPackElement &ResourceManager::GetTexPackElement(
	const std::string &texPackPath, const std::string &eleName)
{
	mTexPacksMutex->Enter();

	std::string allStr = texPackPath + eleName;

	auto it = mPackElements.find(allStr.c_str());
	if (it != mPackElements.end())
	{
		mTexPacksMutex->Leave();
		return it->second;
	}

	mTexPacksMutex->Leave();

	return msIvalidTexPackElement;
}
//----------------------------------------------------------------------------
ResHandle ResourceManager::BackgroundLoad(
	const std::string &filename)
{
	LoadRecord &rec = InsertRecord(filename.c_str(), false);

	{
		ScopedCS scopedCS(mLoadRecordMutex);

		if (LS_UNLOADED == rec.State)
		{
			ScopedCS scopedCSLoadingDeque(mLoadingDequeMutex);
			rec.State = LS_LOADQUE;
			mLoadingDeque.push_front(&rec);

			mLoadingDequeSemaphore->Set();
		}
	}

	return (ResHandle)&rec;
}
//----------------------------------------------------------------------------
ResHandle ResourceManager::BackgroundDoFun(const std::string &funName,
	BackgroundFun func)
{
	LoadRecord &rec = InsertRecord(funName.c_str(), func);

	{
		ScopedCS scopedCS(mLoadRecordMutex);

		if (LS_UNLOADED == rec.State)
		{
			ScopedCS scopedCSLoadingDeque(mLoadingDequeMutex);
			rec.State = LS_LOADQUE;
			mLoadingDeque.push_front(&rec);

			mLoadingDequeSemaphore->Set();
		}
	}

	return (ResHandle)&rec;
}
//----------------------------------------------------------------------------
void ResourceManager::SetUseGarbageCollect(bool use)
{
	mIsUseGarbageCollect = use;

	if (!mIsUseGarbageCollect)
	{
		Clear();
	}
}
//----------------------------------------------------------------------------
Object *ResourceManager::CheckRes(ResHandle handle)
{
	if (0 == handle)
		return 0;

	LoadRecord &rec = *((LoadRecord *)handle);

	if (LS_LOADED == rec.State && rec.TheRecordType == LoadRecord::RT_OBJECT)
	{
		return rec.Obj;
	}

	return 0;
}
//----------------------------------------------------------------------------
bool ResourceManager::CheckFun(ResHandle handle)
{
	if (0 == handle)
		return false;

	LoadRecord &rec = *((LoadRecord *)handle);

	if (LS_LOADED == rec.State && rec.TheRecordType == LoadRecord::RT_FUN)
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
ResourceManager::LoadState ResourceManager::GetResLoadState(ResHandle handle)
{
	LoadRecord &rec = *(LoadRecord *)handle;

	return rec.State;
}
//----------------------------------------------------------------------------
void ResourceManager::SetProjectIDStr(const std::string &projName, 
	const std::string &idStr)
{
	mProjectName = projName;
	mProjectIDStr = idStr;
}
//----------------------------------------------------------------------------
const std::string &ResourceManager::GetProjectName() const
{
	return mProjectName;
}
//----------------------------------------------------------------------------
const std::string &ResourceManager::GetProjectIDStr() const
{
	return mProjectIDStr;
}
//----------------------------------------------------------------------------
void ResourceManager::SetResourcePath(const std::string &resPath)
{
	msResPath = resPath;
}
//----------------------------------------------------------------------------
std::string ResourceManager::GetResourcePath()
{
	return msResPath;
}
//----------------------------------------------------------------------------
std::string ResourceManager::GetWriteablePath()
{
	return mWriteablePath;
}
//----------------------------------------------------------------------------
void ResourceManager::SetWriteablePath(const std::string &path)
{
	mWriteablePath = path;

    Logger *logger = Logger::GetSingletonPtr();
    if (logger)
    {
        PX2_LOG_INFO("WriteablePath:%s", mWriteablePath.c_str());
    }
}
//----------------------------------------------------------------------------
void ResourceManager::GarbageCollect(double appSeconds, double elapsedSeconds)
{
	mGarbageUpdateTime -= (float)elapsedSeconds;
	if (mGarbageUpdateTime > 0.0f)
	{
		return;
	}
	else
	{
		mGarbageUpdateTime = 1.0f;
	}

	ScopedCS dequeCS(mResTableMutex);

	auto iter = mResTable.begin();
	while (iter != mResTable.end())
	{
		ScopedCS recordCS(mLoadRecordMutex);

		LoadRecord &record = iter->second;

		if (LS_LOADED == record.State &&
			appSeconds > (record.LastTouchedTime + mGarbageCollectTime))
		{
			if (LoadRecord::RT_OBJECT == record.TheRecordType)
			{
				if (!record.Obj || 1 == record.Obj.GetCount())
				{
					iter = mResTable.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			else if (LoadRecord::RT_BUFFER == record.TheRecordType)
			{
				if (!record.Buffer || 1 == record.Buffer.GetCount())
				{
					iter = mResTable.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}
}
//----------------------------------------------------------------------------
bool ResourceManager::ZipCompress(unsigned char *dest, unsigned long *destLen,
	const unsigned char *source, unsigned long sourceLen)
{
	if (Z_OK == compress((Bytef *)dest, destLen, (const Bytef *)source,
		sourceLen))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool ResourceManager::ZipUnCompress(unsigned char *dest, unsigned long *destLen,
	const unsigned char *source, unsigned long sourceLen)
{
	if (Z_OK == uncompress((Bytef *)dest, destLen, (const Bytef *)source, 
		sourceLen))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
ResourceManager::LoadRecord::LoadRecord()
{
	TheRecordType = RT_OBJECT;
	Fun = 0;
	LastTouchedTime = 0;
	State = LS_UNLOADED;
	BufferSize = 0;
}
//----------------------------------------------------------------------------
ResourceManager::LoadRecord::~LoadRecord()
{
	Buffer = 0;
}
//----------------------------------------------------------------------------
unsigned int ResourceManager::RunLoadingThread()
{
	LoadRecord *rec = 0;

	while (!mQuitLoading)
	{
		mLoadingDequeSemaphore->Wait();

		if (mQuitLoading)
			break;

		{
			ScopedCS scopedCS(mLoadingDequeMutex);

			if (mLoadingDeque.empty())
				continue;

			rec = mLoadingDeque.front();
			mLoadingDeque.pop_front();
		}

		_LoadTheRecord(*rec);
	}

	return 0;
}
//----------------------------------------------------------------------------
#if defined(__ANDROID__) || defined(__IOS__)
static bool CheckResourceCCZFile(std::string &inoutPath)
{
	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	StringHelp::SplitFullFilename(inoutPath, outPath, outBaseName, outExtention);
	//std::transform(outExtention.begin(), outExtention.end(), outExtention.begin(), std::tolower);

	char *buffer = 0;
	int bufferSize = 0;
	std::string newPath;
	if ("dds"==outExtention || "DDS"==outExtention)
	{
		newPath = outPath + outBaseName + ".pvr.ccz";
		/*
		struct stat fileStat;
		std::string tmppath = newPath.substr(5);
		if(stat(tmppath.c_str(), &fileStat) < 0) //ccz文件不存在
		{
		return false;
		}
		*/

		inoutPath = newPath;
		return true;
	}
	return false;
}
#endif
//----------------------------------------------------------------------------
ResourceManager::LoadRecord &ResourceManager::InsertRecord(
	const FString &fn, bool isBuffer)
{
	std::string writeablePath = PX2_RM.GetWriteablePath();

	std::string dstFilename = (const char*)fn;

#if defined(__ANDROID__) || defined(__IOS__)
	CheckResourceCCZFile(dstFilename);
#endif

	if (!mProjectName.empty() && !mProjectIDStr.empty())
	{
		std::string projNewPathName = mProjectName + "_" + mProjectIDStr;
		size_t pos = dstFilename.find(mProjectName);

		if (pos != std::string::npos)
		{
			size_t posNew = dstFilename.find(projNewPathName);

			if (posNew == std::string::npos)
			{
				std::string strFront = dstFilename.substr(0, pos);
				std::string strEnd = dstFilename.substr(pos + mProjectName.length(), dstFilename.length() - pos - mProjectName.length());
				dstFilename = strFront + projNewPathName + strEnd;
			}
		}
	}

	// adjust path
	bool isHasUpdate = IsHasUpdate(dstFilename, dstFilename);
	PX2_UNUSED(isHasUpdate);

	ScopedCS scopeCS(mResTableMutex);

	auto it = mResTable.find(dstFilename.c_str());
	if (it != mResTable.end())
	{
		it->second.LastTouchedTime = Time::GetTimeInSeconds();
		return it->second;
	}

	auto it1 = mResTable.insert(std::pair<FString, LoadRecord>(dstFilename.c_str(), LoadRecord()));

	LoadRecord &rec = it1.first->second;
	rec.Filename = dstFilename;
	rec.State = LS_UNLOADED;
	rec.TheRecordType = isBuffer ? LoadRecord::RT_BUFFER : LoadRecord::RT_OBJECT;
	rec.LastTouchedTime = Time::GetTimeInSeconds();

	return rec;
}
//----------------------------------------------------------------------------
ResourceManager::LoadRecord &ResourceManager::InsertRecord(
	const FString &funName, BackgroundFun fun)
{
	ScopedCS scopeCS(mResTableMutex);

	auto it = mResTable.insert(std::pair<FString, LoadRecord>(funName, LoadRecord()));

	LoadRecord &rec = it.first->second;
	rec.TheRecordType = LoadRecord::RT_FUN;
	rec.Filename = funName;
	rec.Fun = fun;
	rec.State = LS_UNLOADED;
	rec.LastTouchedTime = Time::GetTimeInSeconds();

	return rec;
}
//----------------------------------------------------------------------------
void ResourceManager::_LoadTheRecord(LoadRecord &rec)
{
	bool needLoad = false;
	{
		ScopedCS scopedCS(mLoadRecordMutex);

		if (LS_UNLOADED == rec.State || LS_LOADQUE == rec.State)
		{
			rec.State = LS_LOADING;
			needLoad = true;
		}
	}

	if (needLoad)
	{
		if (rec.TheRecordType == LoadRecord::RT_FUN)
		{
			rec.Fun();
			rec.LastTouchedTime = Time::GetTimeInSeconds();
		}
		else if (rec.TheRecordType == LoadRecord::RT_OBJECT)
		{
#if defined WIN32 || defined _WIN32
			rec.Obj = _LoadObject(msResPath, rec.Filename); // for special use
#else
			rec.Obj = _LoadObject(msResPath, rec.Filename);
#endif
			rec.LastTouchedTime = Time::GetTimeInSeconds();
		}
		else if (rec.TheRecordType == LoadRecord::RT_BUFFER)
		{
			char *buffer = 0;
			int bufferSize = 0;
#if defined WIN32 || defined _WIN32
			if (_LoadBuffer(msResPath, rec.Filename, bufferSize, buffer))
#else
			if (_LoadBuffer(msResPath, rec.Filename, bufferSize, buffer))
#endif
			{
				rec.Buffer = buffer;
				rec.BufferSize = bufferSize;
			}

			rec.LastTouchedTime = Time::GetTimeInSeconds();
		}

		rec.State = LS_LOADED;
	}

	// 可能其他线程正在加载这个资源
	while (rec.State != LS_LOADED)
	{
		System::SleepSeconds(0.01f);
	}
}
//----------------------------------------------------------------------------
Object *ResourceManager::_LoadObject(const std::string &workDirPath, 
	const std::string &filename)
{
	Object *obj = 0;

	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName, outExtention);

	char *buffer = 0;
	int bufferSize = 0;
	if ("dds" == outExtention || "DDS" == outExtention)
	{
		obj = LoadTextureFromDDS(filename);
	}
	else if (_LoadBuffer(workDirPath, filename, bufferSize, buffer))
	{
		if ("ccz" == outExtention)
		{
			obj = LoadTextureFromPVRTC_CCZ(bufferSize, buffer);
		}
		else if ("png" == outExtention || "PNG" == outExtention || "jpg" == outExtention
			|| "JPG" == outExtention)
		{
			obj = LoadTexFormOtherImagefile(outExtention, bufferSize, buffer);
		}
		else
		{
			InStream inStream;
			inStream.Load1(bufferSize, buffer);
			obj = inStream.GetObjectAt(0);
		}

		delete1(buffer);
		bufferSize = 0;
	}

	if (obj)
	{
		obj->SetResourcePath(filename);
	}

	return obj;
}
//----------------------------------------------------------------------------
bool ResourceManager::_LoadBuffer(const std::string &workDirPath, 
	const std::string &fn, int &bufferSize, char* &buffer)
{
	std::string dstFilename = fn;
	if (dstFilename.empty())
		return false;

	bool fromOut = false;
	if ('/' == dstFilename[0] || '.' == dstFilename[0])
	{
		fromOut = true;
	}

#ifdef __ANDROID__
	if (!fromOut)
	{
		std::string fullFilename = dstFilename;
		fullFilename.insert(0, "assets/");
		return GetFileDataFromZip(msResPath, fullFilename, bufferSize, buffer);
	}
	else
	{
		return FileIO::Load(dstFilename, true, bufferSize, buffer);
	}
#elif defined (_WIN32) || defined (WIN32) || defined (__LINUX__)
	return FileIO::Load(dstFilename, true, bufferSize, buffer);
#elif defined (__IOS__)
	std::string fullFilename = msResPath + dstFilename;
	return FileIO::Load(fullFilename, true, bufferSize, buffer);
#endif
}
//----------------------------------------------------------------------------
void ResourceManager::SetDataUpdateServerType(
	const std::string &dataUpdateServerType)
{
	mDataUpdateServerType = dataUpdateServerType;
}
//----------------------------------------------------------------------------
const std::string &ResourceManager::GetDataUpdateServerType()
{
	return mDataUpdateServerType;
}
//----------------------------------------------------------------------------
void ResourceManager::_RMReWriteProcessXMLNode(ResourceFileTable &table,
	XMLNode node)
{
	XMLNode childNode = node.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();

		if ("dir" == name)
		{
			_RMReWriteProcessXMLNode(table, childNode);
		}
		else if ("file" == name)
		{
			std::string filename = childNode.AttributeToString("filename");
			std::string md5 = childNode.AttributeToString("md5");

			ResourceFileInfoItemPtr item = new0 ResourceFileInfoItem();
			item->Path = filename;
			item->MD5 = md5;

			table[FString(filename.c_str())] = item;
		}

		childNode = node.IterateChild(childNode);
	}
}
//----------------------------------------------------------------------------
bool ResourceManager::LoadFileTableXML(ResourceFileTable &table, 
	const std::string &filename)
{
	table.clear();

	char *buf = 0;
	int bufSize = 0;
	if (LoadBuffer(filename, bufSize, buf))
	{
		XMLData data;
		data.LoadBuffer(buf, bufSize);
		XMLNode rootNode = data.GetRootNode();
		std::string versionStr = rootNode.AttributeToString("version");

		_RMReWriteProcessXMLNode(table, rootNode);

		ClearRes(filename);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void ResourceManager::ClearDataFiletable()
{
	mLocalDataFileTable.clear();
}
//----------------------------------------------------------------------------
void ResourceManager::ClearDataUpdateFiletable()
{
	mDataUpdateFileTable.clear();
}
//----------------------------------------------------------------------------
bool ResourceManager::IsHasUpdate(const std::string &filename)
{
	auto it = mDataUpdateFileTable.find(filename.c_str());
	if (it != mDataUpdateFileTable.end())
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool ResourceManager::IsHasUpdate(const std::string &filename,
	std::string &outUpdatedFilename)
{
	if (mDataUpdateFileTable.empty())
		return false;

	std::string writeablePath = PX2_RM.GetWriteablePath();

	auto it = mDataUpdateFileTable.find(filename.c_str());
	if (it != mDataUpdateFileTable.end())
	{
		std::string replaceFilename = filename;
		std::string left = "Data/";

		replaceFilename = replaceFilename.substr(left.length(),
			replaceFilename.length() - left.length());
		replaceFilename = writeablePath + mDataUpdateWritePath + replaceFilename;

		outUpdatedFilename = replaceFilename;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool ResourceManager::Download(const std::string &fullPath,
	const std::string &url, int type)
{
	CreateFloder("", fullPath);

	if (1 == type)
		return mCurl->Download(fullPath, url);
	else if (2 == type)
		return mCurl->Download(fullPath, url);

	return false;
}
//----------------------------------------------------------------------------
void ResourceManager::SetResourceUpdateAddr(const std::string &updateAddr)
{
	mResourceUpdateAddr = updateAddr;
}
//----------------------------------------------------------------------------
const std::string &ResourceManager::GetResourceUpdateAddr() const
{
	return mResourceUpdateAddr;
}
//----------------------------------------------------------------------------
void ResourceManager::DoResourceUpdateStuffs(const std::string &wwwAddr, 
	const std::string &projName, int type)
{
	PX2_LOG_INFO("%s DoResourceUpdateStuffs from %s", projName.c_str(), wwwAddr.c_str());

	std::string filelist_www = wwwAddr + mDataUpdateFromPath + projName + "/filelist.xml";

	std::string writeablePath = PX2_RM.GetWriteablePath();
	std::string filelistPrepare = writeablePath + mDataUpdateWritePath + projName + "/filelist_prepare.xml";
	std::string filelistToUpdate = writeablePath + mDataUpdateWritePath + projName + "/filelist_temp.xml";
	std::string filelist = writeablePath + mDataUpdateWritePath + projName + "/filelist.xml";

	if (!PX2_RM.Download(filelistPrepare, filelist_www, type)) // 预备下载
		return;

	PX2_LOG_INFO("Downloaded filelist_prepare %s", filelistPrepare.c_str());

	if (!PX2_RM.Download(filelistToUpdate, filelist_www, type)) // 下载
		return;

	PX2_LOG_INFO("Downloaded filelist_toupdate %s", filelistToUpdate.c_str());

	ResourceFileTable tableToUpdate;
	PX2_RM.LoadFileTableXML(tableToUpdate, filelistToUpdate);
	int numUpdateFiles = (int)tableToUpdate.size();

	PX2_LOG_INFO("num %d files to update", numUpdateFiles);

	if (0 == numUpdateFiles)
	{
		if (mResourceUpdateCallback)
			mResourceUpdateCallback(1.0f);
	}
	else
	{
		ResourceFileTable newTable;

		auto itUpdate = tableToUpdate.begin();
		for (; itUpdate != tableToUpdate.end(); itUpdate++)
		{
			FString filename = itUpdate->first;
			ResourceFileInfoItem *itemUpdate = itUpdate->second;
			newTable[filename] = itemUpdate;

			bool isNeedDownload = false;
			std::string downloadFilename;

			auto it = mDataUpdateFileTable.find(filename);
			if (it == mDataUpdateFileTable.end())
			{
				isNeedDownload = true; 
				downloadFilename = filename;
			}
			else
			{
				if (itemUpdate->MD5 != it->second->MD5)
				{
					isNeedDownload = true;
					downloadFilename = filename;
				}
				else
				{
					isNeedDownload = false;
				}
			}

			if (isNeedDownload)
			{
				std::string wwwURL = wwwAddr + downloadFilename;
				std::string dataKey = "Data/";
				std::string dstDownloadFilename = downloadFilename.substr(dataKey.length(), 
					downloadFilename.length() - dataKey.length());
				std::string downloadPath = writeablePath + mDataUpdateWritePath + dstDownloadFilename;


				PX2_LOG_INFO("begin download %s", downloadPath.c_str());

				PX2_RM.Download(downloadPath, wwwURL, type);

				PX2_LOG_INFO("end download %s", downloadPath.c_str());
			}
		}

		// set table
		mDataUpdateFileTable = newTable;

		// down last

		PX2_LOG_INFO("begin download last %s", filelist.c_str());

		if (!PX2_RM.Download(filelist, filelist_www, type))
			return;

		PX2_LOG_INFO("end download last %s", filelist.c_str());

		if (mResourceUpdateCallback)
			mResourceUpdateCallback(1.0f);
	}
}
//----------------------------------------------------------------------------
void ResourceManager::SetResourceUpdateCallback(
	ResourceUpdateStuffsCallback callback)
{
	mResourceUpdateCallback = callback;
}
//----------------------------------------------------------------------------
Texture2D *ResourceManager::LoadTexFormOtherImagefile(std::string outExt,
	int bufferSize, const char*buffer)
{
	if (!buffer || bufferSize <= 0) return 0;

	Texture2D* texture = 0;

	if ("png" == outExt || "PNG" == outExt)
	{
		texture = _initWithPngData(buffer, bufferSize);
	}

	return texture;
}
//----------------------------------------------------------------------------
Texture2D *ResourceManager::_initWithPngData(const char *pData, int nDatalen)
{
	// length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
	Texture2D* texture = 0;
	png_byte        header[PNGSIGSIZE] = { 0 };
	png_structp     png_ptr = 0;
	png_infop       info_ptr = 0;

	do
	{
		// png header len is 8 bytes
		if (nDatalen < PNGSIGSIZE)
			break;

		// check the data is png or not
		memcpy(header, pData, PNGSIGSIZE);
		if (png_sig_cmp(header, 0, PNGSIGSIZE))
			break;

		// init png_struct
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if (!png_ptr)
			break;

		// init png_info
		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
			break;

		//#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
		//		CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
		//#endif

		// set the read call back function
		tImageSource imagesource;
		imagesource.data = (unsigned char*)pData;
		imagesource.size = nDatalen;
		imagesource.offset = 0;
		png_set_read_fn(png_ptr, &imagesource, pngReadCallBack);

		// read png header info

		// read png file info
		png_read_info(png_ptr, info_ptr);

		int width = png_get_image_width(png_ptr, info_ptr);
		int height = png_get_image_height(png_ptr, info_ptr);
		int bitsPerComponent = png_get_bit_depth(png_ptr, info_ptr);
		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

		Texture::Format format = Texture::TF_A8R8G8B8;
		if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		{
			format = Texture::TF_A8R8G8B8;
		}
		else if (color_type == PNG_COLOR_TYPE_RGB)
		{
			format = Texture::TF_R8G8B8;
		}
		texture = new0 Texture2D(format, width, height, 1);

		// force palette images to be expanded to 24-bit RGB
		// it may include alpha channel
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
		}
		// low-bit-depth grayscale images are to be expanded to 8 bits
		if (color_type == PNG_COLOR_TYPE_GRAY && bitsPerComponent < 8)
		{
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		// expand any tRNS chunk data into a full alpha channel
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
		}
		// reduce images with 16-bit samples to 8 bits
		if (bitsPerComponent == 16)
		{
			png_set_strip_16(png_ptr);
		}
		// expand grayscale images to RGB
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(png_ptr);
		}

		// read png data
		// m_nBitsPerComponent will always be 8
		bitsPerComponent = 8;
		png_uint_32 rowbytes;
		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

		png_read_update_info(png_ptr, info_ptr);

		rowbytes = (png_uint_32)png_get_rowbytes(png_ptr, info_ptr);

		unsigned char* data = new unsigned char[rowbytes * height];

		char* textureData = texture->GetData(0);

		for (unsigned short i = 0; i < height; ++i)
		{
			row_pointers[i] = data + i*rowbytes;
		}
		png_read_image(png_ptr, row_pointers);

		png_read_end(png_ptr, NULL);

		png_uint_32 channel = rowbytes / width;
		if (channel == 4)
		{
			unsigned int *tmp = (unsigned int *)textureData;
			for (unsigned short i = 0; i < height; i++)
			{
				for (unsigned int j = 0; j < rowbytes; j += 4)
				{
					*tmp++ = PX2_RGB_PREMULTIPLY_ALPHA(row_pointers[i][j], row_pointers[i][j + 1],
						row_pointers[i][j + 2], row_pointers[i][j + 3]);
				}
			}
		}
		else if (channel == 3)
		{
			unsigned char *tmp = (unsigned char *)textureData;
			for (unsigned short i = 0; i < height; i++)
			{
				for (unsigned int j = 0; j < rowbytes;)
				{
					*tmp++ = row_pointers[i][j];
					*tmp++ = row_pointers[i][j + 1];
					*tmp++ = row_pointers[i][j + 2];
					j = j + 3;
				}
			}
		}
		free(row_pointers);

		delete[] data;

	} while (0);

	if (png_ptr)
	{
		png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	}
	return texture;
}
//----------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma pack(push,1)
#endif
typedef struct {
	uint32_t version;
	uint32_t flags;
	uint64_t pixelFormat;
	uint32_t colorSpace;
	uint32_t channelType;
	uint32_t height;
	uint32_t width;
	uint32_t depth;
	uint32_t numberOfSurfaces;
	uint32_t numberOfFaces;
	uint32_t numberOfMipmaps;
	uint32_t metadataLength;
#ifdef _MSC_VER
} ccPVRv3TexHeader;
#pragma pack(pop)
#else
} __attribute__((packed)) ccPVRv3TexHeader;
#endif

#define CC_HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define CC_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define CC_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define CC_SWAP_INT32_LITTLE_TO_HOST(i) ((CC_HOST_IS_BIG_ENDIAN == true)? CC_SWAP32(i) : (i) )
#define CC_SWAP_INT16_LITTLE_TO_HOST(i) ((CC_HOST_IS_BIG_ENDIAN == true)? CC_SWAP16(i) : (i) )
#define CC_SWAP_INT32_BIG_TO_HOST(i)    ((CC_HOST_IS_BIG_ENDIAN == true)? (i) : CC_SWAP32(i) )
#define CC_SWAP_INT16_BIG_TO_HOST(i)    ((CC_HOST_IS_BIG_ENDIAN == true)? (i):  CC_SWAP16(i) )

/* supported predefined formats */
#define kPVR3TexturePixelFormat_PVRTC_2BPP_RGB   0
#define kPVR3TexturePixelFormat_PVRTC_2BPP_RGBA  1
#define kPVR3TexturePixelFormat_PVRTC_4BPP_RGB   2
#define kPVR3TexturePixelFormat_PVRTC_4BPP_RGBA  3
#define kPVR3TexturePixelFormat_ETC1             6

/* supported channel type formats */
#define kPVR3TexturePixelFormat_BGRA_8888  0x0808080861726762ULL
#define kPVR3TexturePixelFormat_RGBA_8888  0x0808080861626772ULL
#define kPVR3TexturePixelFormat_RGBA_4444  0x0404040461626772ULL
#define kPVR3TexturePixelFormat_RGBA_5551  0x0105050561626772ULL
#define kPVR3TexturePixelFormat_RGB_565    0x0005060500626772ULL
#define kPVR3TexturePixelFormat_RGB_888    0x0008080800626772ULL
#define kPVR3TexturePixelFormat_A_8        0x0000000800000061ULL
#define kPVR3TexturePixelFormat_L_8        0x000000080000006cULL
#define kPVR3TexturePixelFormat_LA_88      0x000008080000616cULL

enum {
	kPVR3TextureFlagPremultipliedAlpha = (1 << 1)	// has premultiplied alpha
};

struct CCZHeader {
	unsigned char   sig[4];             // signature. Should be 'CCZ!' 4 bytes
	unsigned short  compression_type;   // should 0
	unsigned short  version;            // should be 2 (although version type==1 is also supported)
	unsigned int    reserved;           // Reserved for users.
	unsigned int    len;                // size of the uncompressed file
};

enum {
	CCZ_COMPRESSION_ZLIB,               // zlib format.
	CCZ_COMPRESSION_BZIP2,              // bzip2 format (not supported yet)
	CCZ_COMPRESSION_GZIP,               // gzip format (not supported yet)
	CCZ_COMPRESSION_NONE,               // plain (not supported yet)
};

static unsigned int randSeed = 0;
static unsigned int MyRand()
{
	randSeed = randSeed * 171589117 + 892332411;
	return randSeed;
}

static void DecryptBuffer(unsigned char *buffer, int n)
{
	randSeed = n + 123;
	int enclen = (MyRand() % 100) + 23;
	if (enclen > n) enclen = n;

	enclen /= 4;
	unsigned int *pdata = (unsigned int *)buffer;
	for (int i = 0; i < enclen; i++)
	{
		unsigned int src = (unsigned int)buffer[i * 4] + ((unsigned int)buffer[i * 4 + 1] << 8) + ((unsigned int)buffer[i * 4 + 2] << 16) + ((unsigned int)buffer[i * 4 + 3] << 24);
		pdata[i] = src ^ MyRand();
	}
}

Texture2D *ResourceManager::LoadTextureFromPVRTC_CCZ(int bufferSize, const char *buffer)
{
	int headsize = sizeof(CCZHeader);
	if (bufferSize < headsize)
	{
		return NULL;
	}
	CCZHeader *cczheader = (CCZHeader *)buffer;
	unsigned long newlen = CC_SWAP_INT32_BIG_TO_HOST(cczheader->len);
	char *newbuffer = new char[newlen];

	if (cczheader->reserved > 0)
	{
		DecryptBuffer((unsigned char *)buffer + sizeof(CCZHeader), bufferSize - sizeof(CCZHeader));
		cczheader->reserved = 0;
	}

	Texture2D *ptex = NULL;
	if (Z_OK == uncompress((Bytef *)newbuffer, &newlen, (const Bytef *)buffer + headsize, bufferSize - headsize))
	{
		ptex = LoadTextureFromPVRTC(newlen, newbuffer);
	}

	delete[] newbuffer;
	return ptex;
}

Texture2D *ResourceManager::LoadTextureFromPVRTC(int dataLength, const char *dataPointer)
{
	if (dataLength < int(sizeof(ccPVRv3TexHeader)))
	{
		return NULL;
	}

	ccPVRv3TexHeader *header = (ccPVRv3TexHeader *)dataPointer;

	// validate version
	if (CC_SWAP_INT32_BIG_TO_HOST(header->version) != 0x50565203)
	{
		PX2_LOG_INFO("pvr file version mismatch");
		return NULL;
	}

	// parse pixel format
	uint64_t pixelFormat = header->pixelFormat;
	int bpp;
	Texture::Format format;

	if (pixelFormat == kPVR3TexturePixelFormat_PVRTC_2BPP_RGB)
	{
		bpp = 2;
		format = Texture::TF_RGB_PVRTC_2B;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_PVRTC_2BPP_RGBA)
	{
		bpp = 2;
		format = Texture::TF_RGBA_PVRTC_2B;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_PVRTC_4BPP_RGB)
	{
		bpp = 4;
		format = Texture::TF_RGB_PVRTC_4B;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_PVRTC_4BPP_RGBA)
	{
		bpp = 4;
		format = Texture::TF_RGBA_PVRTC_4B;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_ETC1)
	{
		bpp = 4;
		format = Texture::TF_RGB_ETC1;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_RGBA_8888)
	{
		bpp = 32;
		format = Texture::TF_A8R8G8B8;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_RGBA_4444)
	{
		bpp = 16;
		format = Texture::TF_A4R4G4B4;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_RGBA_5551)
	{
		bpp = 16;
		format = Texture::TF_A1R5G5B5;
	}
	else if (pixelFormat == kPVR3TexturePixelFormat_RGB_565)
	{
		bpp = 16;
		format = Texture::TF_R5G6B5;
	}
	else
	{
		PX2_LOG_INFO("Cannot support pvr pixelformat");
		return NULL;
	}

	// PVRv3 specifies premultiply alpha in a flag -- should always respect this in PVRv3 files
	//uint32_t flags = CC_SWAP_INT32_LITTLE_TO_HOST(header->flags);
	//bool premul_alpha = (flags & kPVR3TextureFlagPremultipliedAlpha)!=0;

	// sizing
	uint32_t width = CC_SWAP_INT32_LITTLE_TO_HOST(header->width);
	uint32_t height = CC_SWAP_INT32_LITTLE_TO_HOST(header->height);
	uint32_t dataOffset = 0, dataSize = 0;
	uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0;
	uint8_t *bytes = NULL;

	dataOffset = (sizeof(ccPVRv3TexHeader) + header->metadataLength);
	bytes = (uint8_t *)dataPointer;

	unsigned int mipmaps = header->numberOfMipmaps;
	Texture2D *texture = new0 Texture2D(format, width, height, mipmaps);

	for (unsigned int i = 0; i < mipmaps; i++)
	{
		switch (pixelFormat)
		{
		case kPVR3TexturePixelFormat_PVRTC_2BPP_RGB:
		case kPVR3TexturePixelFormat_PVRTC_2BPP_RGBA:
			blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
			widthBlocks = width / 8;
			heightBlocks = height / 4;
			break;
		case kPVR3TexturePixelFormat_PVRTC_4BPP_RGB:
		case kPVR3TexturePixelFormat_PVRTC_4BPP_RGBA:
			blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
			widthBlocks = width / 4;
			heightBlocks = height / 4;
			break;
		case kPVR3TexturePixelFormat_ETC1:
			blockSize = 4 * 4;
			widthBlocks = width / 4;
			heightBlocks = height / 4;
			break;
		case kPVR3TexturePixelFormat_BGRA_8888:
			return 0;
		default:
			blockSize = 1;
			widthBlocks = width;
			heightBlocks = height;
			break;
		}

		// Clamp to minimum number of blocks
		if (widthBlocks < 2)
		{
			widthBlocks = 2;
		}
		if (heightBlocks < 2)
		{
			heightBlocks = 2;
		}

		dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
		unsigned int packetLength = ((unsigned int)dataLength - dataOffset);
		packetLength = packetLength > dataSize ? dataSize : packetLength;

		assertion(texture->GetNumLevelBytes(i) == int(packetLength), "wrong PVR file packetLength");
		memcpy(texture->GetData(i), bytes + dataOffset, packetLength);

		dataOffset += packetLength;
		assertion(int(dataOffset) <= dataLength, "TexturePVR Invalid lenght");

		width = width >> 1;
		if (width == 0) width = 1;
		height = height >> 1;
		if (height == 0) height = 1;
	}

	return texture;
}
//----------------------------------------------------------------------------

#ifndef DWORD
typedef unsigned long DWORD;
#endif
#ifndef uint32
typedef unsigned int uint32;
#endif

#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))

const DWORD D3DFMT_R16F = 111;
const DWORD D3DFMT_G16R16F = 112;
const DWORD D3DFMT_A16B16G16R16F = 113;
const DWORD D3DFMT_R32F = 114;
const DWORD D3DFMT_G32R32F = 115;
const DWORD D3DFMT_A32B32G32R32F = 116;

const uint32 DDS_PIXELFORMAT_SIZE = 8 * sizeof(uint32);
const uint32 DDS_CAPS_SIZE = 4 * sizeof(uint32);
const uint32 DDS_HEADER_SIZE = 19 * sizeof(uint32) + DDS_PIXELFORMAT_SIZE + DDS_CAPS_SIZE;

#define DDSD_CAPS        0x00000001
#define DDSD_HEIGHT      0x00000002
#define DDSD_WIDTH       0x00000004
#define DDSD_PITCH       0x00000008
#define DDSD_PIXELFORMAT 0x00001000
#define DDSD_MIPMAPCOUNT 0x00020000
#define DDSD_LINEARSIZE  0x00080000
#define DDSD_DEPTH       0x00800000

#define DDPF_ALPHAPIXELS 0x00000001
#define DDPF_FOURCC      0x00000004
#define DDPF_RGB         0x00000040

#define DDSCAPS_COMPLEX  0x00000008 
#define DDSCAPS_TEXTURE  0x00001000 
#define DDSCAPS_MIPMAP   0x00400000 

#define DDSCAPS2_CUBEMAP		   0x00000200 
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400 
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800 
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000 
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000 
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000 
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000 
#define DDSCAPS2_VOLUME			   0x00200000 

struct DDPIXELFORMAT
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwRGBAlphaBitMask;
};

struct DDSCAPS2
{
	DWORD    dwCaps;
	DWORD    dwCaps2;
	DWORD    dwCaps3;
	DWORD    dwCaps4;
};

struct DDSHeader
{
	char          magic[4];
	DWORD         dwSize;
	DWORD         dwFlags;
	DWORD         dwHeight;
	DWORD         dwWidth;
	DWORD         dwPitchOrLinearSize;
	DWORD         dwDepth;
	DWORD         dwMipMapCount;
	DWORD         dwReserved1[11];
	DDPIXELFORMAT ddpfPixelFormat;
	DDSCAPS2      ddsCaps;
	DWORD         dwReserved2;
};

static Texture::Format ConvertFourCCFormat(uint32_t fourcc)
{
	switch (fourcc)
	{
	case FOURCC('D', 'X', 'T', '1'):
		return Texture::TF_DXT1;
	case FOURCC('D', 'X', 'T', '3'):
		return Texture::TF_DXT3;
	case FOURCC('D', 'X', 'T', '5'):
		return Texture::TF_DXT5;
	default:
		assertion(false, "other format is not supportted.");
		return Texture::TF_NONE;
	};
}

static void GetDescInfo(const DDSHeader &header, int &width, int &height,
	int &minmap, Texture::Format &format)
{
	if (strncmp(header.magic, "DDS ", 4) != 0)
	{
		assertion(false, "this is not DDS.");
	}

	if (header.dwFlags & DDSD_CAPS)
	{
		const DDSCAPS2 &caps = header.ddsCaps;
		if (caps.dwCaps2&DDSCAPS2_CUBEMAP)
		{
			assertion(false, "current not supported.");
		}
		else if (caps.dwCaps2&DDSCAPS2_VOLUME)
		{
			assertion(false, "current not supported.");
		}
	}

	assertion((header.dwFlags&DDSD_WIDTH) && (header.dwFlags&DDSD_HEIGHT), "");
	width = (int)header.dwWidth;
	height = (int)header.dwHeight;

	if (header.dwFlags&DDSD_MIPMAPCOUNT)
	{
		assertion(((header.ddsCaps.dwCaps&DDSCAPS_MIPMAP) != 0), "must has minmap.");
		minmap = header.dwMipMapCount;
	}
	else
	{
		minmap = 1;
	}

	const DDPIXELFORMAT &fmt = header.ddpfPixelFormat;

	if (fmt.dwFlags & DDPF_RGB)
	{
		assertion(false, "");
	}
	else if (fmt.dwFlags & DDPF_FOURCC)
	{
		format = ConvertFourCCFormat(fmt.dwFourCC);
	}
	else
	{
		assertion(false, "");
	}
}

Texture2D *ResourceManager::LoadTextureFromDDS(const std::string &filename)
{
	PX2_UNUSED(filename);

	FILE* inFile;
	inFile = fopen(filename.c_str(), "rb");

	if (!inFile)
	{
		assertion(false, "Failed to open file %s\n", filename.c_str());
		return 0;
	}

	DDSHeader header;
	int numRead = (int)fread(&header, sizeof(header), 1, inFile);
	PX2_UNUSED(numRead);

	int width = 0;
	int height = 0;
	int minmap = 1;
	Texture::Format format = Texture::TF_A8R8G8B8;
	GetDescInfo(header, width, height, minmap, format);

	Texture2D *texture = new0 Texture2D(format, width, height, minmap);
	fread(texture->GetData(0), texture->GetNumTotalBytes(), 1, inFile);

	if (fclose(inFile) != 0)
	{
		assertion(false, "Failed to read or close file %s\n",
			filename.c_str());
		return 0;
	}

	return texture;
}
//----------------------------------------------------------------------------
bool ResourceManager::GetFileDataFromZip(const std::string &packageName,
	const std::string &filename, int &bufferSize, char* &buffer)
{
	if (packageName.empty() || filename.empty())
		return 0;

	unzFile ufile = 0;

	ufile = unzOpen(packageName.c_str());
	if (!ufile)
		return false;

	if (UNZ_OK != unzLocateFile(ufile, filename.c_str(), 1))
	{
		assertion(false, "unzLocateFile failed.");

		unzClose(ufile);
		return false;
	}

	char filePathTemp[260];
	unz_file_info fileInfo;
	if (UNZ_OK != unzGetCurrentFileInfo(ufile, &fileInfo, filePathTemp,
		sizeof(filePathTemp), 0, 0, 0, 0))
	{
		assertion(false, "unzGetCurrentFileInfo failed.");

		unzClose(ufile);
		return false;
	}

	if (UNZ_OK != unzOpenCurrentFile(ufile))
	{
		assertion(false, "unzOpenCurrentFile failed.");

		unzClose(ufile);
		return false;
	}

	buffer = new1<char>(fileInfo.uncompressed_size);
	int readedSize = 0;
	readedSize = unzReadCurrentFile(ufile, buffer, fileInfo.uncompressed_size);

	if (0 != readedSize && (fileInfo.uncompressed_size != (unsigned long)readedSize))
	{
		assertion(false, "the file size is wrong.");

		unzClose(ufile);

		delete1<char>(buffer);
		buffer = 0;

		return false;
	}

	bufferSize = (int)fileInfo.uncompressed_size;

	unzCloseCurrentFile(ufile);
	unzClose(ufile);

	return true;
}
//----------------------------------------------------------------------------
void ResourceManager::ShareCopyProcess(Node *node, Node *nodeFrom,
	bool vb, bool ib, bool mi)
{
	for (int i = 0; i < nodeFrom->GetNumChildren(); i++)
	{
		Movable *movFrom = nodeFrom->GetChild(i);
		Movable *movTo = node->GetChild(i);

		Node *theNode = DynamicCast<Node>(movFrom);
		if (theNode)
		{
			ShareCopyProcess((Node*)movTo, theNode, vb, ib, mi);
		}
		Renderable *theRenderable = DynamicCast<Renderable>(movFrom);
		if (theRenderable)
		{
			ShareCopyProcess((Renderable*)movTo, theRenderable, vb, ib, mi);
		}
	}
}
//----------------------------------------------------------------------------
void ResourceManager::ShareCopyProcess(Renderable *renderable,
	Renderable *renderableFrom, bool vb, bool ib, bool mi)
{
	VertexFormat *vf1 = renderableFrom->GetVertexFormat();
	VertexBuffer *vb1 = renderableFrom->GetVertexBuffer();
	IndexBuffer *ib1 = renderableFrom->GetIndexBuffer();
	MaterialInstance *mi1 = renderableFrom->GetMaterialInstance();

	if (vb)
	{
		renderable->SetVertexFormat(vf1);
		renderable->SetVertexBuffer(vb1);
	}

	if (ib)
	{
		renderable->SetIndexBuffer(ib1);
	}

	if (mi)
	{
		renderable->SetMaterialInstance(mi1);
	}
}
//----------------------------------------------------------------------------
void ResourceManager::Run()
{
	RunLoadingThread();
}
//----------------------------------------------------------------------------


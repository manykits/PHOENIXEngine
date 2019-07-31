// PX2ResourceManager.hpp

#ifndef PX2RESOURCEMANAGER_HPP
#define PX2RESOURCEMANAGER_HPP

#include "PX2UnityPre.hpp"
#include "PX2TexPackData.hpp"
#include "PX2Runnable.hpp"
#include "PX2ObjectCopyPool.hpp"
#include "PX2ResourceUpdate.hpp"
#include "PX2HashTable.hpp"
#include "PX2FString.hpp"
#include "PX2XMLData.hpp"
#include "PX2Semaphore.hpp"
#include "PX2Texture2D.hpp"
#include "PX2Node.hpp"
#include "PX2Renderable.hpp"
#include "PX2Thread.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2CURL.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ResourceManager : public Runnable, public Singleton<ResourceManager>
	{
	public:
		ResourceManager ();
		virtual ~ResourceManager ();

		CurlObj *GetCurlObj();

		virtual void Update (double appSeconds, double elapsedSeconds);

		// Load
	public:
		// 设置资源路径.在Android系统中，为apk所在目录
		static void SetResourcePath(const std::string &resPath);
		static void SetWriteablePath(const std::string &path);

		/// 获得可写路径,在PC上为程序当前目录，在移动设备上由该设备API返回。
		static std::string GetWriteablePath();
		static std::string GetResourcePath();

		void SetProjectIDStr(const std::string &projName, const std::string &idStr);
		const std::string &GetProjectName() const;
		const std::string &GetProjectIDStr() const;

		/// 资源加载状态
		enum LoadState
		{
			LS_UNLOADED,
			LS_LOADQUE,
			LS_LOADING,
			LS_LOADED,
			LS_MAX_STATE
		};

		void Clear ();
		void ClearRes (const std::string &resPath);
		Object *BlockLoad (const std::string &filename);
		Object *BlockLoadCopy (const std::string &filename);
		Object *BlockLoadShareCopy (const std::string &filename, bool vb, bool ib, 
			bool mi);
		ResHandle BackgroundLoad (const std::string &filename);
		ResHandle BackgroundDoFun (const std::string &funName, BackgroundFun func);
		Object *CheckRes (ResHandle handle);
		bool CheckFun (ResHandle handle);
		LoadState GetResLoadState (ResHandle handle);
		Object *ShareCopyVB_IB (Object *objFrom);
		Object *ShareCopyVB_IB_MI (Object *objFrom);
		Object *ShareCopyMI (Object *objFrom);
		Object *ShareCopy (Object *objFrom, bool vb, bool ib, bool mi);

		bool LoadBuffer(const std::string &filename, int &bufferSize,
			char* &buffer);
		bool LoadBuffer(const std::string &filename, std::string &buf);
		std::string LoadBuffer(const std::string &filename, bool fromPathRaw);

		bool IsTexPack(const std::string &texPackPath);
		bool AddTexPack(const std::string &texPackPath);
		const TexPack &GetTexPack(const std::string &texPackPath);
		const TexPackElement &GetTexPackElement(
			const std::string &texPackPath, const std::string &eleName);

		void DDSKeepCompressed(bool keep = true);
		bool IsDDSKeepCompressed() const;
		
		void SetUseGarbageCollect (bool use);
		bool IsUseGarbageCollect () const;
		void SetGarbageCollectTime (float seconds);
		float GetGarbageCollectTime () const;

	public_internal:
		struct LoadRecord
		{
			enum RecordType
			{
				RT_OBJECT,
				RT_BUFFER,
				RT_FUN,
				RT_MAX_TYPE
			};

			LoadRecord();
			~LoadRecord();

			RecordType TheRecordType;
			BackgroundFun Fun;
			std::string Filename;
			ObjectPtr Obj;
			Pointer1<char> Buffer;
			int BufferSize;
			LoadState State;

			// 当资源引用计数为1后，超过一定时间长度后，资源将被系统回收。
			double LastTouchedTime;
		};

		unsigned int RunLoadingThread();
		void GarbageCollect(double appSeconds, double elapsedSeconds);

		bool ZipCompress(unsigned char *dest, unsigned long *destLen, 
			const unsigned char *source, unsigned long sourceLen);
		bool ZipUnCompress(unsigned char *dest, unsigned long *destLen,
			const unsigned char *source, unsigned long sourceLen);

		virtual void Run();

	protected:
		LoadRecord &InsertRecord(const FString &filename, bool isBuffer);
		LoadRecord &InsertRecord(const FString &funName, BackgroundFun fun);
		void _LoadTheRecord(LoadRecord &rec);
		Object *_LoadObject(const std::string &workDirPath, const std::string &filename);
		bool _LoadBuffer(const std::string &workDirPath, const std::string &filename, 
			int &bufferSize, char* &buffer);
		Texture2D *LoadTexFormOtherImagefile(std::string outExt,
			int bufferSize, const char*buffer);
		Texture2D *LoadTextureFromDDS(const std::string &filename);
		Texture2D *LoadTextureFromPVRTC(int bufferSize, const char *buffer);
		Texture2D *LoadTextureFromPVRTC_CCZ(int bufferSize, const char *buffer);
		Texture2D *_initWithPngData(const char *pData, int nDatalen);
		bool GetFileDataFromZip(const std::string &packageName,
			const std::string &filename, int &bufferSize, char* &buffer);
		void ShareCopyProcess(Node *node, Node *nodeFrom, bool vb, bool ib,
			bool mi);
		void ShareCopyProcess(Renderable *renderable,
			Renderable *renderableFrom, bool vb, bool ib, bool mi);

	private:
		bool mDDSKeepCompressed;
		Mutex *mLoadRecordMutex;
		Mutex *mLoadingDequeMutex;
		Mutex *mTexPacksMutex;
		Thread *mLoadingThread;
		std::deque<LoadRecord *> mLoadingDeque;
		bool mQuitLoading;
		Semaphore *mLoadingDequeSemaphore;

		bool mIsUseGarbageCollect;
		float mGarbageCollectTime;
		float mGarbageUpdateTime;
		Mutex *mResTableMutex;
		std::map<FString, LoadRecord> mResTable;

		static TexPack msIvalidTexPack;
		std::map < FString, TexPack> mTexPacks;
		static TexPackElement msIvalidTexPackElement;
		std::map < FString, TexPackElement> mPackElements;

		std::string mProjectName;
		std::string mProjectIDStr;

		static std::string msResPath;
		static std::string mWriteablePath;

		// Dump
	public:
		void Dump(const std::string &filename);
		void BeginDumpDiff();
		void EndDumpDiff(const std::string &filename);

	private:
		std::vector<std::string> mBeginDumpDiffFiles;

		// Folder
	public:
		// folder
		// CreateFloder("Data/", "myproject/")
		// IsFloderExist("Data/myproject/")
		bool CreateFloder (const std::string &parentPath, const std::string &path);
		// Folder末尾需要/
		bool IsFileFloderExist (const std::string &path);
		bool CopyTheFile(const std::string &srcFilename, const std::string &dstFilename);

		// 加载文件，重新写到可写目录，destPath无需加writepath，只需如"Stage/vlcvideo.sdp"
		bool ReWriteFileToWriteablePath(const std::string &filename, const std::string &destPath);

		// Update
	public:
		typedef std::map<FString, ResourceFileInfoItemPtr> ResourceFileTable;
		typedef std::map<FString, ResourceFileInfoItemPtr>::iterator ResourceFileTableIterator;

		// 加标记，看是不是从测试服务器更新
		static void SetDataUpdateServerType(const std::string &dataUpdateServerType);
		static const std::string &GetDataUpdateServerType();

		const std::string &GetDataUpdateFromPath () const;
		const std::string &GetDataUpdateWritePath() const;

		void SetVersion(const ResourceVersion &version);
		const ResourceVersion &GetVersion() const;

		bool LoadFileTableXML(ResourceFileTable &table, const std::string &filename);

		void ClearDataFiletable();
		const ResourceVersionItem *GetDataVersionItem() const;
		ResourceFileTable &GetDataFiletable();

		void ClearDataUpdateFiletable();
		const ResourceVersionItem *GetDataUpdateVersionItem() const;
		ResourceFileTable &GetDataUpdateFileTable();

		bool IsHasUpdate(const std::string &filename);
		bool IsHasUpdate(const std::string &filename, std::string &outUpdatedFilename);

		void SetResourceUpdateAddr(const std::string &updateAddr);
		const std::string &GetResourceUpdateAddr() const;
		// 1ftp 2http
		void DoResourceUpdateStuffs(const std::string &wwwAddr, const std::string &projName, int type);
		void SetResourceUpdateCallback(ResourceUpdateStuffsCallback callback);

		bool Download(const std::string &fullPath, const std::string &url, int type);

	protected:
		void ReadVersionList(FileIO &in, ResourceFileTable &table);
		void ReadVersionList(BufferIO &in, ResourceFileTable &table);
		void _RMReWriteProcessXMLNode(ResourceFileTable &table, XMLNode node);

		CurlObj *mCurl;

		static std::string mDataUpdateServerType;

		std::string mDataUpdateFromPath;
		std::string mDataUpdateWritePath;

		ResourceVersion mVersion;

		ResourceFileTable mLocalDataFileTable;
		ResourceFileTable mDataUpdateFileTable;

		std::string mResourceUpdateAddr;
		ResourceUpdateStuffsCallback mResourceUpdateCallback;
	};

#include "PX2ResourceManager.inl"

#define PX2_RM ResourceManager::GetSingleton()

}

#endif
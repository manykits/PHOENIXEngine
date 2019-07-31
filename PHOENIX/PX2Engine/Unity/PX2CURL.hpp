// PX2CURL.hpp

#ifndef PX2CURL_HPP
#define PX2CURL_HPP

#include "PX2UnityPre.hpp"
#include "PX2Mutex.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2Object.hpp"

struct curl_slist;

namespace PX2
{

	class CurlObj;

	typedef void(*CurlWriteCallback) (CurlObj *obj, void *buffer, size_t size, 
		size_t numSize);
	typedef void(*CurlProgressCallback) (CurlObj *obj, double dltotal,
		double dlnow, double ultotal, double ulnow);

	class PX2_ENGINE_ITEM CurlObj : public Object
	{
	public:
		CurlObj();
		virtual ~CurlObj();

		bool Initlize();
		bool Terminate();

		enum GetType
		{
			GT_NONE,
			GT_FILE,
			GT_MEMORY,
			GT_FILEMEMORY,
			GT_MAX_TYPE
		};
		void SetGetType(GetType type);
		GetType GetGetType() const;

		void Stop();
		void Continue();
		double GetSpeed() const;

		bool Download(const std::string &filename, const std::string &url,
			int size, int startFrom);
		// 1 ftp; 2 http
		bool Download(const std::string &filename, const std::string &url);
		bool IsGettedOK() const;
		float GetGettedProgress() const;

		void ClearOptionList();
		void AddOptionListStr(const std::string &optStr);
		int Get(const std::string &url);
		int Post(const std::string &url, const std::string &data);
		int Post(const std::string &url, const char *data, int length);

		char *GetGettedMemory();
		const char *GetGettedMemory() const;
		int GetGettedMemorySize() const;

		void SetCurlWriteCallback(CurlWriteCallback callback);
		CurlWriteCallback GetCurlWriteCallback();
		void SetCurlProgressCallback(CurlProgressCallback callback);
		CurlProgressCallback GetCurlProgressCallback();

		virtual int OnWrite(void *buffer, size_t size, size_t nmemb);
		virtual int OnProgress(double dltotal, double dlnow, double ultotal, double ulnow);

	private:
		curl_slist *_AddOptions();
		static int WriteFunction(void *buffer, size_t size, size_t nmemb,
			void *clientp);
		static int ProgressFunction(void *clientp, double dltotal,
			double dlnow, double ultotal, double ulnow);

		void *mCurl;
		char mCurlErrDesc[256]; // CURL_ERROR_SIZE 256
		unsigned int mStartFrom; // 续传点(开始字节)
		GetType mGetType;

		std::string mFilename;
		FILE *mFile;
		bool mIsUseNeedDownloadSize;
		int mNeedDownloadSize;
		int mCurGettedSize;
		Mutex mMutex;

		std::vector<std::string> mOptionStrs;

		char *mGettedMemory;

		bool mIsGettedOK;
		float mGettedProgress;

		CurlWriteCallback mCurlWriteCallback;
		CurlProgressCallback mCurlProgressCallback;

		static int msNumInsts;
	};
	typedef Pointer0<CurlObj> CurlObjPtr;

#include "PX2CURL.inl"

}

#endif
// PX2CURL.cpp

#include "curl/curl.h"
#include "PX2CURL.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Assert.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
int CurlObj::msNumInsts = 0;
//----------------------------------------------------------------------------
CurlObj::CurlObj() :
mCurl(0),
mFile(0),
mIsUseNeedDownloadSize(false),
mNeedDownloadSize(0),
mCurGettedSize(0),
mGetType(GT_FILE),
mGettedMemory(0),
mIsGettedOK(false),
mGettedProgress(0.0f),
mCurlWriteCallback(0),
mCurlProgressCallback(0)
{
}
//----------------------------------------------------------------------------
CurlObj::~CurlObj()
{
	if (mGettedMemory)
	{
		delete1(mGettedMemory);
	}
}
//----------------------------------------------------------------------------
bool CurlObj::Initlize()
{
	// curl
	if (0 == msNumInsts++)
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	mCurl = curl_easy_init();
	if (!mCurl)
	{
		assertion(false, "curl_easy_init failed.\n");
		PX2_LOG_ERROR("curl_easy_init failed.\n");
		return false;
	}

	curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, &WriteFunction);
	curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this);

	curl_easy_setopt(mCurl, CURLOPT_PROGRESSFUNCTION, &ProgressFunction);
	curl_easy_setopt(mCurl, CURLOPT_PROGRESSDATA, this);
	curl_easy_setopt(mCurl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(mCurl, CURLOPT_ERRORBUFFER, mCurlErrDesc);
	curl_easy_setopt(mCurl, CURLOPT_VERBOSE, 1); // 如果你想CURL报告每一件意外的事情,设置这个选项为一个非零值

	memset(mCurlErrDesc, 0, sizeof(char)*CURL_ERROR_SIZE);

	return true;
}
//----------------------------------------------------------------------------
bool CurlObj::Terminate()
{
	delete1(mGettedMemory);

	if (mCurl)
	{
		curl_easy_cleanup(mCurl);
		mCurl = 0;
	}

	if (0 == --msNumInsts)
	{
		curl_global_cleanup();
	}

	return true;
}
//----------------------------------------------------------------------------
void CurlObj::SetGetType(GetType type)
{
	mGetType = type;
}
//----------------------------------------------------------------------------
void CurlObj::Stop()
{
	ScopedCS cs(&mMutex);

	curl_easy_pause(mCurl, CURLPAUSE_RECV);
}
//----------------------------------------------------------------------------
void CurlObj::Continue()
{
	ScopedCS cs(&mMutex);

	curl_easy_pause(mCurl, CURLPAUSE_RECV_CONT);
}
//----------------------------------------------------------------------------
double CurlObj::GetSpeed() const
{
	double speed = 0.0;

	curl_easy_getinfo(mCurl, CURLINFO_SPEED_DOWNLOAD, &speed);

	return speed;
}
//----------------------------------------------------------------------------
bool CurlObj::Download(const std::string &filename,
	const std::string &url, int size, int startFrom)
{
	ScopedCS cs(&mMutex);

	mIsUseNeedDownloadSize = true;
	mNeedDownloadSize = size;
	mCurGettedSize = 0;
	mStartFrom = startFrom;
	if (mGettedMemory)
		delete1(mGettedMemory);

	if (filename != mFilename)
	{
		if (mFile)
		{
			fclose(mFile);
			mFile = 0;
		}

		mFilename = filename;
	}

	if (!mFile)
	{
		mFile = fopen(mFilename.c_str(), "rb+");

		if (!mFile)
		{
			mFile = fopen(mFilename.c_str(), "wb");
		}

		if (!mFile)
		{
			assertion(false, "File %s opened failed.\n", mFilename.c_str());
			PX2_LOG_ERROR("File %s opened failed.\n", mFilename.c_str());

			return false;
		}
	}

	if (mFile)
		fseek(mFile, mStartFrom, SEEK_SET);

	if (mGetType == GT_MEMORY || mGetType == GT_FILEMEMORY)
	{
		if (mGettedMemory)
		{
			delete1(mGettedMemory);
		}
		mGettedMemory = new1<char>(size);
		memset(mGettedMemory, 0, size*sizeof(char));
	}

	curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(mCurl, CURLOPT_RESUME_FROM, startFrom);

	CURLcode result;
	result = curl_easy_perform(mCurl);
	if (result != CURLE_OK && mCurlErrDesc[0] != 0)
	{
		// 出错
		std::string errString = std::string("Connect to ") + url +
			" failed, error:" + mCurlErrDesc;
		PX2_LOG_ERROR(errString.c_str());

		if (std::string(mCurlErrDesc) != "Failed writing body (1 != 1460)")
			return false;

		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool CurlObj::Download(const std::string &filename,
	const std::string &url)
{
	mGetType = GT_FILE;
	mIsUseNeedDownloadSize = false;
	mIsGettedOK = false;
	mCurGettedSize = 0;
	if (mGettedMemory)
		delete1(mGettedMemory);

	if (filename != mFilename)
	{
		mFilename = filename;
		if (mFile)
		{
			fclose(mFile);
			mFile = 0;
		}
	}

	// 创建文件
	if (!mFile)
	{
		mFile = fopen(mFilename.c_str(), "wb");

		if (!mFile)
		{
			assertion(false, "File %s opened failed.\n", mFilename.c_str());
			PX2_LOG_ERROR("File %s opened failed.\n", mFilename.c_str());

			return false;
		}
	}

	CURLcode result;
	result = curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set URL %s", url.c_str());
		return false;
	}

	result = curl_easy_perform(mCurl);
	if (result != CURLE_OK && mCurlErrDesc[0] != 0)
	{
		std::string errString = std::string("Connect to ") + "failed, error:" 
			+ mCurlErrDesc;

		PX2_LOG_ERROR("%s", errString.c_str());

		if (std::string(mCurlErrDesc) != "Failed writing body (1 != 1460)")
		{
			return false;
		}
	}

	if (mFile)
	{
		fclose(mFile);
		mFile = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
void CurlObj::ClearOptionList()
{
	mOptionStrs.clear();
}
//----------------------------------------------------------------------------
void CurlObj::AddOptionListStr(const std::string &optStr)
{
	mOptionStrs.push_back(optStr);
}
//----------------------------------------------------------------------------
int CurlObj::Get(const std::string &url)
{
	mIsGettedOK = false;
	mCurGettedSize = 0;
	mIsUseNeedDownloadSize = false;
	if (mGettedMemory)
		delete1(mGettedMemory);

	CURLcode result;

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: Agent - 007");
	result = curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, headers);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to change httpHeader: %s", mCurlErrDesc);
		return false;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set URL %s", url.c_str());
		return false;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_POST, 0);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to httpget: %s", mCurlErrDesc);
		return false;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_POSTFIELDS, 0);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set CURLOPT_POSTFIELDS:%s", mCurlErrDesc);
		return -1;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_HTTPGET, 1);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to httpget: %s", mCurlErrDesc);
		return false;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_TIMEOUT, 30);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set time out: %s", mCurlErrDesc);
		return false;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_FOLLOWLOCATION, 1);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set redirect option %s", mCurlErrDesc);
		return false;
	}

	result = curl_easy_perform(mCurl);
	
	if (result != CURLE_OK && mCurlErrDesc[0] != 0)
	{
		std::string errString = std::string("Connect to ") + "failed, error:"
			+ mCurlErrDesc;
		PX2_LOG_ERROR("curl_easy_perform failed: %s", errString.c_str());

		if (std::string(mCurlErrDesc) != "Failed writing body (1 != 1460)")
			return false;
	}

	return 1;
}
//----------------------------------------------------------------------------
curl_slist *CurlObj::_AddOptions()
{
	curl_slist *list = 0;
	for (int i = 0; i < (int)mOptionStrs.size(); i++)
	{
		const std::string &optStr = mOptionStrs[i];
		list = curl_slist_append(list, optStr.c_str());
	}
	
	return list;
}
//----------------------------------------------------------------------------
int CurlObj::Post(const std::string &url, const char *data, int length)
{

	mIsGettedOK = false;
	mCurGettedSize = 0;
	mIsUseNeedDownloadSize = false;
	if (mGettedMemory)
		delete1(mGettedMemory);

	CURLcode result;
	result = curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set URL %s", url.c_str());
		return false;
	}

	struct curl_slist *headers = NULL;
	headers = _AddOptions();
	if (headers)
	{
		result = curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, headers);
		if (result != CURLE_OK)
		{
			PX2_LOG_ERROR("Failed to change httpHeader: %s", mCurlErrDesc);
			return false;
		}
	}

	result = curl_easy_setopt(mCurl, CURLOPT_POST, true);

	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR(" Failed to set CURLOPT_POST:%s", mCurlErrDesc);
		return -1;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_TIMEOUT, 30);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set time out: %s", mCurlErrDesc);
		return false;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_POSTFIELDS, data);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set CURLOPT_POSTFIELDS:%s", mCurlErrDesc);
		return -1;
	}

	result = curl_easy_setopt(mCurl, CURLOPT_POSTFIELDSIZE, length);
	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to set CURLOPT_POSTFIELDSIZE:%s", mCurlErrDesc);
		return -1;
	}

	result = curl_easy_perform(mCurl);

	if (result != CURLE_OK)
	{
		PX2_LOG_ERROR("Failed to post :%s", mCurlErrDesc);

		return -2;
	}

	return 1;
}
//----------------------------------------------------------------------------
int CurlObj::Post(const std::string &url, const std::string &data)
{
	return Post(url, data.c_str(), data.length());
}
//----------------------------------------------------------------------------
int CurlObj::WriteFunction(void *buffer, size_t size, size_t nmemb,
	void *clientp)
{
	CurlObj *curlObj = (CurlObj *)clientp;

	if (!curlObj)
	{
		assertion(false, "WriteFunction curlObj must exist\n");
		PX2_LOG_ERROR("WriteFunction curlObj must exist\n");

		return -1;
	}

	return curlObj->OnWrite(buffer, size, nmemb);
}
//----------------------------------------------------------------------------
void CurlObj::SetCurlWriteCallback(CurlWriteCallback callback)
{
	mCurlWriteCallback = callback;
}
//----------------------------------------------------------------------------
CurlWriteCallback CurlObj::GetCurlWriteCallback()
{
	return mCurlWriteCallback;
}
//----------------------------------------------------------------------------
void CurlObj::SetCurlProgressCallback(CurlProgressCallback callback)
{
	mCurlProgressCallback = callback;
}
//----------------------------------------------------------------------------
CurlProgressCallback CurlObj::GetCurlProgressCallback()
{
	return mCurlProgressCallback;
}
//----------------------------------------------------------------------------
int CurlObj::OnWrite(void *buffer, size_t size, size_t nmemb)
{
	int allSizeWrited = 0;

	int iSize = (int)size;
	int iNum = (int)nmemb;

	int sizeTemp = this->mCurGettedSize + iSize*iNum;
	int needReadSize = iSize*iNum;

	if (0 == this->mGettedMemory)
	{
		this->mGettedMemory = new1<char>(sizeTemp);
	}
	else
	{
		char *newBuffer = new1<char>(sizeTemp);
		memcpy(newBuffer, this->mGettedMemory, this->mCurGettedSize);
		delete1(this->mGettedMemory);
		this->mGettedMemory = newBuffer;
	}

	if (this->mIsUseNeedDownloadSize)
	{
		bool isSizeOver = (sizeTemp >= this->mNeedDownloadSize);
		if (isSizeOver)
			needReadSize = this->mNeedDownloadSize - this->mCurGettedSize;
	}

	if (needReadSize <= 0)
	{
		if (this->mFile)
		{
			fclose(this->mFile);
			this->mFile = 0;
		}

		if (mCurlWriteCallback)
		{
			mCurlWriteCallback(this, buffer, size, nmemb);
		}

		return allSizeWrited;
	}

	int writeSize = 0;
	if (this->mGetType == GT_FILE)
	{
		fwrite((char*)buffer, needReadSize, 1, this->mFile);
		writeSize = needReadSize;
	}
	else if (this->mGetType == GT_MEMORY)
	{
		memcpy(this->mGettedMemory + this->mCurGettedSize, (char*)buffer, needReadSize);
		writeSize = needReadSize;
	}
	else if (this->mGetType == GT_FILEMEMORY)
	{
		fwrite((char*)buffer, needReadSize, 1, this->mFile);
		writeSize = needReadSize;
		memcpy(this->mGettedMemory + this->mCurGettedSize, (char*)buffer, needReadSize);
	}

	this->mCurGettedSize += writeSize;
	allSizeWrited += writeSize;

	if (mCurlWriteCallback)
	{
		mCurlWriteCallback(this, buffer, size, nmemb);
	}

	if (this->mIsUseNeedDownloadSize)
	{
		if (this->mCurGettedSize >= this->mNeedDownloadSize)
		{
			if (this->mFile)
			{
				fclose(this->mFile);
				this->mFile = 0;
			}

			return allSizeWrited;
		}
	}

	return allSizeWrited;
}
//----------------------------------------------------------------------------
int64_t file_size(const char* szFileName)
{
	FILE* f = fopen(szFileName, "rb");
	if (!f)
		return -1;

	fseek(f, 0, SEEK_END);
	int64_t len = ftell(f);
	fclose(f);
	return len;
}
//----------------------------------------------------------------------------
int CurlObj::OnProgress(double dltotal, double dlnow, double ultotal,
	double ulnow)
{
	// 当前文件已下载大小
	int64_t nNowFileSize = 0;
	
	if (GT_FILE == mGetType || GT_FILEMEMORY == mGetType)
	{
		nNowFileSize = file_size(this->mFilename.c_str());
	}
	else if (GT_MEMORY == mGetType)
	{
		nNowFileSize = mCurGettedSize;
	}

	// 需要文件大小
	unsigned int totalfileSize = (unsigned int)dltotal;
	if (0 != totalfileSize)
	{
		this->mGettedProgress = (float)nNowFileSize / (float)totalfileSize;

		if (nNowFileSize == totalfileSize)
		{
			this->mIsGettedOK = true;
		}
		else
		{
			this->mIsGettedOK = false;
		}
	}

	if (mCurlProgressCallback)
	{
		mCurlProgressCallback(this, dltotal, dlnow, ultotal, ulnow);
	}

	return 0;
}
//----------------------------------------------------------------------------
int CurlObj::ProgressFunction(void *clientp, double dltotal,
	double dlnow, double ultotal, double ulnow)
{
	PX2_UNUSED(dlnow);
	PX2_UNUSED(ultotal);
	PX2_UNUSED(ulnow);

	CurlObj *curlObj = static_cast<CurlObj *>(clientp);
	if (!curlObj)
	{
		assertion(false, "curlObj curlObj must exist\n");
		PX2_LOG_ERROR("curlObj curlObj must exist\n");
	}

	return curlObj->OnProgress(dltotal, dlnow, ultotal, dlnow);
}
//----------------------------------------------------------------------------
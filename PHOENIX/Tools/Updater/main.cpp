// main.cpp

#include "PX2Dir.hpp"
#include "PX2FileIO.hpp"
#include "PX2MD5.hpp"
#include "PX2Memory.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ResourceUpdate.hpp"
#include "PX2Application.hpp"
#include "PX2XMLNode.hpp"
#include "PX2System.hpp"
using namespace std;
using namespace PX2;

//----------------------------------------------------------------------------
std::string _GetMD5(const std::string &strBuffer)
{
	char buffer[17], out[33];
	memset(buffer, 0, sizeof(buffer));
	memset(out, 0, sizeof(out));

	Md5HashBuffer(buffer, strBuffer.c_str(), (int)strBuffer.length());
	Md5HexString(buffer, out);

	return std::string(out);
}
//----------------------------------------------------------------------------
void _RefreshDir(XMLNode node, const std::string &pathName)
{
	std::string eachFilename;
	DirP d;
	if (d.Open(pathName))
	{
		if (!d.HasFiles() && !d.HasSubDirs())
			return;

		int flags = DirP::DIR_DIRS | DirP::DIR_FILES;

		if (d.GetFirst(&eachFilename, "", flags))
		{
			do
			{
				if ((eachFilename != ".") && (eachFilename != ".."))
				{
					_RefreshDir(node, pathName + "/" + eachFilename);
				}

			} while (d.GetNext(&eachFilename));
		}
	}
	else
	{
		//if (pathName.find("updatefilelist") == std::string::npos)
		{
			XMLNode childNode = node.NewChild("file");
			childNode.SetAttributeString("filename", pathName);

			char *buffer = 0;
			int bufferSize = 0;
			if (FileIO::Load(pathName, true, bufferSize, buffer))
			{
				std::string strBuf = _GetMD5(std::string(buffer, bufferSize));
				delete1(buffer);
				bufferSize = 0;

				childNode.SetAttributeString("md5", strBuf);
			}
			else
			{
				childNode.SetAttributeString("md5", "");
			}
		}
	}
}
//----------------------------------------------------------------------------
void _DoUpdateStuffs(const std::string &wwwAddr, const std::string toDirPath)
{
	ResourceManager::ResourceFileTable mDataUpdateFileTable;

	std::string fileList_local = toDirPath + "/updatefilelist.xml";
	PX2_RM.LoadFileTableXML(mDataUpdateFileTable, fileList_local);

	std::string filelist_www = wwwAddr + "PHOENIXEngine/updatefilelist.xml";

	std::string filelistPrepare = toDirPath + "/updatefilelist_prepare.xml";
	std::string filelistToUpdate = toDirPath + "/updatefilelist_temp.xml";
	std::string filelist = toDirPath + "/updatefilelist.xml";

	int type = 1;

	if (!PX2_RM.Download(filelistPrepare, filelist_www, type)) // Ô¤±¸ÏÂÔØ
		return;

	PX2_LOG_INFO("Downloaded filelist_prepare %s", filelistPrepare.c_str());

	if (!PX2_RM.Download(filelistToUpdate, filelist_www, type)) // ÏÂÔØ
		return;

	PX2_LOG_INFO("Downloaded filelist_toupdate %s", filelistToUpdate.c_str());

	ResourceManager::ResourceFileTable tableToUpdate;
	PX2_RM.LoadFileTableXML(tableToUpdate, filelistToUpdate);
	int numUpdateFiles = (int)tableToUpdate.size();

	PX2_LOG_INFO("num %d files to update", numUpdateFiles);

	if (0 == numUpdateFiles)
	{
	}
	else
	{
		ResourceManager::ResourceFileTable newTable;
		auto itUpdate = tableToUpdate.begin();
		for (; itUpdate != tableToUpdate.end(); itUpdate++)
		{
			FString filename = itUpdate->first;
			std::string strFilename = filename;
			auto itFind = strFilename.find("PHOENIXEngine/Updater");
			if (itFind == std::string::npos)
			{
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
					PX2_RM.Download("../"+downloadFilename, wwwURL, type);

					PX2_LOG_INFO("downlaod from %s to %s", wwwURL.c_str(), downloadFilename);
				}
			}
		}

		// set table
		mDataUpdateFileTable = newTable;

		// down last

		if (!PX2_RM.Download(filelist, filelist_www, type))
			return;
	}
}
//----------------------------------------------------------------------------
void _StartApp(const std::string &eName, const std::string &strCfg)
{
	std::string renderTag = Renderer::GetRenderTag();
	if ("Dx9" == renderTag)
		renderTag = "";

	std::string exeName = eName + renderTag;

	std::string appName;

#if defined(_WIN32) || defined(WIN32)

#ifdef _DEBUG

#if defined(_WIN64) || defined(WIN64)
	appName = exeName + "64D.exe" + " " + strCfg;
#else
	appName = exeName + "D.exe" + " " + strCfg;
#endif

#else

#if defined(_WIN64) || defined(WIN64)
	appName = exeName + "64.exe" + " " + strCfg;
#else
	appName = exeName + ".exe" + " " + strCfg;
#endif

#endif
	WinExec(appName.c_str(), SW_SHOW);
#endif

}
//----------------------------------------------------------------------------
int main(int numArguments, char** arguments)
{
#if defined(_MSC_VER)
	// Tell MSVC to report any memory leaks
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	// Use _CrtSetBreakAlloc(n) to find a specific memory leak
#endif

	Application *app = new Application();

#ifdef PX2_USE_MEMORY
	Memory::Initialize();
#endif

	StringHelp::Initlize();
	FString::Initlize();

	Logger *logger = new0 Logger();

#if defined(_WIN32) || defined(WIN32) || defined(__LINUX__)
	logger->AddFileHandler("PX2Updater_Log.txt", LT_INFO|LT_ERROR|LT_USER);
#endif

#if defined (__LINUX__)
	logger->AddConsoleHandler(LT_INFO | LT_ERROR | LT_USER);
#endif

	logger->AddOutputWindowHandler(LT_INFO | LT_ERROR | LT_USER);
	logger->StartLogger();

	System::SleepSeconds(1.0f);

	ResourceManager *resMan = new0 ResourceManager();

	std::string cmdStr;
	std::string dirDataPath;
	std::string genFilename;

	if (numArguments > 1)
	{
		cmdStr = arguments[1];
	}
	if (numArguments > 2)
	{
		dirDataPath = arguments[2];
	}
	if (numArguments > 3)
	{
		genFilename = arguments[3];
	}

	PX2_LOG_INFO("cmdStr %s", cmdStr.c_str());

	if ("genfilelist" == cmdStr)
	{
		XMLData data;
		data.Create();

		XMLNode rootNode = data.NewChild("filelist");
		rootNode.SetAttributeString("version", "1.0.0");
		_RefreshDir(rootNode, dirDataPath);

		std::string savePath = dirDataPath + "/" + genFilename;
		data.SaveFile(savePath);
	}
	else if ("update_startlauncher" == cmdStr)
	{
		std::string dirPath = "ftp://182.254.213.85/";
		_DoUpdateStuffs(dirPath, "../PHOENIXEngine");

		PX2_LOG_INFO("Try to start app launcher");
		_StartApp("Launcher", "-n");
	}
	else if ("startlauncher" == cmdStr)
	{
		PX2_LOG_INFO("Try to start app launcher");
		_StartApp("Launcher", "-n");
	}

	if (resMan)
	{
		delete0(resMan);
		ResourceManager::Set(0);
	}

	System::SleepSeconds(1.0f);

	if (resMan)
	{
		resMan->Clear();
		delete0(resMan);
		ResourceManager::Set(0);
	}

	PX2_LOG_INFO("Engine Terminate done.");
	if (logger)
	{
		delete0(logger);
		Logger::Set(0);
	}

	FString::Ternimate();

#ifdef PX2_USE_MEMORY
	Memory::Terminate("Update.txt", false);
#endif

	delete(app);

	return 0;
}
//----------------------------------------------------------------------------
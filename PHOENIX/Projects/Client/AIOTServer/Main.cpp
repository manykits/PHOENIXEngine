// Main.cpp

#include "PX2Net.hpp"
#include "PX2Core.hpp"
#include "PX2Mathematics.hpp"
#include "PX2Application.hpp"
#include "PX2Log.hpp"
#include "PX2System.hpp"
#include "PX2AppInitConfig.hpp"
#include "AIOTServer.hpp"
#include "AIOTUserManager.hpp"
using namespace PX2;

int main()
{
	Application *app = new Application();
	AppInitConfig cfg;
	cfg.IsInitSound = false;
	cfg.IsInitVoice = false;

	if (app->Initlize(&cfg))
	{
		Logger *logger = Logger::GetSingletonPtr();
		logger->CloseLogger();

#if defined(_WIN32) || defined(WIN32) || defined(__LINUX__)
		logger->AddFileHandler("log_server.txt", LT_INFO | LT_ERROR | LT_USER);
#endif
		logger->AddOutputWindowHandler(LT_INFO | LT_ERROR | LT_USER);
		logger->AddConsoleHandler(LT_INFO | LT_ERROR | LT_USER);
		logger->SetLogFileInfo(false);
		logger->StartLogger();

		app->SetInEditor(false);

		AIOTUserManager *userMgr = new0 AIOTUserManager(64);

		AIOTServerPtr vaServer = new0 AIOTServer(Server::ST_IOCP, 6601,
			1024, 10);
		if (!vaServer->Start())
			return 0;

		PX2_LOG_INFO("AIOTServer Start OK!");

		while (!app->IsQuit())
		{
			app->Update();
			float esconds = app->GetElapsedTime();
			vaServer->Run(esconds);
		}

		if (vaServer)
			vaServer->Shutdown();
		System::SleepSeconds(1.0f);

		vaServer = 0;
		AIOTServer::Set(0);
		
		delete0(userMgr);
		AIOTUserManager::Set(0);

		app->Terminate();
	}

	delete(app);

	return 0;
}
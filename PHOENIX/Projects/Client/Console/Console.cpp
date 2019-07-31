// LRMain.cpp

#include "PX2Net.hpp"
#include "PX2Core.hpp"
#include "PX2Mathematics.hpp"
#include "PX2Application.hpp"
#include "PX2Log.hpp"
#include "PX2System.hpp"
#include "PX2AppInitConfig.hpp"
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

		while (!app->IsQuit())
		{
			app->Update();
			System::SleepSeconds(0.01f);
		}
		app->Terminate();
	}

	delete(app);

	return 0;
}
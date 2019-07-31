// PX2STEAMEdu.hpp

#ifndef PX2STEAMEDU_HPP
#define PX2STEAMEDU_HPP

#include "PX2CorePre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2HTTPServer.hpp"
#include "PX2HTTPRequestHandlerFactory.hpp"
#include "PX2HTTPRequestHandler.hpp"
#include "PX2HTTPServerResponse.hpp"
#include "PX2HTTPServerRequest.hpp"
#include "PX2Arduino.hpp"
#include "PX2Eventt.hpp"
#include "PX2Semaphore.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM STEAMEduManager : public Singleton<STEAMEduManager>
	{
	public:
		STEAMEduManager();
		virtual ~STEAMEduManager();

		void Initlize();
		void Terminate();
		void Update(float elapsedSeconds);

		void OpenArduino(const std::string &filename);

		Arduino *GetArduinoByName(const std::string &name);
		std::map<std::string, ArduinoPtr> Arduinos;

		void ProcessHttp(HTTPServerRequest& request, HTTPServerResponse& response);

		HTTPServerPtr mHttpServer;
		Mutex mMutex;

		HTTPServerRequest *CurReq;
		HTTPServerResponse *CurResp;
		Semaphore *TheSemaphore;
		float ProcessSeconds;

	private:
		std::vector<std::pair<std::string, std::string> > mMCPins;

	private:
		bool mIsPlaying;
	};

#define PX2_STEAMEDU STEAMEduManager::GetSingleton()

	class PX2_ENGINE_ITEM STEAMEduHandlerFactory : public HTTPRequestHandlerFactory
	{
	public:
		STEAMEduHandlerFactory();

		virtual HTTPRequestHandler * CreateRequestHandler(const HTTPServerRequest &request);
	};

	class PX2_ENGINE_ITEM Snap_RequestHandler : public HTTPRequestHandler
	{
	public:
		Snap_RequestHandler();
		virtual ~Snap_RequestHandler();

		virtual void HandleRequest(HTTPServerRequest& request,
			HTTPServerResponse& response);
	};

	class PX2_ENGINE_ITEM WebSocketRequestHandler : public HTTPRequestHandler
	{
	public:
		WebSocketRequestHandler();
		virtual ~WebSocketRequestHandler();

		virtual void HandleRequest(HTTPServerRequest& request,
			HTTPServerResponse& response);
	};

}

#endif
// PX2WRLiDar.hpp

#ifndef PX2WRLIDAR_HPP
#define PX2WRLIDAR_HPP

#include "PX2CorePre.hpp"
#include "PX2UDPServer.hpp"
#include "PX2AILidarData.hpp"
#include "PX2DatagramSocket.hpp"
#include "PX2Runnable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM WRLidar : public Runnable
	{
	public:
		WRLidar();
		~WRLidar();

		void Start(const std::string &ip, int port);
		void Stop();
		void Update(float appSeconds, float elapsedSeconds);

		virtual void Run();
		
		bool Set_scanning_parameters(int iFirstStepIndex, int iLastStepIndex);
		bool Start_measurement();

		std::vector<RslidarDataComplete> RDCS;

	private:
		bool mIsStop;
		std::string mIP;
		int mPort;
		SocketAddress mSocketAddress;
		DatagramSocket mSocket;
		ThreadPtr mThread;
		bool is_actived;
	};
	typedef Pointer0<WRLidar> WRLidarPtr;

}

#endif
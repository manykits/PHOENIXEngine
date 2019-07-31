// PX2GPIO.hpp

#ifndef PX2GPIO_HPP
#define PX2GPIO_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM GPIO
	{
	public:
		enum DirectionType 
		{ 
			DT_IN, 
			DT_OUT,
			DT_MAX_TYPE
		};

		GPIO();
		GPIO(unsigned port, DirectionType dir);
		~GPIO();

		void Set(unsigned port, DirectionType dir);
		void SetDirection(DirectionType dir);
		void SetPort(unsigned port);

		void SetLow() const;
		void SetHigh() const;
		void SetValue(int val);
		int GetValue() const;
		DirectionType GetDirection() const;

		// 产生脉冲
		// @lowWidth 低电平时间
		// @highWidth 高电平时间
		// @times 完整的周期数
		void Pulse(unsigned lowWidth = 1, unsigned highWidth = 1, unsigned times = 1) const;

		// 产生上升/下降沿
		void Up(unsigned times = 1) const;
		void Down(unsigned times = 1) const;

	protected:
		void _ExportPort() const;
		void _UnexportPort() const;
		void _Sleep(int val) const;

		unsigned int mPort;
		DirectionType mDir;

	public:
		static bool IsExist(unsigned gpio);

		static bool Export(int gpio);
		static bool Unexport(int gpio);

		//< "in, out"
		static bool SetDriection(int gpio, const std::string &dir);
		static bool SetValue(int gpio, int val);

		static bool GetValue(int gpio, int &val);
	};

}

#endif

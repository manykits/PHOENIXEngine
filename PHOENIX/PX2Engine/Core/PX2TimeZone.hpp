// PX2TimeZone.hpp

#ifndef PX2TIMEZONE_HPP
#define PX2TIMEZONE_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TimeZone
	{
	public:
		static int UTCOffset();

		/// 夏时制（Daylight Saving Time：DST）
		/**
		* 又称“日光节约时制”和“夏令时间”，是一种为节约能源而人为规定地方时
		* 间的制度，在这一制度实行期间所采用的统一时间称为“夏令时间”。一般在
		* 天亮早的夏季人为将时间提前一小时，可以使人早起早睡，减少照明量，以充
		* 分利用光照资源，从而节约照明用电。各个采纳夏时制的国家具体规定不同。
		* 目前全世界有近110个国家每年要实行夏令时。自2011年3月27日开始俄罗斯永
		* 久使用夏令时，把时间拨快一小时，不再调回。
		* local time = UTC + UTCOffset() + Dst()
		*/		
		static int Dst();
		static bool IsDst(const Timestamp& timestamp);

		/// UTCOffset() + Dst().
		static int TZD();
	};

}

#endif
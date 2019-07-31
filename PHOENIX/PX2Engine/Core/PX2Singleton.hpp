// PX2Singleton.hpp

#ifndef PX2SINGETON_HPP
#define PX2SINGETON_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

#define  PX2_SINGLETION(classname) \
	public: \
		static classname &GetSingleton() \
		{\
			static classname inst;\
			return inst;\
		}\
		static classname *GetSingletonPtr() \
		{\
			return &GetSingleton();\
		}\
	protected: \
		classname(); \
		~classname(); \
		classname (const classname&); \
		classname& operator = (const classname&)

}

#endif
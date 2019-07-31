// PX2GeneralAlloctorObject.hpp

#ifndef PX2GENERALALLOCTOROBJECT_HPP
#define PX2GENERALALLOCTOROBJECT_HPP

#include "PX2CorePre.hpp"
#include "PX2GeneralAlloctor.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM GeneralAlloctorObject
	{
	public:
		GeneralAlloctorObject () {}
		virtual ~GeneralAlloctor () {}

		static void *operator new (size_t size)
		{
			GeneralAlloctor::Allocate(size);
		}

		static void operator delete (void *buffer, size_t size)
		{
			GeneralAlloctor::Deallocate(buffer, size);
		}
	};

}

#endif
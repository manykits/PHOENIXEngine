// PX2CollapseRecordArray.hpp

#ifndef PX2COLLAPSERECORDARRAY_HPP
#define PX2COLLAPSERECORDARRAY_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2CollapseRecord.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CollapseRecordArray : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CollapseRecordArray);

	public:
		CollapseRecordArray (int numRecords = 0, CollapseRecord* records = 0);
		virtual ~CollapseRecordArray ();

		inline int GetNumRecords () const;
		inline CollapseRecord* GetRecords () const;

	protected:
		int mNumRecords;
		CollapseRecord* mRecords;
	};

	PX2_REGISTER_STREAM(CollapseRecordArray);
	typedef Pointer0<CollapseRecordArray> CollapseRecordArrayPtr;
#include "PX2CollapseRecordArray.inl"

}

#endif

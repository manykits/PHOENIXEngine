// PX2Query.hpp

#ifndef PX2QUERY_HPP
#define PX2QUERY_HPP

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Query
	{
		// Abstract base class
	protected:
		Query();
	public:
		virtual ~Query();

		// Run-time type information.
		enum PX2_ENGINE_ITEM Type
		{
			QT_INT64,
			QT_INTEGER,
			QT_RATIONAL,
			QT_REAL,
			QT_FILTERED
		};

		virtual Type GetType() const = 0;

		// Support for ordering a set of unique indices into the vertex pool.  On
		// output it is guaranteed that:  v0 < v1 < v2.  This is used to guarantee
		// consistent queries when the vertex ordering of a primitive is permuted,
		// a necessity when using floating-point arithmetic that suffers from
		// numerical round-off errors.  The input indices are considered the
		// positive ordering.  The output indices are either positively ordered
		// (an even number of transpositions occurs during sorting) or negatively
		// ordered (an odd number of transpositions occurs during sorting).  The
		// functions return 'true' for a positive ordering and 'false' for a
		// negative ordering.
		static bool Sort(int& v0, int& v1);
		static bool Sort(int& v0, int& v1, int& v2);
		static bool Sort(int& v0, int& v1, int& v2, int& v3);
	};

}

#endif

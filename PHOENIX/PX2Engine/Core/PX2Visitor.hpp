// PX2Visitor.hpp

#ifndef PX2VISITOR_HPP
#define PX2VISITOR_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class Object;
	class Any;

	class PX2_ENGINE_ITEM Visitor
	{
	public:
		Visitor ();
		virtual ~Visitor ();

		virtual void Visit (Object *obj, int info);
		virtual void Visit (Object *obj, const std::string &info);
		virtual void Visit (Object *obj, const Any &info);
	};

#define  PX2_VISITOR \
public: \
	void AddVisitor (Visitor *visitor) \
	{ \
		for (int i=0; i<(int)mVisitors.size(); i++) \
		{ \
			if (visitor == mVisitors[i]) \
				return; \
		} \
		mVisitors.push_back(visitor); \
	} \
	void RemoveVisitor (Visitor *visitor) \
	{ \
		std::vector<Visitor *>::iterator it = mVisitors.begin(); \
		for (; it!=mVisitors.end(); it++) \
		{ \
			if (visitor == *it) \
			{ \
				mVisitors.erase(it); \
					return; \
			} \
		} \
	} \
	void RemoveAllVisitors () \
	{ \
		mVisitors.clear(); \
	} \
	\
protected: \
	std::vector<Visitor *> mVisitors;
}

#endif
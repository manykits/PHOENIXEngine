// PX2Rtti.hpp

#ifndef PX2RTTI_HPP
#define PX2RTTI_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// 运行识别机制类
	/**
	* 在系统所有的Object中，acName名称必须不同。在PX2的名字空间中，一个名叫Poo的
	* 类的名称为PX2.Poo。如果应用程序有其他的名字空间，如SomeName，那么这个名称
	* 为"SomeName.Poo"。
	*/
	class PX2_ENGINE_ITEM Rtti
	{
	public:
		Rtti (const char* name, unsigned short version, const Rtti* baseType);
		~Rtti ();

		inline const char* GetName () const;
		inline unsigned short GetVersion () const;
		inline bool IsExactly (const Rtti& type) const;
		bool IsDerived (const Rtti& type) const;

	private:
		const char* mName;
		unsigned short mVersion;
		const Rtti* mBaseType;
	};

#include "PX2Rtti.inl"

}

//----------------------------------------------------------------------------
#define PX2_DECLARE_RTTI \
public: \
	static const Rtti TYPE; \
	\
	virtual const Rtti& GetRttiType () const \
	{ \
		return TYPE; \
	} \
	virtual unsigned short GetRttiVersion () const \
	{ \
		return GetRttiType().GetVersion(); \
	}
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_RTTI(nsname, baseclassname, classname) \
	const Rtti classname::TYPE(#nsname"."#classname, 0, &baseclassname::TYPE)
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_RTTI_V(nsname, baseclassname, classname, version) \
	const Rtti classname::TYPE(#nsname"."#classname, version, &baseclassname::TYPE)
//----------------------------------------------------------------------------

#endif

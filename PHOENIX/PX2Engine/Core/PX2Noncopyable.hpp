// PX2Noncopyable.hpp

#ifndef PX2NONCOPYABLE_HPP
#define PX2NONCOPYABLE_HPP

namespace PX2
{

	/// 从此类派生类的对象将不可被拷贝
	class PX2_ENGINE_ITEM Noncopyable
	{
	protected:
		Noncopyable () {}
		~Noncopyable () {}

	private:
		Noncopyable (const Noncopyable &);
		Noncopyable &operator = (const Noncopyable &);
	};

}

#endif
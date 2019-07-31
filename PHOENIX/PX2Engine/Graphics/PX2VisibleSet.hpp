// PX2VisibleSet.hpp

#ifndef PX2VISIBLESET_HPP
#define PX2VISIBLESET_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Assert.hpp"

namespace PX2
{

	class Renderable;

	typedef bool (*SortFun) (const Renderable *renderable0,
		const Renderable *renderable1);
	
	/// 可见对象集
	class PX2_ENGINE_ITEM VisibleSet
	{
	public:
		VisibleSet ();
		~VisibleSet ();

		// 访问可见对象
		inline int GetNumVisible () const;
		inline const std::vector<Renderable*>& GetAllVisible () const;
		inline Renderable* GetVisible (int i) const;

		// 插入可见对象
		void Insert (Renderable* visible);

		void SetSortFun (SortFun fun);
		SortFun GetSortFun () const;

		// 排序
		VisibleSet &Sort ();

		// 清空可见对象列表
		inline void Clear ();

	private:
		SortFun mSortFun;
		int mNumVisible;
		std::vector<Renderable*> mVisible;
 	};

#include "PX2VisibleSet.inl"

}

#endif
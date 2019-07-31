// PX2NotFreeObjectPool.hpp

#ifndef PX2NOTFREEOBJECTPOOL_HPP
#define PX2NOTFREEOBJECTPOOL_HPP

#include "PX2CorePre.hpp"
#include "PX2Any.hpp"

namespace PX2
{

	/// 此对象管池的管理的对象永远不释放，只每次分配时调用初始化
	/// Initlize()一下
	/**
	* T 需要实现
	*	void OnAllocAll (const Any &userData);
	*	void OnAlloc ();
	*	void OnFree ();
	*	void SetObjectID (uint32_t id);
	*	uint32_t GetObjectID ();
	*/
	template <typename T>
	class NotFreeObjectPool
	{
	public: 
		NotFreeObjectPool ();

		// 析构函数不释放，你需要手动调用FreeAllObjects
		virtual ~NotFreeObjectPool (); 

		bool AllocAllObjects (uint32_t numMaxObjects, const Any &initParam);
		bool ClearAllObjects ();
		uint32_t GetNumAllocedObjects ();
		T *GetAllocedObjectAt (uint32_t index);

		int GetNumFree () const;
		T* AllocObject ();
		T *AddAllocedObject ();
		void FreeObject (uint32_t objID);	
		uint32_t GetNumObjects ();

		void FreeAllAllocedObjects();
		const std::set<uint32_t> &GetAllObjectIDs();

		T *GetObjectByID (uint32_t objID);

	private:
		uint32_t mNumMaxObjects;
		std::queue<uint32_t> mIDsQueue;
		std::vector<Pointer0<T> > mObjects;
		std::vector<char> mObjectFlags;
		std::set<uint32_t> mAllAlloctIDs; // 所有有效的对象ID
		Any mInitParam;
	};

#include "PX2NotFreeObjectPool.inl"

}

#endif
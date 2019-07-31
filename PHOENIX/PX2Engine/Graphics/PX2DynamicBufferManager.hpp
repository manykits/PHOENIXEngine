// PX2DynamicBufferManager.hpp

#ifndef PX2DYNAMICBUFFERMANAGER_HPP
#define PX2DYNAMICBUFFERMANAGER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2IndexBuffer.hpp"
#include "PX2DynamicBufferObject.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM DynamicBufferManager : public Singleton<DynamicBufferManager>
	{
	public:
		DynamicBufferManager ();
		~DynamicBufferManager ();

		bool Initlize ();
		bool Terminate ();

		void Reset ();
		void Update ();

		DBObject_V *AllocVertexBuffer (int numVertexs);
		DBObject_I *AllocIndexBuffer (int numIndexs);

	protected:
		bool mIsInited;

		int mNumMaxVertex;
		int mNumMaxIndex;
		VertexBufferPtr mVB;
		int mCurVBOffset;
		IndexBufferPtr mIB;
		int mCurIBOffset;

		std::vector<DBObject_V *> mFreeVBs;
		std::vector<DBObject_V *> mUsedVBs;
		std::vector<DBObject_I *> mFreeIBs;
		std::vector<DBObject_I *> mUsedIBs;
	};

#define PX2_DBM DynamicBufferManager::GetSingleton()

}

#endif
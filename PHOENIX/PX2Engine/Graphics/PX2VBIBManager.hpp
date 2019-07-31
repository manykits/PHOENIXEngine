// PX2VBIBObjManager.hpp

#ifndef PX2VBIBOBJMANAGER_HPP
#define PX2VBIBOBJMANAGER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2IndexBuffer.hpp"
#include "PX2HashTable.hpp"

namespace PX2
{

	struct PX2_ENGINE_ITEM VBIBObj
	{
		VBIBObj()
		{
			IsValued = false;
		}

		bool IsValued;
		VertexBufferPtr mVB;
		IndexBufferPtr mIB;
	};

	class PX2_ENGINE_ITEM VBIBManager : public Singleton<VBIBManager>
	{
	public:
		VBIBManager();
		virtual ~VBIBManager();

		void Clear();
		void AddVBIB(const std::string &texPackFilename, VertexBuffer *vb,
			IndexBuffer *ib);
		VBIBObj &GetVBID(const std::string &texPackFilename);

	protected:
		typedef std::map<FString, VBIBObj> UBIBTable;
		UBIBTable mUBIBTable;
		VBIBObj mNotValied;
	};

}

#endif
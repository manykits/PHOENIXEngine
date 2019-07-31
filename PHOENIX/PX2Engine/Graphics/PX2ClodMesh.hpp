// PX2ClodMesh.hpp

#ifndef PX2CLODMESH_HPP
#define PX2CLODMESH_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2CollapseRecordArray.hpp"
#include "PX2TriMesh.hpp"


namespace PX2
{

	/**
	* ClodMesh会创建一份输入的mesh的顶点索引的拷贝。
	*/
	class PX2_ENGINE_ITEM ClodMesh : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ClodMesh);

	public:
		ClodMesh (TriMesh* mesh, CollapseRecordArray* recordArray);
		virtual ~ClodMesh ();

		inline int GetNumRecords () const;
		inline int& TargetRecord ();

		/// 派生类重载此函数，根据相机距离等，获得目标网格级别。
		inline virtual int GetAutomatedTargetRecord ();

		/// 几何图形更新
		/**
		* 调用该函数，更新几何图形并且调整网格质量。
		*/
		void SelectLevelOfDetail ();

	protected:
		virtual void OnGetVisibleSet (Culler& culler, bool noCull);

		int mCurrentRecord, mTargetRecord;
		CollapseRecordArrayPtr mRecordArray;
	};

	PX2_REGISTER_STREAM(ClodMesh);
	typedef Pointer0<ClodMesh> ClodMeshPtr;
#include "PX2ClodMesh.inl"

}

#endif
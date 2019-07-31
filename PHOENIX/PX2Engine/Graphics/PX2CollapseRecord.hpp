// PX2CollapseRecord.hpp

#ifndef PX2COLLAPSERECORD_HPP
#define PX2COLLAPSERECORD_HPP

#include "PX2GraphicsPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CollapseRecord
	{
	public:
		CollapseRecord (int vKeep = -1, int vThrow = -1, int numVertices = 0,
			int numTriangles = 0);

		~CollapseRecord ();

		// Edge <VKeep,VThrow>，点的VThrow会被VKeep替换掉
		int VKeep, VThrow;

		// 顶点塌陷掉后，顶点的数量
		int NumVertices;

		// 顶点塌陷掉后，三角形数量
		int NumTriangles;

		// 保存[0..NumTriangles-1]之内被扔掉的顶点的索引
		int NumIndices;
		int* Indices;
	};

}

#endif
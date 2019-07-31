// PX2CreateClodMesh.cpp

#include "PX2CreateClodMesh.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
CreateClodMesh::CreateClodMesh (TriMesh* mesh, int& numRecords,
								CollapseRecord*& records)
								:
mNumVertices(mesh->GetVertexBuffer()->GetNumElements()),
mNumIndices(mesh->GetIndexBuffer()->GetNumElements()),
mNumTriangles(mNumIndices/3),
mIndices((int*)mesh->GetIndexBuffer()->GetData()),
mVBA(mesh),
mVertices(mNumVertices),
mHeap(mNumIndices, 0, Mathf::MAX_REAL)
{
	// 确保顶点和顶点索引是合法被用来做顶点塌陷的
	if (!ValidBuffers() )
	{
		numRecords = 0;
		records = 0;
		return;
	}

	// 创建“顶点-边-三角形”邻接信息表
	int* currentIndex = mIndices;
	for (int t = 0; t < mNumTriangles; ++t)
	{
		int v0 = *currentIndex++;
		int v1 = *currentIndex++;
		int v2 = *currentIndex++;
		InsertTriangle(TriangleKey(v0, v1, v2), t);
	}

	// 计算可以被塌陷顶点
	ClassifyCollapsibleVertices();

	// 更新堆信息，将可塌陷顶点权重按照小到大排序
	EdgeMap::iterator emIter = mEdges.begin();
	EdgeMap::iterator emEnd = mEdges.end();
	for (/**/; emIter != emEnd; ++emIter)
	{
		assertion(
			emIter->second.Record->GetIndex() < mHeap.GetNumElements(),
			"Unexpected condition.\n");
		mHeap.Update(emIter->second.Record, ComputeMetric(emIter->first));
	}

	while (mHeap.GetNumElements() > 0)
	{
		EdgeKey ekey;
		float metric;
		mHeap.GetMinimum(ekey, metric);
		if (metric == Mathf::MAX_REAL)
		{ // 所有边都不可以塌陷

			if (!ValidResults())
			{
				numRecords = 0;
				records = 0;
				return;
			}
			break;
		}

		int indexThrow = CanCollapse(ekey);
		if (indexThrow >= 0)
		{ // 可以被塌陷
			Collapse(ekey, indexThrow);
		}
		else
		{ // 不可被塌陷
			emIter = mEdges.find(ekey);
			assertion(
				emIter->second.Record->GetIndex() < mHeap.GetNumElements(),
				"Unexpected condition.\n");
			mHeap.Update(emIter->second.Record, Mathf::MAX_REAL);
		}
	}

	// Reorder the vertex buffer so that the vertices are listed in decreasing
	// time of removal by edge collapses.  For example, the first vertex to be
	// removed during an edge collapse is the last vertex in the buffer.
	// Reorder the index buffer so that the triangles are listed in decreasing
	// time of removal by edge collapses.  For example, the first pair of
	// triangles to be removed during an edge collapse are the last triangles
	// in the buffer.
	// 
	ReorderBuffers();

	// The collapse records store the incremental changes that are used for
	// dynamic LOD changes.
	ComputeRecords(numRecords, records);
}
//----------------------------------------------------------------------------
CreateClodMesh::~CreateClodMesh ()
{
}
//----------------------------------------------------------------------------
bool CreateClodMesh::ValidBuffers () const
{
	TriangleKeySet triangles;
	std::set<int> vertexIndices;
	int* currentIndex = mIndices;
	for (int t = 0; t < mNumTriangles; ++t)
	{
		int v0 = *currentIndex++;
		int v1 = *currentIndex++;
		int v2 = *currentIndex++;
		if (v0 == v1 || v0 == v2 || v1 == v2)
		{
			// 退化三角形的顶点索引必须不一样
			assertion(false, "Degenerate triangles not allowed.\n");
			return false;
		}

		vertexIndices.insert(v0);
		vertexIndices.insert(v1);
		vertexIndices.insert(v2);

		std::pair<TriangleKeySet::iterator, bool> result =
			triangles.insert(TriangleKey(v0, v1, v2));

		if (result.second == false)
		{
			// 顶点索引有代表的重复的三角形
			assertion(false, "Index buffer contains repeated triangles.\n");
			return false;
		}
	}

	// Test for a valid vertex buffer.
	if (mNumVertices > (int)vertexIndices.size()
		||  mNumVertices != (*vertexIndices.rbegin() + 1))
	{
		// If the following assertion is triggered, the vertex buffer has
		// vertices that are not referenced by the index buffer.  This is a
		// problem, because the vertex buffer is reordered based on the order
		// of the edge collapses.  Any other index buffer that references the
		// input vertex buffer is now invalid.
		assertion(false, "Index buffer does not reference all vertices.\n");
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
void CreateClodMesh::InsertTriangle (const TriangleKey& tKey, Triangle t)
{
	// 边
	int v0Key = tKey.V[0];
	int v1Key = tKey.V[1];
	int v2Key = tKey.V[2];
	EdgeKey e0Key(v0Key, v1Key);
	EdgeKey e1Key(v1Key, v2Key);
	EdgeKey e2Key(v2Key, v0Key);

	// 为每个顶点插入边和三角形的邻接信息
	mVertices[v0Key].AdjEdges.insert(e0Key);
	mVertices[v0Key].AdjEdges.insert(e2Key);
	mVertices[v0Key].AdjTriangles.insert(tKey);

	mVertices[v1Key].AdjEdges.insert(e0Key);
	mVertices[v1Key].AdjEdges.insert(e1Key);
	mVertices[v1Key].AdjTriangles.insert(tKey);

	mVertices[v2Key].AdjEdges.insert(e1Key);
	mVertices[v2Key].AdjEdges.insert(e2Key);
	mVertices[v2Key].AdjTriangles.insert(tKey);

	EdgeMap::iterator emIter = mEdges.find(e0Key);
	if (emIter == mEdges.end())
	{
		// 边第一次遇到，将其插入堆中
		Edge &edge = mEdges[e0Key];
		edge.AdjTriangles.insert(tKey);
		edge.Record = mHeap.Insert(e0Key, Mathf::MAX_REAL);
	}
	else
	{
		// 边已经存在，插入三角形到邻接列表
		emIter->second.AdjTriangles.insert(tKey);
	}

	emIter = mEdges.find(e1Key);
	if (emIter == mEdges.end())
	{
		Edge& edge = mEdges[e1Key];
		edge.AdjTriangles.insert(tKey);
		edge.Record = mHeap.Insert(e1Key, Mathf::MAX_REAL);
	}
	else
	{
		emIter->second.AdjTriangles.insert(tKey);
	}

	emIter = mEdges.find(e2Key);
	if (emIter == mEdges.end())
	{
		Edge& edge = mEdges[e2Key];
		edge.AdjTriangles.insert(tKey);
		edge.Record = mHeap.Insert(e2Key, Mathf::MAX_REAL);
	}
	else
	{
		emIter->second.AdjTriangles.insert(tKey);
	}

	// 将三角形插入三角形列表
	mTriangles[tKey] = t;
}
//----------------------------------------------------------------------------
void CreateClodMesh::RemoveTriangle (const TriangleKey& tKey)
{
	int v0Key = tKey.V[0];
	int v1Key = tKey.V[1];
	int v2Key = tKey.V[2];
	EdgeKey e0Key(v0Key, v1Key);
	EdgeKey e1Key(v1Key, v2Key);
	EdgeKey e2Key(v2Key, v0Key);

	// 移除顶点的三角形邻接信息
	mVertices[v0Key].AdjTriangles.erase(tKey);
	mVertices[v1Key].AdjTriangles.erase(tKey);
	mVertices[v2Key].AdjTriangles.erase(tKey);

	EdgeMap::iterator emIter = mEdges.find(e0Key);
	if (emIter != mEdges.end())
	{
		Edge& edge = emIter->second;
		edge.AdjTriangles.erase(tKey);
		if (edge.AdjTriangles.size() == 0)
		{
			// 边不再被三角形共享，将其从堆中移除
			assertion(
				emIter->second.Record->GetIndex() < mHeap.GetNumElements(),
				"Unexpected condition.\n");
			mHeap.Update(emIter->second.Record, -1.0f);
			EdgeKey generator;
			float metric;
			mHeap.Remove(generator, metric);
			assertion(metric == -1.0f, "The metric should be -1.\n");

			// 从顶点的边邻接信息里移除边
			mVertices[v0Key].AdjEdges.erase(e0Key);
			mVertices[v1Key].AdjEdges.erase(e0Key);

			// 从边列表里移除边
			mEdges.erase(e0Key);
		}
	}
	else
	{
		assertion(false, "Unexpected condition.\n");
	}

	emIter = mEdges.find(e1Key);
	if (emIter != mEdges.end())
	{
		Edge& edge = emIter->second;
		edge.AdjTriangles.erase(tKey);
		if (edge.AdjTriangles.size() == 0)
		{
			assertion(
				emIter->second.Record->GetIndex() < mHeap.GetNumElements(),
				"Unexpected condition.\n");
			mHeap.Update(emIter->second.Record, -1.0f);
			EdgeKey generator;
			float metric;
			mHeap.Remove(generator, metric);
			assertion(metric == -1.0f, "The metric should be -1.\n");

			mVertices[v1Key].AdjEdges.erase(e1Key);
			mVertices[v2Key].AdjEdges.erase(e1Key);

			mEdges.erase(e1Key);
		}
	}
	else
	{
		assertion(false, "Unexpected condition.\n");
	}

	emIter = mEdges.find(e2Key);
	if (emIter != mEdges.end())
	{
		Edge& edge = emIter->second;
		edge.AdjTriangles.erase(tKey);
		if (edge.AdjTriangles.size() == 0)
		{
			assertion(
				emIter->second.Record->GetIndex() < mHeap.GetNumElements(),
				"Unexpected condition.\n");
			mHeap.Update(emIter->second.Record, -1.0f);
			EdgeKey generator;
			float metric;
			mHeap.Remove(generator, metric);
			assertion(metric == -1.0f, "The metric should be -1.\n");

			mVertices[v2Key].AdjEdges.erase(e2Key);
			mVertices[v0Key].AdjEdges.erase(e2Key);

			mEdges.erase(e2Key);
		}
	}
	else
	{
		assertion(false, "Unexpected condition.\n");
	}

	// 移除三角形
	mTriangles.erase(tKey);
}
//----------------------------------------------------------------------------
void CreateClodMesh::ClassifyCollapsibleVertices ()
{
	// 只有顶点的每个邻接边，有两个三角形的几何图形的顶点才能够被塌陷
	VertexArray::iterator vaIter = mVertices.begin();
	VertexArray::iterator vaEnd = mVertices.end();
	for (/**/; vaIter != vaEnd; ++vaIter)
	{
		Vertex& vertex = *vaIter;
		EdgeKeySet::iterator ekIter = vertex.AdjEdges.begin();
		EdgeKeySet::iterator ekEnd = vertex.AdjEdges.end();
		for (/**/; ekIter != ekEnd; ++ekIter)
		{
			EdgeMap::iterator emIter = mEdges.find(*ekIter);
			if (emIter->second.AdjTriangles.size() != 2)
			{
				vertex.Collapsible = false;
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------
float CreateClodMesh::ComputeMetric (const EdgeKey& eKey)
{
	// 这些边的权重可以按照你任意的想法进行调整
	const float lengthWeight = 10.0f;
	const float angleWeight = 1.0f;

	// 当且仅当只有两个三角形共享边时，此边才能够被塌陷
	Edge& edge = mEdges[eKey];
	if (edge.AdjTriangles.size() == 2)
	{
		// 长度因子
		Vector3f& end0 = mVBA.Position<Vector3f>(eKey.V[0]);
		Vector3f& end1 = mVBA.Position<Vector3f>(eKey.V[1]);
		Vector3f diff = end1 - end0;
		float metric = lengthWeight*diff.Length();

		// 角度因子
		TriangleKey tKey = *edge.AdjTriangles.begin();
		Vector3f position0 = mVBA.Position<Vector3f>(tKey.V[0]);
		Vector3f position1 = mVBA.Position<Vector3f>(tKey.V[1]);
		Vector3f position2 = mVBA.Position<Vector3f>(tKey.V[2]);
		Vector3f edgeDirection0 = position1 - position0;
		Vector3f edgeDirection1 = position2 - position0;
		Vector3f normal0 = edgeDirection0.Cross(edgeDirection1);

		tKey = *edge.AdjTriangles.rbegin();
		position0 = mVBA.Position<Vector3f>(tKey.V[0]);
		position1 = mVBA.Position<Vector3f>(tKey.V[1]);
		position2 = mVBA.Position<Vector3f>(tKey.V[2]);
		edgeDirection0 = position1 - position0;
		edgeDirection1 = position2 - position0;
		Vector3f normal1 = edgeDirection0.Cross(edgeDirection1);
		Vector3f cross = normal0.Cross(normal1);
		metric += angleWeight*cross.Length();

		return metric;
	}

	// 只有一个三角形才包含的边，3个或者3个以上的三角形包含的边，不允许塌陷。
	return Mathf::MAX_REAL;
}
//----------------------------------------------------------------------------
int CreateClodMesh::CanCollapse (const EdgeKey& eKey)
{
	Vertex& vertex0 = mVertices[eKey.V[0]];
	Vertex& vertex1 = mVertices[eKey.V[1]];

	// 塌陷检测
	int indexKeep, indexThrow;
	if (vertex0.Collapsible)
	{
		indexKeep = 1;
		indexThrow = 0;
	}
	else if (vertex1.Collapsible)
	{
		indexKeep = 0;
		indexThrow = 1;
	}
	else
	{
		return -1;
	}

	// 如果塌陷导致“模型重叠”，将是不允许的
	int vKeep = eKey.V[indexKeep];
	int vThrow = eKey.V[indexThrow];
	Vertex& vertexThrow = mVertices[vThrow];
	Vector3f positionKeep = mVBA.Position<Vector3f>(vKeep);
	Vector3f positionThrow = mVBA.Position<Vector3f>(vThrow);

	// 对每个会被塌陷掉的顶点，相邻接三角形进行计算
	TriangleKeySet::iterator tkIter = vertexThrow.AdjTriangles.begin();
	TriangleKeySet::iterator tkEnd = vertexThrow.AdjTriangles.end();
	for (/**/; tkIter != tkEnd; ++tkIter)
	{
		TriangleKey tKey = *tkIter;

		// 计算每个三角形所在平面的法线（按照逆时针顺序）
		int j0;
		for (j0 = 0; j0 < 3; ++j0)
		{
			if (tKey.V[j0] == vThrow)
			{
				break;
			}
		}
		int jm = (j0 + 2) % 3;
		int jp = (j0 + 1) % 3;
		Vector3f positionM = mVBA.Position<Vector3f>(tKey.V[jm]);
		Vector3f positionP = mVBA.Position<Vector3f>(tKey.V[jp]);
		Vector3f dirP = positionP - positionThrow;
		Vector3f dirM = positionM - positionThrow;
		Vector3f normalThrow = dirP.Cross(dirM);

		dirP = positionP - positionKeep;
		dirM = positionM - positionKeep;
		Vector3f normalKeep = dirP.Cross(dirM);

		// 如果两个三角形的夹角>90度，将不允许塌陷
		if (normalThrow.Dot(normalKeep) < 0.0f)
		{
			return -1;
		}
	}

	return indexThrow;
}
//----------------------------------------------------------------------------
void CreateClodMesh::Collapse (const EdgeKey& eKey, int indexThrow)
{
	// 获得要塌陷的边的顶点
	int vKeep = eKey.V[1 - indexThrow];
	int vThrow = eKey.V[indexThrow];
	Vertex& vertexThrow = mVertices[vThrow];
	CollapseInfo collapse(vKeep, vThrow);

	// Remove all the triangles sharing the throw vertex.  Create the edges
	// opposite the keep vertex for triangle insertion later.  The opposite
	// edges are saved, preserving the vertex ordering.  This information
	// makes it easier to determine which heap edges must be updated when the
	// new triangles are inserted into the graph.
	std::set<Tuple<3,int> > keepInfo;
	TriangleKeySet needRemoval = vertexThrow.AdjTriangles;
	TriangleKeySet::iterator tkIter = needRemoval.begin();
	TriangleKeySet::iterator tkEnd = needRemoval.end();
	for (/**/; tkIter != tkEnd; ++tkIter)
	{
		TriangleKey tKey = *tkIter;

		int j0;
		for (j0 = 0; j0 < 3; ++j0)
		{
			if (tKey.V[j0] == vThrow)
			{
				break;
			}
		}
		assertion(j0 < 3, "Unexpected condition.\n");

		Tuple<3,int> tuple;
		tuple[0] = tKey.V[(j0 + 1) % 3];
		tuple[1] = tKey.V[(j0 + 2) % 3];
		tuple[2] = mTriangles[tKey];

		if (tuple[0] != vKeep && tuple[1] != vKeep)
		{
			keepInfo.insert(tuple);
		}
		else
		{
			if (collapse.TThrow0 == -1)
			{
				collapse.TThrow0 = tuple[2];
			}
			else
			{
				assertion(collapse.TThrow1 == -1, "Unexpected condition.\n");
				collapse.TThrow1 = tuple[2];
				mCollapses.push_back(collapse);
			}
		}

		RemoveTriangle(tKey);
	}

	// Insert the new triangles that share the keep vertex.  Save the edges
	// that need to be updated in the heap.
	EdgeKeySet needUpdate;
	std::set<Tuple<3,int> >::iterator kpIter = keepInfo.begin();
	std::set<Tuple<3,int> >::iterator kpEnd = keepInfo.end();
	for (/**/; kpIter != kpEnd; ++kpIter)
	{
		int v0 = vKeep;
		int v1 = (*kpIter)[0];
		int v2 = (*kpIter)[1];
		Triangle t = (*kpIter)[2];
		InsertTriangle(TriangleKey(v0, v1, v2), t);
		needUpdate.insert(EdgeKey(v0, v1));
		needUpdate.insert(EdgeKey(v1, v2));
		needUpdate.insert(EdgeKey(v2, v0));
	}

	// Update the heap for those edges affected by the collapse.
	EdgeKeySet::iterator ekIter = needUpdate.begin();
	EdgeKeySet::iterator ekEnd = needUpdate.end();
	for (/**/; ekIter != ekEnd; ++ekIter)
	{
		EdgeKey updateKey = *ekIter;
		EdgeMap::iterator emIter = mEdges.find(updateKey);
		assertion(
			emIter->second.Record->GetIndex() < mHeap.GetNumElements(),
			"Unexpected condition.\n");
		mHeap.Update(emIter->second.Record, ComputeMetric(updateKey));
	}
}
//----------------------------------------------------------------------------
bool CreateClodMesh::ValidResults ()
{
	// Save the indices of the remaining triangles.  These are needed for
	// reordering of the index buffer.
	if (mNumTriangles != (int)(2*mCollapses.size() + mTriangles.size()))
	{
		assertion(false, "Incorrect triangle counts.\n");
		return false;
	}
	TriangleMap::iterator tmIter = mTriangles.begin();
	TriangleMap::iterator tmEnd = mTriangles.end();
	for (/**/; tmIter != tmEnd; ++tmIter)
	{
		mTrianglesRemaining.push_back(tmIter->second);
	}

	// Save the indices of the remaining vertices.  These are needed for
	// reordering of the vertex buffer.
	VertexArray::iterator vaIter = mVertices.begin();
	VertexArray::iterator vaEnd = mVertices.end();
	for (int i = 0; vaIter != vaEnd; ++vaIter, ++i)
	{
		bool hasEdges = (vaIter->AdjEdges.size() > 0);
		bool hasTriangles = (vaIter->AdjTriangles.size() > 0);
		if (hasEdges != hasTriangles)
		{
			assertion(false, "Inconsistent edge-triangle adjacency.\n");
			return false;
		}

		if (vaIter->AdjEdges.size() > 0)
		{
			mVerticesRemaining.push_back(i);
		}
	}
	if (mNumVertices != (int)(mCollapses.size() + mVerticesRemaining.size()))
	{
		assertion(false, "Incorrect vertex counts.\n");
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
void CreateClodMesh::ReorderBuffers ()
{
	// Construct the mappings between the old vertex order and the new vertex
	// order.
	std::vector<int> vertexNewToOld(mNumVertices);
	std::vector<int> vertexOldToNew(mNumVertices);
	int vNew = mNumVertices - 1, vOld;

	std::vector<CollapseInfo>::iterator cIter = mCollapses.begin();
	std::vector<CollapseInfo>::iterator cEnd = mCollapses.end();
	for (/**/; cIter != cEnd; ++cIter)
	{
		vOld = cIter->VThrow;
		vertexNewToOld[vNew] = vOld;
		vertexOldToNew[vOld] = vNew--;
	}

	std::vector<int>::iterator vIter = mVerticesRemaining.begin();
	std::vector<int>::iterator vEnd = mVerticesRemaining.end();
	for (/**/; vIter != vEnd; ++vIter)
	{
		vOld = *vIter;
		vertexNewToOld[vNew] = vOld;
		vertexOldToNew[vOld] = vNew--;
	}

	// Reorder the vertex buffer.
	char* oldData = mVBA.GetData();
	int stride = mVBA.GetStride();
	char* newData = new1<char>(mNumVertices*stride);
	char* trgVertex = newData;
	for (vNew = 0; vNew < mNumVertices; ++vNew, trgVertex += stride)
	{
		char* srcVertex = oldData + stride*vertexNewToOld[vNew];
		memcpy(trgVertex, srcVertex, stride);
	}
	memcpy(oldData, newData, mNumVertices*stride);
	delete1(newData);

	// Construct the mappings between the old triangle order and the new
	// triangle order.
	std::vector<int> triangleNewToOld(mNumTriangles);
	int tNew = mNumTriangles - 1, tOld;

	for (cIter = mCollapses.begin(); cIter != cEnd; ++cIter)
	{
		tOld = cIter->TThrow0;
		triangleNewToOld[tNew--] = tOld;
		tOld = cIter->TThrow1;
		triangleNewToOld[tNew--] = tOld;
	}

	std::vector<int>::iterator tIter = mTrianglesRemaining.begin();
	std::vector<int>::iterator tEnd = mTrianglesRemaining.end();
	for (/**/; tIter != tEnd; ++tIter)
	{
		tOld = *tIter;
		triangleNewToOld[tNew--] = tOld;
	}

	// Reorder the index buffer.
	int* newIndices = new1<int>(mNumIndices);
	int* trgIndices = newIndices;
	for (tNew = 0; tNew < mNumTriangles; ++tNew)
	{
		tOld = triangleNewToOld[tNew];
		int* srcIndices = mIndices + 3*tOld;
		for (int j = 0; j < 3; ++j)
		{
			*trgIndices++ = *srcIndices++;
		}
	}
	memcpy(mIndices, newIndices, mNumIndices*sizeof(int));
	delete1(newIndices);

	// Map the old indices to the new indices.
	for (int i = 0; i < mNumIndices; ++i)
	{
		mIndices[i] = vertexOldToNew[mIndices[i]];
	}

	// Map the keep and throw vertices.
	for (cIter = mCollapses.begin(); cIter != cEnd; ++cIter)
	{
		cIter->VKeep = vertexOldToNew[cIter->VKeep];
		cIter->VThrow = vertexOldToNew[cIter->VThrow];
	}
}
//----------------------------------------------------------------------------
void CreateClodMesh::ComputeRecords (int& numRecords,
									 CollapseRecord*& records)
{
	numRecords = (int)mCollapses.size() + 1;
	records = new1<CollapseRecord>(numRecords);

	// 最初的record存储着最初的顶点和三角形数量
	records[0].NumVertices = mNumVertices;
	records[0].NumTriangles = mNumTriangles;

	// 每一次塌陷顶点，会更新到新的indexbuffer，保存在indices中
	int* indices = new1<int>(mNumIndices);
	memcpy(indices, mIndices, mNumIndices*sizeof(int));
	int* vthrowIndices = new1<int>(mNumIndices);

	// 处理塌陷记录
	CollapseRecord* record = &records[1];
	int numVertices = mNumVertices;
	int numTriangles = mNumTriangles;
	std::vector<CollapseInfo>::iterator cIter = mCollapses.begin();
	std::vector<CollapseInfo>::iterator cEnd = mCollapses.end();
	for (/**/; cIter != cEnd; ++cIter, ++record)
	{
		record->VKeep = cIter->VKeep;
		record->VThrow = cIter->VThrow;

		// 一个边塌陷，失去一个顶点
		--numVertices;
		record->NumVertices = numVertices;

		// 一个边塌陷，失去两个三角形
		numTriangles -= 2;
		record->NumTriangles = numTriangles;

		// Collapse the edge and update the indices for the post-collapse
		// index buffer.
		const int numIndices = 3*numTriangles;
		record->NumIndices = 0;
		for (int i = 0; i < numIndices; ++i)
		{
			if (indices[i] == record->VThrow)
			{
				vthrowIndices[record->NumIndices++] = i;
				indices[i] = record->VKeep;
			}
		}
		if (record->NumIndices > 0)
		{
			record->Indices = new1<int>(record->NumIndices);
			size_t numBytes = record->NumIndices*sizeof(int);
			memcpy(record->Indices, vthrowIndices, numBytes);
		}
		else
		{
			record->Indices = 0;
		}
	}

	delete1(vthrowIndices);
	delete1(indices);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CreateClodMesh::Vertex
//----------------------------------------------------------------------------
CreateClodMesh::Vertex::Vertex ()
:
Collapsible(true)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CreateClodMesh::Edge
//----------------------------------------------------------------------------
CreateClodMesh::Edge::Edge ()
:
Record(0)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CreateClodMesh::CollapseInfo
//----------------------------------------------------------------------------
CreateClodMesh::CollapseInfo::CollapseInfo (int vKeep, int vThrow)
:
VKeep(vKeep),
VThrow(vThrow),
TThrow0(-1),
TThrow1(-1)
{
}
//----------------------------------------------------------------------------

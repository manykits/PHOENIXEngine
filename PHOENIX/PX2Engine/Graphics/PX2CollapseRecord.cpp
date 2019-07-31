// PX2CollapseRecord.cpp

#include "PX2CollapseRecord.hpp"
#include "PX2Memory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
CollapseRecord::CollapseRecord (int vKeep, int vThrow, int numVertices,
								int numTriangles)
								:
VKeep(vKeep),
VThrow(vThrow),
NumVertices(numVertices),
NumTriangles(numTriangles),
NumIndices(0),
Indices(0)
{
}
//----------------------------------------------------------------------------
CollapseRecord::~CollapseRecord ()
{
	delete1(Indices);
}
//----------------------------------------------------------------------------

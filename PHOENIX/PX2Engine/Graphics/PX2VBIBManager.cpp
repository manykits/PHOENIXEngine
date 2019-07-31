// PX2VBIBManager.cpp

#include "PX2VBIBManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
VBIBManager::VBIBManager()
{
}
//----------------------------------------------------------------------------
VBIBManager::~VBIBManager()
{
}
//----------------------------------------------------------------------------
void VBIBManager::Clear()
{
	mUBIBTable.clear();
}
//----------------------------------------------------------------------------
void VBIBManager::AddVBIB(const std::string &texPackFilename, VertexBuffer *vb,
	IndexBuffer *ib)
{
	VBIBObj obj;
	obj.IsValued = true;
	obj.mVB = vb;
	obj.mIB = ib;
	mUBIBTable[texPackFilename.c_str()] = obj;
}
//----------------------------------------------------------------------------
VBIBObj &VBIBManager::GetVBID(const std::string &texPackFilename)
{
	auto it = mUBIBTable.find(texPackFilename.c_str());
	if (it != mUBIBTable.end())
		return it->second;

	return mNotValied;
}
//----------------------------------------------------------------------------
// PX2LBlockSaveLoad.cpp

#include "PX2LBlock.hpp"
#include "PX2XMLData.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void LBlock::Save(const std::string &filename)
{
	XMLData data;
	data.Create();

	XMLNode lBlock = data.NewChild("LBlock");
	lBlock.SetAttributeString("name", GetName());

	data.SaveFile(filename);
}
//----------------------------------------------------------------------------
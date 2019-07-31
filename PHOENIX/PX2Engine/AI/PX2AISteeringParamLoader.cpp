// PX2AISteeringParamLoader.cpp

#include "PX2AISteeringParamLoader.hpp"
using namespace PX2;

//------------------------------------------------------------------------
ParamLoader* ParamLoader::Instance()
{
	static ParamLoader instance;
	return &instance;
}
//------------------------------------------------------------------------
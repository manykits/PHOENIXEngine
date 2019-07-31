// PX2FBXUtilities.cpp

#include "PX2FBXUtilities.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
FbxAMatrix Utilities::GetGeometryTransformation(FbxNode* inNode)
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}
//----------------------------------------------------------------------------
HMatrix Utilities::FbxMatrixToPX2(FbxAMatrix &fbxMat)
{
	HMatrix mat;

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			mat(col, row) = (float)fbxMat.Get(row, col);
		}
	}

	return mat;
}
//----------------------------------------------------------------------------
std::string Utilities::GetFileName(const std::string& inInput)
{
	std::string seperator("\\");
	unsigned int pos = inInput.find_last_of(seperator);
	if (pos != std::string::npos)
	{
		return inInput.substr(pos + 1);
	}
	else
	{
		return inInput;
	}
}
//----------------------------------------------------------------------------
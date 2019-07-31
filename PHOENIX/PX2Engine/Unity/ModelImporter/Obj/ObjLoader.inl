// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

//----------------------------------------------------------------------------
inline ObjLoader::ErrorCode ObjLoader::GetCode () const
{
    return mCode;
}
//----------------------------------------------------------------------------
inline const vector<MtlLoader::Material>& ObjLoader::GetMaterials () const
{
    return mMaterials;
}
//----------------------------------------------------------------------------
inline const vector<ObjLoader::Group>& ObjLoader::GetGroups () const
{
    return mGroups;
}
//----------------------------------------------------------------------------
inline const vector<ObjLoader::Float3>& ObjLoader::GetPositions () const
{
    return mPositions;
}
//----------------------------------------------------------------------------
inline const vector<ObjLoader::Float2>& ObjLoader::GetTCoords () const
{
    return mTCoords;
}
//----------------------------------------------------------------------------
inline const vector<ObjLoader::Float3>& ObjLoader::GetNormals () const
{
    return mNormals;
}
//----------------------------------------------------------------------------

// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

//----------------------------------------------------------------------------
inline MtlLoader::ErrorCode MtlLoader::GetCode () const
{
    return mCode;
}
//----------------------------------------------------------------------------
inline const vector<MtlLoader::Material>& MtlLoader::GetMaterials () const
{
    return mMaterials;
}
//----------------------------------------------------------------------------

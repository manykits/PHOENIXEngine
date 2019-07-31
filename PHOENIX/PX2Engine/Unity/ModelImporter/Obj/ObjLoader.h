// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "MtlLoader.h"

class ObjLoader
{
public:
    ObjLoader (const string& path, const string& filename);
    ~ObjLoader ();

    enum ErrorCode
    {
        EC_SUCCESSFUL,
        EC_LOGFILE_OPEN_FAILED,
        EC_FILE_OPEN_FAILED,
        EC_NO_TOKENS,
        EC_TOO_FEW_TOKENS,
        EC_TOO_MANY_TOKENS,
        EC_UNEXPECTED_TOKEN,
        EC_NOT_YET_IMPLEMENTED,
        EC_FAILED_TO_LOAD_MATERIALS,
        EC_FAILED_TO_FIND_MATERIAL,
        EC_INVALID_VERTEX,
        EC_MAX_ERROR_CODES
    };

    class Float2
    {
    public:
        float x, y;
    };

    class Float3
    {
    public:
        float x, y, z;
    };

    class Vertex
    {
    public:
        Vertex ()
            :
            PosIndex(-1),
            TcdIndex(-1),
            NorIndex(-1)
        {
        }

        bool operator< (const Vertex& vertex) const
        {
            if (PosIndex < vertex.PosIndex)
            {
                return true;
            }
            if (PosIndex > vertex.PosIndex)
            {
                return false;
            }
            if (TcdIndex < vertex.TcdIndex)
            {
                return true;
            }
            if (TcdIndex > vertex.TcdIndex)
            {
                return false;
            }
            if (NorIndex < vertex.NorIndex)
            {
                return true;
            }
            return false;
        }

        int PosIndex, TcdIndex, NorIndex;
    };

    class Face
    {
    public:
        vector<Vertex> Vertices;
    };

    class Mesh
    {
    public:
        int MtlIndex;
        vector<Face> Faces;
    };

    class Group
    {
    public:
        string Name;
        int PosStart, TcdStart, NorStart;
        vector<Mesh> Meshes;
    };

    inline ErrorCode GetCode () const;
    inline const vector<MtlLoader::Material>& GetMaterials () const;
    inline const vector<Group>& GetGroups () const;
    inline const vector<Float3>& GetPositions () const;
    inline const vector<Float2>& GetTCoords () const;
    inline const vector<Float3>& GetNormals () const;

private:
    void GetTokens (const string& line, vector<string>& tokens);
    bool GetMaterialLibrary (const string& path,
        const vector<string>& tokens);
    bool GetDefaultGroup (const vector<string>& tokens);
    bool GetPosition (const vector<string>& tokens);
    bool GetTCoord (const vector<string>& tokens);
    bool GetNormal (const vector<string>& tokens);
    bool GetGroup (const vector<string>& tokens);
    bool GetMaterialAndMesh (const vector<string>& tokens);
    bool GetFace (const vector<string>& tokens);

    ErrorCode mCode;
    FILE* mLogFile;
    vector<MtlLoader::Material> mMaterials;
    int mCurrentGroup, mCurrentPos, mCurrentTcd, mCurrentNor;
    int mCurrentMtl, mCurrentMesh;
    vector<Group> mGroups;
    vector<Float3> mPositions;
    vector<Float2> mTCoords;
    vector<Float3> mNormals;
    static const char* msCodeString[EC_MAX_ERROR_CODES];
};

#include "ObjLoader.inl"

#endif

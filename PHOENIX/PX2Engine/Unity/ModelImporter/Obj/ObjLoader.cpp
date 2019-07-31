// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

#include "ObjLoader.h"
#include <cassert>
#include <fstream>

// Disable Microsoft warning about unsafe functions (security).
#pragma warning(disable:4996)

//----------------------------------------------------------------------------
ObjLoader::ObjLoader (const string& path, const string& filename)
    :
    mCode(EC_SUCCESSFUL),
    mCurrentGroup(-1),
    mCurrentPos(-1),
    mCurrentTcd(-1),
    mCurrentNor(-1),
    mCurrentMtl(-1),
    mCurrentMesh(-1)
{
    mLogFile = fopen("ObjLogFile.txt", "wt");
    if (!mLogFile)
    {
        assert(false);
        mCode = EC_LOGFILE_OPEN_FAILED;
        return;
    }

    string filePath = path + filename;
    ifstream inFile(filePath.c_str());
    if (!inFile)
    {
        assert(false);
        mCode = EC_FILE_OPEN_FAILED;
        fprintf(mLogFile, "%s: %s\n", msCodeString[mCode], filePath.c_str());
        fclose(mLogFile);
        return;
    }

    string line;
    vector<string> tokens;

    while (!inFile.eof())
    {
        getline(inFile, line);

        // Skip blank lines.
        if (line == "") { continue; }

        // Skip comments.
        if (line[0] == '#') { continue; }

        GetTokens(line, tokens);
        if (tokens.size() == 0)
        {
            assert(false);
            mCode = EC_NO_TOKENS;
            break;
        }

        // mtllib
        if (GetMaterialLibrary(path, tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // g default
        if (GetDefaultGroup(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // v x y z
        if (GetPosition(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // vt x y
        if (GetTCoord(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // vn x y z
        if (GetNormal(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // Ignore smoothing groups for now (syntax:  's number').
        if (tokens[0] == "s") { continue; }

        // g groupname
        if (GetGroup(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // usemtl mtlname
        if (GetMaterialAndMesh(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // f vertexList
        if (GetFace(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        assert(false);
        mCode = EC_UNEXPECTED_TOKEN;
        break;
    }

    if (mCode != EC_SUCCESSFUL)
    {
        fprintf(mLogFile, "%s: %s\n", msCodeString[mCode], line.c_str());
    }
    else
    {
        fprintf(mLogFile, "%s\n", msCodeString[EC_SUCCESSFUL]);
    }
    fclose(mLogFile);
    inFile.close();
}
//----------------------------------------------------------------------------
ObjLoader::~ObjLoader ()
{
}
//----------------------------------------------------------------------------
void ObjLoader::GetTokens (const string & line, vector<string>& tokens)
{
    tokens.clear();

    string::size_type begin, end = 0;
    string token;

    while ((begin = line.find_first_not_of(" \t", end)) != string::npos)
    {
        end = line.find_first_of(" \t", begin);
        token = line.substr(begin, end-begin);
        tokens.push_back(token);
    }
}
//----------------------------------------------------------------------------
bool ObjLoader::GetMaterialLibrary (const string& path,
    const vector<string>& tokens)
{
    if (tokens[0] == "mtllib")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 2)
        {
            assert(false);
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        MtlLoader loader(path, tokens[1]);
        if (loader.GetCode() != MtlLoader::EC_SUCCESSFUL)
        {
            assert(false);
            mCode = EC_FAILED_TO_LOAD_MATERIALS;
            return false;
        }

        mMaterials = loader.GetMaterials();
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetDefaultGroup (const vector<string>& tokens)
{
    if (tokens[0] == "g")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 2)
        {
            assert(false);
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        if (tokens[1] == "default")
        {
            mCurrentPos = (int)mPositions.size();
            mCurrentTcd = (int)mTCoords.size();
            mCurrentNor = (int)mNormals.size();
            return true;
        }
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetPosition (const vector<string>& tokens)
{
    if (tokens[0] == "v")
    {
        if (tokens.size() < 4)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 4)
        {
            assert(false);
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        Float3 pos;
        pos.x = (float)atof(tokens[1].c_str());
        pos.y = (float)atof(tokens[2].c_str());
        pos.z = (float)atof(tokens[3].c_str());
        mPositions.push_back(pos);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetTCoord (const vector<string>& tokens)
{
    if (tokens[0] == "vt")
    {
        if (tokens.size() < 3)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 3)
        {
            // TODO.  Need to handle 3D texture coordinates.
            assert(false);
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        Float2 tcd;
        tcd.x = (float)atof(tokens[1].c_str());
        tcd.y = (float)atof(tokens[2].c_str());
        mTCoords.push_back(tcd);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetNormal (const vector<string>& tokens)
{
    if (tokens[0] == "vn")
    {
        if (tokens.size() < 4)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 4)
        {
            assert(false);
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        Float3 nor;
        nor.x = (float)atof(tokens[1].c_str());
        nor.y = (float)atof(tokens[2].c_str());
        nor.z = (float)atof(tokens[3].c_str());
        mNormals.push_back(nor);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetGroup (const vector<string>& tokens)
{
    if (tokens[0] == "g")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }

        if (tokens[1] == "default")
        {
            assert(false);
            mCode = EC_UNEXPECTED_TOKEN;
            return false;
        }

        mCurrentGroup = (int)mGroups.size();
        mGroups.push_back(Group());
        Group& group = mGroups[mCurrentGroup];
        group.Name = tokens[1];
        for (int i = 2; i < (int)tokens.size(); ++i)
        {
            group.Name += tokens[i];
        }
        group.PosStart = mCurrentPos;
        group.TcdStart = mCurrentTcd;
        group.NorStart = mCurrentNor;
        mCurrentPos = (int)mPositions.size();
        mCurrentTcd = (int)mTCoords.size();
        mCurrentNor = (int)mNormals.size();
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetMaterialAndMesh (const vector<string>& tokens)
{
    if (tokens[0] == "usemtl")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 2)
        {
            assert(false);
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        int i;
        for (i = 0; i < (int)mMaterials.size(); ++i)
        {
            if (tokens[1] == mMaterials[i].Name)
            {
                break;
            }
        }
        if (i == (int)mMaterials.size())
        {
            assert(false);
            mCode = EC_FAILED_TO_FIND_MATERIAL;
            return false;
        }
        mCurrentMtl = i;

        Group& group = mGroups[mCurrentGroup];
        for (i = 0; i < (int)group.Meshes.size(); ++i)
        {
            if (group.Meshes[i].MtlIndex == mCurrentMtl)
            {
                break;
            }
        }
        if (i == (int)group.Meshes.size())
        {
            // Mesh with this material does not yet exist.
            group.Meshes.push_back(Mesh());
            group.Meshes.back().MtlIndex = mCurrentMtl;
        }

        mCurrentMesh = i;
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetFace (const vector<string>& tokens)
{
    if (tokens[0] == "f")
    {
        if (tokens.size() < 4)
        {
            // A face must have at least three vertices.
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }

        Group& group = mGroups[mCurrentGroup];
        Mesh& mesh = group.Meshes[mCurrentMesh];
        mesh.Faces.push_back(Face());
        Face& face = mesh.Faces.back();

        // A vertex is one of the following.
        // v/vt/vn
        // v/vt/
        // v//vn
        // v//
        const int numVertices = (int)tokens.size() - 1;
        face.Vertices.resize(numVertices);
        for (int i = 0; i < numVertices; ++i)
        {
            string token = tokens[i+1];
            string::size_type slash1 = token.find_first_of("/");
            string::size_type slash2 = token.find_last_of("/");
            if (slash1 == 0 || slash1 == string::npos
            ||  slash2 == 0 || slash2 == string::npos
            ||  slash1 == slash2)
            {
                assert(false);
                mCode = EC_INVALID_VERTEX;
                return false;
            }

            string pos = token.substr(0, slash1);
            face.Vertices[i].PosIndex = atoi(pos.c_str()) - 1;
            int numDigits = slash2 - slash1 - 1;
            if (numDigits > 0)
            {
                string tcd = token.substr(slash1 + 1, numDigits);
                face.Vertices[i].TcdIndex = atoi(tcd.c_str()) - 1;
            }
            if (token.length() > slash2 + 1)
            {
                string nor = token.substr(slash2 + 1, string::npos);
                face.Vertices[i].NorIndex = atoi(nor.c_str()) - 1;
            }
        }
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------

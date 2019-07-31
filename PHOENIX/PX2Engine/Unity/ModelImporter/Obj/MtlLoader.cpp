// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

#include "MtlLoader.h"
#include <cassert>
#include <cfloat>
#include <fstream>

// Disable Microsoft warning about unsafe functions (security).
#pragma warning(disable:4996)

//----------------------------------------------------------------------------
MtlLoader::MtlLoader (const string& path, const string& filename)
    :
    mCode(EC_SUCCESSFUL),
    mCurrent(-1)
{
    mLogFile = fopen("MtlLogFile.txt", "wt");
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

        // newmtl
        if (GetNewMaterial(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // illum
        if (GetIlluminationModel(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // Ka
        if (GetAmbientColor(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // Kd
        if (GetDiffuseColor(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // Ks
        if (GetSpecularColor(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // Tf
        if (GetTransmissionFilter(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // Ni
        if (GetOpticalDensity(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // Ni
        if (GetSpecularExponent(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // map_Kd
        if (GetDiffuseTexture(tokens)) { continue; }
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
MtlLoader::~MtlLoader ()
{
}
//----------------------------------------------------------------------------
void MtlLoader::GetTokens (const string & line, vector<string>& tokens)
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
bool MtlLoader::GetColor (const vector<string>& tokens, float color[3])
{
    if (tokens[1] == "spectral" || tokens[1] == "xyz")
    {
        assert(false);
        mCode = EC_NOT_YET_IMPLEMENTED;
        return false;
    }

    // Get the red channel.
    float fvalue = (float)atof(tokens[1].c_str());
    if (fvalue < 0.0f || fvalue > 1.0f)
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }
    color[0] = fvalue;

    if (tokens.size() == 2)
    {
        // The color is gray scale.
        color[1] = fvalue;
        color[2] = fvalue;
        return true;
    }

    if (tokens.size() == 3)
    {
        assert(false);
        mCode = EC_TOO_FEW_TOKENS;
        return false;
    }
    if (tokens.size() >= 5)
    {
        assert(false);
        mCode = EC_TOO_MANY_TOKENS;
        return false;
    }

    // Get the green channel.
    fvalue = (float)atof(tokens[2].c_str());
    if (fvalue < 0.0f || fvalue > 1.0f)
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }
    color[1] = fvalue;

    // Get the blue channel.
    fvalue = (float)atof(tokens[3].c_str());
    if (fvalue < 0.0f || fvalue > 1.0f)
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }
    color[2] = fvalue;
    return true;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetTexture (const vector<string>& tokens, Texture& texture)
{
    texture.Filename = tokens.back();
    if (texture.Filename[0] == '-')
    {
        assert(false);
        mCode = EC_MISSING_MAP_FILENAME;
        return false;
    }

    if (tokens.size() >= 3)
    {
        // At least one option has been specified.
        const int imin = 1;
        const int imax = (int)tokens.size() - 2;
        for (int i = imin; i <= imax; ++i)
        {
            string token = tokens[i];
            if (token[0] != '-')
            {
                assert(false);
                mCode = EC_INVALID_OPTION;
                return false;
            }
            token = token.substr(1);
            if (token == "blendu")
            {
                if (GetBoolArg(tokens, imax, i, texture.BlendU))
                {
                    continue;
                }
                return false;
            }

            if (token == "blendv")
            {
                if (GetBoolArg(tokens, imax, i, texture.BlendV))
                {
                    continue;
                }
                return false;
            }

            if (token == "bm")
            {
                if (GetFloatArg(tokens, imax, -FLT_MAX, FLT_MAX, i,
                    texture.BumpMultiplier))
                {
                    continue;
                }
                return false;
            }

            if (token == "boost")
            {
                if (GetFloatArg(tokens, imax, 0.0f, FLT_MAX, i,
                    texture.Boost))
                {
                    continue;
                }
                return false;
            }

            if (token == "cc")
            {
                if (GetBoolArg(tokens, imax, i, texture.ColorCorrection))
                {
                    continue;
                }
                return false;
            }

            if (token == "clamp")
            {
                if (GetBoolArg(tokens, imax, i, texture.Clamp))
                {
                    continue;
                }
                return false;
            }

            if (token == "imfchan")
            {
                if (GetCharArg(tokens, imax, "rgbmlz", i, texture.IMFChannel))
                {
                    continue;
                }
                return false;
            }

            if (token == "mm")
            {
                if (GetFloatArg2(tokens, imax, -FLT_MAX, FLT_MAX, -FLT_MAX,
                    FLT_MAX, i, texture.Base, texture.Gain))
                {
                    continue;
                }
                return false;
            }

            if (token == "o")
            {
                if (GetFloatArg3Opt2(tokens, imax, -FLT_MAX, FLT_MAX,
                    -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX, i,
                    texture.Offset))
                {
                    continue;
                }
                return false;
            }

            if (token == "s")
            {
                if (GetFloatArg3Opt2(tokens, imax, FLT_MIN, FLT_MAX,
                    FLT_MIN, FLT_MAX, FLT_MIN, FLT_MAX, i, texture.Scale))
                {
                    continue;
                }
                return false;
            }

            if (token == "t")
            {
                if (GetFloatArg3Opt2(tokens, imax, -FLT_MAX, FLT_MAX,
                    -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX, i,
                    texture.Turbulence))
                {
                    continue;
                }
                return false;
            }

            if (token == "texres")
            {
                if (GetFloatArg(tokens, imax, 0.0f, FLT_MAX, i,
                    texture.TexResolution))
                {
                    continue;
                }
                return false;
            }
        }
    }
    return true;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetBoolArg (const vector<string>& tokens, int imax, int& i,
    bool& value)
{
    if (++i > imax)
    {
        assert(false);
        mCode = EC_TOO_FEW_OPTION_TOKENS;
        return false;
    }

    if (tokens[i] == "on")
    {
        value = true;
        return true;
    }

    if (tokens[i] == "off")
    {
        value = false;
        return true;
    }

    assert(false);
    mCode = EC_INVALID_OPTION_ARGUMENT;
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetCharArg (const vector<string>& tokens, int imax,
    const char* valid, int& i, char& value)
{
    if (++i > imax)
    {
        assert(false);
        mCode = EC_TOO_FEW_OPTION_TOKENS;
        return false;
    }

    if (tokens[i].length() != 0)
    {
        assert(false);
        mCode = EC_INVALID_OPTION_ARGUMENT;
        return false;
    }

    value = tokens[i][0];
    bool isValid = false;
    for (int j = 0; j < (int)strlen(valid); ++j)
    {
        if (value == valid[j])
        {
            isValid = true;
            break;
        }
    }

    if (!isValid)
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }
    return true;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetFloatArg (const vector<string>& tokens, int imax,
    float vmin, float vmax, int& i, float& value)
{
    if (++i > imax)
    {
        assert(false);
        mCode = EC_TOO_FEW_OPTION_TOKENS;
        return false;
    }
    value = (float)atof(tokens[i].c_str());
    if ((vmin > -FLT_MAX && value < vmin) || (vmax < FLT_MAX && value > vmax))
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }
    return true;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetFloatArg2 (const vector<string>& tokens, int imax,
    float vmin0, float vmax0, float vmin1, float vmax1, int& i, float& value0,
    float& value1)
{
    if (++i > imax)
    {
        assert(false);
        mCode = EC_TOO_FEW_OPTION_TOKENS;
        return false;
    }

    value0 = (float)atof(tokens[i].c_str());
    if ((vmin0 > -FLT_MAX && value0 < vmin0)
    ||  (vmax0 <  FLT_MAX && value0 > vmax0))
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }

    if (++i > imax)
    {
        assert(false);
        mCode = EC_TOO_FEW_OPTION_TOKENS;
        return false;
    }
    value1 = (float)atof(tokens[i].c_str());
    if ((vmin1 > -FLT_MAX && value1 < vmin1)
    ||  (vmax1 <  FLT_MAX && value1 > vmax1))
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }
    return true;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetFloatArg3Opt2 (const vector<string>& tokens, int imax,
    float vmin0, float vmax0, float vmin1, float vmax1, float vmin2,
    float vmax2, int& i, float values[3])
{
    if (++i > imax)
    {
        assert(false);
        mCode = EC_TOO_FEW_OPTION_TOKENS;
        return false;
    }

    // Get required argument.
    values[0] = (float)atof(tokens[i].c_str());
    if ((vmin0 > -FLT_MAX && values[0] < vmin0)
    ||  (vmax0 <  FLT_MAX && values[0] > vmax0))
    {
        assert(false);
        mCode = EC_VALUE_OUT_OF_RANGE;
        return false;
    }

    // The last two arguments are optional.  We need logic here to avoid
    // confusing the next option or filename as an argument of the current
    // option.  Some problem statements:
    //   -option 0.2 filename  (Need to know 0.2 is next-to-last token.)
    //   -option -nextoption (Is -nextoption a number or the next option?)
    //   -option 0.1 -0.2 (Is -0.2 a number or the next option?)

    for (int j = 1; j <= 2; ++j)
    {
        if (++i > imax)
        {
            // The next token must be the filename.  No optional arguments to
            // process.
            return true;
        }

        // There might be v and/or w arguments.
        string token = tokens[i];
        if (token[0] == '-')
        {
            // Distinguish between a minus sign and the start of an option.
            if (token.length() == 1)
            {
                assert(false);
                mCode = EC_UNEXPECTED_TOKEN;
                return false;
            }

            string::size_type begin = token.find_first_of("0123456789.", 1);
            if (begin != 0)
            {
                // The token is the next option.
                --i;
                return true;
            }

            token = token.substr(1);
        }

        // The token is a number.
        values[j] = (float)atof(token.c_str());
        if ((vmin1 > -FLT_MAX && values[j] < vmin1)
        ||  (vmax1 <  FLT_MAX && values[j] > vmax1))
        {
            assert(false);
            mCode = EC_VALUE_OUT_OF_RANGE;
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetNewMaterial (const vector<string>& tokens)
{
    // Syntax
    //
    // newmtl name

    if (tokens[0] == "newmtl")
    {
        mCurrent = (int)mMaterials.size();
        mMaterials.push_back(Material());

        if (tokens.size() > 1)
        {
            mMaterials[mCurrent].Name = tokens[1];
            return true;
        }

        assert(false);
        mCode = EC_TOO_FEW_TOKENS;
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetIlluminationModel (const vector<string>& tokens)
{
    // Syntax
    //
    // illum modelNumber

    if (tokens[0] == "illum")
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

        int illum = atoi(tokens[1].c_str());
        if ((illum == 0 && tokens[1][0] != '0') || illum > 10)
        {
            assert(false);
            mCode = EC_VALUE_OUT_OF_RANGE;
            return false;
        }
        mMaterials[mCurrent].IlluminationModel = illum;
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetAmbientColor (const vector<string>& tokens)
{
    // Syntax (only the first is implemented).
    //
    // Ka r [g] [b]
    // Ka spectral file.rfl [factor]
    // Ka xyz x [y] [z]

    if (tokens[0] == "Ka")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        return GetColor(tokens, mMaterials[mCurrent].AmbientColor);
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetDiffuseColor (const vector<string>& tokens)
{
    // Syntax (only the first is implemented).
    //
    // Kd r [g] [b]
    // Kd spectral file.rfl [factor]
    // Kd xyz x [y] [z]

    if (tokens[0] == "Kd")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        return GetColor(tokens, mMaterials[mCurrent].DiffuseColor);
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetSpecularColor (const vector<string>& tokens)
{
    // Syntax (only the first is implemented).
    //
    // Ks r [g] [b]
    // Ks spectral file.rfl [factor]
    // Ks xyz x [y] [z]

    if (tokens[0] == "Ks")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        return GetColor(tokens, mMaterials[mCurrent].SpecularColor);
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetTransmissionFilter (const vector<string>& tokens)
{
    // Syntax (only the first is implemented).
    //
    // Tf r [g] [b]
    // Tf spectral file.rfl [factor]
    // Tf xyz x [y] [z]

    if (tokens[0] == "Tf")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        return GetColor(tokens, mMaterials[mCurrent].TransmissionFilter);
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetOpticalDensity (const vector<string>& tokens)
{
    // Syntax
    //
    // Ni opticalDensity

    if (tokens[0] == "Ni")
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

        float density = (float)atof(tokens[1].c_str());
        if (density < 0.001f || density > 10.0f)
        {
            assert(false);
            mCode = EC_VALUE_OUT_OF_RANGE;
            return false;
        }
        mMaterials[mCurrent].OpticalDensity = density;
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetSpecularExponent (const vector<string>& tokens)
{
    // Syntax
    //
    // Ns exponent

    if (tokens[0] == "Ns")
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

        float exponent = (float)atof(tokens[1].c_str());
        if (exponent < 0.0f || exponent > 1000.0f)
        {
            assert(false);
            mCode = EC_VALUE_OUT_OF_RANGE;
            return false;
        }
        mMaterials[mCurrent].SpecularExponent = exponent;
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetAmbientTexture (const vector<string>& tokens)
{
    // Syntax
    //
    // map_Ka [options] filename
    //
    // The options are
 	//    -blendu on | off
 	//    -blendv on | off
 	//    -cc on | off
 	//    -clamp on | off
 	//    -mm base gain
 	//    -o u v w
 	//    -s u v w
 	//    -t u v w
 	//    -texres value

    if (tokens[0] == "map_Ka")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        return GetTexture(tokens, mMaterials[mCurrent].AmbientMap);
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetDiffuseTexture (const vector<string>& tokens)
{
    // Syntax
    //
    // map_Kd [options] filename
    //
    // The options are
 	//    -blendu on | off
 	//    -blendv on | off
 	//    -cc on | off
 	//    -clamp on | off
 	//    -mm base gain
 	//    -o u v w
 	//    -s u v w
 	//    -t u v w
 	//    -texres value

    if (tokens[0] == "map_Kd")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        return GetTexture(tokens, mMaterials[mCurrent].DiffuseMap);
    }
    return false;
}
//----------------------------------------------------------------------------
bool MtlLoader::GetSpecularTexture (const vector<string>& tokens)
{
    // Syntax
    //
    // map_Kd [options] filename
    //
    // The options are
 	//    -blendu on | off
 	//    -blendv on | off
 	//    -cc on | off
 	//    -clamp on | off
 	//    -mm base gain
 	//    -o u v w
 	//    -s u v w
 	//    -t u v w
 	//    -texres value

    if (tokens[0] == "map_Ks")
    {
        if (tokens.size() == 1)
        {
            assert(false);
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        return GetTexture(tokens, mMaterials[mCurrent].SpecularMap);
    }
    return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// MtlLoader::Texture
//----------------------------------------------------------------------------
MtlLoader::Texture::Texture ()
    :
    Filename(""),
    BlendU(true),
    BlendV(true),
    BumpMultiplier(1.0f),
    Boost(0.0f),
    ColorCorrection(false),
    Clamp(false),
    IMFChannel('m'),
    Base(0.0f),
    Gain(1.0f),
    TexResolution(0)
{
    Offset[0] = 0.0f;
    Offset[1] = 0.0f;
    Offset[2] = 0.0f;
    Scale[0] = 1.0f;
    Scale[1] = 1.0f;
    Scale[2] = 1.0f;
    Turbulence[0] = 0.0f;
    Turbulence[1] = 0.0f;
    Turbulence[2] = 0.0f;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// MtlLoader::Material
//----------------------------------------------------------------------------
MtlLoader::Material::Material ()
    :
    Name(""),
    IlluminationModel(-1),
    OpticalDensity(0.0f),
    SpecularExponent(0.0f)
{
    for (int i = 0; i < 3; ++i)
    {
        AmbientColor[i] = 0.0f;
        DiffuseColor[i] = 0.0f;
        SpecularColor[i] = 0.0f;
        TransmissionFilter[i] = 0.0f;
    }
}
//----------------------------------------------------------------------------

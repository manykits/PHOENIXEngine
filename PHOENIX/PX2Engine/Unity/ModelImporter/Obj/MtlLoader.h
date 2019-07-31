// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

#ifndef MTLLOADER_H
#define MTLLOADER_H

#include <cstdio>
#include <string>
#include <vector>
using namespace std;

class MtlLoader
{
public:
    MtlLoader (const string& path, const string& filename);
    ~MtlLoader ();

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
        EC_VALUE_OUT_OF_RANGE,
        EC_MISSING_MAP_FILENAME,
        EC_INVALID_OPTION,
        EC_TOO_FEW_OPTION_TOKENS,
        EC_INVALID_OPTION_ARGUMENT,
        EC_MAX_ERROR_CODES
    };

    class Texture
    {
    public:
        Texture ();

        string Filename;
        bool BlendU;            // -blendv          on|off
        bool BlendV;            // -blendu          on|off
        float BumpMultiplier;   // -bm value        any
        float Boost;            // -boost value     nonneg
        bool ColorCorrection;   // -cc              on|off
        bool Clamp;             // -clamp           on|off
        char IMFChannel;        // -imfchan         r|g|b|m|l|z
        float Base, Gain;       // -mm base gain    any, any
        float Offset[3];        // -o u [v] [w]     any, any, any
        float Scale[3];         // -s u [v] [w]     pos, pos, pos
        float Turbulence[3];    // -t u [v] [w]     nonneg, nonneg, nonneg
        float TexResolution;    // -texres value    [What is this?]
    };

    class Material
    {
    public:
        Material ();

        string Name;
        int IlluminationModel;          // illum
        float AmbientColor[3];          // Ka
        float DiffuseColor[3];          // Kd
        float SpecularColor[3];         // Ks
        float TransmissionFilter[3];    // Tf
        float OpticalDensity;           // Ni
        float SpecularExponent;         // Ns
        Texture AmbientMap;             // map_Ka
        Texture DiffuseMap;             // map_Kd
        Texture SpecularMap;            // map_Ks
    };

    inline ErrorCode GetCode () const;
    inline const vector<Material>& GetMaterials () const;

private:
    void GetTokens (const string& line, vector<string>& tokens);
    bool GetColor (const vector<string>& tokens, float color[3]);
    bool GetTexture (const vector<string>& tokens, Texture& texture);
    bool GetBoolArg (const vector<string>& tokens, int imax, int& i,
        bool& value);
    bool GetCharArg (const vector<string>& tokens, int imax,
        const char* valid, int& i, char& value);
    bool GetFloatArg (const vector<string>& tokens, int imax, float vmin,
        float vmax, int& i, float& value);
    bool GetFloatArg2 (const vector<string>& tokens, int imax, float vmin0,
        float vmax0, float vmin1, float vmax1, int& i, float& value0,
        float& value1);
    bool GetFloatArg3Opt2 (const vector<string>& tokens, int imax,
        float vmin0, float vmax0, float vmin1, float vmax1, float vmin2,
        float vmax2, int& i, float values[3]);
    bool GetNewMaterial (const vector<string>& tokens);
    bool GetIlluminationModel (const vector<string>& tokens);
    bool GetAmbientColor (const vector<string>& tokens);
    bool GetDiffuseColor (const vector<string>& tokens);
    bool GetSpecularColor (const vector<string>& tokens);
    bool GetTransmissionFilter (const vector<string>& tokens);
    bool GetOpticalDensity (const vector<string>& tokens);
    bool GetSpecularExponent (const vector<string>& tokens);
    bool GetAmbientTexture (const vector<string>& tokens);
    bool GetDiffuseTexture (const vector<string>& tokens);
    bool GetSpecularTexture (const vector<string>& tokens);

    ErrorCode mCode;
    FILE* mLogFile;
    int mCurrent;
    vector<Material> mMaterials;
    static const char* msCodeString[EC_MAX_ERROR_CODES];
};

#include "MtlLoader.inl"

#endif

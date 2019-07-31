// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

#include "ObjLoader.h"

const char* ObjLoader::msCodeString[EC_MAX_ERROR_CODES] =
{
    "Loaded successfully",          // EC_SUCCESSFUL
    "Logfile open failed",          // EC_LOGFILE_OPEN_FAILED
    "File open failed",             // EC_FILE_OPEN_FAILED
    "No tokens",                    // EC_NO_TOKENS
    "Too few tokens",               // EC_TOO_FEW_TOKENS
    "Too many tokens",              // EC_TOO_MANY_TOKENS
    "Unexpected token",             // EC_UNEXPECTED_TOKEN
    "Not yet implemented",          // EC_NOT_YET_IMPLEMENTED
    "Failed to load materials",     // EC_FAILED_TO_LOAD_MATERIALS
    "Failed to find material",      // EC_FAILED_TO_FIND_MATERIAL
    "Invalid vertex"                // EC_INVALID_VERTEX
};

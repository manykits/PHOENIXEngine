// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

#include "MtlLoader.h"

const char* MtlLoader::msCodeString[EC_MAX_ERROR_CODES] =
{
    "Loaded successfully",          // EC_SUCCESSFUL
    "Logfile open failed",          // EC_LOGFILE_OPEN_FAILED
    "File open failed",             // EC_FILE_OPEN_FAILED
    "No tokens",                    // EC_NO_TOKENS
    "Too few tokens",               // EC_TOO_FEW_TOKENS
    "Too many tokens",              // EC_TOO_MANY_TOKENS
    "Unexpected token",             // EC_UNEXPECTED_TOKEN
    "Not yet implemented",          // EC_NOT_YET_IMPLEMENTED
    "Value out of range",           // EC_VALUE_OUT_OF_RANGE
    "Missing map filename",         // EC_MISSING_MAP_FILENAME
    "Invalid option",               // EC_INVALID_OPTION
    "Too few option tokens",        // EC_TOO_FEW_OPTION_TOKENS
    "Invalid option argument"       // EC_INVALID_OPTION_ARGUMENT
};

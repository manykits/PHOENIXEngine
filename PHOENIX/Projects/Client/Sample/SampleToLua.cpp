/*
** Lua binding: Sample
** Generated automatically by tolua++-1.0.92 on 12/07/17 11:21:05.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Sample_open (lua_State* tolua_S);

#include "SampleToLua.hpp"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* Open function */
TOLUA_API int tolua_Sample_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
  int luaopen_Sample (lua_State* tolua_S) {
 return tolua_Sample_open(tolua_S);
};
#endif


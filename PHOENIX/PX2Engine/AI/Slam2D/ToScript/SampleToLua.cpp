/*
** Lua binding: Sample
** Generated automatically by tolua++-1.0.92 on 04/30/19 21:34:36.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Sample_open (lua_State* tolua_S);

#include "SampleToLua.hpp"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_RobotExt (lua_State* tolua_S)
{
 RobotExt* self = (RobotExt*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Robot");
 tolua_usertype(tolua_S,"RobotExt");
}

/* method: new of class  RobotExt */
#ifndef TOLUA_DISABLE_tolua_Sample_RobotExt_new00
static int tolua_Sample_RobotExt_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RobotExt",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RobotExt* tolua_ret = (RobotExt*)  Mtolua_new((RobotExt)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RobotExt");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  RobotExt */
#ifndef TOLUA_DISABLE_tolua_Sample_RobotExt_new00_local
static int tolua_Sample_RobotExt_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RobotExt",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RobotExt* tolua_ret = (RobotExt*)  Mtolua_new((RobotExt)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RobotExt");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  RobotExt */
#ifndef TOLUA_DISABLE_tolua_Sample_RobotExt_delete00
static int tolua_Sample_RobotExt_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RobotExt",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RobotExt* self = (RobotExt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Update of class  RobotExt */
#ifndef TOLUA_DISABLE_tolua_Sample_RobotExt_Update00
static int tolua_Sample_RobotExt_Update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RobotExt",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RobotExt* self = (RobotExt*)  tolua_tousertype(tolua_S,1,0);
  float appseconds = ((float)  tolua_tonumber(tolua_S,2,0));
  float elpasedSeconds = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Update'", NULL);
#endif
  {
   self->Update(appseconds,elpasedSeconds);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: New of class  RobotExt */
#ifndef TOLUA_DISABLE_tolua_Sample_RobotExt_New00
static int tolua_Sample_RobotExt_New00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RobotExt",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RobotExt* tolua_ret = (RobotExt*)  RobotExt::New();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RobotExt");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'New'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Delete of class  RobotExt */
#ifndef TOLUA_DISABLE_tolua_Sample_RobotExt_Delete00
static int tolua_Sample_RobotExt_Delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RobotExt",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"RobotExt",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RobotExt* robot = ((RobotExt*)  tolua_tousertype(tolua_S,2,0));
  {
   RobotExt::Delete(robot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Sample_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"RobotExt","RobotExt","Robot",tolua_collect_RobotExt);
  #else
  tolua_cclass(tolua_S,"RobotExt","RobotExt","Robot",NULL);
  #endif
  tolua_beginmodule(tolua_S,"RobotExt");
   tolua_function(tolua_S,"new",tolua_Sample_RobotExt_new00);
   tolua_function(tolua_S,"new_local",tolua_Sample_RobotExt_new00_local);
   tolua_function(tolua_S,".call",tolua_Sample_RobotExt_new00_local);
   tolua_function(tolua_S,"delete",tolua_Sample_RobotExt_delete00);
   tolua_function(tolua_S,"Update",tolua_Sample_RobotExt_Update00);
   tolua_function(tolua_S,"New",tolua_Sample_RobotExt_New00);
   tolua_function(tolua_S,"Delete",tolua_Sample_RobotExt_Delete00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Sample (lua_State* tolua_S) {
 return tolua_Sample_open(tolua_S);
};
#endif


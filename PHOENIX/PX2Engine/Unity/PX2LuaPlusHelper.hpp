//
/*

#include "luaplus.h"
#include <string>

class LuaConvert
{
public:
	LuaConvert(LuaObject& obj)
		: refobj(obj)
	{
	}
	operator int()
	{
		return refobj.GetInteger();
	}
	operator float()
	{
		return refobj.GetFloat();
	}
	operator double()
	{
		return refobj.GetDouble();
	}
	operator const char* ()
	{
		return refobj.GetString();
	}
	operator const wchar_t* ()
	{
		return refobj.GetWString();
	}
	operator std::string()
	{
		return std::string(refobj.GetString());
	}
	operator std::wstring()
	{
		return std::wstring(refobj.GetWString());
	}
	operator void* ()
	{
		return refobj.GetUserData();
	}
	template<typename T>
	operator T* ()
	{
		return (T*)refobj.GetUserData();
	}
	template<typename R>
	operator LuaFunction<R>()
	{
		return LuaFunction<R>(refobj);
	}
private:
	LuaObject refobj;
};

template<typename Object>
class LuaConstructor
{
private:

	static	int ConstructorHelper(LuaState* state, Object* pObj)
	{
		std::string metaname("MetaClass_");
		metaname += typeid(Object).raw_name();

		LuaObject obj = state->BoxPointer(pObj);
		obj.SetMetaTable(state->GetGlobal(metaname.c_str()));
		obj.PushStack();
		return 1;
	}

public:

	static	int Constructor(LuaState* state)
	{
		return ConstructorHelper(state, new Object());
	}

	template<typename A1>
	static	int Constructor(LuaState* state)
	{
		LuaConvert a1 = LuaObject(state, 1);
		return ConstructorHelper(state, new Object((A1)a1));
	}

	template<typename A1, typename A2>
	static	int Constructor(LuaState* state)
	{
		LuaConvert a1 = LuaObject(state, 1);
		LuaConvert a2 = LuaObject(state, 2);
		return ConstructorHelper(state, new Object((A1)a1, (A2)a2));
	}

	template<typename A1, typename A2, typename A3>
	static	int Constructor(LuaState* state)
	{
		LuaConvert a1 = LuaObject(state, 1);
		LuaConvert a2 = LuaObject(state, 2);
		LuaConvert a3 = LuaObject(state, 3);
		return ConstructorHelper(state, new Object((A1)a1, (A2)a2, (A3)a3));
	}

	template<typename A1, typename A2, typename A3, typename A4>
	static	int Constructor(LuaState* state)
	{
		LuaConvert a1 = LuaObject(state, 1);
		LuaConvert a2 = LuaObject(state, 2);
		LuaConvert a3 = LuaObject(state, 3);
		LuaConvert a4 = LuaObject(state, 4);
		return ConstructorHelper(state, new Object((A1)a1, (A2)a2, (A3)a3, (A4)a4));
	}

	template<typename A1, typename A2, typename A3, typename A4, typename A5>
	static	int Constructor(LuaState* state)
	{
		LuaConvert a1 = LuaObject(state, 1);
		LuaConvert a2 = LuaObject(state, 2);
		LuaConvert a3 = LuaObject(state, 3);
		LuaConvert a4 = LuaObject(state, 4);
		LuaConvert a5 = LuaObject(state, 5);
		return ConstructorHelper(state, new Object((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5));
	}

	static int Destructor(LuaState* state)
	{
		LuaObject o(state, 1);

		delete (Object*)state->UnBoxPointer(1);

		LuaObject meta = state->GetGlobal("MetaClass_Nil");
		if (meta.IsNil())
		{
			meta = state->GetGlobals().CreateTable("MetaClass_Nil");
		}

		o.SetMetaTable(meta);
		return 0;
	}
};

template<typename Object>
class LuaClass
{
public:
	LuaClass(LuaState* state)
	{
		luaGlobals = state->GetGlobals();


		std::string metaname("MetaClass_");
		metaname += typeid(Object).raw_name();

		metaTableObj = luaGlobals.CreateTable(metaname.c_str());
		metaTableObj.SetObject("__index", metaTableObj);
		//metaTableObj.Register("__gc", &Destructor);
		//metaTableObj.Register("Free", &Destructor);
	}

	template<typename Func>
	inline LuaClass& def(const char* name, Func func)
	{
		metaTableObj.RegisterObjectDirect(name, (Object*)0, func);
		return *this;
	}

	inline LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, LuaConstructor<Object>::Constructor);
		return *this;
	}

	template<typename A1>
	inline LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, LuaConstructor<Object>::Constructor<A1>);
		return *this;
	}
	template<typename A1, typename A2>
	inline LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, LuaConstructor<Object>::Constructor<A1, A2>);
		return *this;
	}
	template<typename A1, typename A2, typename A3>
	inline LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, LuaConstructor<Object>::Constructor<A1, A2, A3>);
		return *this;
	}
	template<typename A1, typename A2, typename A3, typename A4>
	inline LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, LuaConstructor<Object>::Constructor<A1, A2, A3, A4>);
		return *this;
	}
	template<typename A1, typename A2, typename A3, typename A4, typename A5>
	inline LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, LuaConstructor<Object>::Constructor<A1, A2, A3, A4, A5>);
		return *this;
	}
	inline LuaClass& destroy(const char* name)
	{
		metaTableObj.Register(name, LuaConstructor<Object>::Destructor);
		return *this;
	}

private:
	LuaObject metaTableObj;
	LuaObject luaGlobals;
};

class LuaModule
{
public:
	LuaModule(LuaState* state)
	{
		luaModuleObj = state->GetGlobals();
	}

	LuaModule(LuaState* state, const char* name)
	{
		luaModuleObj = state->GetGlobals().CreateTable(name);
	}

	template<typename Func>
	inline LuaModule& def(const char* name, Func func)
	{
		luaModuleObj.RegisterDirect(name, func);
		return *this;
	}

	template<typename Object, typename Func>
	inline LuaModule& def(const char* name, Object& o, Func func)
	{
		luaModuleObj.RegisterDirect(name, o, func);
		return *this;
	}

private:
	LuaObject luaModuleObj;
};
*/
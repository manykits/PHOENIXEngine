// PX2Object.hpp

#ifndef PX2OBJECT_HPP
#define PX2OBJECT_HPP

#include "PX2CorePre.hpp"
#include "PX2NameID.hpp"
#include "PX2Property.hpp"
#include "PX2Rtti.hpp"
#include "PX2InStream.hpp"
#include "PX2OutStream.hpp"
#include "PX2Any.hpp"
#include "PX2Visitor.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2FString.hpp"

namespace PX2
{

	class Event;
	class ObjectEventHandler;
	class FunObject;

	/// 对象系统根类
	/** 
	* 对系统提供：运行识别机制，控制器系统，拷贝系统，名称ID系统,以及硬盘持久化
	* 系统支持。
	*/
	class PX2_ENGINE_ITEM Object
	{
		// Enable Disable
	public:
		virtual void Enable(bool enable);
		bool IsEnable() const;

		virtual void SetActivate(bool act);
		bool IsActivated() const;

	protected:
		bool mIsEnable;
		bool mIsActivated;

		// 运行识别信息
	public:
		virtual const Rtti& GetRttiType () const;
		virtual unsigned short GetRttiVersion () const;
		bool IsExactly (const Rtti& type) const;
		bool IsDerived (const Rtti& type) const;
		bool IsExactlyTypeOf (const Object* object) const;
		bool IsDerivedTypeOf (const Object* object) const;
		static const Rtti TYPE;

		// 用户数据
	public:
		void SetUserDataFloat (const std::string &name, float val);
		float GetUserDataFloat (const std::string &name, bool *isValied=0) const;

		void SetUserDataString(const std::string &name, const std::string &val);
		std::string GetUserDataString(const std::string &name, bool *isValied = 0) const;

		void SetUserData (const std::string &name, const Any &any);
		Any GetUserDataAny (const std::string &name, bool *isValied=0) const;
		template <typename T>
		T GetUserData (const std::string &name, bool *isValued=0) const;

		void SetUserDataPointer(const std::string &name, void *p);
		void *GetUserDataPointer(const std::string &name, bool *isValied=0) const;

	private:
		std::map<std::string, float> mUserDataFloats;
		std::map<std::string, std::string> mUserDataStrings;
		std::map<std::string, Any> mUserDatas;
		std::map<std::string, void*> mUserDataPointers;

		// 资源路径
	public:
		void SetResourcePath (const std::string& name);
		const std::string& GetResourcePath () const;
		const FString& GetResourcePathFString() const;

		Stream *mCurStream; //当前用于序列化的

	private:
		std::string mResourcePath;
		FString mResourcePathFString;

		// 名称
	public:
		virtual void SetName(const std::string& name);
		const std::string& GetName() const;
		const FString &GetNameFString() const;
		virtual Object* GetObjectByName(const std::string& name);
		virtual void GetAllObjectsByName(const std::string& name,
			std::vector<Object*>& objects);

		template <typename T>
		T *GetObjectByNameCast (const std::string &name);

	private:
		std::string mName;
		FString mNameFString;

		// ID
	public:
		virtual void SetID(int id);
		int GetID() const;
		virtual Object* GetObjectByID(int id);
		virtual void GetAllObjectsByID(int id,
			std::vector<Object*>& objects);

	private:
		int mID;

		// 事件
	public:
		void ComeInEventWorld();
		void GoOutEventWorld();
		bool IsInEventWorld() const;

		ObjectEventHandler *GetEventHandler();

		virtual void OnEvent(Event *event);

	protected:
		ObjectEventHandler *mEventHandler;

		// Visitor
	public:
		PX2_VISITOR;

		// Script
	public:
		void SetUserScriptName(const std::string &scriptName);
		const std::string &GetUserScriptName() const;
		const std::string &GetScriptName() const;

		virtual bool RegistToScriptSystem();
		virtual bool UnRegistToScriptSystem();
		bool IsRegistedToScriptSystem();

	protected:
		std::string mScriptName;
		std::string mUserScriptName;

		// 属性
		// 编辑器中使用
	public:
		enum PropertyType
		{
			PT_INT,
			PT_INTSLIDER,
			PT_FLOAT,
			PT_FLOATSLIDER,
			PT_BOOL,
			PT_BOOLCHECK,
			PT_COLOR3FLOAT3,
			PT_FLOAT2,
			PT_FLOAT3,
			PT_FLOAT4,
			PT_APOINT3,
			PT_APOINT4,
			PT_AVECTOR3,
			PT_AVECTOR4,
			PT_SIZE,
			PT_RECT,
			PT_STRING,
			PT_STRINGBUTTON,
			PT_ENUM,
			PT_TRANSFORM,
			PT_EMFLOAT,
			PT_EMFLOATRANGE,
			PT_EMFLOAT3,
			PT_EMFLOAT3RANGE,
			PT_EMFLOAT3COLOR,
			PT_EMFLOAT3COLORRANGE,
			PT_CLASS,
			PT_MAX_TYPE
		};

		struct PX2_ENGINE_ITEM PropertyObject
		{
			PropertyObject ();
			~PropertyObject ();

			std::string Tag;
			std::string Name;
			PropertyType Type;
			bool Enable;
			Any Data;
			Any Data1; // 如果是枚举,将枚举选项放在这里
			float SliderMin;
			float SliderMax;

			PropertyObject *GetPropertyByTag (const std::string &tag);
			PropertyObject *GetPropertyByName (const std::string &name);

			std::vector<PropertyObject> SubObjects;
		};

		virtual void RegistProperties ();
		virtual void OnPropertyChanged (const PropertyObject &obj);
		void SetNamePropChangeable (bool isNamePropChangeable);
		bool IsNamePropChangeable () const;

		std::string AddProperty (const std::string &name, PropertyType type,
			const Any &data, bool enable=true);
		std::string AddPropertyClass (const std::string &name);
		std::string AddProperty (PropertyObject &obj);
		std::string AddPropertyFloatSlider(const std::string &name, float val,
			float min, float max, bool enable=true);
		std::string AddPropertyEnum (const std::string &name, int val,
			const std::vector<std::string> &enums, bool enable=true);
		void RemoveAllProperties ();

		std::string AddPropertySub (const std::string &parTag, const std::string &name,
			PropertyType type, const Any &data, bool enable=true);
		std::string AddPropertyEnumSub (const std::string &parTag, const std::string &name,
			int val, const std::vector<std::string> &enums, bool enable=true);

		const std::vector<PropertyObject> &GetProperties () const;
		std::vector<PropertyObject> &GetProperties ();

		PropertyObject *GetPropertyByName (const std::string &name);
		PropertyObject *GetPropertyByTag (const std::string &tag);

	protected:
		std::vector<PropertyObject> mProperties;
	
	private:
		std::string GenPropertyTag ();
		static int msNumProperties;
		bool mIsNameChangeable;

		// 持久化
	public:
		typedef Object* (*FactoryFunction)(InStream& stream);
		typedef std::map<std::string, FactoryFunction> FactoryMap;
		static bool RegisterFactory ();
		static void InitializeFactory ();
		static void TerminateFactory ();
		static FactoryFunction Find (const std::string& name);
		static Object* Factory (InStream& source);
		virtual void Load (InStream& source);
		virtual void Link (InStream& source);
		virtual void PostLink ();
		virtual bool Register (OutStream& target) const;
		virtual void Save (OutStream& target) const;
		virtual int GetStreamingSize (Stream &stream) const;
		Object* Copy (const std::string& uniqueNameAppend);

		unsigned short GetReadedVersion () const;

	private:
		static bool msIsIOWriting;
		unsigned short mReadedVersion;

	protected:
		// Constructor used by the loading system.
		enum LoadConstructor { LC_LOADER };
		Object (LoadConstructor value);
		static FactoryMap* msFactories;
	private:
		static bool msStreamRegistered;

		// 抽象基类
	protected:
		Object ();
	public:
		virtual ~Object ();

	public:
		static Object *New();
		static Object *New(const std::string &name);
	};

	static bool gsStreamRegistered_Object = Object::RegisterFactory();
	typedef Pointer0<Object> ObjectPtr;
#include "PX2Object.inl"

}

#endif

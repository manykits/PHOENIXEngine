// PX2Object.inl

//----------------------------------------------------------------------------
// ÀàÐÍ×ª»»
//----------------------------------------------------------------------------
template <class T>
T* StaticCast (Object* object)
{
	return (T*)object;
}
//----------------------------------------------------------------------------
template <class T>
const T* StaticCast (const Object* object)
{
	return (const T*)object;
}
//----------------------------------------------------------------------------
template <class T>
T* DynamicCast (Object* object)
{
	return object && object->IsDerived(T::TYPE) ? (T*)object : 0;
}
//----------------------------------------------------------------------------
template <class T>
const T* DynamicCast (const Object* object)
{
	return object && object->IsDerived(T::TYPE) ? (const T*)object : 0;
}
//----------------------------------------------------------------------------
template <typename T>
T Object::GetUserData (const std::string &name, bool *isValued) const
{
	std::map<std::string, Any>::const_iterator it = mUserDatas.find(name);
	if (it != mUserDatas.end())
	{
		if (isValued)
		{
			*isValued = true;
		}
		return PX2_ANY_AS(it->second, T);
	}

	if (isValued)
	{
		*isValued = false;
	}

	return T();
}
//----------------------------------------------------------------------------
template <typename T>
T *Object::GetObjectByNameCast (const std::string &name)
{
	Object *obj = GetObjectByName(name);
	return DynamicCast<T>(obj);
}
//----------------------------------------------------------------------------
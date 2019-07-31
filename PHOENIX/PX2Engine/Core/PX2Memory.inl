// PX2Memory.inl

//----------------------------------------------------------------------------
#if defined (PX2_USE_MEMORY)
//----------------------------------------------------------------------------
template <typename T>
T* Memory::New1 (const int bound0)
{
	assertion(bound0>0, "must bigger than 0.\n");

	T* data;

	if (msMap)
	{
		// 插入T[]到msMap
		data = (T*)CreateBlock(bound0*sizeof(T), 1);

		// 调用T的缺省构造函数
		T* object = data;
		for (int i = 0; i < bound0; ++i, ++object)
		{
			::new(object) T;
		}
	}
	else
	{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
		assertion(false, "Pre-main allocations are not tracked.\n");
#endif
		data = new T[bound0];
	}

	return data;
}
//----------------------------------------------------------------------------
template <typename T>
T** Memory::New2 (const int bound0, const int bound1)
{
	assertion(bound0>0 && bound1>0, "must bigger than 0.\n");

	const int bound01 = bound0*bound1;
	T** data;

	if (msMap)
	{
		// T*没有构造函数，将T*[]插入内存msMap
		data = (T**)CreateBlock(bound1*sizeof(T*), 2);

		// 插入T[]到msMap
		data[0] = (T*)CreateBlock(bound01*sizeof(T), 1);

		// 调用T类型缺省构造函数。如果T是一个指针类型，编译器不会为构造函数生成
		// 任何代码。
		T* object = data[0];
		for (int i = 0; i < bound01; ++i, ++object)
		{
			::new(object) T;
		}
	}
	else
	{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
		assertion(false, "Pre-main allocations are not tracked.\n");
#endif
		data = new T*[bound1];
		data[0] = new T[bound01];
	}

	// 衔接2D数组指针
	for (int i1 = 1; i1 < bound1; ++i1)
	{
		int j0 = bound0*i1;  // = bound0*(i1 + j1); j1 = 0
		data[i1] = &data[0][j0];
	}

	return data;
}
//----------------------------------------------------------------------------
template <typename T>
T*** Memory::New3 (const int bound0, const int bound1, const int bound2)
{
	assertion(bound0>0 && bound1>0 && bound2>0, "must bigger than 0.\n");

	const int bound12 = bound1*bound2;
	const int bound012 = bound0*bound12;
	T*** data;

	if (msMap)
	{
		// T**没有构造函数, 将T**[]插入到msMap
		data = (T***)CreateBlock(bound2*sizeof(T**), 3);

		// T*没有构造函数，将T*[]插入到msMap
		data[0] = (T**)CreateBlock(bound12*sizeof(T*), 2);

		// 插入T[]到msMap
		data[0][0] = (T*)CreateBlock(bound012*sizeof(T), 1);

		// 调用T的构造函数。如果T是一个指针类型，编译器不会为构造函数生成
		// 任何代码。
		T* object = data[0][0];
		for (int i = 0; i < bound012; ++i, ++object)
		{
			::new(object) T;
		}
	}
	else
	{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
		assertion(false, "Pre-main allocations are not tracked.\n");
#endif
		data = new T**[bound2];
		data[0] = new T*[bound12];
		data[0][0] = new T[bound012];
	}

	// 衔接3D数组指针
	for (int i2 = 0; i2 < bound2; ++i2)
	{
		int j1 = bound1*i2;  // = bound1*(i2 + j2); j2 = 0
		data[i2] = &data[0][j1];
		for (int i1 = 0; i1 < bound1; ++i1)
		{
			int j0 = bound0*(i1 + j1);
			data[i2][i1] = &data[0][0][j0];
		}
	}

	return data;
}
//----------------------------------------------------------------------------
template <typename T>
T**** Memory::New4 (const int bound0, const int bound1, const int bound2,
					const int bound3)
{
	assertion(bound0>0 && bound1>0 && bound2>0
		&& bound3>0, "must bigger than 0.\n");

	const int bound23 = bound2*bound3;
	const int bound123 = bound1*bound23;
	const int bound0123 = bound0*bound123;
	T**** data;

	if (msMap)
	{
		// T***没有构造函数, 将T***[]插入到msMap
		data = (T****)CreateBlock(bound3*sizeof(T***), 4);

		// T**没有构造函数, 将T**[]插入到msMap
		data[0] = (T***)CreateBlock(bound23*sizeof(T**), 3);

		// T*没有构造函数, 将T*[]插入到msMap
		data[0][0] = (T**)CreateBlock(bound123*sizeof(T*), 2);

		// 将T[]插入到msMap
		data[0][0][0] = (T*)CreateBlock(bound0123*sizeof(T), 1);

		// 调用T的构造函数。如果T是一个指针类型，编译器不会为构造函数生成
		// 任何代码。
		T* object = data[0][0][0];
		for (int i = 0; i < bound0123; ++i, ++object)
		{
			::new(object) T;
		}
	}
	else
	{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
		assertion(false, "Pre-main allocations are not tracked.\n");
#endif
		data = new T***[bound3];
		data[0] = new T**[bound23];
		data[0][0] = new T*[bound123];
		data[0][0][0] = new T[bound0123];
	}

	// 衔接4D数组指针
	for (int i3 = 0; i3 < bound3; ++i3)
	{
		int j2 = bound2*i3;  // = bound2*(i3 + j3) where j3 = 0
		data[i3] = &data[0][j2];
		for (int i2 = 0; i2 < bound2; ++i2)
		{
			int j1 = bound1*(i2 + j2);
			data[i3][i2] = &data[0][0][j1];
			for (int i1 = 0; i1 < bound1; ++i1)
			{
				int j0 = bound0*(i1 + j1);
				data[i3][i2][i1] = &data[0][0][0][j0];
			}
		}
	}

	return data;
}
//----------------------------------------------------------------------------
template <typename T>
void Memory::Delete0 (T*& data)
{
	if (data)
	{
		if (!msMap)
		{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
			assertion(false, "Post-main deallocations are not tracked.\n");
#endif
			delete data;
			data = 0;
			return;
		}

		msMutex->Enter();

		MemoryMap::iterator iter = msMap->find(data);
		if (iter != msMap->end())
		{
			if (iter->second.mNumDimensions == 0)
			{
				size_t numBytes = iter->second.mNumBytes;
				PX2_UNUSED(numBytes);

				// 调用T的析构函数。如果T是一个指针类型，编译器不会为析构函数生
				// 成任何代码。
				data->~T();

				// 将T从msMap中移除
				BeforeEraseInformation(data);
				msMap->erase(data);				
				msDeallocator(data, mFile, mLine);
			}
			else
			{
				assertion(false, "Mismatch in dimensions.\n");
			}
		}
		else
		{
#ifdef PX2_USE_MEMORY_ALLOW_DELETE_ON_FAILED_MAP_LOOKUP
			delete data;
#else
			assertion(false, "Memory block not in map.\n");
#endif
		}

		data = 0;

		msMutex->Leave();
	}
}
//----------------------------------------------------------------------------
template <typename T>
void Memory::Delete1 (T*& data)
{
	if (data)
	{
		if (!msMap)
		{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
			assertion(false, "Post-main deallocations are not tracked.\n");
#endif
			delete[] data;
			data = 0;
			return;
		}

		msMutex->Enter();

		MemoryMap::iterator iter = msMap->find(data);
		if (iter != msMap->end())
		{
			if (iter->second.mNumDimensions == 1)
			{
				// 调用T的析构函数。如果T是一个指针类型，编译器不会为析构函数生
				// 成任何代码。
				size_t numBytes = iter->second.mNumBytes;
				PX2_UNUSED(numBytes);
				const int numElements = iter->second.mNumBytes/sizeof(T);
				T* object = data;
				for (int i = 0; i < numElements; ++i, ++object)
				{
					object->~T();
				}

				// 将T[]从msMap中移除
				BeforeEraseInformation(data);
				msMap->erase(data);
				msDeallocator(data, mFile, mLine);
			}
			else
			{
				assertion(false, "Mismatch in dimensions.\n");
			}
		}
		else
		{
#ifdef PX2_USE_MEMORY_ALLOW_DELETE_ON_FAILED_MAP_LOOKUP
			delete[] data;
#else
			assertion(false, "Memory block not in map.\n");
#endif
		}

		data = 0;

		msMutex->Leave();
	}
}
//----------------------------------------------------------------------------
template <typename T>
void Memory::Delete2 (T**& data)
{
	if (data)
	{
		if (!msMap)
		{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
			assertion(false, "Post-main deallocations are not tracked.\n");
#endif
			delete[] data[0];
			delete[] data;
			data = 0;
			return;
		}

		msMutex->Enter();

		MemoryMap::iterator iter = msMap->find(data);
		if (iter != msMap->end())
		{
			if (iter->second.mNumDimensions == 2)
			{
				// 调用T的析构函数。如果T是一个指针类型，编译器不会为析构函数生
				// 成任何代码。
				size_t numBytes = iter->second.mNumBytes;
				PX2_UNUSED(numBytes);
				const int numElements = iter->second.mNumBytes/sizeof(T);
				T* object = data[0];
				for (int i = 0; i < numElements; ++i, ++object)
				{
					object->~T();
				}

				// 将T[]从msMap中移除
				BeforeEraseInformation(data[0]);
				msMap->erase(data[0]);
				msDeallocator(data[0], mFile, mLine);

				// T*没有析构函数，将T*[]从msMap中移除
				BeforeEraseInformation(data);
				msMap->erase(data);
				msDeallocator(data, mFile, mLine);
			}
			else
			{
				assertion(false, "Mismatch in dimensions.\n");
			}
		}
		else
		{
#ifdef PX2_USE_MEMORY_ALLOW_DELETE_ON_FAILED_MAP_LOOKUP
			delete[] data[0];
			delete[] data;
#else
			assertion(false, "Memory block not in map.\n");
#endif
		}

		data = 0;

		msMutex->Leave();
	}
}
//----------------------------------------------------------------------------
template <typename T>
void Memory::Delete3 (T***& data)
{
	if (data)
	{
		if (!msMap)
		{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
			assertion(false, "Post-main deallocations are not tracked.\n");
#endif
			delete[] data[0][0];
			delete[] data[0];
			delete[] data;
			data = 0;
			return;
		}

		msMutex->Enter();

		MemoryMap::iterator iter = msMap->find(data);
		if (iter != msMap->end())
		{
			if (iter->second.mNumDimensions == 3)
			{
				// 调用T的析构函数。如果T是一个指针类型，编译器不会为析构函数生
				// 成任何代码。
				size_t numBytes = iter->second.mNumBytes;
				PX2_UNUSED(numBytes);
				const int numElements = iter->second.mNumBytes/sizeof(T);
				T* object = data[0][0];
				for (int i = 0; i < numElements; ++i, ++object)
				{
					object->~T();
				}

				// 将T[]从msMap中移除
				BeforeEraseInformation(data[0][0]);
				msMap->erase(data[0][0]);
				msDeallocator(data[0][0], mFile, mLine);

				// T*没有析构函数，将T*[]从msMap中移除
				BeforeEraseInformation(data[0]);
				msMap->erase(data[0]);
				msDeallocator(data[0], mFile, mLine);

				// T**没有析构函数，将T**[]从msMap中移除
				BeforeEraseInformation(data);
				msMap->erase(data);
				msDeallocator(data, mFile, mLine);
			}
			else
			{
				assertion(false, "Mismatch in dimensions.\n");
			}
		}
		else
		{
#ifdef PX2_USE_MEMORY_ALLOW_DELETE_ON_FAILED_MAP_LOOKUP
			delete[] data[0][0];
			delete[] data[0];
			delete[] data;
#else
			assertion(false, "Memory block not in map.\n");
#endif
		}

		data = 0;

		msMutex->Leave();
	}
}
//----------------------------------------------------------------------------
template <typename T>
void Memory::Delete4 (T****& data)
{
	if (data)
	{
		if (!msMap)
		{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
			assertion(false, "Post-main deallocations are not tracked.\n");
#endif
			delete[] data[0][0][0];
			delete[] data[0][0];
			delete[] data[0];
			delete[] data;
			data = 0;
			return;
		}

		msMutex->Enter();

		MemoryMap::iterator iter = msMap->find(data);
		if (iter != msMap->end())
		{
			if (iter->second.mNumDimensions == 4)
			{
				// 调用T的析构函数。如果T是一个指针类型，编译器不会为析构函数生
				// 成任何代码。
				size_t numBytes = iter->second.mNumBytes;
				PX2_UNUSED(numBytes);
				const int numElements = iter->second.mNumBytes/sizeof(T);
				T* object = data[0][0][0];
				for (int i = 0; i < numElements; ++i, ++object)
				{
					object->~T();
				}

				// 将T[]从msMap中移除
				BeforeEraseInformation(data[0][0][0]);
				msMap->erase(data[0][0][0]);
				msDeallocator(data[0][0][0], mFile, mLine);

				// T*没有析构函数，将T*[]从msMap中移除
				BeforeEraseInformation(data[0][0]);
				msMap->erase(data[0][0]);
				msDeallocator(data[0][0], mFile, mLine);

				// T**没有析构函数, 将T**[]从msMap中移除
				BeforeEraseInformation(data[0]);
				msMap->erase(data[0]);
				msDeallocator(data[0], mFile, mLine);

				// T***没有析构函数, 将T***[]从msMap中移除
				BeforeEraseInformation(data);
				msMap->erase(data);
				msDeallocator(data, mFile, mLine);
			}
			else
			{
				assertion(false, "Mismatch in dimensions.\n");
			}
		}
		else
		{
#ifdef PX2_USE_MEMORY_ALLOW_DELETE_ON_FAILED_MAP_LOOKUP
			delete[] data[0][0][0];
			delete[] data[0][0];
			delete[] data[0];
			delete[] data;
#else
			assertion(false, "Memory block not in map.\n");
#endif
		}

		data = 0;

		msMutex->Leave();
	}
}
//----------------------------------------------------------------------------
#else
//----------------------------------------------------------------------------
template <typename T>
T* new1 (const int bound0)
{
	return new T[bound0];
}
//----------------------------------------------------------------------------
template <typename T>
T** new2 (const int bound0, const int bound1)
{
	const int bound01 = bound0*bound1;
	T** data = new T*[bound1];
	data[0] = new T[bound01];

	for (int i1 = 1; i1 < bound1; ++i1)
	{
		int j0 = bound0*i1;  // = bound0*(i1 + j1); j1 = 0
		data[i1] = &data[0][j0];
	}
	return data;
}
//----------------------------------------------------------------------------
template <typename T>
T*** new3 (const int bound0, const int bound1, const int bound2)
{
	const int bound12 = bound1*bound2;
	const int bound012 = bound0*bound12;
	T*** data = new T**[bound2];
	data[0] = new T*[bound12];
	data[0][0] = new T[bound012];

	for (int i2 = 0; i2 < bound2; ++i2)
	{
		int j1 = bound1*i2;  // = bound1*(i2 + j2); j2 = 0
		data[i2] = &data[0][j1];
		for (int i1 = 0; i1 < bound1; ++i1)
		{
			int j0 = bound0*(i1 + j1);
			data[i2][i1] = &data[0][0][j0];
		}
	}
	return data;
}
//----------------------------------------------------------------------------
template <typename T>
T**** new4 (const int bound0, const int bound1, const int bound2,
			const int bound3)
{
	const int bound23 = bound2*bound3;
	const int bound123 = bound1*bound23;
	const int bound0123 = bound0*bound123;
	T**** data = new T***[bound3];
	data[0] = new T**[bound23];
	data[0][0] = new T*[bound123];
	data[0][0][0] = new T[bound0123];

	for (int i3 = 0; i3 < bound3; ++i3)
	{
		int j2 = bound2*i3;  // = bound2*(i3 + j3); j3 = 0
		data[i3] = &data[0][j2];
		for (int i2 = 0; i2 < bound2; ++i2)
		{
			int j1 = bound1*(i2 + j2);
			data[i3][i2] = &data[0][0][j1];
			for (int i1 = 0; i1 < bound1; ++i1)
			{
				int j0 = bound0*(i1 + j1);
				data[i3][i2][i1] = &data[0][0][0][j0];
			}
		}
	}
	return data;
}
//----------------------------------------------------------------------------
template <typename T>
void delete0 (T*& data)
{
	delete data;
	data = 0;
}
//----------------------------------------------------------------------------
template <typename T>
void delete1 (T*& data)
{
	delete[] data;
	data = 0;
}
//----------------------------------------------------------------------------
template <typename T>
void delete2 (T**& data)
{
	if (data)
	{
		delete[] data[0];
		delete[] data;
		data = 0;
	}
}
//----------------------------------------------------------------------------
template <typename T>
void delete3 (T***& data)
{
	if (data)
	{
		delete[] data[0][0];
		delete[] data[0];
		delete[] data;
		data = 0;
	}
}
//----------------------------------------------------------------------------
template <typename T>
void delete4 (T****& data)
{
	if (data)
	{
		delete[] data[0][0][0];
		delete[] data[0][0];
		delete[] data[0];
		delete[] data;
		data = 0;
	}
}
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------

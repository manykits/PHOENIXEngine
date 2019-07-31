// PX2ResourceUpdate.hpp

#ifndef PX2RESOURCEUPDATE_HPP
#define PX2RESOURCEUPDATE_HPP

#include "PX2UnityPre.hpp"
#include "PX2FString.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ResourceVersion
	{
	public:
		ResourceVersion ();
		explicit ResourceVersion (const std::string &version);
		ResourceVersion (const ResourceVersion &sv);
		~ResourceVersion ();

		int GetMain () const;
		int GetLib () const;
		int GetRes () const;

		std::string GetAsString () const;

		bool operator== (const ResourceVersion &b) const;
		bool operator != (const ResourceVersion &b) const;
		friend bool operator > (const ResourceVersion& a, const ResourceVersion& b);
		friend bool operator < (const ResourceVersion& a, const ResourceVersion& b);

	public:
		int mMain;
		int mLib;
		int mRes;
	};

	class PX2_ENGINE_ITEM ResourceVersionItem
	{
	public:
		ResourceVersionItem ();
		~ResourceVersionItem();

		ResourceVersion Version;
		bool IsCurrent;
	};
	typedef Pointer0<ResourceVersionItem> ResourceVersionItemPtr;

	class PX2_ENGINE_ITEM ResourceFileInfoItem
	{
	public:
		ResourceFileInfoItem ();
		~ResourceFileInfoItem ();

		std::string Path;
		std::string MD5;
	};
	typedef Pointer0<ResourceFileInfoItem> ResourceFileInfoItemPtr;

#include "PX2ResourceUpdate.inl"

}

#endif
// PX2NameValueCollection.hpp

#ifndef PX2NAMEVALUECOLLECTION_HPP
#define PX2NAMEVALUECOLLECTION_HPP

#include "PX2NetPre.hpp"
#include "PX2ListMap.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM NameValueCollection
	{
	public:
		typedef ListMap<std::string, std::string> HeaderMap;
		typedef HeaderMap::Iterator Iterator;
		typedef HeaderMap::ConstIterator ConstIterator;

		NameValueCollection();
		NameValueCollection(const NameValueCollection& nvc);
		virtual ~NameValueCollection();

		NameValueCollection& operator = (const NameValueCollection& nvc);

		void swap(NameValueCollection& nvc);
		const std::string& operator [] (const std::string& name) const;
		void set(const std::string& name, const std::string& value);
		void add(const std::string& name, const std::string& value);
		const std::string& get(const std::string& name) const;
		const std::string& get(const std::string& name, const std::string& defaultValue) const;
		bool has(const std::string& name) const;
		ConstIterator find(const std::string& name) const;
		ConstIterator begin() const;
		ConstIterator end() const;
		bool empty() const;
		std::size_t size() const;
		void erase(const std::string& name);
		void clear();

	private:
		HeaderMap _map;
		static std::string NotFoundString;
	};


}

#endif
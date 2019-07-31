// PX2NameValueCollection.cpp

#include "PX2NameValueCollection.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
std::string NameValueCollection::NotFoundString;
//----------------------------------------------------------------------------
NameValueCollection::NameValueCollection()
{
}
//----------------------------------------------------------------------------
NameValueCollection::NameValueCollection(const NameValueCollection& nvc) :
_map(nvc._map)
{
}
//----------------------------------------------------------------------------
NameValueCollection::~NameValueCollection()
{
}
//----------------------------------------------------------------------------
NameValueCollection& NameValueCollection::operator = (const NameValueCollection& nvc)
{
	if (&nvc != this)
	{
		_map = nvc._map;
	}
	return *this;
}
//----------------------------------------------------------------------------
void NameValueCollection::swap(NameValueCollection& nvc)
{
	std::swap(_map, nvc._map);
}
//----------------------------------------------------------------------------
const std::string& NameValueCollection::operator [] (const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		return NotFoundString;
}
//----------------------------------------------------------------------------
void NameValueCollection::set(const std::string& name, const std::string& value)
{
	Iterator it = _map.find(name);
	if (it != _map.end())
		it->second = value;
	else
		_map.insert(HeaderMap::ValueType(name, value));
}
//----------------------------------------------------------------------------
void NameValueCollection::add(const std::string& name, const std::string& value)
{
	_map.insert(HeaderMap::ValueType(name, value));
}
//----------------------------------------------------------------------------
const std::string& NameValueCollection::get(const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		return NotFoundString;
}
//----------------------------------------------------------------------------
const std::string& NameValueCollection::get(const std::string& name, const std::string& defaultValue) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		return defaultValue;
}
//----------------------------------------------------------------------------
bool NameValueCollection::has(const std::string& name) const
{
	return _map.find(name) != _map.end();
}
//----------------------------------------------------------------------------
NameValueCollection::ConstIterator NameValueCollection::find(const std::string& name) const
{
	return _map.find(name);
}
//----------------------------------------------------------------------------
NameValueCollection::ConstIterator NameValueCollection::begin() const
{
	return _map.begin();
}
//----------------------------------------------------------------------------
NameValueCollection::ConstIterator NameValueCollection::end() const
{
	return _map.end();
}
//----------------------------------------------------------------------------
bool NameValueCollection::empty() const
{
	return _map.empty();
}
//----------------------------------------------------------------------------
std::size_t NameValueCollection::size() const
{
	return _map.size();
}
//----------------------------------------------------------------------------
void NameValueCollection::erase(const std::string& name)
{
	_map.erase(name);
}
//----------------------------------------------------------------------------
void NameValueCollection::clear()
{
	_map.clear();
}
//----------------------------------------------------------------------------
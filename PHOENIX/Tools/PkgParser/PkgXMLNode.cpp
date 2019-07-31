// PX2XMLNode.cpp

#include "PkgXMLNode.hpp"
#include "rapidxml.hpp"

using namespace std;
using namespace PkgParse;

//----------------------------------------------------------------------------
XMLNode::XMLNode(XML_NODEPTR element)
: 
mElement(element)
{
}

//----------------------------------------------------------------------------
XMLNode::XMLNode () :
mElement(NULL)
{

}
//----------------------------------------------------------------------------
XMLNode::XMLNode (const XMLNode &rhs)
:
mElement(rhs.mElement)
{
}

//----------------------------------------------------------------------------
XMLNode::~XMLNode ()
{
}
//----------------------------------------------------------------------------
bool XMLNode::IsNull ()
{
	return (mElement == 0);
}
//----------------------------------------------------------------------------
XMLNode XMLNode::NewChild(std::string name)
{
	XML_DOCPTR doc = mElement->document();
	char *pname = doc->allocate_string(name.c_str());
	XML_NODEPTR element = doc->allocate_node(rapidxml::node_element, pname);
	mElement->append_node(element);

	return XMLNode(element);
}
/*
void XMLNode::LinkEndChild (XMLNode node)
{
	if (node.mElement && mElement)
	{
		mElement->append_node(node.mElement);
	}
}*/
//----------------------------------------------------------------------------
const char *XMLNode::GetName ()
{
	return mElement->name();
}
//----------------------------------------------------------------------------
std::string XMLNode::GetNameStr ()
{
	return std::string(mElement->name());
}
//----------------------------------------------------------------------------
char *XMLNode::GetFullName (char *buffer, size_t maxBuffer)
{
	list<string> strs;
	string str;

	XML_NODEPTR pCurElement = mElement;
	while (pCurElement)
	{
		str = pCurElement->name();
		strs.push_front(str);

		pCurElement = pCurElement->parent();
	}

	str = "";
	list<string>::iterator iter = strs.begin();
	for (; iter!=strs.end(); iter++)
	{
		if (!str.empty())
			str += ".";

		str += *iter;
	}

	strncpy(buffer, str.c_str(), maxBuffer);
	buffer[maxBuffer-1] = 0;

	return buffer;
}
//----------------------------------------------------------------------------
const char *XMLNode::GetText ()
{
	return mElement->value();
}
//----------------------------------------------------------------------------
bool XMLNode::HasChild (const char *name)
{
	return mElement->first_node(name)!=NULL;
}
//----------------------------------------------------------------------------
XMLNode XMLNode::GetChild (const char *name)
{
	return XMLNode(mElement->first_node(name));
}
//----------------------------------------------------------------------------
XMLNode XMLNode::IterateChild ()
{
	return XMLNode(mElement->first_node());
}
//----------------------------------------------------------------------------
XMLNode XMLNode::IterateChild (XMLNode previous)
{
	if (previous.IsNull())
	{
		return XMLNode();
	}
	else
	{
		return XMLNode(previous.mElement->next_sibling());
	}
}
//----------------------------------------------------------------------------
bool XMLNode::HasAttribute (const char *name)
{
	return (mElement->first_attribute(name) != NULL);
}
//----------------------------------------------------------------------------
int XMLNode::AttributeToInt (const char *name)
{
	return atoi(AttributeToString(name));
}
//----------------------------------------------------------------------------
float XMLNode::AttributeToFloat (const char *name)
{
	float v;
	sscanf(AttributeToString(name), "%f", &v);
	return v;
}
//----------------------------------------------------------------------------
bool XMLNode::AttributeToBool (const char *name)
{
	const char *pstr = AttributeToString(name);

	if (0 == pstr)
	{
		return false;
	}
	else
	{
		return (strncmp(pstr, "true", 4) == 0)
			||(strncmp(pstr, "TRUE", 4) == 0);
	}
}
//----------------------------------------------------------------------------
const char *XMLNode::AttributeToString (const char *name)
{
	rapidxml::xml_attribute<> *attr = mElement->first_attribute(name);
	if(attr == NULL)
	{
		assert(false);
	}
	return attr->value();
}
//----------------------------------------------------------------------------
const char *XMLNode::AttributeToStringNoAssert (const char *name)
{
	rapidxml::xml_attribute<> *attr = mElement->first_attribute(name);
	if(attr == NULL)
	{
		return NULL;
	}
	return attr->value();
}
//----------------------------------------------------------------------------
bool XMLNode::AttribToInt (const char *name, int &value)
{
	const char *pstr = AttributeToStringNoAssert(name);
	if(pstr == NULL)
	{
		return false;
	}

	value = atoi(pstr);
	return true;
}
//----------------------------------------------------------------------------
bool XMLNode::AttribToFloat (const char *name, float &value)
{
	const char *pstr = AttributeToStringNoAssert(name);
	if(pstr == NULL)
	{
		return false;
	}

	sscanf(pstr, "%f", &value);
	return true;
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeInt (const char *name, int value)
{
	char buffer[64];
	sprintf(buffer, "%d", value);
	SetAttributeString(name, buffer);
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeFloat (const char *name, float value)
{
	char buffer[256];
	sprintf(buffer, "%f", value);
	SetAttributeString(name, buffer);
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeBool (const char *name, bool value)
{
	SetAttributeString(name, value?"true":"false");
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeString (const char *name, const char *str)
{
	rapidxml::xml_attribute<> *attr = mElement->first_attribute(name);
	if(attr) attr->value(str);
	else
	{
		rapidxml::xml_document<> *doc = mElement->document();
		char *name2 = doc->allocate_string(name);
		char *str2 = doc->allocate_string(str);
		mElement->append_attribute(doc->allocate_attribute(name2, str2));
	}
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeString (const char *name, const std::string &str)
{
	SetAttributeString(name, str.c_str());
}
//----------------------------------------------------------------------------
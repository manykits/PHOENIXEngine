// PX2XMLNode.cpp

#include "PX2XMLNode.hpp"
#include "PX2Assert.hpp"
#include "rapidxml.hpp"

using namespace std;
using namespace PX2;

//----------------------------------------------------------------------------
XMLNode::XMLNode(XML_NODEPTR element): 
mElement(element)
{
}
//----------------------------------------------------------------------------
XMLNode::XMLNode ():
mElement(NULL)
{

}
//----------------------------------------------------------------------------
XMLNode::XMLNode (const XMLNode &rhs):
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
//----------------------------------------------------------------------------
std::string XMLNode::GetName ()
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
std::string XMLNode::GetText()
{
	return mElement->value();
}
//----------------------------------------------------------------------------
bool XMLNode::HasChild(const std::string &name)
{
	return mElement->first_node(name.c_str())!=NULL;
}
//----------------------------------------------------------------------------
XMLNode XMLNode::GetChild(const std::string &name)
{
	return XMLNode(mElement->first_node(name.c_str()));
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
bool XMLNode::HasAttribute(const std::string &name)
{
	return (mElement->first_attribute(name.c_str()) != NULL);
}
//----------------------------------------------------------------------------
int XMLNode::AttributeToInt(const std::string &name)
{
	return atoi(AttributeToString(name).c_str());
}
//----------------------------------------------------------------------------
float XMLNode::AttributeToFloat(const std::string &name)
{
	float v;
	sscanf(AttributeToString(name).c_str(), "%f", &v);
	return v;
}
//----------------------------------------------------------------------------
bool XMLNode::AttributeToBool(const std::string &name)
{
	std::string str = AttributeToString(name);

	if (str.empty())
	{
		return false;
	}
	else
	{
		return (strncmp(str.c_str(), "true", 4) == 0)
			|| (strncmp(str.c_str(), "TRUE", 4) == 0);
	}
}
//----------------------------------------------------------------------------
std::string XMLNode::AttributeToString(const std::string &name)
{
	rapidxml::xml_attribute<> *attr = mElement->first_attribute(name.c_str());
	if (attr == NULL)
	{
		return "";
	}
	return attr->value();
}
//----------------------------------------------------------------------------
bool XMLNode::AttribToInt(const std::string &name, int &value)
{
	std::string str = AttributeToString(name);
	if (str.empty())
	{
		return false;
	}

	value = atoi(str.c_str());
	return true;
}
//----------------------------------------------------------------------------
bool XMLNode::AttribToFloat(const std::string &name, float &value)
{
	std::string str = AttributeToString(name);
	if (str.empty())
	{
		return false;
	}

	sscanf(str.c_str(), "%f", &value);
	return true;
}
//----------------------------------------------------------------------------
bool XMLNode::AttribToBool(const std::string &name, bool &value)
{
	std::string str = AttributeToString(name);
	if (str.empty())
	{
		return false;
	}

	if ((strncmp(str.c_str(), "true", 4) == 0)
		|| (strncmp(str.c_str(), "TRUE", 4) == 0))
	{
		value = true;
	}
	else
	{
		value = false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool XMLNode::AttribToString(const std::string &name, std::string &value)
{
	rapidxml::xml_attribute<> *attr = mElement->first_attribute(name.c_str());
	if (attr == NULL)
	{
		return false;
	}

	value = attr->value();

	return true;
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeInt(const std::string &name, int value)
{
	char buffer[64];
	sprintf(buffer, "%d", value);
	SetAttributeString(name, buffer);
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeFloat(const std::string &name, float value)
{
	char buffer[256];
	sprintf(buffer, "%f", value);
	SetAttributeString(name, buffer);
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeBool(const std::string &name, bool value)
{
	SetAttributeString(name, value?"true":"false");
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeString(const std::string &name, 
	const std::string &str)
{
	SetAttributeString(name, str.c_str(), str.length());
}
//----------------------------------------------------------------------------
void XMLNode::SetAttributeString(const std::string &name, const char *str, 
	int length)
{
	rapidxml::xml_attribute<> *attr = mElement->first_attribute(name.c_str());
	if (attr) attr->value(str);
	else
	{
		rapidxml::xml_document<> *doc = mElement->document();
		char *name2 = doc->allocate_string(name.c_str());
		char *str2 = doc->allocate_string(str, length);
		mElement->append_attribute(doc->allocate_attribute(name2, str2,
			name.length(), length));
	}
}
//----------------------------------------------------------------------------
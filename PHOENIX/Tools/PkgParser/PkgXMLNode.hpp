// PkgXMLNode.hpp

#ifndef PKGXMLNODE_HPP
#define PKGXMLNODE_HPP

#include "PkgParsePre.hpp"

namespace rapidxml
{
	// Forward declarations
	template<class Ch> class xml_node;
	template<class Ch> class xml_attribute;
	template<class Ch> class xml_document;
};

typedef rapidxml::xml_node<char> * XML_NODEPTR;
typedef rapidxml::xml_document<char> * XML_DOCPTR;

namespace PkgParse
{

	class XMLNode
	{
	public:
		XMLNode (XML_NODEPTR element);
		XMLNode ();
		XMLNode (const XMLNode &rhs);
		~XMLNode ();

		bool IsNull ();
		XMLNode NewChild(std::string name="");

		const char *GetName ();
		std::string GetNameStr ();
		char *GetFullName (char *buffer, size_t maxBuffer);
		const char *GetText ();

		// 孩子
		bool HasChild (const char *name);
		XMLNode GetChild (const char *name); //< 获得第一个叫name的孩子
		XMLNode IterateChild (); //< 获得第一个孩子
		XMLNode IterateChild (XMLNode previous);		

		// 属性
		bool HasAttribute (const char *name);
		int AttributeToInt (const char *name);
		float AttributeToFloat (const char *name);
		bool AttributeToBool (const char *name);
		const char *AttributeToString (const char *name);
		const char *AttributeToStringNoAssert (const char *name);//不存在此attribute， 返回NULL

		bool AttribToInt(const char *name, int &value);
		bool AttribToFloat(const char *name, float &value);

		void SetAttributeInt (const char *name, int value);
		void SetAttributeFloat (const char *name, float value);
		void SetAttributeBool (const char *name, bool value);
		void SetAttributeString (const char *name, const char *string);
		void SetAttributeString (const char *name, const std::string &str);

	public:
		XML_NODEPTR mElement;
	};

}

#endif
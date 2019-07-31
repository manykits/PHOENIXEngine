// PX2XMLNode.hpp

#ifndef PX2XMLNODE_HPP
#define PX2XMLNODE_HPP

#include "PX2CorePre.hpp"

namespace rapidxml
{
	// Forward declarations
	template<class Ch> class xml_node;
	template<class Ch> class xml_attribute;
	template<class Ch> class xml_document;
};

typedef rapidxml::xml_node<char> * XML_NODEPTR;
typedef rapidxml::xml_document<char> * XML_DOCPTR;

namespace PX2
{

	class PX2_ENGINE_ITEM XMLNode
	{
	public:
		XMLNode (XML_NODEPTR element);
		XMLNode ();
		XMLNode (const XMLNode &rhs);
		~XMLNode ();

		bool IsNull ();
		XMLNode NewChild(std::string name="");

		std::string GetName ();
		char *GetFullName (char *buffer, size_t maxBuffer);
		std::string GetText ();

		// 孩子
		bool HasChild (const std::string &name);
		XMLNode GetChild(const std::string &name); //< 获得第一个叫name的孩子
		XMLNode IterateChild (); //< 获得第一个孩子
		XMLNode IterateChild (XMLNode previous);		

		// 属性
		bool HasAttribute (const std::string &name);
		int AttributeToInt(const std::string &name);
		float AttributeToFloat(const std::string &name);
		bool AttributeToBool(const std::string &name);
		std::string AttributeToString(const std::string &name);

		bool AttribToInt(const std::string &name, int &value);
		bool AttribToFloat(const std::string &name, float &value);
		bool AttribToBool(const std::string &name, bool &value);
		bool AttribToString(const std::string &name, std::string &value);

		void SetAttributeInt(const std::string &name, int value);
		void SetAttributeFloat(const std::string &name, float value);
		void SetAttributeBool(const std::string &name, bool value);
		void SetAttributeString(const std::string &name, const std::string &str);
		void SetAttributeString(const std::string &name, const char *str, int length);

	public:
		XML_NODEPTR mElement;
	};

}

#endif
// XMLData.hpp

#include "PX2XMLData.hpp"
#include "PX2Assert.hpp"
#include "PX2Memory.hpp"

#if defined(_WIN32) || defined(WIN32)
#pragma warning(push) 
#pragma warning(disable : 4100)
#endif
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#if defined(_WIN32) || defined(WIN32)
#pragma warning(pop)
#endif

using namespace PX2;
using namespace std;

namespace rapidxml
{
    void parse_error_handler(const char *what, void *where)
	{
		assertion(false, "parse xml error: %s", what);
		PX2_UNUSED(where);
	}
}
//----------------------------------------------------------------------------
XMLData::XMLData() 
:
mDocument(0), mDocStr(0)
{
}
//----------------------------------------------------------------------------
XMLData::~XMLData()
{
	if (mDocument)
	{
		delete0(mDocument);
		mDocument = 0;
	}
	if(mDocStr)
	{
		delete[] mDocStr;
	}
}
//----------------------------------------------------------------------------
bool XMLData::LoadFile (const string &fileName)
{
	mDocument = new0 rapidxml::xml_document<char>;
	if (!mDocument)
		return false;

	FILE *fp = fopen(fileName.c_str(), "rb");
	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		mDocStr = new char[len+1];
		fseek(fp, 0, SEEK_SET);
		fread(mDocStr, len, 1, fp);
		mDocStr[len] = 0;
		fclose(fp);
	}
	
	if(mDocStr)
	{
		mDocument->parse<0>(mDocStr);
		return true;
	}
	else return false;
}
//----------------------------------------------------------------------------
bool XMLData::LoadBuffer (const char *buffer, int size)
{
	mDocument = new0 rapidxml::xml_document<char>;
	if (!mDocument)
		return false;

	mDocStr = new char[size+1];
	memcpy(mDocStr, buffer, size);
	mDocStr[size] = 0;

	mDocument->parse<0>(mDocStr);
	return true;
}
//----------------------------------------------------------------------------
bool XMLData::SaveFile (const string &fileName)
{
	if (!mDocument)
		return false;

	std::string str;
	rapidxml::print(std::back_inserter(str), *mDocument, 0);

	FILE *fp = fopen(fileName.c_str(), "wb");
	if(fp)
	{
		fwrite(str.c_str(), str.length(), 1, fp);
		fclose(fp);
		return true;
	}
	else return false;
}
//----------------------------------------------------------------------------
void XMLData::Create ()
{
	mDocument = new0 rapidxml::xml_document<char>;
}
//----------------------------------------------------------------------------
XMLNode XMLData::GetRootNode ()
{
	return XMLNode(mDocument->first_node());
}
//----------------------------------------------------------------------------
XMLNode XMLData::NewChild (const std::string &name)
{
	char *pname = mDocument->allocate_string(name.c_str());
	XML_NODEPTR element = mDocument->allocate_node(rapidxml::node_element, pname);
	mDocument->append_node(element);

	return XMLNode(element);
}
//----------------------------------------------------------------------------
XMLNode XMLData::GetNodeByPath(const std::string &path)
{
	char name[256];
	const char *pcur = path.c_str();

	XMLNode node = GetRootNode();
	while (pcur!=0 && pcur!=0)
	{
		const char *pdot = strchr(pcur, '.');
		if (pdot)
		{
			size_t len = pdot - pcur;
			memcpy(name, pcur, len);
			name[len] = 0;
			pcur = pdot + 1;
		}
		else
		{
			strcpy(name, pcur);
			pcur = 0;
		}

		node = node.GetChild(name);
		if (node.IsNull())
			return node;
	}

	return node;
}
//----------------------------------------------------------------------------
void XMLData::LinkEndChild (XMLNode node)
{
	if (node.mElement)
	{
		mDocument->append_node(node.mElement);
	}
}
//----------------------------------------------------------------------------
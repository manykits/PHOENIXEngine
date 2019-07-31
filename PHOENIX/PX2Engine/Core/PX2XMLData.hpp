 // PX2XMLData.hpp

#ifndef PX2XMLDATA_HPP
#define PX2XMLDATA_HPP

#include "PX2XMLNode.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM XMLData
	{
	public:
		XMLData ();
		~XMLData ();

		bool LoadFile (const std::string &fileName);
		bool LoadBuffer (const char *buffer, int size);
		bool SaveFile (const std::string &fileName);
		void Create ();

		XMLNode GetRootNode ();
		XMLNode NewChild (const std::string &name);

		// path="Config.RenderSystem.FXSetting",如果中间任何一个节点不存在,返回
		// 空节点, 名字不需要包含根节点
		XMLNode GetNodeByPath (const std::string &path);

		void LinkEndChild (XMLNode node);

		XML_DOCPTR GetDoc()
		{
			return mDocument;
		}

	private:
		XMLData (const XMLData &);
		XMLData &operator= (const XMLData &);

		XML_DOCPTR mDocument;
		char *mDocStr;
	};

}

#endif
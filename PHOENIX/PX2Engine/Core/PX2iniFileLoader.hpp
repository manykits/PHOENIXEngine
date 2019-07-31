// PX2DateTimeFormat.hpp

#ifndef PX2DATETIMEFORMAT_HPP
#define PX2DATETIMEFORMAT_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM iniFileLoaderBase
	{
	public:

		//helper methods. They convert the next parameter value found into the 
		//relevant type
		double      GetNextParameterDouble() { if (m_bGoodFile) return atof(GetNextParameter().c_str()); return 0.0; }
		float       GetNextParameterFloat() { if (m_bGoodFile) return (float)atof(GetNextParameter().c_str()); return 0.0f; }
		int         GetNextParameterInt() { if (m_bGoodFile) return atoi(GetNextParameter().c_str()); return 0; }
		bool        GetNextParameterBool() { return (bool)(atoi(GetNextParameter().c_str())); return false; }

		double      GetNextTokenAsDouble() { if (m_bGoodFile) return atof(GetNextToken().c_str()); return 0.0; }
		float       GetNextTokenAsFloat() { if (m_bGoodFile) return (float)atof(GetNextToken().c_str()); return 0.0f; }
		int         GetNextTokenAsInt() { if (m_bGoodFile) return atoi(GetNextToken().c_str()); return 0; }
		std::string GetNextTokenAsString() { if (m_bGoodFile) return GetNextToken(); return ""; }

		bool        eof()const { if (m_bGoodFile) return file.eof(); return false; }
		bool        FileIsGood()const { return m_bGoodFile; }

		iniFileLoaderBase(char* filename) :CurrentLine(""), m_bGoodFile(true)
		{
			file.open(filename);

			if (!file) { m_bGoodFile = false; }
		}

	private:
		std::ifstream file;
		std::string   CurrentLine;

		void GetParameterValueAsString(std::string& line);
		std::string GetNextParameter();
		std::string GetNextToken();

		bool m_bGoodFile;
	};



}


#endif
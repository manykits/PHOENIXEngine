// PX2LProject.hpp

#ifndef PX2LPROJECT_HPP
#define PX2LPROJECT_HPP

#include "PX2LogicPre.hpp"
#include "PX2Object.hpp"
#include "PX2LBlock.hpp"
#include "PX2LFile.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LProject : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LProject);

	public:
		LProject();
		virtual ~LProject();

		bool AddFile(LFile *file);
		bool IsHasFile(LFile *file);
		bool RemoveFile(LFile *file);

		std::vector<LFilePtr> &GetLFiles();

		void Compile();
		void Save();

	protected:
		std::vector<LFilePtr> mLFiles;
	};
	typedef Pointer0<LProject> LProjectPtr;

}

#endif
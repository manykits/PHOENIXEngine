// PX2DirData.hpp

#ifndef PX2DIRDATA_HPP
#define PX2DIRDATA_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DirData
	{
	public:
		DirData(const std::string& dirname);
		~DirData();

		void SetFileSpec(const std::string& filespec);
		void SetFlags(int flags);

		void Close();
		void Rewind();
		bool Read(std::string *filename);

		std::string GetName();

	private:
		void *mFinddata;

		std::string mDirname;
		std::string mFilespec;
		int mFlags;
	};

}

#endif
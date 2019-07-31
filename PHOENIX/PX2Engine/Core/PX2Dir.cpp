// PX2Dir.cpp

#include "PX2Dir.hpp"
#include "PX2Memory.hpp"
#include "PX2Log.hpp"
#include "PX2FileFn.hpp"
#include "PX2DirData.hpp"
#include "PX2Assert.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
DirP::DirP() :
mData(0)
{
}
//----------------------------------------------------------------------------
DirP::DirP(const std::string& dirname) :
mData(0)
{
	Open(dirname);
}
//----------------------------------------------------------------------------
DirP::~DirP()
{
	delete0(mData);
}
//----------------------------------------------------------------------------
bool DirP::Open(const std::string& dirname)
{
	delete0(mData);

	if (IsDirExists(dirname))
	{
		mData = new0 DirData(dirname);

		return true;
	}
	else
	{
		mData = NULL;

		return false;
	}
}
//----------------------------------------------------------------------------
bool DirP::IsOpened() const
{
	return 0 != mData;
}
//----------------------------------------------------------------------------
std::string DirP::GetName() const
{
	std::string name;

	if (mData)
	{
		name = mData->GetName();
	}

	return name;
}
//----------------------------------------------------------------------------
std::string DirP::GetNameWithSep() const
{
	std::string name = GetName();
	if (!name.empty())
	{
		if (name[name.length()-1] != PX2_FILE_SEP_PATH)
			name += PX2_FILE_SEP_PATH;
	}

	return name;
}
//----------------------------------------------------------------------------
void DirP::Close()
{
	if (mData)
		delete0(mData);
}
//----------------------------------------------------------------------------
bool DirP::GetFirst(std::string *filename,
	const std::string &filespec, int flags) const
{
	assertion(IsOpened(), "must openedd first.");

	mData->Rewind();

	mData->SetFileSpec(filespec);
	mData->SetFlags(flags);

	return GetNext(filename);
}
//----------------------------------------------------------------------------
bool DirP::GetNext(std::string *filename) const
{
	assertion(IsOpened(), "must DirP::Open() first");

	return mData->Read(filename);
}
//----------------------------------------------------------------------------
bool DirP::HasFiles(const std::string& spec) const
{
	std::string s;
	return GetFirst(&s, spec, DIR_FILES|DIR_HIDDEN);
}
//----------------------------------------------------------------------------
bool DirP::HasSubDirs(const std::string& spec) const
{
	std::string s;
	return GetFirst(&s, spec, DIR_DIRS|DIR_HIDDEN);
}
//----------------------------------------------------------------------------
size_t DirP::Traverse(DirTraverser& sink, const std::string &filespec,
	int flags) const
{
	size_t nFiles = 0;

	const std::string prefix = GetNameWithSep();

	if (flags & DIR_DIRS)
	{
		std::string dirname;
		for (bool cont = GetFirst(&dirname, "", (flags & ~(DIR_FILES | DIR_DOTDOT))|DIR_DIRS);
			cont; cont = cont && GetNext(&dirname))
		{
			const std::string fulldirname = prefix + dirname;

			switch (sink.OnDir(fulldirname))
			{
			default:
			case DIR_STOP:
				cont = false;
				break;

			case DIR_CONTINUE:
			{
								   DirP subdir;

								   bool ok;
								   do
								   {
									   ok = subdir.Open(fulldirname);
									   if (!ok)
									   {
										   // ask the user code what to do
										   bool tryagain;
										   switch (sink.OnOpenError(fulldirname))
										   {
										   default:
											   PX2_LOG_ERROR("unexpected OnOpenError() return value");
											   // fall through

										   case DIR_STOP:
											   cont = false;
											   // fall through

										   case DIR_IGNORE:
											   tryagain = false;
											   break;

										   case DIR_CONTINUE:
											   tryagain = true;
										   }

										   if (!tryagain)
											   break;
									   }
								   } while (!ok);

								   if (ok)
								   {
									   nFiles += subdir.Traverse(sink, filespec, flags);
								   }
			}
				break;

			case DIR_IGNORE:
				// nothing to do
				;
			}
		}
	}

	// now enum our own files
	if (flags & DIR_FILES)
	{
		flags &= ~DIR_DIRS;

		std::string filename;
		bool cont = GetFirst(&filename, filespec, flags);
		while (cont)
		{
			DirTraverseResult res = sink.OnFile(prefix + filename);
			if (res == DIR_STOP)
				break;

			assertion(res == DIR_CONTINUE, "unexpected OnFile() return value");

			nFiles++;

			cont = GetNext(&filename);
		}
	}

	return nFiles;

}
//----------------------------------------------------------------------------
class DirTraverserSimple : public DirTraverser
{
public:
	DirTraverserSimple(std::vector<std::string>& files) : mFiles(files) { }

	virtual DirTraverseResult OnFile(const std::string& filename)
	{
		mFiles.push_back(filename);
		return DIR_CONTINUE;
	}

	virtual DirTraverseResult OnDir(const std::string &dirname)
	{
		return DIR_CONTINUE;
	}

private:
	std::vector<std::string> & mFiles;
};
//----------------------------------------------------------------------------
size_t DirP::GetAllFiles(const std::string& dirname,
	std::vector<std::string> *files, const std::string& filespec, 
	int flags)
{
	size_t nFiles = 0;

	DirP dir(dirname);
	if (dir.IsOpened())
	{
		DirTraverserSimple traverser(*files);

		nFiles += dir.Traverse(traverser, filespec, flags);
	}

	return nFiles;
}
//----------------------------------------------------------------------------
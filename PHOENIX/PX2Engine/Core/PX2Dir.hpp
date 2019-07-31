// PX2Dir.hpp

#ifndef PX2DIR_HPP
#define PX2DIR_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DirData;

	enum DirTraverseResult
	{
		DIR_IGNORE = -1,      // ignore this directory but continue with others
		DIR_STOP,             // stop traversing
		DIR_CONTINUE          // continue into this directory
	};

	class PX2_ENGINE_ITEM DirTraverser
	{
	public:
		virtual ~DirTraverser() { }
		virtual DirTraverseResult OnFile(const std::string& filename) = 0;
		virtual DirTraverseResult OnDir(const std::string& dirname) = 0;
		virtual DirTraverseResult OnOpenError(const std::string& dirname)
		{
			return DIR_IGNORE;
		}
	};

	class PX2_ENGINE_ITEM DirP
	{
	public:
		DirP();
		DirP(const std::string& dir);
		~DirP();

		enum DirFlags
		{
			DIR_FILES = 0x0001,			// files
			DIR_DIRS = 0x0002,			// directories
			DIR_HIDDEN = 0x0004,		// hidden files
			DIR_DOTDOT = 0x0008,		// '.' and '..'
			DIR_NO_FOLLOW = 0x0010,     // don't dereference any symlink
			// by default, enumerate everything except '.' and '..'
			DIR_DEFAULT = DIR_FILES | DIR_DIRS | DIR_HIDDEN
		};

		bool Open(const std::string& dirname);
		void Close();
		bool IsOpened() const;

		std::string GetName() const;

		std::string GetNameWithSep() const;

		bool GetFirst(std::string *filename, const std::string &filespec ="",
			int flags = DIR_DEFAULT) const;
		bool GetNext(std::string *filename) const;
		bool HasFiles(const std::string& spec = "") const;
		bool HasSubDirs(const std::string& spec = "") const;

		size_t Traverse(DirTraverser& sink,
			const std::string &filespec = "",
			int flags = DIR_DEFAULT) const;

		static size_t GetAllFiles(const std::string& dirname,
			std::vector<std::string> *files,
			const std::string& filespec = "",
			int flags = DIR_DEFAULT);

		static std::string FindFirst(const std::string& dirname,
			const std::string& filespec,
			int flags = DIR_DEFAULT);

		// ¼ì²âÄ¿Â¼ÊÇ·ñ´æÔÚ
		static bool Exists(const std::string& dir);
		static bool Make(const std::string& dir, int flags = 0);
		static bool Remove(const std::string &dir, int flags = 0);

	private:
		friend class DirData;
		DirData *mData;
	};

}

#endif
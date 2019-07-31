// PX2Environment.hpp

#ifndef PX2ENVIRONMENT_HPP
#define PX2ENVIRONMENT_HPP

#include "PX2CorePre.hpp"
#include "PX2InitTerm.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Environment
	{
		PX2_DECLARE_INITIALIZE;
		PX2_DECLARE_TERMINATE;

	public:
		// 根据字符串获得相应的环境变量
		static std::string GetVariable (const std::string& varName);

		/*
		* 建立一个目录列表.为了平台独立性，使用"/"作为路径分割符.在目录的末尾插
		* 入"/"。如果操作成功，Insert/Remove函数返回真。
		*/
		static int GetNumDirectories ();
		static std::string GetDirectory (int i);
		static bool InsertDirectory (const std::string& directory);
		static bool RemoveDirectory (const std::string& directory);
		static void RemoveAllDirectories ();

		/* 
		* GetPath*函数遍历目录列表，返回完整文件名。如果文件存在，返回真；如果
		* 不存在返回""。GetPathR用来对文件只读访问检测，返回完整路径；GetPathW
		* 对文件进行写访问检测，返回完整路径；GetPathRW对文件进行读写访问检测，
		* 返回完整路径。
		*/
		static std::string GetPath (const std::string& fileName, int mode);
		static std::string GetPathR (const std::string& fileName);
		static std::string GetPathW (const std::string& fileName);
		static std::string GetPathRW (const std::string& fileName);

	private:
		static std::string GetPath (const std::string& filename,
			const char* attributes);

		// 环境变量列表.列表在InitTerm::ExecuteInitializers被调用时被创建。
		static std::vector<std::string>* msDirectories;
	};

	PX2_REGISTER_INITIALIZE(Environment);
	PX2_REGISTER_TERMINATE(Environment);

}

#endif
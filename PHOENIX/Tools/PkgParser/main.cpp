// main.cpp

#include "PkgParser.hpp"
#include "PkgInfoManager.hpp"
using namespace PkgParse;
using namespace std;

int main(int numArguments, char** arguments)
{
#if defined(_MSC_VER)
	// Tell MSVC to report any memory leaks
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	// Use _CrtSetBreakAlloc(n) to find a specific memory leak
#endif

	if (numArguments > 2)
	{
		std::string filename = arguments[1];
		std::string genFilename = arguments[2];

		PkgParser parser;
		parser.ParseFile(filename);
		PkgInfoManager::GetSingleton().CalInherit();

		PkgInfoManager::GetSingleton().Save("pkg.xml");

		//PkgInfoManager::GetSingleton().GenToASFile(genFilename);

		PkgInfoManager::GetSingleton().Ternimate();
	}
	else
	{
		cout << "No File." << endl;
	}
	
	return 0;
}
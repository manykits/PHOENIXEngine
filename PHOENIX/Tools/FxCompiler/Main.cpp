// Main.cpp

#include <iostream>
#include "FxCompiler.hpp"
#include "PX2FString.hpp"

// 命令行
int gNumArguments;
char** gArguments;

int main (int numArguments, char* arguments[])
{
#ifdef PX2_USE_MEMORY
	Memory::Initialize();
#endif

	FString::Initlize();

	InitTerm::ExecuteInitializers();

	// 命令行
	gNumArguments = numArguments;
	if (numArguments > 0)
	{
		gArguments = new1<char*>(numArguments);
		for (int i=0; i<numArguments; ++i)
		{
			gArguments[i] = arguments[i];
		}
	}
	else
	{
		gArguments = 0;
	}

	if (numArguments!=2)
	{
		cout << "numArguments is wrong." << endl;
		return -1;
	}

	// 编译
	FxCompiler *compiler = new0 FxCompiler();
	bool result = compiler->Compile(gArguments[1]);
	if (!result)
	{
		cout << "Compile material failed." << endl;
	}

	// 打印信息
	for (int i=0; i<(int)compiler->Messages.size(); i++)
	{
		cout << compiler->Messages[i].c_str();
	}

	// 退出
	delete0(compiler);

	delete1<char*>(gArguments);
	gArguments = 0;

	InitTerm::ExecuteTerminators();

	FString::Ternimate();

#ifdef PX2_USE_MEMORY
	Memory::Terminate("PX2FxCompiler_MemoryReport.txt", false);
#endif

	return 0;
}
// PX2DLLMain.cpp

#include "PX2MaxClassDesc.hpp"

static int sControlsInit = FALSE;
static PX2::GraphicsRoot *sRoot = 0;
static PX2::ResourceManager *sResMan = 0;
static PX2::Logger *sLogger = 0;
static PX2::EventWorld *sEventWorld = 0;
//----------------------------------------------------------------------------
BOOL WINAPI DllMain (HINSTANCE instance, ULONG dwReason, LPVOID)
{
	// 这个函数被调用时，由Windows DLL被加载
	if (!sControlsInit) 
	{
		sControlsInit = true;

		// Max初始化
#if MAX_RELEASE < 10000
		InitCustomControls(instance); // max 常用控件
#endif
		InitCommonControls(); // windows控件
	}

	switch (dwReason) 
	{
	case DLL_PROCESS_ATTACH:
		PX2MaxClassDesc::msInstance = instance;

#ifdef PX2_USE_MEMORY
		PX2::Memory::Initialize();
#endif

#if MAX_RELEASE < 10000
		InitCustomControls(instance);
#endif
		InitCommonControls();

		PX2::FString::Initlize();
		sLogger = new0 PX2::Logger();

#if defined(_WIN32) || defined(WIN32)
		//sLogger->AddFileHandler("PX2MaxToPX2_Log.txt", PX2::LT_INFO|PX2::LT_ERROR|PX2::LT_USER);
#endif
		sLogger->AddOutputWindowHandler(PX2::LT_INFO|PX2::LT_ERROR|PX2::LT_USER);
		sLogger->StartLogger();

		PX2::PX2_LOG_INFO("Logger Started.");

		sEventWorld = new0 PX2::EventWorld();

		sRoot = new0 PX2::GraphicsRoot();
		sRoot->Initlize();

		sResMan = new0 PX2::ResourceManager();
		break;
	case DLL_PROCESS_DETACH:
		sEventWorld->Shutdown(true);

		sRoot->Terminate();
		delete0(sRoot);

		delete0(sResMan);

		delete0(sEventWorld);

		PX2::PX2_LOG_INFO("Logger Ended.");
		sLogger->CloseLogger();
		delete0(sLogger);
		PX2::FString::Ternimate();

#ifdef PX2_USE_MEMORY
		PX2::Memory::Terminate("MaxToPX2_MemoryReport.txt", false);
#endif
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
//----------------------------------------------------------------------------
__declspec(dllexport) TCHAR const *LibDescription()
{
	return "Phoenix2 3DMax Plugin";
}
//----------------------------------------------------------------------------
__declspec(dllexport) int LibNumberClasses ()
{
	// 函数返回在这个DLL里插件类的个数。
	return 1;
}
//----------------------------------------------------------------------------
__declspec(dllexport) ClassDesc* LibClassDesc (int i)
{
	// 这个函数这个导出插件的描述类
	(void)i;
	return &PX2MaxClassDesc::TheClassDesc;
}
//----------------------------------------------------------------------------
__declspec(dllexport) ULONG LibVersion ()
{
	// 函数返回一个预定义的常量标识Max系统编译的版本号。这个让系统运行时捕获
	// 正确的DLL。
	return VERSION_3DSMAX;
}
//----------------------------------------------------------------------------
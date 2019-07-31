// PX2SceneExport.hpp

#ifndef PX2SCENEEXPORT_HPP
#define PX2SCENEEXPORT_HPP

#include "PX2MaxToPX2PluginPre.hpp"
#include "PX2ExportSettings.hpp"

class PX2SceneExport : public SceneExport
{
public:
	PX2SceneExport ();
	virtual ~PX2SceneExport ();

	// 重载
	virtual int ExtCount ();                    
	virtual const TCHAR* Ext (int iExt);    
	virtual const TCHAR* LongDesc ();           
	virtual const TCHAR* ShortDesc ();          
	virtual const TCHAR* AuthorName ();         
	virtual const TCHAR* CopyrightMessage ();   
	virtual const TCHAR* OtherMessage1 ();      
	virtual const TCHAR* OtherMessage2 ();      
	virtual unsigned int Version ();                
	virtual void ShowAbout (HWND wnd);         
	virtual int DoExport (const TCHAR *filename, ExpInterface *ept,
		Interface* max, BOOL suppressPrompts = FALSE, DWORD options = 0);
	virtual BOOL SupportsOptions (int ext, DWORD options);

	// 用户定义的对话框处理函数
	static BOOL CALLBACK DialogProcedure (HWND wnd, UINT msg,
		WPARAM wParam, LPARAM lParam);

	// 用户定义的对话框消息
	BOOL OnEnableFrameSpinners (HWND wnd, BOOL enabled);
	BOOL OnStartFrameSpinnerChanged (HWND wnd);
	BOOL OnEndFrameSpinnerChanged (HWND wnd);
	BOOL OnInitDialog (HWND wnd);
	BOOL OnMeshChecked (HWND wnd, BOOL enabled);
	BOOL OnTexCoordChecked (HWND wnd, BOOL enabled);
	BOOL OnModifiersChecked (HWND wnd, BOOL enabled);
	BOOL OnObjectsChecked (HWND wnd, BOOL enabled);
	BOOL OnButtonSrcRootDir (HWND wnd);
	BOOL OnButtonDstRootDir (HWND wnd);
	void OnOK (HWND wnd);

private:
	enum { MAXTOPX2_VERSION = 101 };

	// 配置文件
	void ReadConfiguration ();
	void WriteConfiguration ();

	Interface *mMax;
	ExportSettings mSettings, mEffectiveSettings;
	TSTR mStrConfigFile;
};

#endif
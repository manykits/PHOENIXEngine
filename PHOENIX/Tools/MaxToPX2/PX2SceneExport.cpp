// PX2SceneExport.cpp

#include "PX2SceneExport.hpp"
#include "PX2SceneBuilder.hpp"
#include "PX2MaxClassDesc.hpp"
#include "PX2StringHelp.hpp"
#include <Shlobj.h>
#include "resource.h"

//----------------------------------------------------------------------------
PX2SceneExport::PX2SceneExport ()
{
}
//----------------------------------------------------------------------------
PX2SceneExport::~PX2SceneExport ()
{
}
//----------------------------------------------------------------------------
int PX2SceneExport::ExtCount ()
{
	// 导出插件导出的文件扩展名数量，只需要一个（*.px2obj）。
	return 1;
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::Ext (int)
{       
	// 返回文件扩展名。
	return _T("px2obj");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::LongDesc ()
{
	// 文件长描述
	return _T("Phoenix2 model file.");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::ShortDesc () 
{           
	// 文件短描述
	return _T("Phoenix2");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::AuthorName ()
{           
	// 导出插件作者姓名
	return _T("realmany");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::CopyrightMessage () 
{   
	// 版权信息
	return
		_T("Phoenix3D, Inc. Copyright (c) 2009-2012. All Rights Reserved.");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::OtherMessage1 () 
{       
	// 返回附加信息。现在为空。
	return _T("");
}
//----------------------------------------------------------------------------
const TCHAR* PX2SceneExport::OtherMessage2 () 
{       
	// 返回更多的附加信息。现在为空。
	return _T("");
}
//----------------------------------------------------------------------------
unsigned int PX2SceneExport::Version ()
{               
	// 返回导出插件版本号。
	return MAXTOPX2_VERSION;
}
//----------------------------------------------------------------------------
void PX2SceneExport::ShowAbout (HWND)
{           
	// 让导出插件可以显示一个关于对话框。
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::SupportsOptions (int, DWORD)
{
	return TRUE;
}
//----------------------------------------------------------------------------
static DWORD WINAPI ProgressBarCallback(LPVOID)
{
	return 0;
}
//----------------------------------------------------------------------------
int PX2SceneExport::DoExport (const TCHAR *filename, ExpInterface *ept,
							 Interface* max, BOOL suppressPrompts, 
							 DWORD options)
{
	// 从硬盘加载导出插件配置文件，设置用户导出选项。建立并保存Phoenix2的场景图，
	// 并且再次将新的配置写入配置文件。
	//	filename:
	//		用户要保存的Phoenix2场景图文件名。
	//	export:
	//		Max的导出功能指针接口。
	//	max:
	//		Max的一般职能接口。
	//	suppressPrompts:
	//		是否允许用户输入。
	//	options:
	//		确定是导出整个场景，还是导出选择选择的。

	mMax = max;
	INode* exportNode = NULL;
	bool exportSelected;

	if (options == SCENE_EXPORT_SELECTED)
	{
		// 用户选择“导出选择的物体”，确信，选择的节点只有一个。（相当于用户选择
		// 了要到处的场景树的最顶层节点）。
		if (mMax->GetSelNodeCount() != 1)
		{
			assertion (false, "只能导出一个选择节点。");
			return 0;
		}

		exportNode = mMax->GetSelNode(0);
		exportSelected = true;
	}
	else
	{
		// 导出整个场景，从Max的跟节点开始导出
		exportNode = mMax->GetRootNode();
		exportSelected = false;
	}

	// 获得导出插件配置文件所在位置
	mStrConfigFile = mMax->GetDir(APP_PLUGCFG_DIR);
	mStrConfigFile += "\\MaxToPX2.cfg";

	// 读取导出插件的配置信息
	ReadConfiguration();

	// 显示导出插件对话框
	if (!suppressPrompts)
	{
		assertion(PX2MaxClassDesc::msInstance!=0, "msInstance must not be 0");

		INT_PTR piResult = DialogBoxParam(PX2MaxClassDesc::msInstance,
			MAKEINTRESOURCE(IDD_MAXTOPX2_DIALOG), GetActiveWindow(), 
			(DLGPROC)DialogProcedure, (LPARAM)this);

		if (!piResult)
			return 1;
	}

	max->ProgressStart(_T("正在导出:"), TRUE, ProgressBarCallback, NULL);

	std::string allFileName = filename;
	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	PX2::StringHelp::SplitFullFilename(allFileName, outPath, outBaseName, outExtention);
	outExtention = PX2::ToLower<std::string>(outExtention);
	std::string lastFilename = outPath + outBaseName + "." + outExtention;

	// 将Max场景转换到Phoenix2场景
	SceneBuilder(lastFilename.c_str(), exportSelected, &mEffectiveSettings, ept,
		max, exportNode);

	max->ProgressEnd();

	// 保存导出插件的配置信息
	WriteConfiguration();

	return 1;
}
//----------------------------------------------------------------------------
void PX2SceneExport::ReadConfiguration ()
{
	// 从硬盘加载配置文件，设置导出插件的配置信息。如果配置文件不存在，
	// 使用缺省配置。

	// 从磁盘读取导出插件的配置文件。
	FILE *configFile = fopen(mStrConfigFile, "rbS");
	if (configFile == NULL)
	{
		return;
	}

	int version;
	if (!fread(&version, sizeof(version), 1, configFile))
	{
		assertion(false, "MaxToPX2 config file is damaged.");
		fclose(configFile);
		return;
	}

	if (version != MAXTOPX2_VERSION)
	{
		fclose(configFile);
		return;
	}

	// 读取配置设置
	ExportSettings settings;
	if ( fread(&settings, sizeof(settings), 1, configFile) != 1 )
	{
		assertion(false, "MaxToPX2 config file is damaged.");
		fclose(configFile);
		return;
	}

	// 覆盖缺省配置
	memcpy(&mSettings, &settings, sizeof(settings));
}
//----------------------------------------------------------------------------
void PX2SceneExport::WriteConfiguration ()
{
	FILE *configFile = fopen(mStrConfigFile, "wbS");
	if (configFile == NULL)
	{
		// 不能创建配置文件（可能是目录写保护了！）
		assertion(false, "Can't create configFile.");
		return;
	}

	// 写入版本号
	int version = MAXTOPX2_VERSION;
	if (fwrite(&version, sizeof(version), 1, configFile) != 1)
	{
		assertion(false, "Can't write configFile.");
		fclose(configFile);
		return;
	}

	// 写入导出插件的配置信息
	if (fwrite(&mSettings, sizeof(mSettings), 1, configFile) != 1)
	{
		assertion(false, "Can't write configFile.");
		fclose(configFile);
		return;
	}

	fclose(configFile);
}
//----------------------------------------------------------------------------
BOOL CALLBACK PX2SceneExport::DialogProcedure (HWND hWnd, UINT uiMsg,
											   WPARAM wParam, LPARAM lParam) 
{
	static PX2SceneExport *exporter = 0;

	if (uiMsg == WM_INITDIALOG)
	{
		exporter = (PX2SceneExport*)lParam;

		CenterWindow(hWnd, GetParent(hWnd)); 
		SetFocus(hWnd);

		// 初始化每个实例的对话框窗口
		return ((PX2SceneExport*)lParam)->OnInitDialog(hWnd);
	}

	switch (uiMsg)
	{
	case WM_COMMAND:
		// 用户改变了导出选项。
		switch (LOWORD(wParam)) 
		{
		case IDC_CHECK_MESHES:
			return exporter->OnMeshChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES));
		case IDC_CHECK_TEXTURECOORDS:
			return exporter->OnTexCoordChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_TEXTURECOORDS));
		case IDC_CHECK_OBJECTS:
			return exporter->OnObjectsChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_OBJECTS));
		case IDC_CHECK_MODIFIERS:
			return exporter->OnModifiersChecked(hWnd,
				IsDlgButtonChecked(hWnd, IDC_CHECK_MODIFIERS));
		case IDC_RADIO_ALLFRAMES:
		case IDC_RADIO_KEYFRAMES:
			return exporter->OnEnableFrameSpinners(hWnd, TRUE);
		case IDC_RADIO_CURRENTFRAME:
			return exporter->OnEnableFrameSpinners(hWnd, FALSE);
		case IDC_BUTTON_SRC:
			return exporter->OnButtonSrcRootDir(hWnd);
		case IDC_BUTTON_DST:
			return exporter->OnButtonDstRootDir(hWnd);
		case IDOK:
			exporter->OnOK(hWnd);
			EndDialog(hWnd,1);
			break;
		case IDCANCEL:
			EndDialog(hWnd,0);
			break;
		default:
			return FALSE;
		}
		break;

	case CC_SPINNER_CHANGE:
		// 用户改变了spinner控件
		switch( LOWORD(wParam) )
		{
		case IDC_SPINNER_ENDFRAME:
			return exporter->OnEndFrameSpinnerChanged(hWnd);
		case IDC_SPINNER_STARTFRAME:
			return exporter->OnStartFrameSpinnerChanged(hWnd);
		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd,0);
		return TRUE;
	}

	return FALSE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnEnableFrameSpinners (HWND hWnd, BOOL bEnabled)
{
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME)); 
	spinner->Enable(bEnabled);
	ReleaseISpinner(spinner); 
	spinner = GetISpinner(GetDlgItem(hWnd, IDC_SPINNER_ENDFRAME)); 
	spinner->Enable(bEnabled);
	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnStartFrameSpinnerChanged (HWND hWnd)
{
	// start frame
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME)); 
	int startFrame = spinner->GetIVal();
	ReleaseISpinner(spinner);

	// end frame
	spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME)); 
	int endFrame = spinner->GetIVal();
	if ( endFrame < startFrame )
		spinner->SetValue(startFrame,FALSE);
	ReleaseISpinner(spinner); 

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnEndFrameSpinnerChanged (HWND hWnd)
{
	// start frame
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME)); 
	int startFrame = spinner->GetIVal();
	ReleaseISpinner(spinner);

	// start frame < end fream
	spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME)); 
	int endFrame = spinner->GetIVal();

	if ( startFrame > endFrame )
		spinner->SetValue(startFrame,FALSE);

	ReleaseISpinner(spinner); 
	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnInitDialog (HWND hWnd)
{
	// 当对话框获得WM_INITDIALOG消息时，进行初始化。

	// 设置用户定义的导出选项。如果配置文件存在，这些导出选项和用户最后一次
	// 使用导出插件的选项一样。否则，使用缺省的设置。
	CheckDlgButton(hWnd,IDC_CHECK_OBJECTS,mSettings.IncludeObjects);
	CheckDlgButton(hWnd,IDC_CHECK_LIGHTS,mSettings.IncludeLights);
	CheckDlgButton(hWnd,IDC_CHECK_MESHES,mSettings.IncludeMeshes);
	CheckDlgButton(hWnd,IDC_CHECK_VERTEXCOLORS,mSettings.IncludeVertexColors);
	CheckDlgButton(hWnd,IDC_CHECK_NORMALS,mSettings.IncludeNormals);    
	CheckDlgButton(hWnd, IDC_CHECK_TANGENTBINORMAL, 
		mSettings.IncludeTargentBiNormal);
	CheckDlgButton(hWnd,IDC_CHECK_TEXTURECOORDS,mSettings.IncludeTexCoords);

	SendMessage(GetDlgItem(hWnd,IDC_COMMBO_NUMTCS),CB_ADDSTRING,0,(LPARAM)"1");
	SendMessage(GetDlgItem(hWnd,IDC_COMMBO_NUMTCS),CB_ADDSTRING,0,(LPARAM)"2");
	SendMessage(GetDlgItem(hWnd,IDC_COMMBO_NUMTCS), CB_SETCURSEL, 
		mSettings.NumTexCoords-1,0);

	for (int i = 0; i < (int)mSettings.MtlTypes.size(); i++)
	{
		std::string text = mSettings.MtlTypes[i];
		SendMessage(GetDlgItem(hWnd, IDC_COMMBO_MTLTYPE), CB_ADDSTRING, 0, (LPARAM)text.c_str());
	}
	SendMessage(GetDlgItem(hWnd, IDC_COMMBO_MTLTYPE), CB_SETCURSEL, mSettings.MtlType, 0);

	CheckDlgButton(hWnd,IDC_CHECK_MODIFIERS,mSettings.IncludeModifiers);
	CheckDlgButton(hWnd,IDC_CHECK_SKINS,mSettings.IncludeSkins);
	CheckDlgButton(hWnd,IDC_RADIO_CURRENTFRAME,mSettings.IncludeCurrentFrame);
	CheckDlgButton(hWnd,IDC_RADIO_KEYFRAMES,mSettings.IncludeKeyFrames);
	CheckDlgButton(hWnd,IDC_RADIO_ALLFRAMES,mSettings.IncludeAllFrames);
	CheckDlgButton(hWnd,IDC_CHECK_USELOCALTIME,mSettings.UseLocalTime);

	if(!mSettings.IncludeObjects)
	{
		// 取消激活“Objects”选择框
		OnObjectsChecked(hWnd, FALSE);
	}
	else if (!mSettings.IncludeMeshes)
	{
		// 取消激活“Mesh”选择框
		OnMeshChecked(hWnd, FALSE);
	}

	// 取消激活"Modifiers"选择框
	if (!mSettings.IncludeModifiers)
		OnModifiersChecked(hWnd, FALSE);

	// 获得max场景中的帧数
	int startFrame = mMax->GetAnimRange().Start()/GetTicksPerFrame();
	int endFrame = mMax->GetAnimRange().End()/GetTicksPerFrame();
	int numFrames = mMax->GetAnimRange().Duration()/GetTicksPerFrame();
	PX2_UNUSED(numFrames);

	// spinner controls
	ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
		IDC_SPINNER_STARTFRAME));
	spinner->LinkToEdit(GetDlgItem(hWnd,IDC_EDIT_STARTFRAME), EDITTYPE_INT);
	spinner->SetLimits(0, endFrame, TRUE);
	spinner->SetScale(1.0f);
	spinner->SetValue(startFrame, FALSE);
	if ( mSettings.IncludeCurrentFrame )
	{
		spinner->Enable(FALSE);
	}
	ReleaseISpinner(spinner);

	spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME));
	spinner->LinkToEdit(GetDlgItem(hWnd,IDC_EDIT_ENDFRAME),EDITTYPE_INT);
	spinner->SetLimits(0, endFrame,TRUE);
	spinner->SetScale(1.0f);
	spinner->SetValue(endFrame,FALSE);
	if (mSettings.IncludeCurrentFrame)
	{
		spinner->Enable(FALSE);
	}
	ReleaseISpinner(spinner);

	SetDlgItemText(hWnd, IDC_EDIT_SRCROOTDIR, mSettings.SrcRootDir);
	SetDlgItemText(hWnd, IDC_EDIT_DSTROOTDIR, mSettings.DstRootDir);

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnMeshChecked (HWND hWnd, BOOL bEnabled)
{
	// “Mesh”选择框被更新，更新子选择框
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_VERTEXCOLORS), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_NORMALS), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TANGENTBINORMAL), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTURECOORDS), bEnabled);
	OnTexCoordChecked(hWnd, bEnabled);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnTexCoordChecked (HWND wnd, BOOL enabled)
{
	EnableWindow(GetDlgItem(wnd,IDC_COMMBO_NUMTCS), enabled);
	EnableWindow(GetDlgItem(wnd,IDC_STATIC_NUMTC), enabled);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnModifiersChecked (HWND hWnd, BOOL bEnabled)
{
	// “Modifiers”选择框被更新，更新子选择框
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_SKINS), bEnabled);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnObjectsChecked (HWND hWnd, BOOL bEnabled)
{
	// 当“Objects”被更新，更新子选择框
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_MESHES), bEnabled);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_LIGHTS), bEnabled);   

	// “Mesh”选择框被跟新，更新子选择框
	OnMeshChecked(hWnd,bEnabled && IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES));

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
static int SelectFolder (HWND hParent, char* folder)
{
	LPMALLOC lpMalloc;

	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return 0;

	char szDisplayName[_MAX_PATH];
	char szBuffer[_MAX_PATH];

	ITEMIDLIST *ppidl;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP ,&ppidl);

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hParent;
	browseInfo.pidlRoot = ppidl; // set root at Desktop
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = "选择目录";
	browseInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
	browseInfo.lpfn = NULL; //BrowseCallbackProc;
	browseInfo.lParam = 0;

	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the    item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
			// At this point, szBuffer contains the path the user chose.
			if (szBuffer[0] == '\0')
				return 0;

			// We have a path in szBuffer! Return it.
			strcpy(folder, szBuffer);
			size_t len = strlen(folder);
			if(folder[len]=='\\')
				folder[len] = 0;

			lpMalloc->Free(lpItemIDList);
			lpMalloc->Release();

			return 1;
		}
	}

	return 1;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnButtonSrcRootDir (HWND wnd)
{
	SelectFolder(wnd, mSettings.SrcRootDir);
	SetDlgItemText(wnd, IDC_EDIT_SRCROOTDIR, mSettings.SrcRootDir);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL PX2SceneExport::OnButtonDstRootDir (HWND wnd)
{
	SelectFolder(wnd, mSettings.DstRootDir);
	SetDlgItemText(wnd, IDC_EDIT_DSTROOTDIR, mSettings.DstRootDir);

	WriteConfiguration();

	return TRUE;
}
//----------------------------------------------------------------------------
void PX2SceneExport::OnOK (HWND hWnd)
{
	// 当用户按下对话框的OK键。获得导出插件的导出选项。

	mSettings.IncludeObjects =
		IsDlgButtonChecked(hWnd, IDC_CHECK_OBJECTS) == 1;
	mSettings.IncludeLights =
		IsDlgButtonChecked(hWnd, IDC_CHECK_LIGHTS) == 1;
	mSettings.IncludeMeshes =
		IsDlgButtonChecked(hWnd, IDC_CHECK_MESHES) == 1;
	mSettings.IncludeVertexColors =
		IsDlgButtonChecked(hWnd, IDC_CHECK_VERTEXCOLORS) == 1;
	mSettings.IncludeNormals =
		IsDlgButtonChecked(hWnd, IDC_CHECK_NORMALS) == 1;
	mSettings.IncludeTargentBiNormal =
		IsDlgButtonChecked(hWnd, IDC_CHECK_TANGENTBINORMAL) == 1;
	mSettings.IncludeTexCoords =
		IsDlgButtonChecked(hWnd, IDC_CHECK_TEXTURECOORDS) == 1;
	mSettings.NumTexCoords = 
		1 + (int)SendMessage(GetDlgItem(hWnd, IDC_COMMBO_NUMTCS), CB_GETCURSEL, 0, 0);

	mSettings.MtlType = 
		(int)SendMessage(GetDlgItem(hWnd, IDC_COMMBO_MTLTYPE), CB_GETCURSEL, 0, 0);

	mSettings.IncludeModifiers =
		IsDlgButtonChecked(hWnd, IDC_CHECK_MODIFIERS) == 1;
	mSettings.IncludeSkins =
		IsDlgButtonChecked(hWnd, IDC_CHECK_SKINS) == 1;
	mSettings.IncludeAllFrames =
		IsDlgButtonChecked(hWnd, IDC_RADIO_ALLFRAMES) == 1;
	mSettings.IncludeKeyFrames =
		IsDlgButtonChecked(hWnd, IDC_RADIO_KEYFRAMES) == 1;
	mSettings.IncludeCurrentFrame =
		IsDlgButtonChecked(hWnd, IDC_RADIO_CURRENTFRAME) == 1;
	mSettings.UseLocalTime =
		IsDlgButtonChecked(hWnd, IDC_CHECK_USELOCALTIME) == 1;

	// Start and End frames
	if ( mSettings.IncludeAllFrames || mSettings.IncludeKeyFrames )
	{
		ISpinnerControl* spinner = GetISpinner(GetDlgItem(hWnd,
			IDC_SPINNER_STARTFRAME)); 
		mSettings.StartFrame = spinner->GetIVal();
		ReleaseISpinner(spinner);

		spinner = GetISpinner(GetDlgItem(hWnd,IDC_SPINNER_ENDFRAME)); 
		mSettings.EndFrame = spinner->GetIVal();
		ReleaseISpinner(spinner);
	}

	mEffectiveSettings = mSettings;
	if (!mSettings.IncludeObjects)
	{
		mEffectiveSettings.IncludeLights = false;
		mEffectiveSettings.IncludeMeshes = false;
	}
	if (!mEffectiveSettings.IncludeMeshes)
	{
		mEffectiveSettings.IncludeVertexColors = false;
		mEffectiveSettings.IncludeNormals = false;
		mEffectiveSettings.IncludeTargentBiNormal = false;
		mEffectiveSettings.IncludeTexCoords = false;
	}

	if (!mSettings.IncludeModifiers)
	{
		mEffectiveSettings.IncludeSkins = false;
	}

	GetDlgItemText(hWnd, IDC_EDIT_SRCROOTDIR, mSettings.SrcRootDir, 256);
	GetDlgItemText(hWnd, IDC_EDIT_DSTROOTDIR, mSettings.DstRootDir, 256);

	WriteConfiguration();
}
//----------------------------------------------------------------------------
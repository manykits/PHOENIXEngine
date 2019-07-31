// SamplePlugin.hpp

#ifndef FBXIMPORTERPLUGIN_HPP
#define FBXIMPORTERPLUGIN_HPP

#include "PX2Plugin.hpp"
#include "SamplePre.hpp"

#include "PX2UI.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIColorSelect.hpp"
#include "PX2UIList.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Controller.hpp"
#include "PX2Animation.hpp"
#include "PX2AnimationFrames.hpp"
#include "PX2ModelController.hpp"
#include "PX2SizeNode.hpp"
#include "PX2InterpCurveUniformScaleCtrl.hpp"
#include "PX2VoiceSDK.hpp"
#include "PX2ScriptContext.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2Bluetooth.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2Application.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{
	class SAMPLE_DLL_ITEM SamplePlugin : public Plugin, public EventHandler, public Singleton<SamplePlugin>
	{
	public:
		SamplePlugin();
		virtual ~SamplePlugin();
		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();

		// 添加数据
		void PrintMsg(const std::string &str);
		// 是否在界面显示
		virtual void Show();
		// 按键ABCD显示
		virtual void ShowABCD();
		// “创建框”的显示
		virtual void ShowProject();
		// “创建框”的创建
		virtual void OnCreateProject();

	protected:
		// 场景运动
		virtual void _Play();
		
		// 事件响应
		virtual void OnEvent(Event *event);

		// 添加音乐
		void AddMusic();
		
		// 主按钮设置
		void MainButton();

		// 添加菜单框属性
		void AddMenuBoxProperties();

		// 添加框架类型
		void AddFrameType();

		// “关于”框
		void OnCreateUIFrameAbout();

		// “框架”框
		void OnCreateUIFrame();

		// “图片”框
		void OnCreateUIFPicBox();

		// “文字”框
		void OnCreateUIFText();

		// “按钮”框
		void OnCreateUIButton();

		// “判断”框
		void OnCreateUICheckBox();

		// “选择”框
		void OnCreateUIComboBox();


		virtual void Progressmove();
		
		//是否创建新项目
		bool mIsNewProject;

	private:
		static Plugin *sPlugin;

		UI* mUI;
		//按钮
		UIButton* mButBag;
		//背景
		UIFrame* frame;
		UIFrame* MenuBg;
		//菜单框
		UITabFrame* mMenuCase;
		//关于框
		UIFrame* mFrameAbout;
		UIFText* mAboutfText;
		//框架框
		UIFrame* frameUIFrame;
		//图片框
		UIFrame* frameUIFPicBox;
		//文字框
		UIFrame* frameUIFText;
		//按钮框
		UIFrame* frameUIButtom;
		//判断框
		UIFrame* frameUICheckBox;
		//选择框
		UIFrame* frameUIComboBox;
		//编辑框
		UIFrame* frameUIEditBox;
		//进度条
		UIFrame* frameUIProgress;
		//颜色选择
		UIFrame* frameUIColorSelect;
		//列表
		UIFrame* frameUIList;
		//树
		UIFrame* frameUITree;
		//分隔线
		UIFrame* frameUISplitter;
		//滑动条
		UIFrame* frameUISlider;
		//方向盘
		UIFrame* frameUIRound;
		//服务器
		UIFrame* frameServer;
		//添加一个链表输入数据
		UIList* MsgList;
		//按钮A
		UIButton* ButtonA;
		//按钮B
		UIButton* ButtonB;
		//按钮C
		UIButton* ButtonC;
		//按钮D
		UIButton* ButtonD;
		//打开按钮
		UIFrame* ProjectBg;
		UIButton* ButtonMenu;
		//智能机器人背景
		UIFrame* background;
		//外圆
		UIFrame* circleOut;
		//内圆
		UIFrame* circleIn;
		//圆中心
		UIButton* uiBtn;
		//圆中心的太极贴图
		UIFrame* yiBackground;
		//文本
		UIFText* text;
		//外圆控制
		InterpCurveUniformScaleController* ctrlOut;
		InterpCurveUniformScaleController* circleOutCtrl;
		//内圆控制
		InterpCurveUniformScaleController* ctrlIn;
		InterpCurveUniformScaleController* circleInCtrl;
		//圆脸控制
		InterpCurveUniformScaleController* ctrlBtn;
		InterpCurveUniformScaleController* circleBtnCtrl;
		//滑动条1
		UIProgressBar* progressBar1;
		float ch_MT;
		float ch_LT;
		float ch_LastDelay;
	};
}
#endif
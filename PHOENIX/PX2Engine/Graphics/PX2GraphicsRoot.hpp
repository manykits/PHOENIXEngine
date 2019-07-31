// PX2GraphicsRoot.hpp

#ifndef PX2GRAPHICSROOT_HPP
#define PX2GRAPHICSROOT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Size.hpp"
#include "PX2Camera.hpp"
#include "PX2Light.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Rect.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2Canvas.hpp"
#include "PX2FString.hpp"
#include "PX2Projector.hpp"
#include "PX2TriMesh.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2Controlledable.hpp"

namespace PX2
{

	class TexPack;
	class TexPackElement;

	typedef Object *(*ObjectLoadFun) (const char *filename);
	typedef Object *(*ObjectLoadFun1) (const char *filename, bool shareVB, bool shareIB, bool shareM);
	typedef bool (*BufferLoadFun)(const char *filename, int &bufferSize,
		char* &buffer);

	typedef bool (*AddTexPackFun)(const std::string &textPackPath);
	typedef const TexPack &(*GetTexPackFun)(const std::string &textPackPath);
	typedef const TexPackElement &(*GetTexPackElementFun)(const std::string &texPackPath, 
		const std::string &eleName);

	class Renderer;
	class VisibleSet;

	class PX2_ENGINE_ITEM GraphicsRoot : public Singleton<GraphicsRoot>
	{
	public:
		GraphicsRoot ();
		virtual ~GraphicsRoot ();

		bool Initlize ();
		bool Terminate ();

		void Update(double appSeconds, double elapsedSeconds);
		void Draw();
		
		void SetInEditor (bool isInEditor);
		bool IsInEditor () const;

		void SetUIShareDraw(bool shareDraw);
		bool IsUIShareDraw() const;

		const static std::string sEmptyResPath;
		const static std::string sTerResPath;

		void SendGeneralEvent(const std::string &eventDataStr0, float timeDelay=0.0f);
		void SendGeneralEvent(const std::string &eventDataStr0, 
			const std::string &eventDataStr1, float timeDelay = 0.0f);

	private:
		bool mIsInEditor;
		bool mIsUIShareDraw;

		// Environment
	public:
		// 环境参数是用来保存当前要渲染的3D物件，所收到的光照，雾的信息。
		// 每一个Scene中都会与一个环境参数，在加载Scene的时候设置该Scene的环境
		// 参数到当前。
		// 有一种情况你需要同时渲染两个场景，比如在编辑器中，的主场景和，
		// PreView场景，你想需要他们有不同环境感觉，需要在加载和渲染该物体的之
		// 前设置一下当前环境参数。在渲染完成之后，再置回原来的参数。
		void SetCurEnvirParamController(EnvirParamController *param);
		EnvirParamController *GetCurEnvirParamController();

	public_internal:
		void SetCurUpdateCamera(Camera *camera);
		Camera *GetCurUpdateCamera();

	protected:
		EnvirParamControllerPtr mCurEnvirParamController;
		CameraPtr mCurUpdateCamera;

		//window
	public:
		bool AddRenderWindow(const std::string &name, RenderWindow *rw);
		bool IsHasRenderWindow(const std::string &name);
		bool RemoveRenderWindow(const std::string &name);
		RenderWindow *GetRenderWindow(const std::string &name);
		RenderWindow *GetMainWindow();

	protected:
		std::map<std::string, RenderWindowPtr> mRenderWindows;
		RenderWindowPtr mMainRenderWindow;

		// Play
	public:
		enum PlayType
		{
			PT_NONE,
			PT_PLAY,
			PT_MAX_TYPE
		};
		void SetPlayType(PlayType type);
		PlayType GetPlayType() const;

	protected:
		PlayType mPlayType;

		// VertexFormat
	public:
		enum VertexFormatType
		{
			VFT_P,
			VFT_PC,
			VFT_PT1,
			VFT_PT2,
			VFT_PCT1,
			VFT_PCT2,
			VFT_PN,
			VFT_PNT1,
			VFT_PNT2,
			VFT_PCN,
			VFT_PCNT1,
			VFT_PCNT2,
			VFT_PNT1TB,
			VFT_MAX_TYPE
		};

		VertexFormat *GetVertexFormat(VertexFormatType type);

	protected:
		std::map<VertexFormatType, VertexFormatPtr > mCreatedVFs;

		// HelpMesh
	public:
		TriMesh *GetXYPlane();
		TriMesh *GetXZPlane();
		TriMesh *GetYZPlane();

	protected:
		Pointer0<TriMesh> mTriMeshXY;
		Pointer0<TriMesh> mTriMeshXZ;
		Pointer0<TriMesh> mTriMeshYZ;

		// Load
	public:
		static void SetUserLoadFun(ObjectLoadFun userLoadFun);
		static ObjectLoadFun msUserLoadFun;

		static void SetUserLoadFun1(ObjectLoadFun1 userLoadFun);
		static ObjectLoadFun1 msUserLoadFun1;

		static void SetBufferLoadFun(BufferLoadFun bufferLoadFun);
		static BufferLoadFun msBufferLoadFun;

		static void SetAddTexPackFun(AddTexPackFun fun);
		static AddTexPackFun msAddTexPackFun;

		static void SetGetTexPackFun(GetTexPackFun fun);
		static GetTexPackFun msGetTexPackFun;

		static void SetGetTexPackElementFun(GetTexPackElementFun fun);
		static GetTexPackElementFun msGetTexPackElementFun;

	public:
		const std::string *GetShaderStr(const char *filename);
	
	protected:
		std::map<FString, std::string> mShadersMap;

		// Info
	public:
		void LogInfo(const std::string &infoStr);

		// Untity
	public:
		void PlayScale(Controlledable *contrable);
		void PlayNormal(Controlledable *contrable);
	};

#include "PX2GraphicsRoot.inl"

#define PX2_GR GraphicsRoot::GetSingleton()

}

#endif
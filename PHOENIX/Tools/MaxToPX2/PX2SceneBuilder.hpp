// PX2SceneBuilder.hpp

#ifndef PX2SCENEBUILDER_HPP
#define PX2SCENEBUILDER_HPP

#include "PX2MaxToPX2PluginPre.hpp"
#include "PX2ExportSettings.hpp"
#include "PX2Mtls.hpp"
#include "PX2MtlTree.hpp"
#include "PX2UniMaterialMesh.hpp"

class SceneBuilder
{
public:
	/// 构造函数
	/**
	@param filename:
	场景要保存的文件名。
	@param exportSelected:
	true: 保存选择的节点
	false: 保存整个场景	
	@param settings:
	保存着导出选项
	@param export:
	Max的导出插件的导出方法接口
	@param max:
	Max的常规职能接口
	@param pkRootNode:
	导出的场景的最顶级别节点（如果用户选择“Export Selected”，就可以
	不是Max场景的根节点）。
	*/
	SceneBuilder (const TCHAR* filename, BOOL exportSelected, 
		ExportSettings *settings, ExpInterface *ept, Interface *max,
		INode *exportNode);
	~SceneBuilder ();

private:
	class AnimationTiming
	{
	public:
		AnimationTiming () : Active(FALSE), Start(0), End(0) { /**/ }

		BOOL Active;
		TimeValue Start;
		TimeValue End;
	};

	class ModifierInfo
	{
	public:
		INode *Node;
		std::vector<Modifier*> Modifiers;
	};

	enum KeyType
	{
		KT_TRANSLATION,
		KT_ROTATION,
		KT_SCALE
	};

	class KeyInfo
	{
	public:
		KeyInfo (TimeValue iTime, KeyType eType)
		{
			Time = iTime;
			Type = eType;
		}

		TimeValue Time;
		KeyType Type; 
	};
	
	// Traverse and build
	bool Traverse (INode *maxNode, PX2::Node *relatParent); // movbale对应maxNode的父节点
	PX2::Movable *BuildGeometry (INode *maxNode, PX2::Node *relatParent, bool hasSkin);
	PX2::Node *BuildNode (INode *maxNode, PX2::Node *relatParent);
	PX2::Movable *BuildMesh (INode *maxNode, PX2::Node *relatParentOrEqualNode, bool hasSkin);
	PX2::Transform GetLocalTransform (INode *node, TimeValue iTime);

	// Mesh
	TriObject* GetTriObject (INode* maxNode, bool *needDel);
	void SplitGeometry (Mesh *maxMesh, int mtlIndex, std::vector<UniMaterialMesh*>
		&uMeshes);
	PX2::Float3 GetVertexNormal (Mesh *maxMesh, int faceIndex, int vertex);
	void PackColors (UniMaterialMesh *mesh, Mesh *maxMesh, 
		std::vector<int> &faceIndexParts);
	void PackAlphas (UniMaterialMesh *mesh, Mesh *maxMesh, 
		std::vector<int> &faceIndexParts);
	void PackTextureCoords (UniMaterialMesh *mesh, Mesh *maxMesh, 
		std::vector<int> &faceIndexParts);
	void PackVertices (UniMaterialMesh *mesh, Mesh *maxMesh, 
		std::vector<int> &faceIndexParts, PX2::Float3 *normal);

	// Material
	void CollectMaterials (INode *node);
	void ConvertMaterials ();
	void ConvertMaterial (Mtl &mtl, MtlTree &mtlTree);
	/*void ConvertTexture (Texmap &tex, Class_ID classID, int subNo, TexTree &tree);*/

private:
	Mtls mMtls; // 收集Max中所有Mtl 
	std::vector<MtlTree> mMtlTreeList;

	static const TCHAR* msMapName[NTEXMAPS];
	static const char* msEnvName[5];
	static const TCHAR* msMapEnvironment;
	static const TCHAR* msMapGeneric;

	// Material 
	bool IsValidName (const char* str);

	// Light
	void BuildAmbientLight ();
	void BuildLight (INode *maxNode, PX2::Movable *movbale);
	PX2::APoint GetLightLocation (INode *node);
	PX2::Light *BuildPointLight (INode *node);
	PX2::Light *BuildSpotLight (INode *node, LightState &lightState);
	PX2::Light *BuildDirectionalLight (INode *node);

	// Animation
	static bool AreEqual (const Point3 &point1, const Point3 &point2);
	static bool AreEqual (const Quat &quat1, const Quat &quat2);
	static bool CompareKeyTimes (KeyInfo *first, KeyInfo *second);
	void BuildKeyFrameController (INode *node, PX2::Movable *movable);
	void BuildFrameController (INode *node, PX2::Movable *movable);
	bool GetAnimationTiming (INode *node, AnimationTiming &tTiming,
		AnimationTiming &rTiming, AnimationTiming &sTiming);
	void GetTrnKeyInfo (int &numKeys, Class_ID classID, IKeyControl *keyCtl,
		AnimationTiming &tTiming, std::vector<KeyInfo*> &keyInfos);
	void GetRotKeyInfo (int &numKeys, Class_ID classID, IKeyControl *keyCtl, 
		AnimationTiming &rTiming, std::vector<KeyInfo*> &keyInfos);
	void GetScaleKeyInfo (int &numKeys, Class_ID classID, IKeyControl *keyCtl,
		AnimationTiming &sTiming, std::vector<KeyInfo*> &keyInfos);

	// Modifier
	void ApplyModifiers ();
	void CollectModifiers (INode *node, std::vector<Modifier*> &modifiers);
	bool IsNodeHasModifierSkin (INode *node);
	int GetNumModifiers (INode *node);
	void ProcessSkin (INode *node, Modifier *skinMod);
	void ProcessPhysique (INode *node, Modifier *phyMod);

	TCHAR *mFileName;
	BOOL mExportSelected;
	ExportSettings *mSettings;
	ExpInterface *mExport;
	Interface *mMax;
	INode *mExportMaxNode;
	int mTicksPerFrame;
	TimeValue mTimeStart;
	TimeValue mTimeEnd;
	TimeValue mTimeOffset;
	PX2::NodePtr mScene;
	PX2::LightPtr mAmbientLight;
	std::vector<PX2::LightPtr> mLights;
	std::vector<ModifierInfo*> mModifierList;
	
	static const float ATTENUATION;
	static const float MIN_DIFFERENCE;
};

#endif
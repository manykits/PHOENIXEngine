// PX2Scene.hpp

#ifndef PX2SCENE_HPP
#define PX2SCENE_HPP

#include "PX2Node.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2AmbientRegionController.hpp"
#include "PX2CameraNode.hpp"
#include "PX2Terrain.hpp"
#include "PX2AIAgentWorld.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Scene : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(Scene);
		PX2_DECLARE_STREAM(Scene);

	public:
		Scene();
		virtual ~Scene();

		static int NextID(int solt);

		// Environment
		EnvirParamController *GetEnvirParamController();
		AmbientRegionController *GetAmbientRegionController();

		void SetTerrain(Terrain *terrain);
		Terrain *GetTerrain();

		std::vector<CameraPtr> &GetCameras();

		void SetMainCameraNode(CameraNode *camNode);
		CameraNode *GetMainCameraNode();

		void AdjustCameraFrustum(const Sizef &canvasSize);

	protected:
		static void _CollectCameras(Scene *scene, Node *node);
		static const int sNumIDGroup = 10;
		static int sNextIDs[sNumIDGroup];

		bool AddCamera(Camera *camera);
		void ClearCameras();
		bool RemoveCamera(Camera *camera);
		virtual void UpdateWorldData(double applicationTime, 
			double elapsedTime);

		EnvirParamControllerPtr mEnvirParam;
		AmbientRegionControllerPtr mAmbientRegionController;
		TerrainPtr mTerrain;
		std::vector<CameraPtr> mCameras;
		CameraNodePtr mMainCameraNode;

		bool mIsNeedReCalCameras;

	public:
		virtual void OnEvent(Event *ent);

		// Paths
	public:
		Node *GetPathRoot();

	private:
		NodePtr mPathRoot;

		// Areas
	public:
		Node *GetAreaRoot();

	private:
		NodePtr mAreaRoot;

		// AI
	public:
		AIAgentWorld *GetAIAgentWorld();

	private:
		AIAgentWorldPtr mAIAgentWorld;
	};

#include "PX2Scene.inl"
	PX2_REGISTER_STREAM(Scene);
	typedef Pointer0<Scene> ScenePtr;

}

#endif
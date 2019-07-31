// PX2ProjectEventController.cpp

#include "PX2ProjectEventController.hpp"
#include "PX2EngineSceneCanvas.hpp"
#include "PX2Canvas.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2Scene.hpp"
#include "PX2UI.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ProjectEventController::ProjectEventController()
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
ProjectEventController::~ProjectEventController()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void ProjectEventController::OnEvent(Event *event)
{
	Canvas *canvas = DynamicCast<Canvas>(GetControlledable());
	EngineSceneCanvas *engineSceneCanvas = DynamicCast<EngineSceneCanvas>(canvas);

	if (canvas)
	{
		const std::string &name = canvas->GetName();

		if ("ProjectSceneCanvas" == name)
		{
			if (ProjectES::IsEqual(event, ProjectES::NewScene))
			{
				Scene *scene = event->GetData<Scene*>();

				canvas->RemoveAllCameras();
				std::vector<CameraPtr> &cams = scene->GetCameras();
				for (int i = 0; i < (int)cams.size(); i++)
				{
					canvas->AddCamera(cams[i]);
				}

				canvas->SetRenderNode(scene);
				if (engineSceneCanvas)
				{
					engineSceneCanvas->SetShadowMapConfigChanged(true);
					engineSceneCanvas->SetBloomConfigChanged(true);
				}
			}
			else if (ProjectES::IsEqual(event, ProjectES::CloseScene))
			{
				canvas->SetRenderNode(0);
				canvas->RemoveAllCameras();
				if (engineSceneCanvas)
				{
					engineSceneCanvas->SetShadowMapConfigChanged(true);
					engineSceneCanvas->SetBloomConfigChanged(true);
				}
			}			
			else if (ProjectES_Internal::IsEqual(event, ProjectES_Internal::SceneUICamerasChanged))
			{
				Object *obj = event->GetData<Object*>();
				Scene *scene = DynamicCast<Scene>(obj);
				if (scene && scene == canvas->GetRenderNode())
				{
					canvas->RemoveAllCameras();
					std::vector<CameraPtr> &cams = scene->GetCameras();
					for (int i = 0; i < (int)cams.size(); i++)
					{
						canvas->AddCamera(cams[i]);
					}

					const Sizef &canvasSize = canvas->GetSize();
					scene->AdjustCameraFrustum(canvasSize);
				}
			}
		}
		else if ("ProjectUICanvas" == name)
		{
			if (ProjectES::IsEqual(event, ProjectES::NewUI))
			{
				UI *ui = event->GetData<UI*>();

				canvas->RemoveAllCameras();
				std::vector<CameraPtr> &cams = ui->GetCameras();
				for (int i = 0; i < (int)cams.size(); i++)
				{
					if (cams[i])
					{
						canvas->AddCamera(cams[i]);
					}
				}

				// ÉèÖÃäÖÈ¾½Úµã
				canvas->SetRenderNode(ui);
			}
			else if (ProjectES::IsEqual(event, ProjectES::CloseUI))
			{
				canvas->SetRenderNode(0);
				canvas->RemoveAllCameras();
			}			
			else if (ProjectES_Internal::IsEqual(event, ProjectES_Internal::SceneUICamerasChanged))
			{
				Object *obj = event->GetData<Object*>();
				UI *ui = DynamicCast<UI>(obj);
				if (ui && ui == canvas->GetRenderNode())
				{
					canvas->RemoveAllCameras();
					std::vector<CameraPtr> &cams = ui->GetCameras();
					for (int i = 0; i < (int)cams.size(); i++)
					{
						if (cams[i])
						{
							canvas->AddCamera(cams[i]);
						}
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
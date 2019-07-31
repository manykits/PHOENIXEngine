// PX2Creater.cpp

#include "PX2Creater.hpp"
#include "PX2Texture2D.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ModelController.hpp"
#include "PX2EffectModule.hpp"
#include "PX2EffectableController.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2RawTerrain.hpp"
#include "PX2LogicManager.hpp"
#include "PX2LuaScriptController.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2Actor.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Creater::Creater()
{
}
//----------------------------------------------------------------------------
Creater::~Creater()
{
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_Box()
{
	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	TriMesh *mesh = stdMesh.Box1(0.5f, 0.5f, 0.5f);
	mesh->SetName("Poly");

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_lightshadow", false);
	mesh->SetMaterialInstance(mi);

	mi->SetPixelTexture(0, "SampleBase", tex);

	return mesh;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_Sphere()
{
	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	TriMesh *mesh = stdMesh.Sphere(32, 32, 1);
	mesh->SetName("Mesh");

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_lightshadow", false);
	mesh->SetMaterialInstance(mi);

	mi->SetPixelTexture(0, "SampleBase", tex);

	return mesh;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_Rectangle()
{
	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		"Data/engine/default.png"));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	TriMesh *mesh = stdMesh.Rectangle(2, 2, 1.0f, 1.0f, Float2(0.5f, 0.5f));
	mesh->SetName("Mesh");

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_lightshadow", false);
	mesh->SetMaterialInstance(mi);

	mi->SetPixelTexture(0, "SampleBase", tex);

	return mesh;
}
//----------------------------------------------------------------------------
Node *Creater::CreateBlockFrame(int num, float halfExtend, 
	const std::string &image)
{
	Node *node = Node::New("NodeFrame");

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(image));
	if (!tex) return 0;

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PNT1);

	StandardMesh stdMesh(vf);
	for (int i = 0; i < num; i++)
	{
		TriMesh *mesh = stdMesh.Box1(halfExtend, halfExtend, halfExtend);
		node->AttachChild(mesh);
		mesh->LocalTransform.SetTranslateX((0.5f + i) * halfExtend*2.0f);
		mesh->LocalTransform.SetTranslateY(halfExtend);
		mesh->LocalTransform.SetTranslateZ(halfExtend);
		mesh->SetName("Mesh");

		MaterialInstance *mi = new0 MaterialInstance(
			"Data/engine_mtls/std/std.px2obj", "std_light", false);
		mesh->SetMaterialInstance(mi);
		mi->SetPixelTexture(0, "SampleBase", tex);
	}

	return node;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateSegment_Rectangle(float xLength, float yLength,
	const Float4 &color)
{
	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);

	StandardMesh stdMesh(vf);
	stdMesh.SetVertexColor(color);
	Polysegment *segment = stdMesh.Rectangle(xLength, yLength);
	segment->SetName("Mesh");

	MaterialInstance *mi = VertexColor4Material::CreateUniqueInstance();
	segment->SetMaterialInstance(mi);

	return segment;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateSegment_Circle(float radius, int numSamples,
	const Float4 &color)
{
	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);

	StandardMesh stdMesh(vf);
	stdMesh.SetVertexColor(color);
	Polysegment *segment = stdMesh.Circle(radius, numSamples);
	segment->SetName("Mesh");

	MaterialInstance *mi = VertexColor4Material::CreateUniqueInstance();
	segment->SetMaterialInstance(mi);

	return segment;
}
//----------------------------------------------------------------------------
Node *Creater::CreateNode()
{
	return new0 Node();
}
//----------------------------------------------------------------------------
CameraNode *Creater::CreateNode_Camera(bool createCamera)
{
	CameraNode *camNode = new0 CameraNode();
	camNode->EnableUpdate(true);

	if (createCamera)
	{
		Camera *camera = new0 Camera();
		camNode->SetCamera(camera);
		camera->SetFrustum(50.0f, 1.0f, 0.1f, 1000.0f);
	}

	return camNode;
}
//----------------------------------------------------------------------------
Node *Creater::CreateNode_Model(const std::string &filename)
{
	Node *node = new0 Node();
	node->SetName("NodeModel");
	
	ModelController *mc = new0 ModelController();
	node->AttachController(mc);
	mc->SetAnimType(ModelController::AT_SKELETON);
	mc->SetMovableFilename(filename);
	mc->RegistToScriptSystem();
	mc->ResetPlay();

	return node;
}
//----------------------------------------------------------------------------
Node *Creater::CreateNode_Model(Movable *movable)
{
	Node *node = new0 Node();
	node->SetName("NodeModel");

	ModelController *mc = new0 ModelController();
	node->AttachController(mc);
	mc->SetAnimType(ModelController::AT_SKELETON);
	mc->SetMovable(movable);
	mc->RegistToScriptSystem();
	mc->ResetPlay();

	return node;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateMovable_FromRes(const std::string &filename)
{
	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName, 
		outExtention);

	if ("fbx" == outExtention || "FBX" == outExtention)
	{
		Object *obj = PX2_RM.BlockLoadShareCopy(filename, true, true, true);
		Movable *mov = DynamicCast<Movable>(obj);
		return mov;
	}
	else if ("px2obj" == outExtention)
	{
		Object *obj = PX2_RM.BlockLoadShareCopy(filename, true, true, true);
		Movable *mov = DynamicCast<Movable>(obj);
		return mov;
	}

	return 0;
}
//----------------------------------------------------------------------------
ScriptController *Creater::CreateScriptController_FromRes(
	const std::string &filename)
{
	std::string outPath;
	std::string outBaseName;
	std::string outExtention;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName,
		outExtention);

	ScriptController *sc = 0;
	if ("lua" == outExtention || "LUA" == outExtention)
	{
		sc = new0 LuaScriptController();
		sc->SetFileClass(filename, outBaseName);
	}

	return sc;
}
//----------------------------------------------------------------------------
ScriptController *Creater::CreateScriptController_FromBuffer(
	const std::string &bufStr, const std::string &className)
{
	ScriptController *sc = new0 LuaScriptController();
	sc->SetStringClass(bufStr, className);

	return sc;
}
//----------------------------------------------------------------------------
ParticleEmitter *Creater::CreateParticleEmitter()
{
	ParticleEmitter *pe = new0 ParticleEmitter();
	return pe;
}
//----------------------------------------------------------------------------
Billboard *Creater::CreateBillboard()
{
	Billboard *bd = new0 Billboard();
	return bd;
}
//----------------------------------------------------------------------------
BeamEmitter *Creater::CreateBeamEmitter()
{
	BeamEmitter *be = new0 BeamEmitter();
	return be;
}
//----------------------------------------------------------------------------
RibbonEmitter *Creater::CreateRibbonEmitter()
{
	RibbonEmitter *re = new0 RibbonEmitter();
	return re;
}
//----------------------------------------------------------------------------
Soundable *Creater::CreateSoundable()
{
	Soundable *soundable = new0 Soundable();
	return soundable;
}
//----------------------------------------------------------------------------
EffectModule *Creater::CreateEffectModule(const std::string &typeName)
{
	EffectModule *module = EffectModule::CreateModule(typeName);
	return module;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor()
{
	Actor *actor = new0 Actor();
	actor->SetPhysicsShapeType(Actor::PST_GENERAL);
	return actor;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActorBox()
{
	Actor *actor = new0 Actor(Actor::AIT_AGENTOBJECT);

	Movable *mov = CreateMovable_Box();
	actor->AttachChild(mov);
	mov->LocalTransform.SetUniformScale(0.2f);

	actor->SetPhysicsShapeType(Actor::PST_MESH, mov);

	return actor;
}
//----------------------------------------------------------------------------
Actor *Creater::CreateActor_InfinitePlane(float planeViewSizeScale,
	const AVector &normal, float originOffset)
{
	Actor *actor = new0 Actor(Actor::AIT_AGENTOBJECT);
	Movable *movRectangle = CreateMovable_Rectangle();
	actor->AttachChild(movRectangle);
	actor->SetPhysicsShapeType(Actor::PST_INFINITEPLANE);
	movRectangle->LocalTransform.SetUniformScale(planeViewSizeScale);

	return actor;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateSkyBox(const std::string &skyDirPath)
{
	if (skyDirPath.find(".") != std::string::npos)
		return 0;

	float skyBoxSize = 500.0f;

	Node *node = new0 Node();
	node->SetName("Sky");

	VertexBuffer* vbuffer;
	TriMesh* wall;
	std::string textureName;

	// Index buffer shared by the room walls.
	IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
	int* indices = (int*)ibuffer->GetData();
	indices[0] = 0;  indices[1] = 1;  indices[2] = 3;
	indices[3] = 0;  indices[4] = 3;  indices[5] = 2;

	// The vertex format shared by the room walls.
	VertexFormat* vformat = VertexFormat::Create(2,
		VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
		VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
	int vstride = vformat->GetStride();
	VertexBufferAccessor vba;

	// +x wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(+skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(+skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	std::string texPath = skyDirPath + "right.png";
	Object *tex2DObj = PX2_RM.BlockLoad(texPath);
	MaterialInstance *mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", 
		"tex2d", false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// -x wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(-skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(-skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "left.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance(
		"Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d", false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// +y wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "front.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d",
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// -y wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(+skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(-skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(+skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(-skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "back.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d", 
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// +z wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, +skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, -skyBoxSize, +skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "up.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d",
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	// -z wall
	vbuffer = new0 VertexBuffer(4, vstride);
	vba.ApplyTo(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(-skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(1) = Float3(+skyBoxSize, -skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(2) = Float3(-skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.Position<Float3>(3) = Float3(+skyBoxSize, +skyBoxSize, -skyBoxSize);
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(0.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(1.0f, 1.0f);
	wall = new0 TriMesh(vformat, vbuffer, ibuffer);
	node->AttachChild(wall);

	texPath = skyDirPath + "down.png";
	tex2DObj = PX2_RM.BlockLoad(texPath);
	mi = new0 MaterialInstance("Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d",
		false);
	wall->SetMaterialInstance(mi);
	mi->SetPixelTexture(0, "SampleBase", DynamicCast<Texture2D>(tex2DObj));

	return node;
}
//----------------------------------------------------------------------------
Movable *Creater::CreateTerrain()
{
	RawTerrain *rawTerrain = new0 RawTerrain();
	rawTerrain->SetSize(129);
	rawTerrain->SetRowFromTo(-4, 4);
	rawTerrain->SetColFromTo(-4, 4);
	rawTerrain->SetSpacing(2);
	rawTerrain->AllocateRawTerrainPages();

	return rawTerrain;
}
//----------------------------------------------------------------------------
UIFrame *Creater::CreateUIFrame()
{
	UIFrame *frame = new0 UIFrame();	
	return frame;
}
//----------------------------------------------------------------------------
UIFPicBox *Creater::CreateUIFPicBox()
{
	UIFPicBox *fPicBox = new0 UIFPicBox();
	return fPicBox;
}
//----------------------------------------------------------------------------
UIFText *Creater::CreateUIFText()
{
	UIFText *fText = new0 UIFText();
	return fText;
}
//----------------------------------------------------------------------------
UIButton *Creater::CreateUIButton()
{
	UIButton *but = new0 UIButton();
	return but;
}
//----------------------------------------------------------------------------
UICheckButton *Creater::CreateUICheckButton()
{
	UICheckButton *checkButton = new0 UICheckButton();
	return checkButton;
}
//----------------------------------------------------------------------------
UIComboBox *Creater::CreateUIComboBox()
{
	UIComboBox *comboBox = new0 UIComboBox();
	return comboBox;
}
//----------------------------------------------------------------------------
UIEditBox *Creater::CreateUIEditBox()
{
	UIEditBox *editBox = new0 UIEditBox();
	return editBox;
}
//----------------------------------------------------------------------------
UIProgressBar *Creater::CreateUIProgressBar()
{
	UIProgressBar *pb = new0 UIProgressBar();
	return pb;
}
//----------------------------------------------------------------------------
UIRound *Creater::CreateUIRound()
{
	UIRound *round = new0 UIRound();
	return round;
}
//----------------------------------------------------------------------------
UIVlc *Creater::CreateUIVlc()
{
	UIVlc *vlc = new0 UIVlc();
	return vlc;
}
//----------------------------------------------------------------------------
InterpCurveAlphaController *Creater::CreateICC_Alpha()
{
	InterpCurveAlphaController *ctrl = new0 InterpCurveAlphaController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveColorController *Creater::CreateICC_Color()
{
	InterpCurveColorController *ctrl = new0 InterpCurveColorController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveUniformScaleController *Creater::CreateICC_UniformScale()
{
	InterpCurveUniformScaleController *ctrl =
		new0 InterpCurveUniformScaleController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveScaleController *Creater::CreateICC_Scale()
{
	InterpCurveScaleController *ctrl = new0 InterpCurveScaleController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveRotateController *Creater::CreateICC_Rotate()
{
	InterpCurveRotateController *ctrl = new0 InterpCurveRotateController();
	return ctrl;
}
//----------------------------------------------------------------------------
InterpCurveTranslateController *Creater::CreateICC_Translate()
{
	InterpCurveTranslateController *ctrl = new0 InterpCurveTranslateController();
	return ctrl;
}
//----------------------------------------------------------------------------
TriggerController *Creater::CreateTriggerController()
{
	TriggerController *ctrl = new0 TriggerController();
	return ctrl;
}
//----------------------------------------------------------------------------
CameraPlayController *Creater::CreateCameraPlayController()
{
	CameraPlayController *ctrl = new0 CameraPlayController();
	return ctrl;
}
//----------------------------------------------------------------------------
void Creater::AddObject(Object *parent, Object *obj,
	bool command)
{
	Movable *mov = DynamicCast<Movable>(obj);
	Controller *ctrl = DynamicCast<Controller>(obj);
	EffectModule *eftModule = DynamicCast<EffectModule>(obj);

	Node *nodePar = DynamicCast<Node>(parent);
	Controlledable *ctrlablePar = DynamicCast<Controlledable>(parent);
	EffectableController *effectableCtrl = DynamicCast<EffectableController>(parent);

	bool added = false;
	if (mov && nodePar)
	{
		nodePar->AttachChild(mov);
		added = true;
	}
	else if (ctrlablePar && ctrl)
	{
		ctrlablePar->AttachController(ctrl);
		added = true;
	}
	else if (effectableCtrl && eftModule)
	{
		effectableCtrl->AddModule(eftModule);
		added = true;
	}

	if (added && command)
	{
		ObjectAddDeleteURDo *cmd = new0 ObjectAddDeleteURDo(true, obj);
		PX2_URDOM.PushUnDo(cmd);
	}
}
//----------------------------------------------------------------------------
bool Creater::RemoveObject(Object *obj, bool command)
{
	Movable *mov = DynamicCast<Movable>(obj);
	Controller *ctrl = DynamicCast<Controller>(obj);
	EffectModule *module = DynamicCast<EffectModule>(obj);
	Animation *anim = DynamicCast<Animation>(obj);
	PX2_UNUSED(anim);

	URDoPtr theCommand = new0 ObjectAddDeleteURDo(false, obj);

	bool removed = false;

	if (mov)
	{
		Node *parent = DynamicCast<Node>(mov->GetParent());
		if (parent)
		{
			int ret = parent->DetachChild(mov);
			if (-1 != ret)
			{
				removed = true;
			}
		}
	}
	else if (ctrl)
	{
		Controlledable *ctrlAble = ctrl->GetControlledable();
		ctrlAble->DetachController(ctrl);

		removed = true;
	}
	else if (module)
	{
		EffectableController *effCtrl = module->GetEffectableController();
		effCtrl->RemoveModule(module);

		removed = true;
	}

	if (removed && command)
	{
		PX2_URDOM.PushUnDo(theCommand);
	}

	return true;
}
//----------------------------------------------------------------------------
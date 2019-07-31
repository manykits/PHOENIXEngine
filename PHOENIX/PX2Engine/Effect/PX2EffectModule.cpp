// PX2EffectModule.cpp

#include "PX2EffectModule.hpp"
#include "PX2EffectObject.hpp"
#include "PX2Effectable.hpp"
#include "PX2EffectableController.hpp"
#include "PX2EffectModuleCallbacks_Effectable.hpp"
#include "PX2EffectModuleCallbacks_ParticleEmitter.hpp"
#include "PX2EffectModuleCallbacks_BeamEmitter.hpp"
#include "PX2EffectModuleCallbacks_Soundable.hpp"
#include "PX2EffectModuleCallbacks_CameraShake.hpp"
#include "PX2ParticleEmitter.hpp"
#include "PX2BeamEmitter.hpp"
#include "PX2RibbonEmitter.hpp"
#include "PX2Billboard.hpp"
#include "PX2Soundable.hpp"
#include "PX2CameraShake.hpp"
#include "PX2EffectModel.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, EffectModule);
PX2_IMPLEMENT_STREAM(EffectModule);
PX2_IMPLEMENT_FACTORY(EffectModule);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, EffectModule);

//----------------------------------------------------------------------------
bool EffectModule::msIsRegisterInitFinal = false;
std::map<std::string, EffectModulePtr> EffectModule::msPreCreatedModules;
//----------------------------------------------------------------------------
void EffectModule::EffectModuleInit()
{
	// Effectable
	PreCreateModuleFloat_EA("EmitSizeX", 1.0f, EA_Callbacks::EA_EmitSizeX);
	PreCreateModuleFloat_EA("EmitSizeY", 1.0f, EA_Callbacks::EA_EmitSizeY);
	PreCreateModuleFloat_EA("EmitSizeZ", 1.0f, EA_Callbacks::EA_EmitSizeZ);
	PreCreateModuleFloat_EA("EmitSizeXYZ", 1.0f, EA_Callbacks::EA_EmitSizeXYZ);

	PreCreateModuleFloat3_EA("EmitUV0_Offset", Float3::ZERO, EA_Callbacks::EA_EmitUV0_Offset);
	PreCreateModuleFloat3_EA("EmitUV0_Speed", Float3::UNIT, EA_Callbacks::EA_EmitUV0_Speed);
	PreCreateModuleFloat3_EA("EmitUV0_Repeat", Float3::UNIT, EA_Callbacks::EA_EmitUV0_Repeat);

	PreCreateModuleFloat3Color_EA("EmitColor", Float3::WHITE, EA_Callbacks::EA_EmitColor);
	PreCreateModuleFloat_EA("EmitAlpha", 1.0f, EA_Callbacks::EA_EmitAlpha);
	PreCreateModuleFloat_EA("EmitLife", 1.0f, EA_Callbacks::EA_EmitLife);

	// EffectObject
	PreCreateModuleFloat_EO("SizeXAtAge", 1.0f, EO_Callbacks::EO_SizeXAtAge);
	PreCreateModuleFloat_EO("SizeYAtAge", 1.0f, EO_Callbacks::EO_SizeYAtAge);
	PreCreateModuleFloat_EO("SizeZAtAge", 1.0f, EO_Callbacks::EO_SizeZAtAge);
	PreCreateModuleFloat_EO("SizeXYZAtAge", 1.0f, EO_Callbacks::EO_SizeXYZAtAge);

	PreCreateModuleFloat_EO("SizeXByAge", 1.0f, EO_Callbacks::EO_SizeXByAge);
	PreCreateModuleFloat_EO("SizeYByAge", 1.0f, EO_Callbacks::EO_SizeYByAge);
	PreCreateModuleFloat_EO("SizeZByAge", 1.0f, EO_Callbacks::EO_SizeZByAge);
	PreCreateModuleFloat_EO("SizeXYZByAge", 1.0f, EO_Callbacks::EO_SizeXYZByAge);

	PreCreateModuleFloat3_EO("UV0_OffsetAtAge", Float3::ZERO, EO_Callbacks::EO_UV0_OffsetAtAge);
	PreCreateModuleFloat3_EO("UV0_SpeedAtAge", Float3::UNIT, EO_Callbacks::EO_UV0_SpeedAtAge);
	PreCreateModuleFloat3_EO("UV0_SpeedByAge", Float3::UNIT, EO_Callbacks::EO_UV0_SpeedByAge);
	PreCreateModuleFloat3_EO("UV0_RepeatAtAge", Float3::UNIT, EO_Callbacks::EO_UV0_RepeatAtAge);
	PreCreateModuleFloat3_EO("UV0_RepeatByAge", Float3::UNIT, EO_Callbacks::EO_UV0_RepeatByAge);

	PreCreateModuleFloat3Color_EO("ColorAtAge", Float3::WHITE, EO_Callbacks::EO_ColorAtAge);
	PreCreateModuleFloat_EO("AlphaAtAge", 1.0f, EO_Callbacks::EO_AlphaAtAge);

	// ParticleEmitter
	PreCreateModuleFloat_EA("PE_EmitRate", 10.0f, PE_Callbacks::PE_EmitRate);
	PreCreateModuleFloat3_EA("PE_EmitDir", Float3(0.0f, 0.0f, 1.0f), PE_Callbacks::PE_EmitDir);
	PreCreateModuleFloat_EA("PE_EmitSpeed", 10.0f, PE_Callbacks::PE_EmitSpeed);
	PreCreateModuleFloat3_EA("PE_EmitAccelerateDir", Float3(0.0f, 0.0f, -1.0f), PE_Callbacks::PE_EmitAccelerateDir);
	PreCreateModuleFloat_EA("PE_EmitAccelerate", 9.8f, PE_Callbacks::PE_EmitAccelerate);
	PreCreateModuleFloat3_EA("PE_EmitRotateAxis", Float3(0.0f, 0.0f, 1.0f), PE_Callbacks::PE_EmitRotateAxis);
	PreCreateModuleFloat_EA("PE_EmitRotateSpeed", 1.0f, PE_Callbacks::PE_EmitRotateSpeed);
	PreCreateModuleFloat_EA("PE_EmitRotateDegree", 1.0f, PE_Callbacks::PE_EmitRotateDegree);

	PreCreateModuleFloat_EA("PE_PlacerInLength", 0.0f, PE_Callbacks::PE_PlacerInLength);
	PreCreateModuleFloat_EA("PE_PlacerOutLength", 1.0f, PE_Callbacks::PE_PlacerOutLength);
	PreCreateModuleFloat_EA("PE_PlacerInWidth", 0.0f, PE_Callbacks::PE_PlacerInWidth);
	PreCreateModuleFloat_EA("PE_PlacerOutWidth", 1.0, PE_Callbacks::PE_PlacerOutWidth);
	PreCreateModuleFloat_EA("PE_PlacerInHeight", 0.0f, PE_Callbacks::PE_PlacerInHeight);
	PreCreateModuleFloat_EA("PE_PlacerOutHeight", 1.0f, PE_Callbacks::PE_PlacerOutHeight);

	// ParticleObject
	PreCreateModuleFloat3_EO("PO_SpeedDirAtAge", Float3(0.0f, 0.0f, 1.0f), PO_Callbacks::PO_SpeedDirAtAge);
	PreCreateModuleFloat_EO("PO_SpeedAtAge", 10.0f, PO_Callbacks::PO_SpeedAtAge);
	PreCreateModuleFloat_EO("PO_SpeedByAge", 10.0f, PO_Callbacks::PO_SpeedByAge);
	PreCreateModuleFloat3_EO("PO_AccelerateDirAtAge", Float3(0.0f, 0.0f, -1.0f), PO_Callbacks::PO_AccelerateDirAtAge);
	PreCreateModuleFloat_EO("PO_AccelerateAtAge", 10.0f, PO_Callbacks::PO_AccelerateAtAge);
	PreCreateModuleFloat_EO("PO_AccelerateByAge", 10.0f, PO_Callbacks::PO_AccelerateByAge);
	PreCreateModuleFloat3_EO("PO_ObstructByAtAge", Float3(0.0f, 0.0f, 0.0f), PO_Callbacks::PO_ObstructByAtAge);
	PreCreateModuleFloat3_EO("PO_RotAxisAtAge", Float3(0.0f, 0.0f, 1.0f), PO_Callbacks::PO_RotAxisAtAge);
	PreCreateModuleFloat_EO("PO_RotSpeedAtAge", 1.0f, PO_Callbacks::PO_RotSpeedAtAge);
	PreCreateModuleFloat_EO("PO_RotSpeedByAge", 1.0f, PO_Callbacks::PO_RotSpeedByAge);
	PreCreateModuleFloat_EO("PO_RotDegreeAtAge", 1.0f, PO_Callbacks::PO_RotDegreeAtAge);
	PreCreateModuleFloat_EO("PO_RotDegreeByAge", 1.0f, PO_Callbacks::PO_RotDegreeByAge);

	// BeamEmitter
	PreCreateModuleFloat_EA("BE_EmitRate", 10.0f, BE_Callbacks::BE_EmitRate);
	PreCreateModuleFloat3_EA("BE_EmitStartPos", Float3::ZERO, BE_Callbacks::BE_EmitStartPos);
	PreCreateModuleFloat3_EA("BE_EmitEndPos", Float3(10.0f, 0.0f, 0.0f), BE_Callbacks::BE_EmitEndPos);

	// Soundable
	PreCreateModuleFloat_EA("SE_EmitVolume", 1.0f, SE_Callbacks::SE_EmitVolume);

	// SoundableObject
	PreCreateModuleFloat_EO("SO_VolumeAtAge", 1.0f, SO_Callbacks::SO_VolumeAtAge);
	PreCreateModuleFloat_EO("SO_VolumeByAge", 1.0f, SO_Callbacks::SO_VolumeByAge);

	// CameraShake
	PreCreateModuleFloat_EA("CSE_EmitStrength", 1.0f, CSE_Callbacks::CSE_EmitStrength);

	// CameraShakeObject
	PreCreateModuleFloat_EO("CSO_StrengthAtAge", 1.0f, CSO_Callbacks::CSO_StrengthAtAge);
	PreCreateModuleFloat_EO("CSO_StrengthByAge", 1.0f, CSO_Callbacks::CSO_StrengthByAge);

	// ParticleEmitter
	ParticleEmitter::msModuleNames_EA.push_back("EmitSizeX");
	ParticleEmitter::msModuleNames_EA.push_back("EmitSizeY");
	ParticleEmitter::msModuleNames_EA.push_back("EmitSizeZ");
	ParticleEmitter::msModuleNames_EA.push_back("EmitSizeXYZ");
	ParticleEmitter::msModuleNames_EA.push_back("EmitUV0_Offset");
	ParticleEmitter::msModuleNames_EA.push_back("EmitUV0_Speed");
	ParticleEmitter::msModuleNames_EA.push_back("EmitUV0_Repeat");
	ParticleEmitter::msModuleNames_EA.push_back("EmitColor");
	ParticleEmitter::msModuleNames_EA.push_back("EmitAlpha");
	ParticleEmitter::msModuleNames_EA.push_back("EmitLife");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitRate");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitDir");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitSpeed");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitAccelerateDir");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitAccelerate");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitRotateAxis");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitRotateSpeed");
	ParticleEmitter::msModuleNames_EA.push_back("PE_EmitRotateDegree");
	ParticleEmitter::msModuleNames_EA.push_back("PE_PlacerInLength");
	ParticleEmitter::msModuleNames_EA.push_back("PE_PlacerOutLength");
	ParticleEmitter::msModuleNames_EA.push_back("PE_PlacerInWidth");
	ParticleEmitter::msModuleNames_EA.push_back("PE_PlacerOutWidth");
	ParticleEmitter::msModuleNames_EA.push_back("PE_PlacerInHeight");
	ParticleEmitter::msModuleNames_EA.push_back("PE_PlacerOutHeight");
	ParticleEmitter::msModuleNames_EO.push_back("SizeXAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("SizeYAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("SizeZAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("SizeXYZAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("SizeXByAge");
	ParticleEmitter::msModuleNames_EO.push_back("SizeYByAge");
	ParticleEmitter::msModuleNames_EO.push_back("SizeZByAge");
	ParticleEmitter::msModuleNames_EO.push_back("SizeXYZByAge");
	ParticleEmitter::msModuleNames_EO.push_back("UV0_OffsetAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("UV0_SpeedAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("UV0_SpeedByAge");
	ParticleEmitter::msModuleNames_EO.push_back("UV0_RepeatAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("UV0_RepeatByAge");
	ParticleEmitter::msModuleNames_EO.push_back("ColorAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("AlphaAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_SpeedDirAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_SpeedAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_SpeedByAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_AccelerateDirAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_AccelerateAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_AccelerateByAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_ObstructByAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_RotAxisAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_RotSpeedAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_RotSpeedByAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_RotDegreeAtAge");
	ParticleEmitter::msModuleNames_EO.push_back("PO_RotDegreeByAge");

	// BeamEmitter
	BeamEmitter::msModuleNames_EA.push_back("EmitSizeX");
	BeamEmitter::msModuleNames_EA.push_back("EmitUV0_Offset");
	BeamEmitter::msModuleNames_EA.push_back("EmitUV0_Speed");
	BeamEmitter::msModuleNames_EA.push_back("EmitUV0_Repeat");
	BeamEmitter::msModuleNames_EA.push_back("EmitColor");
	BeamEmitter::msModuleNames_EA.push_back("EmitAlpha");
	BeamEmitter::msModuleNames_EA.push_back("EmitLife");
	BeamEmitter::msModuleNames_EA.push_back("BE_EmitRate");
	BeamEmitter::msModuleNames_EA.push_back("BE_EmitStartPos");
	BeamEmitter::msModuleNames_EA.push_back("BE_EmitEndPos");
	BeamEmitter::msModuleNames_EO.push_back("SizeXAtAge");
	BeamEmitter::msModuleNames_EO.push_back("SizeXByAge");
	BeamEmitter::msModuleNames_EO.push_back("UV0_OffsetAtAge");
	BeamEmitter::msModuleNames_EO.push_back("UV0_SpeedAtAge");
	BeamEmitter::msModuleNames_EO.push_back("UV0_SpeedByAge");
	BeamEmitter::msModuleNames_EO.push_back("UV0_RepeatAtAge");
	BeamEmitter::msModuleNames_EO.push_back("UV0_RepeatByAge");
	BeamEmitter::msModuleNames_EO.push_back("ColorAtAge");
	BeamEmitter::msModuleNames_EO.push_back("AlphaAtAge");

	// RibbonEmitter
	RibbonEmitter::msModuleNames_EA.push_back("EmitSizeX");
	RibbonEmitter::msModuleNames_EA.push_back("EmitUV0_Offset");
	RibbonEmitter::msModuleNames_EA.push_back("EmitUV0_Speed");
	RibbonEmitter::msModuleNames_EA.push_back("EmitUV0_Repeat");
	RibbonEmitter::msModuleNames_EA.push_back("EmitColor");
	RibbonEmitter::msModuleNames_EA.push_back("EmitAlpha");
	RibbonEmitter::msModuleNames_EO.push_back("SizeXAtAge");
	RibbonEmitter::msModuleNames_EO.push_back("SizeXByAge");
	RibbonEmitter::msModuleNames_EO.push_back("UV0_OffsetAtAge");
	RibbonEmitter::msModuleNames_EO.push_back("UV0_SpeedAtAge");
	RibbonEmitter::msModuleNames_EO.push_back("UV0_SpeedByAge");
	RibbonEmitter::msModuleNames_EO.push_back("UV0_RepeatAtAge");
	RibbonEmitter::msModuleNames_EO.push_back("UV0_RepeatByAge");
	RibbonEmitter::msModuleNames_EO.push_back("ColorAtAge");
	RibbonEmitter::msModuleNames_EO.push_back("AlphaAtAge");

	// Billboard
	Billboard::msModuleNames_EA.push_back("EmitSizeX");
	Billboard::msModuleNames_EA.push_back("EmitSizeY");
	Billboard::msModuleNames_EA.push_back("EmitSizeZ");
	Billboard::msModuleNames_EA.push_back("EmitSizeXYZ");
	Billboard::msModuleNames_EA.push_back("EmitUV0_Offset");
	Billboard::msModuleNames_EA.push_back("EmitUV0_Speed");
	Billboard::msModuleNames_EA.push_back("EmitUV0_Repeat");
	Billboard::msModuleNames_EA.push_back("EmitColor");
	Billboard::msModuleNames_EA.push_back("EmitAlpha");
	Billboard::msModuleNames_EA.push_back("EmitLife");
	Billboard::msModuleNames_EO.push_back("SizeXAtAge");
	Billboard::msModuleNames_EO.push_back("SizeYAtAge");
	Billboard::msModuleNames_EO.push_back("SizeZAtAge");
	Billboard::msModuleNames_EO.push_back("SizeXYZAtAge");
	Billboard::msModuleNames_EO.push_back("SizeXByAge");
	Billboard::msModuleNames_EO.push_back("SizeYByAge");
	Billboard::msModuleNames_EO.push_back("SizeZByAge");
	Billboard::msModuleNames_EO.push_back("SizeXYZByAge");
	Billboard::msModuleNames_EO.push_back("UV0_OffsetAtAge");
	Billboard::msModuleNames_EO.push_back("UV0_SpeedAtAge");
	Billboard::msModuleNames_EO.push_back("UV0_SpeedByAge");
	Billboard::msModuleNames_EO.push_back("UV0_RepeatAtAge");
	Billboard::msModuleNames_EO.push_back("UV0_RepeatByAge");
	Billboard::msModuleNames_EO.push_back("ColorAtAge");
	Billboard::msModuleNames_EO.push_back("AlphaAtAge");

	// Soundable
	Soundable::msModuleNames_EA.push_back("SE_EmitVolume");
	Soundable::msModuleNames_EO.push_back("SO_VolumeAtAge");
	Soundable::msModuleNames_EO.push_back("SO_VolumeByAge");

	// CameraShake
	CameraShake::msModuleNames_EA.push_back("CSE_EmitStrength");
	CameraShake::msModuleNames_EO.push_back("CSO_StrengthAtAge");
	CameraShake::msModuleNames_EO.push_back("CSO_StrengthByAge");

	// EffectModel
	EffectModel::msModuleNames_EA.push_back("EmitSizeX");
	EffectModel::msModuleNames_EA.push_back("EmitSizeY");
	EffectModel::msModuleNames_EA.push_back("EmitSizeZ");
	EffectModel::msModuleNames_EA.push_back("EmitSizeXYZ");
	EffectModel::msModuleNames_EA.push_back("EmitUV0_Offset");
	EffectModel::msModuleNames_EA.push_back("EmitUV0_Speed");
	EffectModel::msModuleNames_EA.push_back("EmitUV0_Repeat");
	EffectModel::msModuleNames_EA.push_back("EmitColor");
	EffectModel::msModuleNames_EA.push_back("EmitAlpha");

	EffectModel::msModuleNames_EO.push_back("UV0_OffsetAtAge");
	EffectModel::msModuleNames_EO.push_back("UV0_SpeedAtAge");
	EffectModel::msModuleNames_EO.push_back("UV0_SpeedByAge");
	EffectModel::msModuleNames_EO.push_back("UV0_RepeatAtAge");
	EffectModel::msModuleNames_EO.push_back("UV0_RepeatByAge");
	EffectModel::msModuleNames_EO.push_back("ColorAtAge");
	EffectModel::msModuleNames_EO.push_back("AlphaAtAge");
}
//----------------------------------------------------------------------------
void EffectModule::EffectModuleFinal()
{
	msPreCreatedModules.clear();

	ParticleEmitter::msModuleNames_EA.clear();
	ParticleEmitter::msModuleNames_EO.clear();
}
//----------------------------------------------------------------------------
bool EffectModule::RegisterInitFinal()
{
	if (!msIsRegisterInitFinal)
	{
		InitTerm::AddInitializer(EffectModuleInit);
		InitTerm::AddTerminator(EffectModuleFinal);

		msIsRegisterInitFinal = true;
	}

	return msIsRegisterInitFinal;
}
//----------------------------------------------------------------------------
EffectModule *EffectModule::PreCreateModuleFloat_EA (
	const std::string &name, 
	float initVal, UpdateEffectableCallback callback)
{
	EffectModule *module = new0 EffectModule(EffectModule::MT_FLOAT, true,
		initVal);
	module->SetName(name);
	module->SetTypeName(name);
	module->SetUpdateEffectableCallback(callback);

	msPreCreatedModules[name] = module;

	return module;
}
//----------------------------------------------------------------------------
EffectModule *EffectModule::PreCreateModuleFloat3_EA (
	const std::string &name, 
	Float3 initVal, UpdateEffectableCallback callback)
{
	EffectModule *module = new0 EffectModule(EffectModule::MT_FLOAT3, true,
		0.0f, initVal);
	module->SetName(name);
	module->SetTypeName(name);
	module->SetUpdateEffectableCallback(callback);

	msPreCreatedModules[name] = module;

	return module;
}
//----------------------------------------------------------------------------
EffectModule *EffectModule::PreCreateModuleFloat3Color_EA (const std::string &name, 
	Float3 initVal, UpdateEffectableCallback callback)
{
	EffectModule *module = new0 EffectModule(EffectModule::MT_FLOAT3COLOR, true,
		0.0f, initVal);
	module->SetName(name);
	module->SetTypeName(name);
	module->SetUpdateEffectableCallback(callback);

	msPreCreatedModules[name] = module;

	return module;
}
//----------------------------------------------------------------------------
EffectModule *EffectModule::PreCreateModuleFloat_EO (
	const std::string &name, 
	float initVal, UpdateEffectObjectCallback callback)
{
	EffectModule *module = new0 EffectModule(EffectModule::MT_FLOAT, false,
		initVal);
	module->SetName(name);
	module->SetTypeName(name);
	module->SetUpdateEffectObjectCallback(callback);

	msPreCreatedModules[name] = module;

	return module;
}
//----------------------------------------------------------------------------
EffectModule *EffectModule::PreCreateModuleFloat3_EO (
	const std::string &name, 
	Float3 initVal, UpdateEffectObjectCallback callback)
{
	EffectModule *module = new0 EffectModule(EffectModule::MT_FLOAT3, false,
		0.0f, initVal);
	module->SetName(name);
	module->SetTypeName(name);
	module->SetUpdateEffectObjectCallback(callback);

	msPreCreatedModules[name] = module;

	return module;
}
//----------------------------------------------------------------------------
EffectModule *EffectModule::PreCreateModuleFloat3Color_EO (const std::string &name, 
	Float3 initVal, UpdateEffectObjectCallback callback)
{
	EffectModule *module = new0 EffectModule(EffectModule::MT_FLOAT3COLOR, false,
		0.0f, initVal);
	module->SetName(name);
	module->SetTypeName(name);
	module->SetUpdateEffectObjectCallback(callback);

	msPreCreatedModules[name] = module;

	return module;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void EffectModule::RegistProperties ()
{
	Object::RegistProperties();

	AddPropertyClass("EffectModule");
	AddProperty("TypeName", PT_STRING, GetTypeName(), false);

	std::vector<std::string> moduleRepeatTypes;
	moduleRepeatTypes.push_back("RT_NONE");
	moduleRepeatTypes.push_back("RT_CLAMP");
	moduleRepeatTypes.push_back("RT_WRAP");
	moduleRepeatTypes.push_back("RT_CYCLE");
	AddPropertyEnum("RepeatType", (int)GetRepeatType(), moduleRepeatTypes);
	AddProperty("RepeatTime", PT_FLOAT, GetRepeatTime());

	ModuleType moduleType = GetModuleType ();
	std::vector<std::string> moduleTypes;
	moduleTypes.push_back("MT_FLOAT");
	moduleTypes.push_back("MT_FLOAT2");
	moduleTypes.push_back("MT_FLOAT3");
	moduleTypes.push_back("MT_FLOAT3COLOR");
	AddPropertyEnum("ModuleType", (int)moduleType, moduleTypes, false);

	AddProperty("IsRange", PT_BOOL, IsRange());
	
	AddProperty("NumKeyPoints", PT_INT, GetNumKeyPoints(), false);
	Object::PropertyType type = PT_EMFLOAT;
	if (moduleType == MT_FLOAT)
	{
		if (IsRange())
			type = PT_EMFLOATRANGE;
		else
			type = PT_EMFLOAT;
	}
	else if (moduleType == MT_FLOAT3)
	{
		if (IsRange())
			type = PT_EMFLOAT3RANGE;
		else 
			type = PT_EMFLOAT3;
	}
	else if (moduleType == MT_FLOAT3COLOR)
	{
		if (IsRange())
			type = PT_EMFLOAT3COLORRANGE;
		else
			type = PT_EMFLOAT3COLOR;
	}

	for (int i=0; i<GetNumKeyPoints(); i++)
	{
		std::string name = "["+StringHelp::IntToString(i)+"]";

		if (type == PT_EMFLOAT)
		{
			InterpCurvePointFloat *point = &(GetCurveFloatMin().Points[i]);
			AddProperty(name, PT_EMFLOAT, *point);
		}
		else if (type == PT_EMFLOATRANGE)
		{
			InterpCurvePointFloat *pointMin = &(GetCurveFloatMin().Points[i]);
			InterpCurvePointFloat *pointMax = &(GetCurveFloatMax().Points[i]);

			PropertyObject obj;
			obj.Name = name;
			obj.Type = PT_EMFLOATRANGE;
			obj.Data = *pointMin;
			obj.Data1 = *pointMax;
			AddProperty(obj);
		}
		else if (type == PT_EMFLOAT3)
		{
			InterpCurvePointFloat3 *point = &(GetCurveFloat3Min().Points[i]);
			AddProperty(name, PT_EMFLOAT3, *point);
		}
		else if (type == PT_EMFLOAT3RANGE)
		{
			InterpCurvePointFloat3 *pointMin = &(GetCurveFloat3Min().Points[i]);
			InterpCurvePointFloat3 *pointMax = &(GetCurveFloat3Max().Points[i]);

			PropertyObject obj;
			obj.Name = name;
			obj.Type = PT_EMFLOAT3RANGE;
			obj.Data = *pointMin;
			obj.Data1 = *pointMax;
			AddProperty(obj);
		}
		else if (type == PT_EMFLOAT3COLOR)
		{
			InterpCurvePointFloat3 *point = &(GetCurveFloat3Min().Points[i]);
			AddProperty(name, PT_EMFLOAT3COLOR, *point);
		}
		else if (type == PT_EMFLOAT3COLORRANGE)
		{
			InterpCurvePointFloat3 *pointMin = &(GetCurveFloat3Min().Points[i]);
			InterpCurvePointFloat3 *pointMax = &(GetCurveFloat3Max().Points[i]);

			PropertyObject obj;
			obj.Name = name;
			obj.Type = PT_EMFLOAT3COLORRANGE;
			obj.Data = *pointMin;
			obj.Data1 = *pointMax;
			AddProperty(obj);
		}
	}
}
//----------------------------------------------------------------------------
void EffectModule::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("RepeatType" == obj.Name)
	{
		SetRepeatType((RepeatType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("RepeatTime" == obj.Name)
	{
		SetRepeatTime(PX2_ANY_AS(obj.Data, float));
	}
	else if ("IsRange" == obj.Name)
	{
		SetRange(PX2_ANY_AS(obj.Data, bool));
	}

	size_t begin = obj.Name.find_first_of("[");
	size_t end = obj.Name.find_first_of("]");
	if (begin!=std::string::npos && end!=std::string::npos)
	{	
		std::string valStr = obj.Name.substr(begin+1, end-begin);
		int i = atoi(valStr.c_str());

		if (obj.Type == PT_EMFLOAT)
		{
			InterpCurvePointFloat *point = &(GetCurveFloatMin().Points[i]);

			*point = PX2_ANY_AS(obj.Data, InterpCurvePointFloat);
		}
		else if (obj.Type == PT_EMFLOATRANGE)
		{
			InterpCurvePointFloat *pointMin = &(GetCurveFloatMin().Points[i]);
			InterpCurvePointFloat *pointMax = &(GetCurveFloatMax().Points[i]);

			*pointMin = PX2_ANY_AS(obj.Data, InterpCurvePointFloat);
			*pointMax = PX2_ANY_AS(obj.Data1, InterpCurvePointFloat);
		}
		else if (obj.Type == PT_EMFLOAT3)
		{
			InterpCurvePointFloat3 *point = &(GetCurveFloat3Min().Points[i]);

			*point = PX2_ANY_AS(obj.Data, InterpCurvePointFloat3);
		}
		else if (obj.Type == PT_EMFLOAT3RANGE)
		{
			InterpCurvePointFloat3 *pointMin = &(GetCurveFloat3Min().Points[i]);
			InterpCurvePointFloat3 *pointMax = &(GetCurveFloat3Max().Points[i]);

			*pointMin = PX2_ANY_AS(obj.Data, InterpCurvePointFloat3);
			*pointMax = PX2_ANY_AS(obj.Data1, InterpCurvePointFloat3);
		}
		else if (obj.Type == PT_EMFLOAT3COLOR)
		{
			InterpCurvePointFloat3 *point = &(GetCurveFloat3Min().Points[i]);

			*point = PX2_ANY_AS(obj.Data, InterpCurvePointFloat3);
		}
		else if (obj.Type == PT_EMFLOAT3COLORRANGE)
		{
			InterpCurvePointFloat3 *pointMin = &(GetCurveFloat3Min().Points[i]);
			InterpCurvePointFloat3 *pointMax = &(GetCurveFloat3Max().Points[i]);

			*pointMin = PX2_ANY_AS(obj.Data, InterpCurvePointFloat3);
			*pointMax = PX2_ANY_AS(obj.Data1, InterpCurvePointFloat3);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// EffectModule
//----------------------------------------------------------------------------
EffectModule::EffectModule (ModuleType type, bool isForEffectable, 
	float initVal, const Float3 &initVal1)
	:
mModuleType(type),
mRepeatType(RT_NONE),
mRepeatTime(0.0f),
mIsForEffectable(isForEffectable),
mIsRange(false),
mEffectableCtrl(0),
mEffectableCallback(0),
mEffectObjectCallback(0)
{
	mCurveFloatMin.AddPoint(0.0f, initVal, 0.0f, 0.0f, ICM_CURVE_AUTO);
	mCurveFloatMax.AddPoint(0.0f, initVal, 0.0f, 0.0f, ICM_CURVE_AUTO);

	mCurveFloat3Min.AddPoint(0.0f, initVal1, Float3::ZERO, Float3::ZERO,
		ICM_CURVE_AUTO);
	mCurveFloat3Max.AddPoint(0.0f, initVal1, Float3::ZERO, Float3::ZERO,
		ICM_CURVE_AUTO);
}
//----------------------------------------------------------------------------
EffectModule::EffectModule ()
	:
mModuleType(MT_MAX_TYPE),
mRepeatType(RT_NONE),
mRepeatTime(0.0f),
mIsForEffectable(true),
mIsRange(false),
mEffectableCtrl(0),
mEffectableCallback(0),
mEffectObjectCallback(0)
{
}
//----------------------------------------------------------------------------
EffectModule::~EffectModule ()
{
}
//----------------------------------------------------------------------------
void EffectModule::SetRange (bool range)
{
	if (mIsRange == range)
		return;

	mIsRange = range;

	ModuleType mt = GetModuleType();
	if (mIsRange)
	{
		if (EffectModule::MT_FLOAT == mt)
		{
			mCurveFloatMax.Points = mCurveFloatMin.Points;
		}
		else if (EffectModule::MT_FLOAT3 == mt)
		{
			mCurveFloat3Max.Points = mCurveFloat3Min.Points;
		}
		else if (EffectModule::MT_FLOAT3COLOR == mt)
		{
			mCurveFloat3Max.Points = mCurveFloat3Min.Points;
		}
	}
}
//----------------------------------------------------------------------------
int EffectModule::GetNumKeyPoints () const
{
	if (MT_FLOAT == mModuleType)
	{
		return mCurveFloatMin.GetNumPoints();
	}
	else if (MT_FLOAT3 == mModuleType)
	{
		return mCurveFloat3Min.GetNumPoints();
	}
	else if (MT_FLOAT3COLOR == mModuleType)
	{
		return mCurveFloat3Min.GetNumPoints();
	}

	return 0;
}
//----------------------------------------------------------------------------
float *EffectModule::GetKeyPointInVal (int i)
{
	if (MT_FLOAT == mModuleType)
	{
		return &(mCurveFloatMin.Points[i].InVal);
	}
	else if (MT_FLOAT3 == mModuleType)
	{
		return &(mCurveFloat3Min.Points[i].InVal);
	}
	else if (MT_FLOAT3COLOR == mModuleType)
	{
		return &(mCurveFloat3Min.Points[i].InVal);
	}

	return 0;
}
//----------------------------------------------------------------------------
float *EffectModule::GetKeyPointOutValFloatMin (int i)
{
	if (MT_FLOAT == mModuleType)
	{
		return &(mCurveFloatMin.Points[i].OutVal);
	}

	return 0;
}
//----------------------------------------------------------------------------
float *EffectModule::GetKeyPointOutValFloatMax (int i)
{
	if (MT_FLOAT == mModuleType)
	{
		return &(mCurveFloatMax.Points[i].OutVal);
	}

	return 0;
}
//----------------------------------------------------------------------------
Float3 *EffectModule::GetKeyPointOutValFloat3Min (int i)
{
	if (MT_FLOAT3 == mModuleType || MT_FLOAT3COLOR == mModuleType)
	{
		return &(mCurveFloat3Min.Points[i].OutVal);
	}

	return 0;
}
//----------------------------------------------------------------------------
Float3 *EffectModule::GetKeyPointOutValFloat3Max (int i)
{
	if (MT_FLOAT3 == mModuleType || MT_FLOAT3COLOR == mModuleType)
	{
		return &(mCurveFloat3Max.Points[i].OutVal);
	}

	return 0;
}
//----------------------------------------------------------------------------
float *EffectModule::GetKeyPointArriveTangentFloatMin (int i)
{
	if (MT_FLOAT == mModuleType)
	{
		return &(mCurveFloatMin.Points[i].ArriveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
float *EffectModule::GetKeyPointArriveTangentFloatMax (int i)
{
	if (MT_FLOAT == mModuleType)
	{
		return &(mCurveFloatMax.Points[i].ArriveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
float *EffectModule::GetKeyPointLeaveTangentFloatMin (int i)
{
	if (MT_FLOAT == mModuleType)
	{
		return &(mCurveFloatMin.Points[i].LeaveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
float *EffectModule::GetKeyPointLeaveTangentFloatMax (int i)
{
	if (MT_FLOAT == mModuleType)
	{
		return &(mCurveFloatMax.Points[i].LeaveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
Float3 *EffectModule::GetKeyPointArriveTangentFloat3Min (int i)
{
	if (MT_FLOAT3 == mModuleType || MT_FLOAT3COLOR == mModuleType)
	{
		return &(mCurveFloat3Min.Points[i].ArriveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
Float3 *EffectModule::GetKeyPointArriveTangentFloat3Max (int i)
{
	if (MT_FLOAT3 == mModuleType || MT_FLOAT3COLOR == mModuleType)
	{
		return &(mCurveFloat3Max.Points[i].ArriveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
Float3 *EffectModule::GetKeyPointLeaveTangentFloat3Min (int i)
{
	if (MT_FLOAT3 == mModuleType || MT_FLOAT3COLOR == mModuleType)
	{
		return &(mCurveFloat3Min.Points[i].LeaveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
Float3 *EffectModule::GetKeyPointLeaveTangentFloat3Max (int i)
{
	if (MT_FLOAT3 == mModuleType || MT_FLOAT3COLOR == mModuleType)
	{
		return &(mCurveFloat3Max.Points[i].LeaveTangent);
	}

	return 0;
}
//----------------------------------------------------------------------------
void EffectModule::UpdateEffectable(Effectable *able, float ctrlTime)
{
	float lastTime = GetModuleTime(ctrlTime);

	if (mEffectableCallback)
		(*mEffectableCallback)(this, able, lastTime);
}
//----------------------------------------------------------------------------
void EffectModule::UpdateEffectObject (EffectObject *obj)
{
	if (mEffectObjectCallback)
		(*mEffectObjectCallback)(this, obj);
}
//----------------------------------------------------------------------------
float EffectModule::GetModuleTime (float time)
{
	float lastTime = time;

	if (RT_CLAMP == mRepeatType)
	{
		lastTime = Mathf::Clamp(time, 0.0f, mRepeatTime);
	}
	else if (RT_NONE != mRepeatType)
	{
		if (mRepeatTime > 0.0)
		{
			float multiples = time/mRepeatTime;
			float integerTime = Mathf::Floor(multiples);
			float fractionTime = multiples - integerTime;
			if (RT_WRAP == mRepeatType)
			{
				lastTime = fractionTime*mRepeatTime;
			}
			else if (RT_CYCLE == mRepeatType)
			{
				if (((int)integerTime) & 1)
				{
					// 后退.
					lastTime =  mRepeatTime - fractionTime*mRepeatTime;
				}
				else
				{
					// 前进
					lastTime = fractionTime*mRepeatTime;
				}
			}
		}
	}

	return lastTime;
}
//----------------------------------------------------------------------------
void EffectModule::SetUpdateEffectableCallback (
	UpdateEffectableCallback callback)
{
	mEffectableCallback = callback;
}
//----------------------------------------------------------------------------
void EffectModule::SetUpdateEffectObjectCallback (
	UpdateEffectObjectCallback callback)
{
	mEffectObjectCallback = callback;
}
//----------------------------------------------------------------------------
EffectModule *EffectModule::CreateModule (const std::string &name)
{
	std::map<std::string, Pointer0<EffectModule> >::iterator it = 
		msPreCreatedModules.find(name);
	if (it != msPreCreatedModules.end())
	{
		EffectModule *module = DynamicCast<EffectModule>(it->second->Copy(""));

		module->SetUpdateEffectableCallback(it->second->GetUpdateEffectableCallback());
		module->SetUpdateEffectObjectCallback(it->second->GetUpdateEffectObjectCallback());

		return module;
	}
	else
	{
		assertion(false, "module %s does not exist.\n", name.c_str());
	}

	return 0;
}
//----------------------------------------------------------------------------
bool EffectModule::LessThan (const EffectModule *mod0,
	const EffectModule *mod1)
{
	PX2_UNUSED(mod1);

	if (mod0->IsForEffectable())
	{
		if (mod1->IsForEffectable())
		{
			return mod0->GetName().size() > mod1->GetName().size();
		}
		else
		{
			return true;
		}
	}
	else
	{
		if (mod1->IsForEffectable())
		{
			return false;
		}
		else
		{
			return mod0->GetName().size() > mod1->GetName().size();
		}
	}
}
//----------------------------------------------------------------------------
void EffectModule::OnRemove (EffectableController &ctrl)
{
	PX2_UNUSED(ctrl);
}
//----------------------------------------------------------------------------
void EffectModule::SetEffectableController (EffectableController *ctrl)
{
	mEffectableCtrl = ctrl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EffectModule::EffectModule (LoadConstructor value)
	:
Object(value),
	mModuleType(MT_MAX_TYPE),
	mRepeatType(RT_NONE),
	mRepeatTime(0.0f),
	mIsForEffectable(true),
	mIsRange(false),
	mEffectableCtrl(0),
	mEffectableCallback(0),
	mEffectObjectCallback(0)
{
}
//----------------------------------------------------------------------------
void EffectModule::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadString(mTypeName);
	source.ReadBool(mIsForEffectable);
	source.ReadBool(mIsRange);
	source.ReadEnum(mModuleType);
	source.ReadEnum(mRepeatType);
	source.Read(mRepeatTime);
	source.ReadAggregate(mCurveFloatMin);
	source.ReadAggregate(mCurveFloatMax);
	source.ReadAggregate(mCurveFloat3Min);
	source.ReadAggregate(mCurveFloat3Max);

	PX2_END_DEBUG_STREAM_LOAD(EffectModule, source);
}
//----------------------------------------------------------------------------
void EffectModule::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void EffectModule::PostLink ()
{
	Object::PostLink();

	std::map<std::string, Pointer0<EffectModule> >::iterator it = 
		msPreCreatedModules.find(GetTypeName());
	if (it != msPreCreatedModules.end())
	{
		SetUpdateEffectableCallback(it->second->GetUpdateEffectableCallback());
		SetUpdateEffectObjectCallback(it->second->GetUpdateEffectObjectCallback());
	}
}
//----------------------------------------------------------------------------
bool EffectModule::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void EffectModule::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteString(mTypeName);
	target.WriteBool(mIsForEffectable);
	target.WriteBool(mIsRange);
	target.WriteEnum(mModuleType);
	target.WriteEnum(mRepeatType);
	target.Write(mRepeatTime);
	target.WriteAggregate(mCurveFloatMin);
	target.WriteAggregate(mCurveFloatMax);
	target.WriteAggregate(mCurveFloat3Min);
	target.WriteAggregate(mCurveFloat3Max);

	PX2_END_DEBUG_STREAM_SAVE(EffectModule, target);
}
//----------------------------------------------------------------------------
int EffectModule::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_STRINGSIZE(mTypeName);
	size += PX2_BOOLSIZE(mIsForEffectable);
	size += PX2_BOOLSIZE(mIsRange);
	size += PX2_ENUMSIZE(mModuleType);
	size += PX2_ENUMSIZE(mRepeatType);
	size += sizeof(mRepeatTime);
	size += mCurveFloatMin.GetStreamSize();
	size += mCurveFloatMax.GetStreamSize();
	size += mCurveFloat3Min.GetStreamSize();
	size += mCurveFloat3Max.GetStreamSize();

	return size;
}
//----------------------------------------------------------------------------
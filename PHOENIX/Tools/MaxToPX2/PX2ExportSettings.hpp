// PX2ExportSettings.hpp

#ifndef PX2EXPORTSETTINGS_HPP
#define PX2EXPORTSETTINGS_HPP

#include "PX2MaxToPX2PluginPre.hpp"

class ExportSettings
{
public:
	ExportSettings ();

	// object 设置
	bool IncludeObjects;
	bool IncludeLights;
	bool IncludeMeshes;

	// mesh 设置
	bool IncludeVertexColors; 
	bool IncludeNormals;
	bool IncludeTargentBiNormal;
	bool IncludeTexCoords;
	int NumTexCoords;

	static std::vector<std::string> MtlTypes;
	int MtlType;
	const std::string &GetMtlTypeStr() const;

	// modifier 设置
	bool IncludeModifiers;
	bool IncludeSkins;

	// animation 设置
	bool IncludeCurrentFrame;
	bool IncludeKeyFrames;
	bool IncludeAllFrames;
	bool UseLocalTime;
	int StartFrame;
	int EndFrame;

	// 目录
	char SrcRootDir[255];
	char DstRootDir[255];
};

#endif
// PX2Graphics.hpp

#ifndef PX2GRAPHICS_HPP
#define PX2GRAPHICS_HPP

// Controllers
#include "PX2Controlledable.hpp"
#include "PX2Controller.hpp"
#include "PX2InterpCurveAlphaCtrl.hpp"
#include "PX2InterpCurveColorCtrl.hpp"
#include "PX2InterpCurveFloatController.hpp"
#include "PX2InterpCurveShakeCtrl.hpp"
#include "PX2InterpCurveUniformScaleCtrl.hpp"
#include "PX2InterpCurveRotateCtrl.hpp"
#include "PX2InterpCurveScaleCtrl.hpp"
#include "PX2InterpCurveTranslateCtrl.hpp"
#include "PX2InterpCurveSpeedCtrl.hpp"
#include "PX2KeyframeController.hpp"
#include "PX2SkinController.hpp"
#include "PX2TransformController.hpp"
#include "PX2BlendTransformController.hpp"
#include "PX2TargetTransformController.hpp"
#include "PX2PushTransformController.hpp"
#include "PX2InterpCurveAnchorParamCtrl.hpp"

// Animation
#include "PX2ModelController.hpp"
#include "PX2Animation.hpp"
#include "PX2AnimationFrames.hpp"
#include "PX2Animation3DSkeleton.hpp"

// CurvesSurfaces
#include "PX2BoxSurface.hpp"
#include "PX2CurveMesh.hpp"
#include "PX2CurveSegment.hpp"
#include "PX2Float2Array.hpp"
#include "PX2FloatArray.hpp"

// DataTypes
#include "PX2Bound.hpp"
#include "PX2Color.hpp"
#include "PX2HalfFloat.hpp"
#include "PX2SpecializedIO.hpp"
#include "PX2Transform.hpp"

// Detail
#include "PX2BillboardNode.hpp"
#include "PX2ClodMesh.hpp"
#include "PX2CollapseRecord.hpp"
#include "PX2CollapseRecordArray.hpp"
#include "PX2CreateClodMesh.hpp"
#include "PX2DlodNode.hpp"
#include "PX2SwitchNode.hpp"

// GlobalMaterials
#include "PX2GlobalMaterial.hpp"

// LocalMaterials
#include "PX2VertexColor4Material.hpp"

// DynamicBuffer
#include "PX2DynamicBufferManager.hpp"

// Renderers
#include "PX2Renderer.hpp"
#include "PX2Canvas.hpp"

// Resources
#include "PX2Buffer.hpp"
#include "PX2IndexBuffer.hpp"
#include "PX2RenderTarget.hpp"
#include "PX2Texture.hpp"
#include "PX2Texture1D.hpp"
#include "PX2Texture2D.hpp"
#include "PX2Texture3D.hpp"
#include "PX2TextureCube.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2VertexBufferAccessor.hpp"
#include "PX2VertexFormat.hpp"

// SceneGraph
#include "PX2Camera.hpp"
#include "PX2CameraNode.hpp"
#include "PX2Culler.hpp"
#include "PX2EnvirParamController.hpp"
#include "PX2Light.hpp"
#include "PX2LightNode.hpp"
#include "PX2Movable.hpp"
#include "PX2Node.hpp"
#include "PX2Picker.hpp"
#include "PX2PickRecord.hpp"
#include "PX2Polypoint.hpp"
#include "PX2Polysegment.hpp"
#include "PX2Projector.hpp"
#include "PX2Renderable.hpp"
#include "PX2ScreenTarget.hpp"
#include "PX2Shine.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2Triangles.hpp"
#include "PX2TriFan.hpp"
#include "PX2TriMesh.hpp"
#include "PX2TriStrip.hpp"
#include "PX2VisibleSet.hpp"

// ShaderFloats
#include "PX2CameraModelDVectorConstant.hpp"
#include "PX2CameraModelPositionConstant.hpp"
#include "PX2CameraWorldDVectorConstant.hpp"
#include "PX2CameraWorldPositionConstant.hpp"
#include "PX2LightAmbientConstant.hpp"
#include "PX2LightAttenuationConstant.hpp"
#include "PX2LightDiffuseConstant.hpp"
#include "PX2LightModelDVectorConstant.hpp"
#include "PX2LightModelPositionConstant.hpp"
#include "PX2LightSpecularConstant.hpp"
#include "PX2LightSpotConstant.hpp"
#include "PX2LightWorldDVectorConstant.hpp"
#include "PX2LightWorldPositionConstant.hpp"
#include "PX2PMatrixConstant.hpp"
#include "PX2PVMatrixConstant.hpp"
#include "PX2PVWMatrixConstant.hpp"
#include "PX2ProjectorMatrixConstant.hpp"
#include "PX2ProjectorWorldPositionConstant.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2ShineAmbientConstant.hpp"
#include "PX2ShineDiffuseConstant.hpp"
#include "PX2ShineEmissiveConstant.hpp"
#include "PX2ShineSpecularConstant.hpp"
#include "PX2VMatrixConstant.hpp"
#include "PX2VWMatrixConstant.hpp"
#include "PX2WMatrixConstant.hpp"

// ShaderFloats
#include "PX2CameraModelDVectorConstant.hpp"
#include "PX2CameraModelPositionConstant.hpp"
#include "PX2CameraWorldDVectorConstant.hpp"
#include "PX2CameraWorldPositionConstant.hpp"
#include "PX2FogColorDistConstant.hpp"
#include "PX2FogColorHeightConstant.hpp"
#include "PX2FogParamConstant.hpp"
#include "PX2LightAmbientConstant.hpp"
#include "PX2LightAttenuationConstant.hpp"
#include "PX2LightDiffuseConstant.hpp"
#include "PX2LightModelDVectorConstant.hpp"
#include "PX2LightModelPositionConstant.hpp"
#include "PX2LightSpecularConstant.hpp"
#include "PX2LightSpotConstant.hpp"
#include "PX2LightWorldDVectorConstant.hpp"
#include "PX2LightWorldPositionConstant.hpp"
#include "PX2PMatrixConstant.hpp"
#include "PX2PVMatrixConstant.hpp"
#include "PX2PVWMatrixConstant.hpp"
#include "PX2ProjectorMatrixConstant.hpp"
#include "PX2ProjectorWorldPositionConstant.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2ShineAmbientConstant.hpp"
#include "PX2ShineDiffuseConstant.hpp"
#include "PX2ShineEmissiveConstant.hpp"
#include "PX2ShineSpecularConstant.hpp"
#include "PX2SkinMatrixConstant.hpp"
#include "PX2UserConstant.hpp"
#include "PX2VMatrixConstant.hpp"
#include "PX2VWMatrixConstant.hpp"
#include "PX2WMatrixConstant.hpp"

// Shaders
#include "PX2AlphaProperty.hpp"
#include "PX2CullProperty.hpp"
#include "PX2DepthProperty.hpp"
#include "PX2OffsetProperty.hpp"
#include "PX2PixelShader.hpp"
#include "PX2Shader.hpp"
#include "PX2ShaderParameters.hpp"
#include "PX2StencilProperty.hpp"
#include "PX2VertexShader.hpp"
#include "PX2Material.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2MaterialPass.hpp"
#include "PX2MaterialTechnique.hpp"
#include "PX2WireProperty.hpp"
#include "PX2ShaderKeys.hpp"

// CanvasWindow
#include "PX2Canvas.hpp"
#include "PX2RenderWindow.hpp"

// GraphicsRoot
#include "PX2GraphicsRoot.hpp"

#endif

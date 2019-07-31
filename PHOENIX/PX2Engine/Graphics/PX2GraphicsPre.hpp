// PX2GraphicsPre.hpp

#ifndef PX2GRAPHICSPRERE_HPP
#define PX2GRAPHICSPRERE_HPP

#include "PX2MathematicsPre.hpp"

// 加上此宏会对输入的相机是否正交化进行检查，只进行一次正交化检查，只
// 需要进行一次正交化。
#ifdef _DEBUG
    #define PX2_VALIDATE_CAMERA_FRAME_ONCE
#endif

// 当改变了Shader::MAX_PROFILES，在shader持久化读取时使用此宏产生断言。
#ifdef _DEBUG
    #define PX2_ASSERT_ON_CHANGED_MAX_PROFILES
#endif

// 设置这个宏，Renderer::Draw (const Renderable*, const MaterialInstance*)渲染
// 完毕，内部会置回到缺省渲染状态。
#ifdef _DEBUG
	//#define PX2_RESET_STATE_AFTER_DRAW
#endif

// 加上此宏进行渲染像素量的计算
// Renderer::DrawPrimitive.
//#define PX2_QUERY_PIXEL_COUNT

#ifdef PX2_USE_OPENGL
    #ifndef __LINUX__
        #define PX2_USE_TEXT_DISPLAY_LIST
    #endif

    #ifndef __APPLE__
        #define PX2_USE_OPENGL2_NORMAL_ATTRIBUTES
    #endif
#endif

// Dx9渲染器中的一些检测
#ifdef PX2_USE_DX9
    #ifdef _DEBUG
        //#define PX2_PDR_DEBUG
    #endif
#endif

#endif
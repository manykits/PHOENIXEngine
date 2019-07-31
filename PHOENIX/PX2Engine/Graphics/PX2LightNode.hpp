// PX2LightNode.hpp

#ifndef PX2LIGHTNODE_H
#define PX2LIGHTNODE_H

#include "PX2GraphicsPre.hpp"
#include "PX2Light.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	/// 光源节点（Node）类
	/**
	* Node的世界变换被用作Light的空间方位。Node的世界旋转矩阵被用作Light的坐
	* 标轴方向。世界变换的第0列是光源方向（direction）向量，第1列是光源的上（up）
	* 向量，第2列是光源的右（right）向量。
	*/
	class PX2_ENGINE_ITEM LightNode : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightNode);

	public:
		/// 构造函数
		/**
		* 在构造函数中，光源的当前坐标系统被作为Node的本地变换。
		*	local translation		= light location
		*	local rotation column 0 = light direction
		*	local rotation column 1 = light up
		*	local rotation column 2 = light right
		*/
		LightNode (Light* light = 0);
		virtual ~LightNode ();

		/// 设置光源
		/**
		* 光源的当前坐标系统被设置为Node的本地变换,然后调用Update更新
		* 到世界变换;
		*/
		void SetLight (Light* light);

		inline Light* GetLight ();
		inline const Light* GetLight () const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		bool mIsNeedUpdate;
		LightPtr mLight;
	};

	PX2_REGISTER_STREAM(LightNode);
	typedef Pointer0<LightNode> LightNodePtr;
#include "PX2LightNode.inl"

}

#endif

// PX2Picker.hpp

#ifndef PX2PICKER_HPP
#define PX2PICKER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2PickRecord.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	/// 选择器类
	/** 
	* 线的表现形式为P + t*D，P是线原点，D是单位长度的方向向量，t是方向上的
	* 缩放参数，在区间[tmin,tmax]内，tmin < tmax。P和D必须是世界坐标系里的值。
	* 不同类型参数选择，决定线的类型。
	* 直线line:		tmin = -Mathf::MAX_REAL, tmax = Mathf::MAX_REAL
	* 射线ray：		tmin = 0, tmax = Mathf::MAX_REAL
	* 线段segment：	tmin = 0, tmax > 0
	*/
	class PX2_ENGINE_ITEM Picker
	{
	public:
		Picker (bool isDoMovPickCall=false, int pickInfo=-1);
		~Picker ();

		/// 执行检测，将记录保存到Records中。
		/**
		* 调用此函数自动清空纪录数组。如果先前记录集里就有信息，你必须先保存。
		*/
		void Execute (Movable* scene, const APoint& origin,
			const AVector& direction, float tmin, float tmax);

		// 下面的3个函数，返回挑选的场景节点，当且仅当Records.size() > 0时，才
		// 可被调用。

		/// 返回绝对值T最接近0的三角形记录
		/**
		* 如果Records为空，返回无效值。
		*/
		const PickRecord& GetClosestToZero () const;

		/// 返回非负的T最接近0的三角形记录
		/** 
		* 如果Records为空，返回无效值。
		*/
		const PickRecord& GetClosestNonnegative () const;

		/// 返回非正的T最接近0的三角形记录
		/** 
		* 如果Records为空，返回无效值。
		*/
		const PickRecord& GetClosestNonpositive () const;

		/// 挑选的记录集
		std::vector<PickRecord> Records;

	private:
		void ExecuteRecursive (Movable *object, bool &hasMeshPicked);
		int mPickInfo;
		bool mIsDoMovPickCall;
		APoint mOrigin;
		AVector mDirection;
		float mTMin, mTMax;

		PickRecord sNotValiedRecord;
	};

}

#endif

// PX2VertexFormat.hpp

#ifndef PX2VERTEXFORMAT_HPP
#define PX2VERTEXFORMAT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{
	
	/// 顶点格式类
	class PX2_ENGINE_ITEM VertexFormat : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(VertexFormat);

	public:
		enum
		{
			// 一个顶点格式最大通道数
			AM_MAX_ATTRIBUTES = 16,

			// 一个顶点格式最大纹理坐标通道数
			AM_MAX_TCOORD_UNITS = 8,

			// 一个顶点格式颜色通道数
			AM_MAX_COLOR_UNITS = 2
		};

		/// 每个通道的数据格式
		enum AttributeType
		{
			AT_NONE,
			AT_FLOAT1,
			AT_FLOAT2,
			AT_FLOAT3,
			AT_FLOAT4,
			AT_HPPALF1,
			AT_HPPALF2,
			AT_HPPALF3,
			AT_HPPALF4,
			AT_UBYTE4,
			AT_SHORT1,
			AT_SHORT2,
			AT_SHORT4,
			AT_QUANTITY
		};

		/// 顶点格式每个通道语义
		enum AttributeUsage
		{
			AU_NONE,
			AU_POSITION,      // attr 0
			AU_NORMAL,        // attr 2
			AU_TANGENT,       // attr 14
			AU_BINORMAL,      // attr 15
			AU_TEXCOORD,      // attr 8-15
			AU_COLOR,         // attr 3-4
			AU_BLENDINDICES,  // attr 7
			AU_BLENDWEIGHT,   // attr 1
			AU_FOGCOORD,      // attr 5
			AU_PSIZE,         // attr 6
			AU_QUANTITY
		};

		VertexFormat ();
		VertexFormat (int numAttributes);
		virtual ~VertexFormat ();

		/// 创建顶点格式
		/**
		* numAttributes表示顶点通道的个数。剩下参数是下面的顺序
		*	(AttributeUsage, AttributeType, usage index)
		*	 ...
		*/
		static VertexFormat* Create (int numAttributes, ...);

		/// 创建顶点格式
		void Add (AttributeUsage usage, AttributeType type, unsigned int usageIndex);
		void Create ();

		/// 设置各个通道的顶点属性
		void SetAttribute (int attribute, unsigned int streamIndex,
			unsigned int offset, AttributeType type, AttributeUsage usage,
			unsigned int usageIndex);

		void SetStride (int stride);

		/// 获得通道数量
		inline int GetNumAttributes () const;

		inline unsigned int GetStreamIndex (int attribute) const;
		inline unsigned int GetOffset (int attribute) const;
		inline AttributeType GetAttributeType (int attribute) const;
		inline AttributeUsage GetAttributeUsage (int attribute) const;
		inline unsigned int GetUsageIndex (int attribute) const;

		void GetAttribute (int attribute, unsigned int& streamIndex,
			unsigned int& offset, AttributeType& type, AttributeUsage& usage,
			unsigned int& usageIndex) const;

		inline int GetStride () const;

		/// 获得usage属性的通道索引
		/**
		* 如果usage在顶点格式中存在，返回的索引i满足，0 <= i < GetNumAttributes()；
		* 如果usage不存在，返回-1。
		*/
		int GetIndex (AttributeUsage usage, unsigned int usageIndex = 0) const;

		/// 获得type的每个元组的字节大小
		inline static int GetComponentSize (AttributeType type);

		/// 获得type的元组个数
		inline static int GetNumComponents (AttributeType type);

		/// 每个通道的字节大小
		inline static int GetTypeSize (AttributeType type);

		// pdr
	public:
		void *PdrPointer;

		/// 顶点格式通道信息
		class Element
		{
		public:
			unsigned int StreamIndex;
			unsigned int Offset;
			AttributeType Type;
			AttributeUsage Usage;
			unsigned int UsageIndex;
		};

		int mOffsetTemp; // 创建顶点格式用，不用存储

		int mNumAttributes;
		Element mElements[AM_MAX_ATTRIBUTES];
		int mStride;

		static int msComponentSize[AT_QUANTITY]; //< 每个AttributeType的每个元组的字节大小
		static int msNumComponents[AT_QUANTITY]; //< 每个AttributeType的元组个数
		static int msTypeSize[AT_QUANTITY]; //< 每个AttributeType的字节大小（msComponentSize[i]*msNumComponents[i]）
	};

	PX2_REGISTER_STREAM(VertexFormat);
	typedef Pointer0<VertexFormat> VertexFormatPtr;
#include "PX2VertexFormat.inl"

}

#endif

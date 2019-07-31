// PX2SpecializedIO.hpp

#ifndef PX2SPECIALIZEDIO_HPP
#define PX2SPECIALIZEDIO_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2InStream.hpp"
#include "PX2OutStream.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2Bound.hpp"
#include "PX2Float1.hpp"
#include "PX2Float2.hpp"
#include "PX2Float3.hpp"
#include "PX2Float4.hpp"
#include "PX2HMatrix.hpp"
#include "PX2HPlane.hpp"
#include "PX2HQuaternion.hpp"
#include "PX2Transform.hpp"
#include "PX2Vector3.hpp"
#include "PX2Size.hpp"
#include "PX2Rect.hpp"
#include "PX2InterpCurve.hpp"

namespace PX2
{
	// Bound
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Bound&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Bound*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Bound*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Bound*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Bound&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Bound*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Bound*);

	// Float1
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Float1&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Float1*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Float1*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Float1*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Float1&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Float1*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Float1*);

	// Float2
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Float2&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Float2*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Float2*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Float2*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Float2&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Float2*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Float2*);

	// Float3
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Float3&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Float3*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Float3*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Float3*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Float3&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Float3*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Float3*);

	// Float4
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Float4&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Float4*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Float4*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Float4*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Float4&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Float4*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Float4*);

	// HMatrix
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(HMatrix&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, HMatrix*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, HMatrix*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, HMatrix*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const HMatrix&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const HMatrix*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const HMatrix*);

	// HPlane
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(HPlane&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, HPlane*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, HPlane*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, HPlane*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const HPlane&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const HPlane*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const HPlane*);

	// APoint
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(APoint&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, APoint*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, APoint*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, APoint*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const APoint&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const APoint*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const APoint*);

	// AVector
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(AVector&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, AVector*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, AVector*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, AVector*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const AVector&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const AVector*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const AVector*);

	// HQuaternion
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(HQuaternion&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, HQuaternion*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, HQuaternion*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, HQuaternion*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const HQuaternion&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const HQuaternion*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const HQuaternion*);

	// Transform
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Transform&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Transform*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Transform*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Transform*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Transform&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Transform*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Transform*);

	// Vector3f
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Vector3f&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Vector3f*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Vector3f*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Vector3f*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Vector3f&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Vector3f*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Vector3f*);

	// Sizef
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Sizef&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Sizef*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Sizef*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Sizef*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Sizef&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Sizef*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Sizef*);

	// Rectf
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(Rectf&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, Rectf*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, Rectf*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, Rectf*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const Rectf&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const Rectf*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const Rectf*);

	// InterpCurvePoint
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(InterpCurvePointFloat&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, InterpCurvePointFloat*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, InterpCurvePointFloat*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, InterpCurvePointFloat*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const InterpCurvePointFloat&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const InterpCurvePointFloat*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const InterpCurvePointFloat*);

	// InterpCurvePoint3
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(InterpCurvePointFloat3&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, InterpCurvePointFloat3*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, InterpCurvePointFloat3*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, InterpCurvePointFloat3*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const InterpCurvePointFloat3&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const InterpCurvePointFloat3*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const InterpCurvePointFloat3*);

	// InterpCurveFloat
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(InterpCurveFloat&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, InterpCurveFloat*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, InterpCurveFloat*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, InterpCurveFloat*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const InterpCurveFloat&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const InterpCurveFloat*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const InterpCurveFloat*);

	// InterpCurveFloat3
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregate(InterpCurveFloat3&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVV(int, InterpCurveFloat3*);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateVR(int, InterpCurveFloat3*&);
	template <> PX2_ENGINE_ITEM bool InStream::ReadAggregateRR(int&, InterpCurveFloat3*&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregate(const InterpCurveFloat3&);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateW(int, const InterpCurveFloat3*);
	template <> PX2_ENGINE_ITEM bool OutStream::WriteAggregateN(int, const InterpCurveFloat3*);

}

#endif

// PX2InterpCurve.inl

//----------------------------------------------------------------------------
static std::string gsInterpCurveModeString[ICM_MAX_MODE] =
{
	"ICM_LINEAR",
	"ICM_CONSTANT",
	"ICM_CURVE_AUTO",
	"ICM_CURVE_AUTOCLAMPED",
	"ICM_CURVE_USER",
	"ICM_CURVE_BREAK"
};
//----------------------------------------------------------------------------
inline std::string ICMToString (InterpCurveMode mode)
{
	return gsInterpCurveModeString[mode];
}
//----------------------------------------------------------------------------
inline InterpCurveMode StringToICM (std::string str)
{
	if (str == "ICM_LINEAR")
		return ICM_LINEAR;
	else if (str == "ICM_CONSTANT")
		return ICM_CONSTANT;
	else if (str == "ICM_CURVE_AUTO")
		return ICM_CURVE_AUTO;
	else if (str == "ICM_CURVE_AUTOCLAMPED")
		return ICM_CURVE_AUTOCLAMPED;
	else if (str == "ICM_CURVE_USER")
		return ICM_CURVE_USER;
	else if (str == "ICM_CURVE_BREAK")
		return ICM_CURVE_BREAK;

	return ICM_MAX_MODE;
}
//----------------------------------------------------------------------------
template <typename Real>
InterpCurvePoint<Real>::InterpCurvePoint ()
	:
InVal(0.0f),
InterpMode(ICM_LINEAR)
{
}
//----------------------------------------------------------------------------
template <typename Real>
InterpCurvePoint<Real>::InterpCurvePoint(const float inVal, const Real &outVal)
	:
InVal(inVal),
OutVal(outVal),
InterpMode(ICM_LINEAR)
{
}
//----------------------------------------------------------------------------
template <typename Real>
InterpCurvePoint<Real>::InterpCurvePoint (const float inVal, 
	const Real &outVal,	const Real &inArriveTangent, const Real &inLeaveTangent,
	InterpCurveMode interpMode)
	:
InVal(inVal),
OutVal(outVal), 
ArriveTangent(inArriveTangent),
LeaveTangent(inLeaveTangent),
InterpMode(interpMode)
{
}
//----------------------------------------------------------------------------
template <typename Real>
bool InterpCurvePoint<Real>::IsCurveKey () const
{
	return ((InterpMode == ICM_CURVE_AUTO) 
		|| (InterpMode == ICM_CURVE_AUTOCLAMPED)
		|| (InterpMode == ICM_CURVE_USER) 
		|| (InterpMode == ICM_CURVE_BREAK));
}
//----------------------------------------------------------------------------
template <typename Real>
int InterpCurvePoint<Real>::GetStreamSize () const
{
	int size = 0;
	size += sizeof(InVal);
	size += sizeof(OutVal);
	size += sizeof(ArriveTangent);
	size += sizeof(LeaveTangent);
	size += 4;
	return size;
}
//----------------------------------------------------------------------------
template <typename Real>
int InterpCurve<Real>::AddPoint (const float inVal, const Real &outVal)
{
	typename std::vector<InterpCurvePoint<Real> >::iterator it = Points.begin();
	int i = 0;
	for (; it!=Points.end()&&(*it).InVal<inVal; it++, i++);

	Points.insert(it, InterpCurvePoint<Real>(inVal, outVal));

	return i;
}
//----------------------------------------------------------------------------
template <typename Real>
int InterpCurve<Real>::AddPoint (const float inVal, const Real &outVal,
	const Real &arriveTangent, const Real &leaveTangent,
	InterpCurveMode interpMode)
{
	typename std::vector<InterpCurvePoint<Real> >::iterator it = Points.begin();
	int i = 0;
	for (; it!=Points.end()&&(*it).InVal<inVal; it++, i++);

	Points.insert(it, InterpCurvePoint<Real>(inVal, outVal, arriveTangent,
		leaveTangent, interpMode));

	return i;
}
//----------------------------------------------------------------------------
template <typename Real>
int InterpCurve<Real>::MovePoint (int i, float newInVal)
{
	if (i<0 || i>(int)Points.size())
		return 0;

	const Real outVal = Points[i].OutVal;
	const InterpCurveMode mode = Points[i].InterpMode;
	const Real arriveTan = Points[i].ArriveTangent;
	const Real leaveTan = Points[i].LeaveTangent;

	int tempI = 0;
	typename std::vector<InterpCurvePoint<Real> >::iterator it=Points.begin();
	for (; it!=Points.end(); it++, tempI++)
	{
		if (i == tempI)
		{
			Points.erase(it);
			break;
		}
	}

	int newIndex = AddPoint(newInVal, outVal);
	Points[newIndex].InterpMode = mode;
	Points[newIndex].ArriveTangent = arriveTan;
	Points[newIndex].LeaveTangent = leaveTan;

	return newIndex;
}
//----------------------------------------------------------------------------
template <typename Real>
void InterpCurve<Real>::RemovePoint (int i)
{
	if (i<0 || i>(int)Points.size())
		return;

	int tempI = 0;
	typename std::vector<InterpCurvePoint<Real> >::iterator it=Points.begin();
	for (; it!=Points.end(); it++, tempI++)
	{
		if (i == tempI)
		{
			Points.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
template <typename Real>
int InterpCurve<Real>::GetNumPoints () const
{
	return (int)Points.size();
}
//----------------------------------------------------------------------------
template <typename Real>
void InterpCurve<Real>::Reset ()
{
	Points.clear();
}
//----------------------------------------------------------------------------
template <typename Real>
Real InterpCurve<Real>::Eval (const float inVal, const Real &defVal, 
	int *ptIndex) const
{
	const int numPoints = (int)Points.size();

	if (0 == numPoints)
	{
		if (ptIndex)
			*ptIndex = -1;

		return defVal;
	}

	// only one point
	if (numPoints<2 || inVal<=Points[0].InVal)
	{
		if (ptIndex)
			*ptIndex = 0;

		return Points[0].OutVal;
	}

	// last point
	if (inVal >= Points[numPoints-1].InVal)
	{
		if (ptIndex)
			*ptIndex = numPoints-1;

		return Points[numPoints-1].OutVal;
	}

	// in range
	for (int i=1; i<(int)Points.size(); i++)
	{
		if (inVal < Points[i].InVal)
		{
			const float diff = Points[i].InVal - Points[i-1].InVal;

			if (diff>0.0f && Points[i-1].InterpMode!=ICM_CONSTANT)
			{
				const float alpha = (inVal - Points[i-1].InVal)/diff;

				if (ptIndex)
					*ptIndex = i-1;

				if (ICM_LINEAR == Points[i-1].InterpMode)
				{
					return Lerp(Points[i-1].OutVal, Points[i].OutVal, alpha);
				}
				else
				{
					if (IMT_BROKEN_TANGENT_EVAL == InterpMethod)
					{
						return CubicInterp(
							Points[i-1].OutVal,	Points[i-1].LeaveTangent, 
							Points[i].OutVal, Points[i].ArriveTangent, alpha);
					}
					else
					{
						Real leaveTangent = Points[i-1].LeaveTangent*diff;
						Real arriveTangent = Points[i].ArriveTangent*diff;
						return CubicInterp(
							Points[i-1].OutVal, leaveTangent, 
							Points[i].OutVal, arriveTangent, alpha);
					}
				}
			}
			else
			{
				if (ptIndex)
					*ptIndex = i-1;

				return Points[i-1].OutVal;
			}
		}
	}

	assertion(false, "Shouldn't really reach here.\n");
	if (ptIndex)
	{
		*ptIndex = numPoints-1;
	}

	return Points[numPoints-1].OutVal;
}
//----------------------------------------------------------------------------
template <typename Real>
Real InterpCurve<Real>::EvalDerivative (const float inVal, const Real &defVal,
	int *ptIndex) const
{
	const int numPoints = (int)Points.size();

	if (0 == numPoints)
	{
		if (ptIndex)
			*ptIndex = -1;

		return defVal;
	}

	// only one point
	if (numPoints<2 || inVal<=Points[0].InVal)
	{
		if (ptIndex)
			*ptIndex = 0;

		return Points[0].LeaveTangent;
	}

	// last point
	if (inVal >= Points[numPoints-1].InVal)
	{
		if (ptIndex)
			*ptIndex = numPoints-1;

		return Points[numPoints-1].ArriveTangent;
	}

	// in range
	for (int i=1; i<(int)Points.size(); i++)
	{
		if (inVal < Points[i].InVal)
		{
			const float diff = Points[i].InVal - Points[i-1].InVal;

			if (diff>0.0f && Points[i-1].InterpMode!=ICM_CONSTANT)
			{
				const float alpha = (inVal - Points[i-1].InVal)/diff;

				if (ptIndex)
					*ptIndex = i-1;

				if (ICM_LINEAR == Points[i-1].InterpMode)
				{
					return Lerp(Points[i-1].OutVal, Points[i].OutVal, alpha);
				}
				else
				{
					return CubicInterpDerivative(
						Points[i-1].OutVal, Points[i-1].LeaveTangent*diff, 
						Points[i].OutVal, Points[i].ArriveTangent*diff, alpha);
				}
			}
			else
			{
				if (ptIndex)
					*ptIndex = i-1;

				return Points[i-1].OutVal;
			}
		}
	}

	assertion(false, "Shouldn't really reach here.\n");
	if (ptIndex)
	{
		*ptIndex = numPoints-1;
	}

	return Points[numPoints-1].OutVal;
}
//----------------------------------------------------------------------------
template <typename Real>
Real InterpCurve<Real>::EvalSecondDerivative (const float inVal,
	const Real &defVal,	int *ptIndex) const
{
	const int numPoints = (int)Points.size();

	if (0 == numPoints)
	{
		if (ptIndex)
			*ptIndex = -1;

		return defVal;
	}

	// only one point
	if (numPoints<2 || inVal<=Points[0].InVal)
	{
		if (ptIndex)
			*ptIndex = 0;

		return Points[0].LeaveTangent;
	}

	// last point
	if (inVal >= Points[numPoints-1].InVal)
	{
		if (ptIndex)
			*ptIndex = numPoints-1;

		return Points[numPoints-1].ArriveTangent;
	}

	// in range
	for (int i=1; i<(int)Points.size(); i++)
	{
		if (inVal < Points[i].InVal)
		{
			const float diff = Points[i].InVal - Points[i-1].InVal;

			if (diff>0.0f && Points[i-1].InterpMode!=ICM_CONSTANT)
			{
				const float alpha = (inVal - Points[i-1].InVal)/diff;

				if (ptIndex)
					*ptIndex = i-1;

				if (ICM_LINEAR == Points[i-1].InterpMode)
				{
					return Lerp+	
						+(Points[i-1].OutVal, Points[i].OutVal, alpha);
				}
				else
				{
					return CubicInterpSecondDerivative(
						Points[i-1].OutVal, Points[i-1].LeaveTangent*diff, 
						Points[i].OutVal, Points[i].ArriveTangent*diff, alpha);
				}
			}
			else
			{
				if (ptIndex)
					*ptIndex = i-1;

				return Points[i-1].OutVal;
			}
		}
	}

	assertion(false, "Shouldn't really reach here.\n");
	if (ptIndex)
	{
		*ptIndex = numPoints-1;
	}

	return Points[numPoints-1].OutVal;
}
//----------------------------------------------------------------------------
template <typename Real>
bool InterpCurve<Real>::IsUsingLegacyInterpMethod() const
{
	return (InterpMethod != IMT_FIXED_TANGENT_EVAL_NEWAUTOTANGENTS);
}
//----------------------------------------------------------------------------
template <typename Real>
void InterpCurve<Real>::UpgradeInterpMethod()
{
	if (IsUsingLegacyInterpMethod())
	{
		for (int i=0; i<(int)Points.size(); i++)
		{
			InterpCurvePoint<Real> &point = Points[i];

			if (ICM_CURVE_AUTO == point.InterpMode ||
				ICM_CURVE_AUTOCLAMPED == point.InterpMode)
			{
				point.InterpMode = ICM_CURVE_USER;
			}
		}

		InterpMethod = IMT_FIXED_TANGENT_EVAL_NEWAUTOTANGENTS;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
void InterpCurve<Real>::AutoSetTangents (float tension)
{
	int pointsNum = (int)Points.size();

	for (int i=0; i<pointsNum; i++)
	{
		Real arriveTangent = Points[i].ArriveTangent;
		Real leaveTangent = Points[i].LeaveTangent;

		if (0 == i)
		{
			if (i < pointsNum-1) // start point
			{
				if (ICM_CURVE_AUTO==Points[i].InterpMode ||
					ICM_CURVE_AUTOCLAMPED==Points[i].InterpMode)
				{
					memset(&leaveTangent, 0, sizeof(Real));
				}
			}
			else // only one point
			{
				memset(&leaveTangent, 0, sizeof(Real));
			}
		}
		else
		{
			if (i < pointsNum-1) // Inner point
			{
				if (ICM_CURVE_AUTO==Points[i].InterpMode ||
					ICM_CURVE_AUTOCLAMPED==Points[i].InterpMode)
				{
					if (Points[i-1].IsCurveKey() && Points[i].IsCurveKey())
					{
						if (IMT_FIXED_TANGENT_EVAL_NEWAUTOTANGENTS == InterpMethod)
						{
							const bool wantClamping = (ICM_CURVE_AUTOCLAMPED==Points[i].InterpMode);

							ComputeCurveTangent(
								Points[i - 1].InVal,	// Previous time
								Points[i - 1].OutVal,	// Previous point
								Points[i].InVal,		// Current time
								Points[i].OutVal,		// Current point
								Points[i + 1].InVal,	// Next time
								Points[i + 1].OutVal,	// Next point
								tension,				// Tension
								wantClamping,			// Want clamping?
								arriveTangent );		// Out
						}
						else
						{
							LegacyAutoCalcTangent(
								Points[i - 1].OutVal,
								Points[i].OutVal,
								Points[i + 1].OutVal,
								tension,
								arriveTangent);
						}
					}
					else if(ICM_CONSTANT == Points[i-1].InterpMode
						|| ICM_CONSTANT == Points[i].InterpMode)
					{
						memset(&arriveTangent, 0, sizeof(Real));
						memset(&leaveTangent, 0, sizeof(Real));
					}
				}
			}
			else
			{
				if (ICM_CURVE_AUTO == Points[i].InterpMode ||
					ICM_CURVE_AUTOCLAMPED == Points[i].InterpMode)
				{
					memset(&arriveTangent, 0, sizeof(Real));
				}
			}
		}

		Points[i].ArriveTangent = arriveTangent;
		Points[i].LeaveTangent = leaveTangent;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
int InterpCurve<Real>::GetStreamSize () const
{
	int size = 0;

	int numPoinst = (int)Points.size();

	size += sizeof(numPoinst);
	for (int i=0; i<numPoinst; i++)
	{
		size += Points[i].GetStreamSize();
	}
	size += 4;

	return size;
}
//----------------------------------------------------------------------------
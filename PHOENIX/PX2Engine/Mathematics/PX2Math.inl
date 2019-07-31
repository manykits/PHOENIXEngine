// PX2Math.inl

//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::ACos (Real value)
{
	if (-(Real)1 < value)
	{
		if (value < (Real)1)
		{
			return (Real)acos(value);
		}
		else
		{
			return (Real)0;
		}
	}
	else
	{
		return PI;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::ASin (Real value)
{
	if (-(Real)1 < value)
	{
		if (value < (Real)1)
		{
			return (Real)asin(value);
		}
		else
		{
			return HALF_PI;
		}
	}
	else
	{
		return -HALF_PI;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::ATan (Real value)
{
	return (Real)atan(value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::ATan2 (Real y, Real x)
{
	if (x != (Real)0 || y != (Real)0)
	{
		return (Real)atan2(y, x);
	}
	else
	{
		// 数学上，ATan2(0,0)是非法的。但是ANSI的标准会返回0。
		return (Real)0;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Ceil (Real value)
{
	return (Real)ceil(value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Cos (Real value)
{
	return (Real)cos(value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Exp (Real value)
{
	return (Real)exp(value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::FAbs (Real value)
{
	return (Real)fabs((float)value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Floor (Real value)
{
	return (Real)floor((float)value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::FMod (Real x, Real y)
{
	if (y != (Real)0)
	{
		return (Real)fmod(x, y);
	}
	else
	{
		assertion(false, "Zero input to FMod\n");
		return (Real)0;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::InvSqrt (Real value)
{
	if (value != (Real)0)
	{
		return ((Real)1)/sqrt(value);
	}
	else
	{
		assertion(false, "Division by zero in InvSqr\n");
		return (Real)0;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Log (Real value)
{
	if (value > (Real)0)
	{
		return (Real)log(value);
	}
	else
	{
		assertion(false, "Nonpositive input to Log\n");
		return (Real)0;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Log2 (Real value)
{
	if (value > (Real)0)
	{
		return (Real)(Math<Real>::INV_LN_2 * log(value));
	}
	else
	{
		assertion(false, "Nonpositive input to Log2\n");
		return (Real)0;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Log10 (Real value)
{
	if (value > (Real)0)
	{
		return (Real)(Math<Real>::INV_LN_10 * log(value));
	}
	else
	{
		assertion(false, "Nonpositive input to Log10\n");
		return (Real)0;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Pow (Real base, Real exponent)
{
	if (base >= (Real)0)
	{
		return (Real)pow(base, exponent);
	}
	else
	{
		assertion(false, "Negative base not allowed in Pow\n");
		return Math<Real>::MAX_REAL;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Sin (Real value)
{
	return (Real)sin(value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Sqr (Real value)
{
	return value*value;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Sqrt (Real value)
{
	if (value >= (Real)0)
	{
		return (Real)sqrt(value);
	}
	else
	{
		//assertion(false, "Negative input to Sqrt\n");
		return (Real)0;
	}
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Tan (Real value)
{
	return (Real)tan(value);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Max (Real a, Real b)
{
	return a>b ? a : b;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Min (Real a, Real b)
{
	return a<b ? a : b;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Sign (Real value)
{
	if (value > (Real)0)
	{
		return (Real)1;
	}

	if (value < (Real)0)
	{
		return (Real)-1;
	}

	return (Real)0;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::UnitRandom (unsigned int seed)
{
	if (seed > 0)
	{
		srand(seed);
	}

	Real ratio = ((Real)rand())/((Real)(RAND_MAX));
	return (Real)ratio;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::SymmetricRandom (unsigned int seed)
{
	return ((Real)2)*UnitRandom(seed) - (Real)1;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::IntervalRandom (Real min, Real max, unsigned int seed)
{
	return min + (max - min)*UnitRandom(seed);
}
//----------------------------------------------------------------------------
template <typename Real>
int Math<Real>::IntRandom (int min, int max, unsigned int seed)
{
	return (int)(UnitRandom(seed) * ((float)max - 0.000001f - (float)min)
		+ min);
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Clamp (Real value, Real minValue, Real maxValue)
{
	if (value <= minValue)
	{
		return minValue;
	}
	if (value >= maxValue)
	{
		return maxValue;
	}
	return value;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Math<Real>::Saturate (Real value)
{
	if (value <= (Real)0)
	{
		return (Real)0;
	}
	if (value >= (Real)1)
	{
		return (Real)1;
	}
	return value;
}
//----------------------------------------------------------------------------
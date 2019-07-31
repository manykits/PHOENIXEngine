// PX2Smoother.hpp

#ifndef PX2SMOOTHER_HPP
#define PX2SMOOTHER_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	template <typename T>
	class Smoother
	{
	public:
		Smoother(int sampleSize, T zeroValue) :
			mHistory(sampleSize, zeroValue),
			mZeroValue(zeroValue),
			mNextUpdateSlot(0),
			mCurUpdateSeconds(0.0f)
		{
			mCurValue = mZeroValue;
		}
		~Smoother() {}

		T Update(const T &mostRecentValue, float elapsedSeconds)
		{
			mCurUpdateSeconds += elapsedSeconds;
			if (mCurUpdateSeconds > 0.03f)
			{
				mHistory[mNextUpdateSlot++] = mostRecentValue;

				if (mNextUpdateSlot == (int)mHistory.size())
					mNextUpdateSlot = 0;

				T sum = mZeroValue;

				typename std::vector<T>::iterator it = mHistory.begin();
				for (; it != mHistory.end(); ++it)
				{
					sum += *it;
				}

				mCurUpdateSeconds = 0.0f;

				mCurValue =  sum / (float)mHistory.size();
			}

			return mCurValue;
		}

	protected:
		std::vector<T> mHistory;
		int mNextUpdateSlot;
		T mZeroValue;
		float mCurUpdateSeconds;
		T mCurValue;
	};

}

#endif
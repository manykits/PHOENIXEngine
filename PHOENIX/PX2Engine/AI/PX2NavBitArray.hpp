// PX2NavBitArray.hpp

#ifndef PX2NAVBITARRAY_HPP
#define PX2NAVBITARRAY_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class BitArray1D
	{
	public:
		BitArray1D(int len);
		~BitArray1D();
		void Clear();
		int GetBit(int i);
		void SetBit(int i, int bit);

	private:
		unsigned int *m_pData;
		int m_NumBits;
		int m_BufLen;
	};

	class BitArray2D
	{
	public:
		BitArray2D(int nbitx, int nbity);
		~BitArray2D();

		void Clear();
		int GetBit(int x, int y);
		void SetBit(int x, int y, int bit);
		int GetWidth() const;
		int GetHeight() const;

	private:
		BitArray1D mBitArray;
		int mBitX;
		int mBitY;
	};

#include "PX2NavBitArray.inl"

}

#endif
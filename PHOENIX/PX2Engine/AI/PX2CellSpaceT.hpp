#ifndef PX2CELLSPACET_HPP
#define PX2CELLSPACET_HPP

#include "PX2CorePre.hpp"
#include "PX2AxisAlignedBox3.hpp"

using namespace std;

namespace PX2
{

	template <class T>
	class CellT
	{
	public:
		CellT (const APoint &min, const APoint &max);
		~CellT ();

		std::list<T> Members;
		AxisAlignedBox3f AlignedBox;

	private:
		CellT ();
	};

	template <class T>
	class CellSpaceT
	{
	public:
		CellSpaceT (float length, float width, int numCellsLength,
			int numCellsWidth, int maxT);

		void Add (const T &t);
		void Remove (const T &t);
		void Update (const T &t, const APoint &oldPos);
		void CalculateNeighbors (const APoint &targetPos, float queryRadius);

		T &Begin ();
		T &Next ();
		bool End ();

		void EmptyCells ();

	private:
		CellSpaceT ();
		int PositionToIndex (const APoint &pos) const;

		std::vector<CellT<T> > mCells;
		std::vector<T> mNeighbors;
		typename std::vector<T>::iterator mCurNeighbor;

		float mSpaceLength;
		float mSpaceWidth;

		int mNumCellsLength;
		int mNumCellsWidth;

		float mCellLength;
		float mCellWidth;
	};

#include "PX2CellSpaceT.inl"

}

#endif
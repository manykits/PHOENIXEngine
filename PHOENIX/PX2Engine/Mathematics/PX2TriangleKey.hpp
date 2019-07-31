// PX2TriangleKey.hpp

#ifndef PX2TRIANGLEKEY_HPP
#define PX2TRIANGLEKEY_HPP

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	class TriangleKey
	{
	public:
		TriangleKey (int v0 = -1, int v1 = -1, int v2 = -1);

		bool operator< (const TriangleKey& key) const;
		operator size_t () const;

		int V[3];
	};

#include "PX2TriangleKey.inl"

}

#endif
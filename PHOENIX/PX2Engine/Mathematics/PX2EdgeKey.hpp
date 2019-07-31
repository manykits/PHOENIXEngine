// PX2EdgeKey.hpp

#ifndef PX2EDGEKEY_H
#define PX2EDGEKEY_H

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	class EdgeKey
	{
	public:
		EdgeKey (int v0 = -1, int v1 = -1);

		bool operator< (const EdgeKey& key) const;
		operator size_t () const;

		int V[2];
	};

	class OrderedEdgeKey
	{
	public:
		OrderedEdgeKey(int v0 = -1, int v1 = -1);
		bool operator< (const OrderedEdgeKey& key) const;
		int V[2];
	};

#include "PX2EdgeKey.inl"

}

#endif

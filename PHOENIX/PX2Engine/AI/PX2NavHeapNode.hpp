// PX2NavHeapNode.hpp

#ifndef PX2NAVHEAPNODE_HPP
#define PX2NAVHEAPNODE_HPP

#include "PX2CorePre.hpp"
#include <functional>

namespace PX2
{

	inline unsigned int MAKEINDEX(int x, int y)
	{
		return (y << 16) | (x);
	}

	inline int INDEX_TO_X(unsigned int index)
	{
		return index & 0xffff;
	}

	inline int INDEX_TO_Y(unsigned int index)
	{
		return index >> 16;
	}

	struct stPathHeapNode
	{
		unsigned int from;
		unsigned int index;
		int hval;  // heuristics_cost
		int aval; // 从开始点到达当前点的实际cost
	};

	inline bool operator <(const stPathHeapNode &node1, const stPathHeapNode &node2)
	{
		return (node1.hval + node1.aval < node2.hval + node2.aval);
	}

	inline bool operator >(const stPathHeapNode &node1, const stPathHeapNode &node2)
	{
		return (node1.hval + node1.aval > node2.hval + node2.aval);
	}

	inline bool operator ==(const stPathHeapNode &node1, const stPathHeapNode &node2)
	{
		return (node1.hval + node1.aval == node2.hval + node2.aval);
	}

	class NavPathHeap
	{
	public:
		void Init()
		{
			mAllNodes.resize(0);
		}
		void PushNode(const stPathHeapNode &node)
		{
			mAllNodes.push_back(node);
			std::push_heap(mAllNodes.begin(), mAllNodes.end(), mComp);
		}
		void PopNode(stPathHeapNode &node)
		{
			node = mAllNodes.front();
			std::pop_heap(mAllNodes.begin(), mAllNodes.end(), mComp);
			mAllNodes.pop_back();
		}
		bool HasNode()
		{
			return mAllNodes.size() > 0;
		}
		void AdjustNode(std::vector<stPathHeapNode>::iterator iter)
		{
			std::push_heap(mAllNodes.begin(), iter + 1, mComp);
		}

	public:
		std::vector<stPathHeapNode> mAllNodes;
		std::greater<stPathHeapNode> mComp;
	};

}

#endif
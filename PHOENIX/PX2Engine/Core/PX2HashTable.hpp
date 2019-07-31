// PX2HashTable.hpp

#ifndef PX2HASHTABLE_HPP
#define PX2HASHTABLE_HPP

#include "PX2CorePre.hpp"
#include "PX2FString.hpp"
#include "PX2Assert.hpp"
#include "PX2Memory.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIntHashCoder
	{
	public:
		unsigned int operator () (unsigned int i) const;
	};

	class PX2_ENGINE_ITEM FixedStringHashCoder
	{
	public:
        int64_t operator () (const FString &str) const;
	};

	template<typename TKey, class TValue, class THashCoder>
	class HashTable
	{
	public:
		struct Element
		{
			TKey key;
			int64_t hash;
			TValue value;
			Element *pnext;
		};

	public:
		HashTable (size_t maxtable) 
			:
		mMaxSlot(maxtable)
		{
			assertion((int)mMaxSlot>0, "mMaxSlot must bigger than 0");
			mTable = new1<Element *>((int)mMaxSlot);
			for (int i=0; i<(int)mMaxSlot; i++)
			{
				mTable[i] = 0;
			}
		}
		
		~HashTable()
		{
			Clear ();
			delete1(mTable);
		}

		void Clear ()
		{
			for (size_t i=0; i<(int)mMaxSlot; i++)
			{
				Element *pcur = mTable[i];
				while (pcur)
				{
					Element *ptmp = pcur;
					pcur = pcur->pnext;
					delete0(ptmp);
				}
			}

			for (int i=0; i<(int)mMaxSlot; i++)
			{
				mTable[i] = 0;
			}
		}
		
		Element *Iterate (Element *pelement)
		{
			size_t islot = 0;
			Element *pnext = 0;
			if (pelement)
			{
				islot = pelement->hash%mMaxSlot;
				pnext = pelement->pnext;
			}
			else
			{
				islot = 0;
				pnext = mTable[0];
			}

			while (0 == pnext)
			{
				islot++;
				if(islot == mMaxSlot)
					return 0;
				pnext = mTable[islot];
			}

			return pnext;
		}
		
		void Erase (Element *pelement)
		{
			assertion(0!=pelement, "pelement must not be 0.\n");
			size_t islot = pelement->hash%mMaxSlot;
			Element *pcur = mTable[islot];
			if(pcur == pelement)
			{
				mTable[islot] = pcur->pnext;
			}
			else
			{
				while(pcur->pnext!=pelement)
				{
					pcur = pcur->pnext;
					assert(0!=pcur);
				}

				pcur->pnext = pelement->pnext;
			}

			delete0(pelement);
		}
		
		Element *Find(const TKey &key) const
		{
			int64_t index = mKeyCoderFunc(key)%mMaxSlot;
			Element *ptmp = mTable[index];
			if (!ptmp)
			{
				return 0;
			}
			else
			{
				while (ptmp->key != key)
				{
					if (!ptmp->pnext)
						return 0;
					else
						ptmp = ptmp->pnext;
				}
			}

			return ptmp;
		}
		
		Element *Insert(const TKey &key)
		{
			int64_t hash = mKeyCoderFunc(key);
			int64_t index = hash % mMaxSlot;
			Element *ptmp = mTable[index];
			if (!ptmp)
			{
				mTable[index] = ptmp = AllocElement(key);
			}
			else
			{
				while (ptmp->key != key)
				{
					if (!ptmp->pnext)
					{
						ptmp->pnext = AllocElement(key);
						ptmp = ptmp->pnext;
						break;
					}
					else 
					{
						ptmp = ptmp->pnext;
					}
				}
			}

			ptmp->hash = hash;
			return ptmp;
		}
		
		Element *Insert (const TKey &key, 
			const TValue &value)
		{
			Element *pelement = Insert(key);
			pelement->value = value;
			return pelement;
		}
		
		TValue &operator[] (const TKey &key)
		{
			Element *pelement = Insert(key);
			return pelement->value;
		}
	
	private:
		Element *AllocElement (TKey key)
		{
			Element *p = new0 Element;
			p->key = key;
			p->pnext = 0;
			return p;
		}

		THashCoder mKeyCoderFunc;
		Element ** mTable;
		size_t mMaxSlot;
	};   

}

#endif
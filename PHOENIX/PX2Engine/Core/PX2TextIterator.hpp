// PX2TextIterator.hpp

#ifndef PX2TEXTITERATOR_HPP
#define PX2TEXTITERATOR_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class TextEncoding;

	class PX2_ENGINE_ITEM TextIterator
	{
	public:
		TextIterator();
		TextIterator(const std::string& str, const TextEncoding& encoding);
		TextIterator(const std::string::const_iterator& begin, const std::string::const_iterator& end, 
			const TextEncoding& encoding);
		TextIterator(const std::string& str);
		TextIterator(const std::string::const_iterator& end);
		~TextIterator();

		TextIterator(const TextIterator& it);
		TextIterator& operator = (const TextIterator& it);

		void swap(TextIterator& it);
		int operator * () const;
		TextIterator& operator ++ ();
		TextIterator operator ++ (int);
		bool operator == (const TextIterator& it) const;
		bool operator != (const TextIterator& it) const;
		TextIterator end() const;

	private:
		const TextEncoding* _pEncoding;
		std::string::const_iterator _it;
		std::string::const_iterator _end;
	};

#include "PX2TextIterator.inl"

}

#endif
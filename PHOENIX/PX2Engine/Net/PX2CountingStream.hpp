// PX2CountingStream.hpp

#ifndef PX2COUNTINGSTREAM_HPP
#define PX2COUNTINGSTREAM_HPP

#include "PX2NetPre.hpp"
#include "PX2UnbufferedStreamBuf.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM CountingStreamBuf : public UnbufferedStreamBuf
	{
	public:
		CountingStreamBuf();
		CountingStreamBuf(std::istream& istr);
		CountingStreamBuf(std::ostream& ostr);
		~CountingStreamBuf();

		int chars() const;
		int lines() const;
		int pos() const;
		void reset();
		void SetCurrentLineNumber(int line);
		int GetCurrentLineNumber() const;
		void AddChars(int chars);
		void AddLines(int lines);
		void AddPos(int pos);

	protected:
		int ReadFromDevice();
		int WriteToDevice(char c);

	private:
		std::istream* _pIstr;
		std::ostream* _pOstr;
		int _chars;
		int _lines;
		int _pos;
	};

	class PX2_ENGINE_ITEM CountingIOS : public virtual std::ios
	{
	public:
		CountingIOS();
		CountingIOS(std::istream& istr);
		CountingIOS(std::ostream& ostr);
		~CountingIOS();

		int chars() const;
		int lines() const;
		int pos() const;
		void reset();
		void SetCurrentLineNumber(int line);
		int GetCurrentLineNumber() const;
		void AddChars(int chars);
		void AddLines(int lines);
		void AddPos(int pos);
		CountingStreamBuf* rdbuf();

	protected:
		CountingStreamBuf _buf;
	};

	class PX2_ENGINE_ITEM CountingInputStream : public CountingIOS, public std::istream
	{
	public:
		CountingInputStream(std::istream& istr);
		~CountingInputStream();
	};

	class PX2_ENGINE_ITEM CountingOutputStream : public CountingIOS, public std::ostream
	{
	public:
		CountingOutputStream();
		CountingOutputStream(std::ostream& ostr);
		~CountingOutputStream();
	};

#include "PX2CountingStream.inl"

}

#endif
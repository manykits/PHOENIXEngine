// PX2CSVParser.hpp

#ifndef PX2CSVPARSER_HPP
#define PX2CSVPARSER_HPP

#include "PX2UnityPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CSVParser
	{
	public:
		class PX2_ENGINE_ITEM TableItem
		{
		public:
			TableItem (const char* szItemString);
			
			bool IsValid() const;

			operator const char*() const;
			const char *Str() const;
			std::string String() const;
			bool GetBool() const;
			char Char() const;
			unsigned char UChar() const;
			unsigned char Byte() const;
			short Short() const;
			unsigned short UShort() const;
			unsigned short Word() const;
			int Int() const;
			unsigned int UInt() const;
			long Long() const;
			long ULong() const;
			unsigned long DWord() const;
			float Float() const;
			double Double() const;

		private:
			const char* mItemString;
		};

		class TableLine
		{
		public:			
			friend class CSVParser;
			
			const TableItem operator[] (int index) const;
			const TableItem operator[] (const char* szIdx) const;

		protected:
			TableLine ();
			void SetLine (CSVParser* pTable, int iLineIdx);

		private:
			CSVParser *mTable;
			int mLineIdx;
		};

		CSVParser();
		virtual ~CSVParser();

		// 从文件中加载（使用PX2_RM获得buffer后加载）
		bool Load (const std::string &filename); 
		bool IsLoaded ();
		void Clear();

		int GetNumLines ();
		int GetNumCols ();

		// 设置从哪一行是“标题行”，Phoenix3d的项目中，第0行是中文注释，
		// 第1行是“标题行”，index的值为1
		void SetTitleLine (int index);
		int GetTitleLine ();

		// 设置从哪一行是“标题列”
		void SetTitleCol (int index);
		int GetTitleCol ();

		const TableLine& operator[] (int index) const;
		const TableLine& operator[] (const char* szIdx) const;

		// 取得指定行和列的字符串
		const char* GetString(int line, int col) const;
		const char* GetString(int line, const char* szColIdx) const;
		const char* GetString(const char* szLineIdx, const char* szColIdx) const;
		bool Char  (size_t line, size_t col, char &val) const;
		bool Byte  (size_t line, size_t col, unsigned char &val) const;
		bool Short (size_t line, size_t col, short &val) const;
		bool Word  (size_t line, size_t col, unsigned short &val) const;
		bool Int   (size_t line, size_t col, int &val) const;
		bool UInt  (size_t line, size_t col, unsigned int &val) const;
		bool Long  (size_t line, size_t col, long &val) const;
		bool DWord (size_t line, size_t col, unsigned long &val) const;
		bool Float (size_t line, size_t col, float &val) const;
		bool Double(size_t line, size_t col, double &val) const;

		// 查找关键字str第一次出现的位置，找到返回true并且返回行列到line, col否则返回false
		bool FindPosByString (const char* str, int& line, int& col);
		int FindLineByString (const char* str); // 查找str第一次出现的行，找不到返回-1
		int FindColByString (const char* str);  // 查找str第一次出现的列，找不到返回-1

	protected:
		bool ParseTextTable();

		char mSeparator;
		std::string mFilename;
		char *mContent;
		char **mItems;
		TableLine *mLines;
		TableLine mInvalidLine;
		int mNumLines;
		int mNumCols;
		int mTitleLine;
		int mTitleCol;
	};

#include "PX2CSVParser.inl"

}

#endif
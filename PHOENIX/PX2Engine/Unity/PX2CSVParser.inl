// PX2CSVParser.inl

//----------------------------------------------------------------------------
// TableItem
//----------------------------------------------------------------------------
inline bool CSVParser::TableItem::IsValid() const
{
	return mItemString[0]!=0;
}
//----------------------------------------------------------------------------
inline CSVParser::TableItem::operator const char* () const
{
	return mItemString; 
}
//----------------------------------------------------------------------------
inline std::string CSVParser::TableItem::String() const 
{ 
	return std::string(mItemString); 
}
//----------------------------------------------------------------------------
inline const char* CSVParser::TableItem::Str() const  
{
	return mItemString; 
}
//----------------------------------------------------------------------------
inline bool CSVParser::TableItem::GetBool() const
{
	return atoi(mItemString)!=0; 
}
//----------------------------------------------------------------------------
inline char CSVParser::TableItem::Char() const 
{
	return (char)mItemString[0];
}
//----------------------------------------------------------------------------
inline unsigned char CSVParser::TableItem::UChar() const
{
	return (unsigned char)atoi(mItemString); 
}
//----------------------------------------------------------------------------
inline unsigned char CSVParser::TableItem::Byte() const 
{
	return (unsigned char)atoi(mItemString); 
}
//----------------------------------------------------------------------------
inline short CSVParser::TableItem::Short() const 
{
	return (short)atoi(mItemString);
}
//----------------------------------------------------------------------------
inline unsigned short CSVParser::TableItem::UShort() const
{
	return (unsigned short)atoi(mItemString); 
}
//----------------------------------------------------------------------------
inline unsigned short CSVParser::TableItem::Word() const  
{
	return (unsigned short)atoi(mItemString); 
}
//----------------------------------------------------------------------------
inline int CSVParser::TableItem::Int() const   
{
	return atoi(mItemString); 
}
//----------------------------------------------------------------------------
inline unsigned int CSVParser::TableItem::UInt() const 
{
	return (unsigned int)atoi(mItemString); 
}
//----------------------------------------------------------------------------
inline long CSVParser::TableItem::Long() const   
{
	return atol(mItemString); 
}
//----------------------------------------------------------------------------
inline long CSVParser::TableItem::ULong() const
{
	return (unsigned long)atol(mItemString); 
}
//----------------------------------------------------------------------------
inline unsigned long CSVParser::TableItem::DWord() const
{
	return (unsigned long)atol(mItemString); 
}
//----------------------------------------------------------------------------
inline float CSVParser::TableItem::Float() const
{
	return (float)atof(mItemString); 
}
//----------------------------------------------------------------------------
inline double CSVParser::TableItem::Double() const
{
	return atof(mItemString); 
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// TableLine
//----------------------------------------------------------------------------
inline const CSVParser::TableItem CSVParser::TableLine::operator[](int index) const
{
	return TableItem(mTable->GetString(mLineIdx, index));
}
//----------------------------------------------------------------------------
inline const CSVParser::TableItem CSVParser::TableLine::operator[] (
	const char* szIdx) const
{
	return TableItem(mTable->GetString(mLineIdx, szIdx));
}
//----------------------------------------------------------------------------
inline void CSVParser::TableLine::SetLine (CSVParser* pTable, int iLineIdx) 
{ 
	mTable = pTable; 
	mLineIdx = iLineIdx;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CSVParser
//----------------------------------------------------------------------------
inline bool CSVParser::IsLoaded() 
{
	return mContent != 0; 
}
//----------------------------------------------------------------------------
inline const CSVParser::TableLine& CSVParser::operator[](int index) const
{
	return mLines[index]; 
}
//----------------------------------------------------------------------------
inline int CSVParser::GetNumLines() 
{
	return mNumLines; 
}
//----------------------------------------------------------------------------
inline int CSVParser::GetNumCols() 
{
	return mNumCols; 
}
//----------------------------------------------------------------------------
inline void CSVParser::SetTitleLine(int index) 
{
	mTitleLine=index; 
}
//----------------------------------------------------------------------------
inline int CSVParser::GetTitleLine()    
{
	return mTitleLine; 
}
//----------------------------------------------------------------------------
inline void CSVParser::SetTitleCol(int index) 
{
	mTitleCol=index;
}
//----------------------------------------------------------------------------
inline int CSVParser::GetTitleCol() 
{
	return mTitleCol;
}
//----------------------------------------------------------------------------
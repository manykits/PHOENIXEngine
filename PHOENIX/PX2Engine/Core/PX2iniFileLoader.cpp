// PX2iniFileLoader.cpp

#include "PX2iniFileLoader.hpp"
using namespace PX2;
using namespace std;


//----------------------------------------------------------------------------
void RemoveCommentingFromLine(string& line)
{
	//search for any comment and remove
	string::size_type idx = line.find('//');

	if (idx != string::npos)
	{
		//cut out the comment
		line = line.substr(0, idx);
	}
}
//----------------------------------------------------------------------------
string iniFileLoaderBase::GetNextParameter()
{

	//this will be the string that holds the bext parameter
	std::string line;

	std::getline(file, line);

	RemoveCommentingFromLine(line);

	//if the line is of zero length, get the next line from
	//the file
	if (line.length() == 0)
	{
		return GetNextParameter();
	}

	GetParameterValueAsString(line);

	return line;
}
//----------------------------------------------------------------------------
void iniFileLoaderBase::GetParameterValueAsString(string& line)
{
	//find beginning of parameter description
	string::size_type begIdx;
	string::size_type endIdx;

	//define some delimiters
	const string delims(" \;=,");

	begIdx = line.find_first_not_of(delims);

	//find the end of the parameter description
	if (begIdx != string::npos)
	{
		endIdx = line.find_first_of(delims, begIdx);

		//end of word is the end of the line
		if (endIdx == string::npos)
		{
			endIdx = line.length();
		}
	}

	//find the beginning of the parameter value
	begIdx = line.find_first_not_of(delims, endIdx);
	//find the end of the parameter value
	if (begIdx != string::npos)
	{
		endIdx = line.find_first_of(delims, begIdx);

		//end of word is the end of the line
		if (endIdx == string::npos)
		{
			endIdx = line.length();
		}
	}

	line = line.substr(begIdx, endIdx);
}
//----------------------------------------------------------------------------
std::string iniFileLoaderBase::GetNextToken()
{
	//strip the line of any commenting
	while (CurrentLine.length() == 0)
	{
		std::getline(file, CurrentLine);

		RemoveCommentingFromLine(CurrentLine);
	}

	//find beginning of parameter description
	string::size_type begIdx;
	string::size_type endIdx;

	//define some delimiters
	const string delims(" \;=,");

	begIdx = CurrentLine.find_first_not_of(delims);

	//find the end of the parameter description
	if (begIdx != string::npos)
	{
		endIdx = CurrentLine.find_first_of(delims, begIdx);

		//end of word is the end of the line
		if (endIdx == string::npos)
		{
			endIdx = CurrentLine.length();
		}
	}

	string s = CurrentLine.substr(begIdx, endIdx);

	if (endIdx != CurrentLine.length())
	{
		//strip the token from the line
		CurrentLine = CurrentLine.substr(endIdx + 1, CurrentLine.length());
	}

	else { CurrentLine = ""; }

	return s;
}
//----------------------------------------------------------------------------
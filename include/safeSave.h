///////////////////////////////////////////
//do not remove this notice
//(c) Luta Vlad
///////////////////////////////////////////

#pragma once
#include <fstream>
#include <vector>

#ifdef _MSC_VER
#pragma warning( disable : 26812 )
#endif

namespace sfs
{
	enum Errors : int
	{
		noError,
		couldNotOpenFinle,
		fileSizeDitNotMatch,
	};


	Errors readEntireFile(std::vector<char>& data, const char* name);
	Errors readEntireFile(char* data, size_t size, const char* name, bool shouldMatchSize, int *bytesRead = nullptr);

	Errors writeEntireFile(const std::vector<char>& data, const char* name);
	Errors writeEntireFile(const char *data, size_t size, const char* name);


};
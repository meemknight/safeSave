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
		checkSumFailed,
		couldNotMakeBackup,
		readBackup,
	};

	//can return error: couldNotOpenFinle
	Errors readEntireFile(std::vector<char>& data, const char* name);
	
	//can return error: couldNotOpenFinle
	Errors readEntireFile(char* data, size_t size, const char* name, bool shouldMatchSize, int *bytesRead = nullptr);

	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors readEntireFileWithCheckSum(char* data, size_t size, const char* name);

	//can return error: couldNotOpenFinle
	Errors writeEntireFileWithCheckSum(const char* data, size_t size, const char* name);

	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const std::vector<char>& data, const char* name);
	
	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const char *data, size_t size, const char* name);

	//can return error: couldNotOpenFinle, couldNotMakeBackup (but will still save the first file)
	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension);

	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension);
};
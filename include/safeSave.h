///////////////////////////////////////////
//do not remove this notice
//(c) Luta Vlad
// 
// safeSave 1.0.0
// 
///////////////////////////////////////////

#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>

#ifdef _MSC_VER
#pragma warning( disable : 26812 )
#endif

namespace sfs
{
	enum Errors : int
	{
		noError = 0,
		couldNotOpenFinle,
		fileSizeDitNotMatch,
		checkSumFailed,
		couldNotMakeBackup,
		readBackup,
		warningEntryAlreadyExists,
		entryNotFound,
		entryHasDifferentDataType,
		couldNotParseData,
	};
	
	const char* getErrorString(Errors e);
	
	//can return error: couldNotOpenFinle
	Errors readEntireFile(std::vector<char>& data, const char* name);
	
	//can return error: couldNotOpenFinle
	Errors readEntireFile(void* data, size_t size, const char* name, bool shouldMatchSize, int *bytesRead = nullptr);

	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors readEntireFileWithCheckSum(void* data, size_t size, const char* name);

	//can return error: couldNotOpenFinle
	Errors writeEntireFileWithCheckSum(const void* data, size_t size, const char* name);

	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const std::vector<char>& data, const char* name);
	
	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const void*data, size_t size, const char* name);

	//can return error: couldNotOpenFinle, 
	//	couldNotMakeBackup (if reportnotMakingBackupAsAnError is true, but will still save the first file)
	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension, bool reportnotMakingBackupAsAnError);

	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed, 
	//	readBackup (if reportLoadingBackupAsAnError but data will still be loaded with the backup)
	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension, bool reportLoadingBackupAsAnError);

	//same as safeLoad but only loads the backup file.
	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors safeLoadBackup(void* data, size_t size, const char* nameWithoutExtension);


	//used to save data and read it
	struct SafeSafeKeyValueData
	{
		struct Entry
		{
			enum Types
			{
				no_type = 0,
				rawData_type,
				int_type,
				float_type,
				bool_type,
				string_type,
			};

			std::vector<unsigned char> data;
			unsigned char type = 0;
			union Primitives
			{
				std::int32_t intData;
				float floatData;
				bool boolData;
			}primitives;
		};

		std::unordered_map<std::string, Entry> entries;

		bool entryExists(std::string at);

		//can return error: entryNotFound
		Errors getEntryType(std::string at, unsigned char &type);

		//can return error: warningEntryAlreadyExists, if so it will overwrite data
		Errors setRawData(std::string at, void *data, size_t size);

		//can return error: entryNotFound, entryHasDifferentDataType
		Errors getRawDataPointer(std::string at, void* &data, size_t &size);

		//won't change i if failed
		//can return error: entryNotFound, entryHasDifferentDataType
		Errors getInt(std::string at, int32_t &i);

		//can return error: warningEntryAlreadyExists, if so it will overwrite data
		Errors setInt(std::string at, int32_t i);

		//won't change f if failed
		//can return error: entryNotFound, entryHasDifferentDataType
		Errors getFloat(std::string at, float &f);

		//can return error: warningEntryAlreadyExists, if so it will overwrite data
		Errors setFloat(std::string at, float f);

		//won't change b if failed
		//can return error: entryNotFound, entryHasDifferentDataType
		Errors getBool(std::string at, bool &b);

		//can return error: warningEntryAlreadyExists, if so it will overwrite data
		Errors setBool(std::string at, bool b);

		//can return error: warningEntryAlreadyExists, if so it will overwrite data
		Errors setString(std::string at, std::string value);

		//won't change s if failed
		//can return error: entryNotFound, entryHasDifferentDataType
		Errors getString(std::string at, std::string &s);

		std::vector<unsigned char> formatIntoFileData();

		//can return error: couldNotParseData
		Errors loadFromFileData(unsigned char *data, size_t size);
	};


#if defined WIN32 || defined _WIN32 || defined __WIN32__ || defined __NT__

	struct FileMapping
	{
		void* pointer = {};
		size_t size = 0;
		struct
		{
			void* fileHandle = 0;
			void* fileMapping = 0;
		}internal = {};
	};

#elif defined __linux__

	struct FileMapping
	{
		void* pointer = {};
		size_t size = 0;
		struct
		{
			int fd = 0;
		}internal = {};
	};

#endif

	//can return error: couldNotOpenFinle
	Errors openFileMapping(FileMapping& fileMapping, const char* name, size_t size, bool createIfNotExisting);

	void closeFileMapping(FileMapping& fileMapping);

};
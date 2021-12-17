#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include <safeSave.h>


namespace sfs
{

	const char* errorsString[] =
	{
		"noError",
		"couldNotOpenFinle",
		"fileSizeDitNotMatch",
		"checkSumFailed",
		"couldNotMakeBackup",
		"readBackup",
	};

	const char* getErrorString(Errors e)
	{
		if (e >= 0 && e < sizeof(errorsString) / sizeof(errorsString[0]))
		{
			return errorsString[e];
		}
		else
		{
			return "unknown error";
		}
	}

	Errors readEntireFile(std::vector<char>& data, const char* name)
	{
		data.clear();
		std::ifstream f(name, std::ios::binary);

		if(f.is_open())
		{
			f.seekg(0, std::ios::_Seekend);
			size_t size = f.tellg();
			f.seekg(0, std::ios::_Seekbeg);

			data.resize(size);

			f.read(&data[0], size);

			return noError;
		}else
		{
			return couldNotOpenFinle;
		}
	}

	Errors readEntireFile(void* data, size_t size, const char* name, bool shouldMatchSize, int* bytesRead)
	{
		if (bytesRead)
		{
			*bytesRead = 0;
		}

		std::ifstream f(name, std::ios::binary);
		if (f.is_open())
		{
			f.seekg(0, std::ios::_Seekend);
			size_t readSize = f.tellg();
			f.seekg(0, std::ios::_Seekbeg);

			if (shouldMatchSize)
			{
				if (readSize != size)
				{
					return fileSizeDitNotMatch;
				}
				else
				{
					f.read((char*)data, readSize);
					if (bytesRead)
					{
						*bytesRead = readSize;
					}

					return noError;
				}
			}
			else
			{
				f.read((char*)data, std::min(size, readSize));

				if (bytesRead)
				{
					*bytesRead = std::min(size, readSize);
				}

				return noError;
			}
		}
		else
		{
			return couldNotOpenFinle;
		}
	}
	
	using HashType = unsigned long long;
	
	//https://stackoverflow.com/questions/34595/what-is-a-good-hash-function
	unsigned long long fnv_hash_1a_64(const void* key, int len)
	{
		const unsigned char* p = (const unsigned char*)key;
		unsigned long long h = 0xcbf29ce484222325ULL;
		for (int i = 0; i < len; i++)
			h = (h ^ p[i]) * 0x100000001b3ULL;
		return h;
	}

	Errors readEntireFileWithCheckSum(void* data, size_t size, const char* name)
	{
		std::ifstream f(name, std::ios::binary);
		if (f.is_open())
		{
			f.seekg(0, std::ios::_Seekend);
			size_t readSize = f.tellg();
			f.seekg(0, std::ios::_Seekbeg);
			
			size_t sizeWithCheckSum = size + sizeof(HashType);

			if (readSize != sizeWithCheckSum)
			{
				return fileSizeDitNotMatch;
			}
			else
			{
				f.read((char*)data, size);
				HashType checkSum = 0;
				f.read((char*)&checkSum, sizeof(HashType));

				auto testCheck = fnv_hash_1a_64(data, size);

				if (testCheck != checkSum)
				{
					return checkSumFailed;
				}
				else
				{
					return noError;
				}
			}
			
		}
		else
		{
			return couldNotOpenFinle;
		}
	}

	Errors writeEntireFileWithCheckSum(const void* data, size_t size, const char* name)
	{
		std::ofstream f(name, std::ios::binary);

		if (f.is_open())
		{
			f.write((char*)data, size);
			auto testCheck = fnv_hash_1a_64(data, size);

			f.write((char*)&testCheck, sizeof(testCheck));

			return noError;
		}
		else
		{
			return couldNotOpenFinle;
		}
	}

	Errors writeEntireFile(const std::vector<char>& data, const char* name)
	{
		return writeEntireFile(data.data(), data.size(), name);
	}

	Errors writeEntireFile(const void* data, size_t size, const char* name)
	{
		std::ofstream f(name, std::ios::binary);
	
		if (f.is_open())
		{
			f.write((char*)data, size);
			return noError;
		}
		else
		{
			return couldNotOpenFinle;
		}
	}

	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension, bool reportnotMakingBackupAsAnError)
	{
		std::string file1 = nameWithoutExtension; file1 += "1.bin";
		std::string file2 = nameWithoutExtension; file2 += "2.bin";

		auto err = writeEntireFileWithCheckSum((char*)data, size, file1.c_str());

		if (err == couldNotOpenFinle)
		{
			return couldNotOpenFinle;
		}
		else
		{
			auto err2 = writeEntireFileWithCheckSum((char*)data, size, file2.c_str());	

			if (err2 == couldNotOpenFinle)
			{
				if (reportnotMakingBackupAsAnError)
				{
					return couldNotMakeBackup;
				}
				else
				{
					return noError;
				}
			}
			else
			{
				return noError;
			}
		}
	}

	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension, bool reportLoadingBackupAsAnError)
	{
		std::string file1 = nameWithoutExtension; file1 += "1.bin";
		std::string file2 = nameWithoutExtension; file2 += "2.bin";

		auto err = readEntireFileWithCheckSum((char*)data, size, file1.c_str());

		if (err == noError)
		{
			return noError;
		}
		else 
		{
			//load backup
			auto err2 = readEntireFileWithCheckSum((char*)data, size, file2.c_str());

			if (err2 == noError)
			{
				if (reportLoadingBackupAsAnError)
				{
					return readBackup;
				}
				else
				{
					return noError;
				}
			}
			else
			{
				return err2;
			}
		}
	}


}

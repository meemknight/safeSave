#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include "..\include\safeSave.h"
#include <safeSave.h>


namespace sfs
{

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

	Errors readEntireFile(char* data, size_t size, const char* name, bool shouldMatchSize, int* bytesRead)
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
					f.read(data, readSize);
					if (bytesRead)
					{
						*bytesRead = readSize;
					}

					return noError;
				}
			}
			else
			{
				f.read(data, std::min(size, readSize));

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
	
	using HashType = uint32_t;

	inline HashType getHash(const char* data, size_t size)
	{
		HashType ret = 0;
		for (size_t i = 0; i < size; i++)
		{
			ret += (data[i]*i) % 1523;
		}
		return ret;
	}

	Errors readEntireFileWithCheckSum(char* data, size_t size, const char* name)
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
				f.read(data, size);
				HashType checkSum = 0;
				f.read((char*)&checkSum, sizeof(HashType));

				auto testCheck = getHash(data, size);

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

	Errors writeEntireFileWithCheckSum(const char* data, size_t size, const char* name)
	{
		std::ofstream f(name, std::ios::binary);

		if (f.is_open())
		{
			f.write(data, size);
			auto testCheck = getHash(data, size);

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

	Errors writeEntireFile(const char* data, size_t size, const char* name)
	{
		std::ofstream f(name, std::ios::binary);
	
		if (f.is_open())
		{
			f.write(data, size);
			return noError;
		}
		else
		{
			return couldNotOpenFinle;
		}
	}

	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension)
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
				return couldNotMakeBackup;
			}
			else
			{
				return noError;
			}
		}
	}

	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension)
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
				return readBackup;
			}
			else
			{
				return err2;
			}
		}
	}


}

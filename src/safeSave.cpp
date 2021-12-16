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


}

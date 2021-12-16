#include <iostream>
#include <safeSave.h>


int main()
{

	{
		bool passed = 0;
		std::vector<char> data;
		if (sfs::readEntireFile(data, RESOURCES_PATH "test1.txt") == sfs::noError)
		{
				
			if (data == std::vector<char>{'t', 'e', 's', 't', '1', '2', '3', '4'})
			{
				passed = 1;
			}

		}
		if (passed)
		{
			std::cout << "test 1: passed\n";
		}
		else
		{
			std::cout << "test 1: didn't pass\n";
		}
	}

	struct Test
	{
		char a = 'a';
		int b = 12;
		float c = '0.12';
		char d = 'b';

		bool operator==(const Test& othre)
		{
			return
				othre.a == a &&
				othre.b == b &&
				othre.c == c &&
				othre.d == d;
		}
	};

	{
		bool passed = 0;
		Test a, b;

		if (sfs::writeEntireFile((char*)&a, sizeof(a), RESOURCES_PATH "test2.bin") == sfs::noError)
		{
			if (sfs::readEntireFile((char*)&b, sizeof(b), RESOURCES_PATH "test2.bin", true) == sfs::noError)
			{
				if (a == b)
				{
					passed = 1;
				}
			}
		}

		if (passed)
		{
			std::cout << "test 2: passed\n";
		}
		else
		{
			std::cout << "test 2: didn't pass\n";
		}
	}

	{
		bool passed = 0;
		Test a, b;

		if (sfs::writeEntireFileWithCheckSum((char*)&a, sizeof(a), RESOURCES_PATH "test3.bin") == sfs::noError)
		{
			if (sfs::readEntireFileWithCheckSum((char*)&b, sizeof(b), RESOURCES_PATH "test3.bin") == sfs::noError)
			{
				if (a == b)
				{
					passed = 1;
				}
			}
		}

		if (passed)
		{
			std::cout << "test 3: passed\n";
		}
		else
		{
			std::cout << "test 3: didn't pass\n";
		}
	}




	std::cin.get();
	return 0;
}
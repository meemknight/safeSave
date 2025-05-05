#include <iostream>
#include <safeSave.h>
#include <memory.h>
#include <iterator>

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
		float c = 0.12f;
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

	{
		bool passed = 0;
		Test a, b;

		if (sfs::safeSave((char*)&a, sizeof(a), RESOURCES_PATH "test4", true) == sfs::noError)
		{

			if (sfs::safeLoad((char*)&b, sizeof(b), RESOURCES_PATH "test4", true) == sfs::noError)
			{
				if (a == b)
				{
					std::ofstream f(RESOURCES_PATH "test41.bin", std::ios::binary);
					f.close();

					if (sfs::safeLoad((char*)&b, sizeof(b), RESOURCES_PATH "test4", true) == sfs::readBackup)
					{
						if (a == b)
						{

							std::ifstream in(RESOURCES_PATH "test42.bin", std::ios::binary);
							std::vector<char> data;
							char c=0;
							while (in.read(&c, 1)) { data.push_back(c); }
							in.close();
							data[2] += 1;

							std::ofstream of(RESOURCES_PATH "test42.bin", std::ios::binary);
							std::copy(data.begin(), data.end(), std::ostream_iterator<char>(of));
							of.close();

							if (sfs::safeLoad((char*)&b, sizeof(b), RESOURCES_PATH "test4", true) == sfs::checkSumFailed)
							{
								passed = 1;
							}
						}
					}
				}
			}
		}

		if (passed)
		{
			std::cout << "test 4: passed\n";
		}
		else
		{
			std::cout << "test 4: didn't pass\n";
		}
	}

	{
		bool passed = 0;
		
		sfs::FileMapping fileMap;

		if (sfs::openFileMapping(fileMap, RESOURCES_PATH "test5.bin", sizeof(Test), true) == sfs::noError)
		{
			Test a;
			a.a += 10;
			memcpy(fileMap.pointer, &a, sizeof(a));

			sfs::closeFileMapping(fileMap);
			//*((char*)fileMap.pointer) += 10;
			
			Test b;
			if (sfs::readEntireFile(&b, sizeof(b), RESOURCES_PATH "test5.bin", true) == sfs::noError) 
			{
				if (a == b)
				{
					passed = true;
				}
			};
		}

		if (passed)
		{
			std::cout << "test 5: passed\n";
		}
		else
		{
			std::cout << "test 5: didn't pass\n";
		}
	}

	{
		bool passed = 1;

		sfs::SafeSafeKeyValueData data;
		Test t{'b', 12,1.2, 'c'};

		if (data.setRawData("test1", &t, sizeof(t)) != sfs::Errors::noError) { passed = 0; };
		if (data.setRawData("test2", &t, sizeof(t)) != sfs::Errors::noError) { passed = 0; };
		if (data.setRawData("test1", &t, sizeof(t)) != sfs::Errors::warningEntryAlreadyExists) { passed = 0; };
		if (data.setBool("bool", 1) != sfs::Errors::noError) { passed = 0; };
		if (data.setInt("int", 69960000) != sfs::Errors::noError) { passed = 0; };
		if (data.setString("str", "testString") != sfs::Errors::noError) { passed = 0; };
		if (data.setFloat("fl", 0.5) != sfs::Errors::noError) { passed = 0; };
		if (data.setuInt64("uint64", 10000000001) != sfs::Errors::noError) { passed = 0; };
		if (data.setInt64("int64", -10000000001) != sfs::Errors::noError) { passed = 0; };

	
		//if (sfs::safeSave(data, RESOURCES_PATH "test6", true) != sfs::noError) { passed = 0; };
		//data = {};
		//if (sfs::safeLoad(data, RESOURCES_PATH "test6", true) != sfs::noError) { passed = 0; };

		auto binary = data.formatIntoFileDataBinary();
		data = {};
		data.loadFromFileData(&binary[0], binary.size());


		int i = 0;
		if (data.getInt("test1", i) != sfs::Errors::entryHasDifferentDataType) { passed = 0; };

		char type = 0;
		if (data.getEntryType("test__2", type) != sfs::Errors::entryNotFound) { passed = 0; }
		if (data.getEntryType("test1", type) != sfs::Errors::noError) { passed = 0; }


		if (type != sfs::SafeSafeKeyValueData::Entry::Types::rawData_type) { passed = 0; }

		Test read = {};

		if (!data.entryExists("test1")) { passed = 0; }
		if (!data.entryExists("test2")) { passed = 0; }
		if (data.entryExists("test________")) { passed = 0; }

		{
			bool b2 = 0;
			if (data.getBool("bool", b2) != sfs::Errors::noError || b2 != true)
			{ passed = 0; }
		}

		{
			uint64_t i = 0;
			if (data.getuInt64("uint64", i) != sfs::Errors::noError || i != 10000000001)
			{
				passed = 0;
			}
		}
		{
			int64_t i = 0;
			if (data.getInt64("int64", i) != sfs::Errors::noError || i != -10000000001)
			{
				passed = 0;
			}
		}


		{
			int i2 = 0;
			if (data.getInt("int", i2) != sfs::Errors::noError || i2 != 69960000)
			{
				passed = 0;
			}
		}

		{
			std::string s2 = {};
			if (data.getString("str", s2) != sfs::Errors::noError || s2 != "testString")
			{
				passed = 0;
			}
		}

		{
			float read = 0;
			if (data.getFloat("fl", read) != sfs::Errors::noError || read != 0.5)
			{
				passed = 0;
			}

		}

		void *ptr = 0;
		size_t s = 0;
		data.getRawDataPointer("test1", ptr, s);
		
		if (s != sizeof(Test) || ptr == 0) 
		{ 
			passed = 0; 
		}
		else
		{
			memcpy(&read, ptr, s);
		}

		if (!(t == read)) { passed = 0; }

		if (passed)
		{
			std::cout << "test 6: passed\n";
		}
		else
		{
			std::cout << "test 6: didn't pass\n";
		}
	}

	{
		bool passed = 1;

		std::vector<char> data = {1,2,3,4,65,-77,5,3,2,12,-3,5,1,69,4,3,22,34,2,8,8,7,65};

		if (sfs::writeEntireFileWithCheckSum(data.data(), data.size(), RESOURCES_PATH "test7") == sfs::noError)
		{
			std::vector<char> data2;

			if (sfs::readEntireFileWithCheckSum(data2, RESOURCES_PATH "test7") == sfs::noError)
			{
				if (data2.size() != data.size()) { passed = 0; }
				else
				{
					for (int i = 0; i < data.size(); i++)
					{
						if (data[i] != data2[i]) { passed = 0; break; }
					}
				}
			}
			else
			{
				passed = 0;
			}
		}
		else
		{
			passed = 0;
		}


		if (passed)
		{
			std::cout << "test 7: passed\n";
		}
		else
		{
			std::cout << "test 7: didn't pass\n";
		}
	}

	{
		bool passed = 1;

		std::vector<char> data = {1,2,3,4,65,-77,5,3,2,12,-3,5,1,69,4,3,22,34,2,8,8,7,65};

		if (sfs::safeSave(data.data(), data.size(), RESOURCES_PATH "test8", true) == sfs::noError)
		{
			std::vector<char> data2;

			if (sfs::safeLoad(data2, RESOURCES_PATH "test8", true) == sfs::noError)
			{
				if (data2.size() != data.size()) { passed = 0; }
				else
				{
					for (int i = 0; i < data.size(); i++)
					{
						if (data[i] != data2[i]) { passed = 0; break; }
					}
				}
			}
			else
			{
				passed = 0;
			}
		}
		else
		{
			passed = 0;
		}


		if (passed)
		{
			std::cout << "test 8: passed\n";
		}
		else
		{
			std::cout << "test 8: didn't pass\n";
		}
	}

	{
		bool passed = 1;

		sfs::SafeSafeKeyValueData data;
		Test t{'b', 12,1.2, 'c'};
		if (data.setRawData("test1", &t, sizeof(t)) != sfs::Errors::noError) { passed = 0; };

		if (sfs::safeSave(data, RESOURCES_PATH "test9", true) != sfs::noError) { passed = 0; };
		
		sfs::SafeSafeKeyValueData data2;
		if (sfs::safeLoad(data2, RESOURCES_PATH "test9", true) != sfs::noError) { passed = 0; };

	
		void *ptr = 0;
		size_t s = 0;
		data.getRawDataPointer("test1", ptr, s);
		Test read = {};
		if (s != sizeof(Test) || ptr == 0)
		{
			passed = 0;
		}
		else
		{
			memcpy(&read, ptr, s);
		}
		
		if (!(t == read)) { passed = 0; }


		if (passed)
		{
			std::cout << "test 9: passed\n";
		}
		else
		{
			std::cout << "test 9: didn't pass\n";
		}
	}


	{
		bool passed = 1;

		sfs::SafeSafeKeyValueData subData;
		subData.setInt("subInt", 15);
		subData.setInt("intTest", 55);
		subData.setFloat("float", 5.5);
		subData.setBool("bool", true);


		sfs::SafeSafeKeyValueData data;
		Test t{'b', 12,1.2, 'c'};
		if (data.setRawData("test1", &t, sizeof(t)) != sfs::Errors::noError) { passed = 0; };

		data.setInt("intTest", 10);
		data.setKeyValueData("subData", subData);
		data.setFloat("floatTest", 3.14f);

		auto dataBinary = data.formatIntoFileDataBinary();

		sfs::SafeSafeKeyValueData data2;
		data2.loadFromFileData(dataBinary.data(), dataBinary.size());


		void *ptr = 0;
		size_t s = 0;
		data2.getRawDataPointer("test1", ptr, s);
		Test read = {};
		if (s != sizeof(Test) || ptr == 0)
		{
			passed = 0;
		}
		else
		{
			memcpy(&read, ptr, s);
		}

		if (!(t == read)) { passed = 0; }


		{
			int i = 0;
			if (data2.getInt("intTest", i) != sfs::Errors::noError || i != 10)
			{
				passed = 0;
			}
		}

		{
			float i = 0;
			if (data2.getFloat("floatTest", i) != sfs::Errors::noError || i != 3.14f)
			{
				passed = 0;
			}
		}

		{
			sfs::SafeSafeKeyValueData subData2 = {};

			if (data2.getKeyValueData("subData", subData2) != sfs::Errors::noError || subData != subData2)
			{
				passed = 0;
			}


		}

		if (passed)
		{
			std::cout << "test 10: passed\n";
		}
		else
		{
			std::cout << "test 10: didn't pass\n";
		}
	}

	{
		bool passed = true;

		float rawData[] = {1,2,3};

		sfs::SafeSafeKeyValueData subData;
		subData.setInt("subInt", 15);
		subData.setInt("intTest", 55);
		subData.setFloat("float", 5.5);
		subData.setBool("bool", true);
		subData.setRawData("raw", rawData, sizeof(rawData));

		sfs::SafeSafeKeyValueData subData2 = subData;

		subData2.setInt("subInt", 16);
		if (subData == subData2) { passed = 0; }

		subData2.setInt("subInt", 15);
		if (subData != subData2) { passed = 0; }

		subData2.setInt("newStuff", 15);
		if (subData == subData2) { passed = 0; }


		if (passed)
		{
			std::cout << "test 11: passed\n";
		}
		else
		{
			std::cout << "test 11: didn't pass\n";
		}
	}

	{
		bool passed = true;

		sfs::SafeSafeKeyValueData obj = 
		{
			{"name", "John"},
			{"age", 30},
			{"negative", -30},
			{"height", 1.82f},
			{"is_alive", true},
			{"id", std::uint64_t(123456789)}
		};

		sfs::SafeSafeKeyValueData obj2;

		obj2.setString("name", "John");
		obj2.setInt("age", 30);
		obj2.setInt("negative", -30);
		obj2.setFloat("height", 1.82f);
		obj2.setBool("is_alive", true);
		obj2.setuInt64("id", 123456789);

		if (obj != obj2) { passed = false; }

		if (passed)
		{
			std::cout << "test 12: passed\n";
		}
		else
		{
			std::cout << "test 12: didn't pass\n";
		}
	}

	{
		bool passed = true;


		sfs::SafeSafeKeyValueData data =
		{
			{"vec3", {3.f, 7.f, 999999999.f}},
			{"vec4", {2.f, 5.f, 10.9999f, 11.f}},
			{"vec4_", {0.f, 0.f, 0.f, 0.f}},
			{"ivec2", {10, 57}},
			{"ivec3", {3, 7, 999999999}},
			{"ivec4", {2, 5, 10, 11}},
			{"ivec4_", {0, 0, 0, 0}},
			{"double", 60.0},
			{"uint", (unsigned int)15},
		};

		data.setVec2("vec2", 10.f, 57.f);

		auto dataBinary = data.formatIntoFileDataBinary();
		sfs::SafeSafeKeyValueData data2;
		data2.loadFromFileData(dataBinary.data(), dataBinary.size());


		if (data != data2)
		{
			passed = false;
		}

		data2.setIVec2("vec2", 10, 56);

		if (data == data2)
		{
			passed = false;
		}

		if (passed)
		{
			std::cout << "test 13: passed\n";
		}
		else
		{
			std::cout << "test 13: didn't pass\n";
		}
	}


	{
		//sfs::SafeSafeKeyValueData data;
		//
		//data.setInt("intTest", 10);
		//data.setFloat("float test", 10.f);
		//data.setBool("yes", true);
		//data.setBool("no", false);
		//data.setString("str", "string tests");
		//
		//auto rez = data.formatIntoFileDataTextBased();
		//
		//sfs::writeEntireFile(rez, RESOURCES_PATH "test10.txt");

	}

	std::cin.get();
	return 0;
}
# SafeSave

---

Allows you to save data and don't worry if your save will get corupted. In the program is closed while the file is being saved the library will load the backup.
If the original file has been modified, the library will load the backup. If both the backup and the original file are corrupted, the library will 
report this error.

Has other functionalities like file mappings or a key-based dictionary (like a simplified JSON) where you can save data: SafeSafeKeyValueData

---

Integration: paste the include/safeSave.h file and the src/safeSave.cpp file into your project.

---

Basic Functions:

```cpp
  	//can return error: couldNotOpenFinle, 
	//	couldNotMakeBackup (if reportnotMakingBackupAsAnError is true, but will still save the first file)
	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension, bool reportnotMakingBackupAsAnError);

	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed, 
	//	readBackup (if reportLoadingBackupAsAnError but data will still be loaded with the backup)
	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension, bool reportLoadingBackupAsAnError);

	//same as safeLoad but only loads the backup file.
	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors safeLoadBackup(void* data, size_t size, const char* nameWithoutExtension);
```

Error reporting:
Every function returns an error code. 
You can use
```cpp
	const char* getErrorString(Errors e);
```
to get the error string 

These are the possible error codes:
```cpp
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
		fileSizeNotBigEnough,
	};
```

---

All Functions:

```cpp
 	const char* getErrorString(Errors e);
	
	//can return error: couldNotOpenFinle
	Errors readEntireFile(std::vector<char>& data, const char* name);
	
	//reades the content of a file (size bytes), if shouldMatchSize is false will read the entire fill untill size bytes are read or the entire file was read
	//can return error: couldNotOpenFinle, fileSizeDitNotMatch
	Errors readEntireFile(void* data, size_t size, const char* name, bool shouldMatchSize,
		int *bytesRead = nullptr);

	//gets the file size
	//can return error: couldNotOpenFinle
	Errors getFileSize(const char *name, size_t &size);

	//reades the entire content of the data to a file and uses checkSum
	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors readEntireFileWithCheckSum(void* data, size_t size, const char* name);

	//reades the entire content of the data to a file and uses checkSum
	//can return error: couldNotOpenFinle, fileSizeNotBigEnough
	Errors readEntireFileWithCheckSum(std::vector<char> &data, const char *name);

	//writes the entire content of the data to a file and uses checkSum
	//can return error: couldNotOpenFinle
	Errors writeEntireFileWithCheckSum(const void* data, size_t size, const char* name);

	//writes the entire content of the data to a file
	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const std::vector<char>& data, const char* name);
	
	//writes the entire content of the data to a file
	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const void*data, size_t size, const char* name);

	//saved the data with a check sum and a backup
	// It will also use a temporary file to make sure the saving is safe.
	//can return error: couldNotOpenFinle, 
	//	couldNotMakeBackup (if reportnotMakingBackupAsAnError is true, but will still save the first file)
	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension, bool reportnotMakingBackupAsAnError);


	//saved the data with a check sum. It will use a temporary file to make sure the saving is safe.
	//can return error: couldNotOpenFinle
	Errors safeSaveNoBackup(const void *data, size_t size, const char *nameWithoutExtension);


	//loads the data that was saved using safeSave
	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed, 
	//	readBackup (if reportLoadingBackupAsAnError but data will still be loaded with the backup)'
	// is checkSumFailed is returned, the data was still read!
	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension, bool reportLoadingBackupAsAnError);

	//loads the data that was saved using safeSave and stored as a SafeSafeKeyValueData structure
	//can return error: couldNotOpenFinle, checkSumFailed, fileSizeNotBigEnough
	//	readBackup (if reportLoadingBackupAsAnError but data will still be loaded with the backup)
	Errors safeLoad(std::vector<char> &data, const char *nameWithoutExtension, 
		bool reportLoadingBackupAsAnError);

	//same as safeLoad but only loads the backup file.
	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors safeLoadBackup(void* data, size_t size, const char* nameWithoutExtension);

	//saved the data stored as a SafeSafeKeyValueData structure in a binary format with a check sum and a backup
	//Uses a temporary file to make sure the saving is safe
	//can return error: couldNotOpenFinle, 
	//	couldNotMakeBackup (if reportnotMakingBackupAsAnError is true, but will still save the first file)
	Errors safeSave(SafeSafeKeyValueData &data, const char *nameWithoutExtension, bool reportnotMakingBackupAsAnError);


	//saved the data stored as a SafeSafeKeyValueData structure in a binary format with a check.
	//Uses a temporary file to make sure the saving is safe
	//can return error: couldNotOpenFinle, 
	Errors safeSaveNoBackup(SafeSafeKeyValueData &data, const char *nameWithoutExtension);


	//loads the data that was saved using safeSave and stored as a SafeSafeKeyValueData structure
	//can return error: couldNotOpenFinle, fileSizeNotBigEnough, checkSumFailed, couldNotParseData
	//	readBackup (if reportLoadingBackupAsAnError but data will still be loaded from the backup)
	Errors safeLoad(SafeSafeKeyValueData &data, const char *nameWithoutExtension, bool reportLoadingBackupAsAnError);

	//a file mapping maps the content of a file to ram
	//can return error: couldNotOpenFinle
	Errors openFileMapping(FileMapping& fileMapping, const char* name, size_t size, bool createIfNotExisting);

	void closeFileMapping(FileMapping& fileMapping);

```







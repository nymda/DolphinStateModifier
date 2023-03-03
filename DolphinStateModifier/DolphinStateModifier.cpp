// DolphinStateModifier.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

typedef struct {
	uint8_t icounter_daily_limit[7];
	uint8_t butthurt_daily_limit;

	uint32_t flags;
	uint32_t icounter;
	int32_t butthurt;
	uint64_t timestamp;
} DolphinStoreData;

int main(int argc, char* argv[])
{

	DolphinStoreData data = { 0 };
	FILE* ddFile;
	
	if (argc < 2) {
		// No arguments
		// Print help
		return 0;
	}

	if (strcmp(argv[1], "-r") == 0) {
		// Read
		// Read from file
		fopen_s(&ddFile, ".dolphin.state", "rb");
		if (!ddFile) { return;  }
		fread_s(&data, sizeof(DolphinStoreData), sizeof(DolphinStoreData), 1, ddFile);
		// Print to console
		return 0;
	}

	if (strcmp(argv[1], "-l") == 0) {
		// Set level
	}

	if (strcmp(argv[1], "-b") == 0) {
		// Set butthurt
	}
}


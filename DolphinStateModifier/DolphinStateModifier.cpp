// DolphinStateModifier.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#define BUTTHURT_MAX 14
#define BUTTHURT_MIN 0

#define LEVEL1_THRESHOLD 0
#define LEVEL2_THRESHOLD 300
#define LEVEL3_THRESHOLD 1800

typedef struct {
	uint8_t magic;
	uint8_t version;
	uint8_t checksum;
	uint8_t flags;
	uint32_t timestamp;
} SavedStructHeader;

typedef struct {
	uint8_t icounter_daily_limit[7];
	uint8_t butthurt_daily_limit;

	uint32_t flags;
	uint32_t icounter;
	int32_t butthurt;
	uint64_t timestamp;
} DolphinStoreData;

int xpToLevel(int xp) {
	if (xp >= LEVEL1_THRESHOLD && xp < LEVEL2_THRESHOLD) {
		return 1;
	}
	else if (xp >= LEVEL2_THRESHOLD && xp < LEVEL3_THRESHOLD) {
		return 2;
	}
	else if (xp >= LEVEL3_THRESHOLD) {
		return 3;
	}
	else {
		return -1;
	}
}

int main(int argc, char* argv[])
{
	char* inputPath = 0;

	int nextLevel = -1;
	int nextButthurt = -1;

	SavedStructHeader header = { 0 };
	DolphinStoreData data = { 0 };
	FILE* ddFile;

	bool saveRequired = false;

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-f") == 0 && i + 1 <= argc && !inputPath) {
			inputPath = argv[i + 1];
		}

		if (strcmp(argv[i], "-l") == 0 && i + 1 <= argc && nextLevel == -1) {
			nextLevel = (int)strtol(argv[i + 1], NULL, 10);
		}

		if (strcmp(argv[i], "-b") == 0 && i + 1 <= argc && nextButthurt == -1) {
			nextButthurt = (int)strtol(argv[i + 1], NULL, 10);
		}
	}

	if (!inputPath) {
		printf_s("Please specify an input path (-f)\n");
		return 0;
	}

	fopen_s(&ddFile, inputPath, "r+b");
	if (!ddFile) { return 0; }
	fread_s(&header, sizeof(SavedStructHeader), sizeof(SavedStructHeader), 1, ddFile);
	fread_s(&data, sizeof(data), sizeof(DolphinStoreData), 1, ddFile);

	if (nextLevel >= LEVEL1_THRESHOLD) {
		data.icounter = nextLevel;
		saveRequired = true;
	}

	if (nextButthurt >= BUTTHURT_MIN && nextButthurt < BUTTHURT_MAX) {
		data.butthurt = nextButthurt;
		saveRequired = true;
	}

	printf_s("Flags     : %i\n", data.flags);
	printf_s("XP        : %i (LVL %i) %s\n", data.icounter, xpToLevel(data.icounter), nextLevel == -1 ? "" : "*");
	printf_s("Butthurt  : %i %s\n", data.butthurt, nextButthurt == -1 ? "" : "*");
	printf_s("Timestamp : %ld\n", (long)data.timestamp);

	if (saveRequired) {
		uint8_t checksum = 0;
		uint8_t* source = (uint8_t*)&data;
		for (size_t i = 0; i < sizeof(DolphinStoreData); i++) {
			checksum += source[i];
		}
		header.checksum = checksum;

		fseek(ddFile, 0, SEEK_SET);
		fwrite(&header, sizeof(SavedStructHeader), 1, ddFile);
		fwrite(&data, sizeof(data), 1, ddFile);
	}

	fclose(ddFile);
}


#pragma once
#include "hwlib.hpp"
class FAT32_BPB {
private:
	uint16_t BytesPerSector;
	uint8_t SectorsPerCluster;
	uint16_t ReservedSectors;
	uint8_t NumberOfFATs;
	uint16_t RootEntries;
	uint16_t TotalSectors;

public:
	FAT32_BPB();
	~FAT32_BPB();
};


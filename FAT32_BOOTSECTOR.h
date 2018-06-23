#pragma once
#include "hwlib.hpp"
class FAT32_BOOTSECTOR {
private:
	uint8_t jmp[3];
	char OemName[8];
	uint8_t DriveNumber;
	
public:
	FAT32_BOOTSECTOR();
	~FAT32_BOOTSECTOR();
};


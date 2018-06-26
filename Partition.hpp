#ifndef PHAT_PARTITION_HPP
#define PHAT_PARTITION_HPP
#include "hwlib.hpp"

class Partition {
private:
	enum class BootIndicator : bool {
		NOBOOT = 0,
		BOOT = 1
	};
	enum class SystemID : uint8_t {
		EXT_FAT32_LBA = 12,
		NOT_SUPPORTED = 0
	};
	BootIndicator bootable;
	uint8_t StartingHead;
	uint16_t StartingSectCylinder;
	SystemID Sys_ID;
	uint8_t EndingHead;
	uint16_t EndingSectCylinder;
	uint32_t RelativeSector;
	uint32_t TotalSectors;

public:
	/// \brief empty constructor for empty partition;
	Partition(){}
	
	/// \brief Constructor for a partition table
	/// \details Constructs a partition table based on the 16 byte entry found in the raw data of the SD card.
	Partition( uint8_t PartitionTable[16] );

	/// \brief Checks whether the partition is FAT32. 
	bool isFAT32() {
		return Sys_ID == SystemID::EXT_FAT32_LBA;
	}

	/// \brief Returns the fat table location
	uint32_t getFatTableLocation() {
		return RelativeSector;
	}

	/// \brief Returns the total amounf of sectors of the partition. 
	uint32_t getAmountOfSectors() {
		return TotalSectors;
	}

};

#endif
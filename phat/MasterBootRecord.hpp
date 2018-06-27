///file MasterBootRecord.hpp
/// \brief Data structure containing the Master Boot record.
/// \details This contains the 4 partition tables that can be found on an SD card formatted with FAT32.

#ifndef PHAT_MASTER_BOOT_RECORD_HPP
#define PHAT_MASTER_BOOT_RECORD_HPP
#include "hwlib.hpp"
#include "Partition.hpp"

class MasterBootRecord {
protected:
	Partition partition_tables[4];
	bool usableDrive = false;

public:
	/// \brief Default constructor
	/// \details sets the usableDrive to false. Since if there are no partitions, the drive is useless
	MasterBootRecord() : usableDrive( false ) {};
	
	/// \brief Constructor with the 4 partition tables as argument
	/// \details Also checks whether the drive is usable or not and sets the flag.
	MasterBootRecord(Partition partition_table, Partition partition_table2, Partition partition_table3, Partition partition_table4);
	
	/// \brief Checks wether this drive is usable;
	bool isUsableDrive() {
		return usableDrive;
	}

	/// \brief Returns the FAT address of the chosen partition.
	/// \details the partitionNumber is 0-index based.
	uint32_t getFatAddress(const uint8_t & PartitionNumber) {
		return partition_tables[PartitionNumber].getFatTableLocation();
	}
};

#endif
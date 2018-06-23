#ifndef PHAT_MASTER_BOOT_RECORD_HPP
#define PHAT_MASTER_BOOT_RECORD_HPP
#include "hwlib.hpp"
#include "Partition.hpp"

class MasterBootRecord {
private:
	Partition partition_tables[4];
	bool usableDrive = false;

public:
	MasterBootRecord() : usableDrive( false ) {};
	MasterBootRecord(Partition partition_table, Partition partition_table2, Partition partition_table3, Partition partition_table4);
	bool isUsableDrive() {
		return usableDrive;
	}
	uint32_t getFatAddress(const uint8_t & PartitionNumber) {
		return partition_tables[PartitionNumber].getFatTableLocation();
	}
};

#endif
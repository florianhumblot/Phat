#pragma once
#include "hwlib.hpp"
class BootSector {
private:
	uint16_t BytesPerSector;
	uint8_t SectorsPerCluster;
	uint16_t ReserverdSectorCount;
	uint8_t NumberOfFats;
	uint32_t SectorsPerFat;
	uint32_t RootCluster;
	uint16_t RootEntryCount;
	uint32_t FirstDataSector;
	uint32_t RootDirSectors;
	uint32_t FatTablesAddress;
	uint32_t PartitionStart;
public:
	BootSector();
	BootSector(const uint8_t BootSectorInformation[16], uint32_t PartitionStartAddress );
	uint32_t GetRootDirAddress();
	uint32_t GetFirstSectorForCluster( uint32_t ClusterNumber );

	friend hwlib::ostream & operator<< ( hwlib::ostream & stream, BootSector BPB ) {
		stream << hwlib::dec;
		stream << "\nBytesPerSector: " << +BPB.BytesPerSector << "\n";
		stream << "SectorsPerCluster: " << +BPB.SectorsPerCluster << "\n";
		stream << "ReserverdSectorCount: " << +BPB.ReserverdSectorCount << "\n";
		stream << "NumberOfFats: " << +BPB.NumberOfFats << "\n";
		stream << "SectorsPerFat: " << +BPB.SectorsPerFat << "\n";
		stream << "RootCluster: " << +BPB.RootCluster << "\n";
		stream << "RootEntryCount: " << +BPB.RootEntryCount << "\n";
		stream << "FirstDataSector: " << +BPB.FirstDataSector << "\n";
		stream << "RootDirSectors: " << +BPB.RootDirSectors << "\n";
		return stream;
	}

	~BootSector();
};


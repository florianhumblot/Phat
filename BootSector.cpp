#include "BootSector.hpp"



BootSector::BootSector() {}


BootSector::BootSector( const uint8_t BootSectorInformation[16], uint32_t PartitionStartAddress ) {
	BytesPerSector = (BootSectorInformation[0] << 8) + BootSectorInformation[1];
	SectorsPerCluster = BootSectorInformation[2];
	ReserverdSectorCount = ( BootSectorInformation[3] << 8 ) + BootSectorInformation[4];
	NumberOfFats = BootSectorInformation[5];
	SectorsPerFat = ( BootSectorInformation[6] << 24 ) + ( BootSectorInformation[7] << 16 )
		+ ( BootSectorInformation[8] << 8 ) + ( BootSectorInformation[9] );
	RootCluster = ( BootSectorInformation[10] << 24 ) + ( BootSectorInformation[11] << 16 )
		+ ( BootSectorInformation[12] << 8 ) + ( BootSectorInformation[13] );
	RootEntryCount = ( BootSectorInformation[14] << 8 ) + BootSectorInformation[15];
	RootDirSectors = ( ( RootEntryCount * 32 ) + ( BytesPerSector - 1 ) ) / BytesPerSector;
	PartitionStart = PartitionStartAddress;
	FatTablesAddress = PartitionStart + ReserverdSectorCount;
	FirstDataSector = PartitionStart + ReserverdSectorCount + ( SectorsPerFat * NumberOfFats );
}

uint32_t BootSector::GetRootDirAddress() {
	return GetFirstSectorForCluster(RootCluster);
}

uint32_t BootSector::GetFirstSectorForCluster( uint32_t ClusterNumber ) {
	return ( ( ClusterNumber - 2 ) * SectorsPerCluster ) + FirstDataSector;
}


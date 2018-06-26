///file BootSector.hpp
/// \brief Datastructure for the boot rescord information
/// \details Contains all the necessary information about the SD card.
/// Also provides some conversion methods such as from logical cluster number to an address.

#ifndef PHAT_BOOTSECTOR_HPP
#define PHAT_BOOTSECTOR_HPP


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
	/// \brief Constructor for the bootsector.
	/// \details Accepts an array of 16 bytes containing the information of the bootsector as well as the start address of the partition (this is because there may be multiple partitions).
	BootSector(const uint8_t BootSectorInformation[16], uint32_t PartitionStartAddress );
	/// \brief Returns the Root Directory address
	/// \details this calls BootSector::GetRootDirAddress() with ClusterNumber 0;
	uint32_t GetRootDirAddress();
	/// \brief Gets the sector address for a cluster number
	/// \details This will calculate the sector address for a given cluster number by using the appropriate formula and the data stored in this class.
	uint32_t GetFirstSectorForCluster( uint32_t ClusterNumber );

	/// \brief A stream operator to be able to print the informtion to the terminal
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

};

#endif // !PHAT_BOOTSECTORY_HPP
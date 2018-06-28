// ==========================================================================
//
// File      : Partition.cpp
// Part of   : Phat - FAT32 reader
// Copyright : Florian Humblot - me@florianhumblot.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================
#include "Partition.hpp"

Partition::Partition( uint8_t PartitionTable[16] ) {
	bootable = ( BootIndicator ) PartitionTable[0];
	StartingHead = PartitionTable[1];
	StartingSectCylinder = ( PartitionTable[3] << 8 ) + PartitionTable[2];
	Sys_ID = ( PartitionTable[4] == 12 ) ? SystemID::EXT_FAT32_LBA : SystemID::NOT_SUPPORTED;
	EndingHead = PartitionTable[5];
	EndingSectCylinder = ( PartitionTable[7] << 8 ) + PartitionTable[6];
	RelativeSector = ( PartitionTable[11] << 24 ) + ( PartitionTable[10] << 16 ) + ( PartitionTable[9] << 8 ) + PartitionTable[8];
	hwlib::cout << "The relative sector is: " << +RelativeSector << " Consisting out of these numbers: " << +PartitionTable[11] << +PartitionTable[10] << +PartitionTable[9] << +PartitionTable[8] << hwlib::endl;
	TotalSectors = ( PartitionTable[15] << 24 ) + ( PartitionTable[14] << 16 ) + ( PartitionTable[13] << 8 ) + PartitionTable[12];
}

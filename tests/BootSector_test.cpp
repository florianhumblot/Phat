#ifndef BPB_TEST
#define BPB_TEST
#include "test.hpp"
#include <iostream>
#include "../phat/BootSector.hpp"
class BPB_test : public BootSector {
	static const uint16_t address = 8192;
public:
	BPB_test( uint8_t data[16] ) : BootSector( data, address ) {}
	void test_constructor( TestSuite & TestSuite ) {
		TestSuite.assertEquals( BytesPerSector, ( uint16_t ) 512, "BytesPerSector" );
		TestSuite.assertEquals( SectorsPerCluster, ( uint8_t ) 8, "SectorsPerCluster" );
		TestSuite.assertEquals( ReserverdSectorCount, ( uint16_t ) 1304, "ReserverdSectorCount" );
		TestSuite.assertEquals( NumberOfFats, ( uint8_t ) 2, "NumberOfFats" );
		TestSuite.assertEquals( SectorsPerFat, ( uint32_t ) 7540, "SectorsPerFat" );
		TestSuite.assertEquals( RootCluster, ( uint32_t ) 2, "RootCluster" );
		TestSuite.assertEquals( RootEntryCount, ( uint16_t ) 0, "RootEntryCount" );
		TestSuite.assertEquals( FirstDataSector, ( uint32_t ) 24576, "FirstDataSector" );
		TestSuite.assertEquals( FatTablesAddress, ( uint32_t ) 9496, "FatTablesAddress" );
		TestSuite.assertEquals( PartitionStart, ( uint32_t ) 8192, "PartitionStart" );
		TestSuite.assertEquals( RootDirSectors, ( uint32_t ) 0, "RootDirSectors" );
	}

	void testClusterToSector( TestSuite & TestSuite ) {
		TestSuite.assertEquals( GetFirstSectorForCluster( 0 ), ( uint32_t ) 24560, "GetFirstSectorForCluster(0)" );
		TestSuite.assertEquals( GetFirstSectorForCluster( 6 ), ( uint32_t ) 24608, "GetFirstSectorForCluster(6)" );
	}
};

#endif
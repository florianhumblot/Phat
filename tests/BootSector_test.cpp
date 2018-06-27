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
		TestSuite.assertEquals( BytesPerSector, ( uint16_t ) 512 );
		TestSuite.assertEquals( SectorsPerCluster, ( uint8_t ) 8 );
		TestSuite.assertEquals( ReserverdSectorCount, ( uint16_t ) 1304 );
		TestSuite.assertEquals( NumberOfFats, ( uint8_t ) 2 );
		TestSuite.assertEquals( SectorsPerFat, ( uint32_t ) 7540 );
		TestSuite.assertEquals( RootCluster, ( uint32_t ) 2 );
		TestSuite.assertEquals( RootEntryCount, ( uint16_t ) 0 );
		TestSuite.assertEquals( FirstDataSector, ( uint32_t ) 24576 );
		TestSuite.assertEquals( FatTablesAddress, ( uint32_t ) 9496 );
		TestSuite.assertEquals( PartitionStart, ( uint32_t ) 8192 );
		TestSuite.assertEquals( RootDirSectors, ( uint32_t ) 0 );
	}

	void testClusterToSector( TestSuite & TestSuite ) {
		TestSuite.assertEquals( GetFirstSectorForCluster( 0 ), ( uint32_t ) 24560 );
		TestSuite.assertEquals( GetFirstSectorForCluster( 6 ), ( uint32_t ) 24808 );
	}
};

#endif
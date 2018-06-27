#include "test.hpp"
#include "hwlib.hpp"
#include "BootSector_test.cpp"
#include "DirectoryEntry_test.cpp"
#include "Partition_test.cpp"
#include <iostream>
#include <bitset>
#include "main.h"
int main() {
	TestSuite test = TestSuite();
	uint8_t data_[16] = { 0x02, 0x00, 0x08, 0x05, 0x18, 0x02, 0x00, 0x00, 0x1D, 0x74, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00 };
	BPB_test bootsectorTest = BPB_test( data_ );
	bootsectorTest.test_constructor( test );
	bootsectorTest.testClusterToSector( test );

	uint8_t data_short_entry[32] = { 0x54, 0x45, 0x53, 0x54, 0x20, 0x20, 0x20, 0x20, 0x54, 0x58, 0x54, 0x20, 0x18, 0x46, 0x86, 0x55,
		0xD9, 0x4C, 0xD9, 0x4C, 0x00, 0x00, 0x8B, 0x55, 0xD9, 0x4C, 0x08, 0x00, 0x24, 0x00, 0x00, 0x00 };

	uint8_t data_long_entry[32] = { 0x54, 0x45, 0x53, 0x54, 0x46, 0x4F, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x10, 0x00, 0x97, 0x77, 0x55,
		0xD9, 0x4C, 0xD9, 0x4C, 0x00, 0x00, 0x78, 0x55, 0xD9, 0x4C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 };
	hwlib::string<255> lfn = "test folder";

	DirectoryEntryTest sfn = DirectoryEntryTest( data_short_entry, 0 );
	DirectoryEntryLFNTest lfnDir = DirectoryEntryLFNTest( data_long_entry, lfn, 0 );

	sfn.testDirectoryEntry( test );
	lfnDir.testLFNDirectoryEntry( test );

	uint8_t partition_data[16] = { 0x00, 0x82, 0x03, 0x00, 0x0C, 0xFE, 0x7F, 0xE1, 0x00, 0x20, 0x00, 0x00, 0x00, 0x0C, 0x76, 0x00 };
	Partition_test partition = Partition_test( partition_data );

	partition.testPartition( test );

	std::cout << "Amount of tests: " << +test.getTestCount() << std::endl;
	std::cout << "Tests Passed: " << +test.getPassedTests() << std::endl;
	std::cout << "Failed tests: " << std::endl;
	for ( unsigned int i = 0; i < test.test.size(); i++ ) {
		std::cout << "lhs = " << test.test[i][0] << " rhs = " << test.test[i][1];
		std::cout << " lhs = " << std::bitset<32>( test.test[i][0] ) << " rhs = " << std::bitset<32>( test.test[i][1] ) << " Bitset equal: " << ( std::bitset<32>( test.test[i][0] ) == std::bitset<32>( test.test[i][1] ) ) << std::endl;
	}
}
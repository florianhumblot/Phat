#include "test.hpp"
#include "hwlib.hpp"
#include "BootSector_test.cpp"
#include <iostream>
#include <bitset>
int main() {
	TestSuite test = TestSuite();
	uint8_t data_[16] = { 0x02, 0x00, 0x08, 0x05, 0x18, 0x02, 0x00, 0x00, 0x1D, 0x74, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00 };
	BPB_test bootsectorTest = BPB_test( data_ );
	bootsectorTest.test_constructor( test );
	bootsectorTest.testClusterToSector( test );
	std::cout << "Amount of tests: " << +test.getTestCount() << std::endl;
	std::cout << "Tests Passed: " << +test.getPassedTests() << std::endl;
	std::cout << "Failed tests: " << std::endl;
	for ( unsigned int i = 0; i < test.test.size(); i++ ) {
		std::cout << "lhs = " << test.test[i][0] << " rhs = " << test.test[i][1];
		std::cout << " lhs = " << std::bitset<32>( test.test[i][0] ) << " rhs = " << std::bitset<32>( test.test[i][1] ) << " Bitset equal: " << ( std::bitset<32>( test.test[i][0] ) == std::bitset<32>( test.test[i][1] ) ) << std::endl;
	}
}
// ==========================================================================
//
// File      : MaterBootRecord_test.cpp
// Part of   : Phat - FAT32 reader - TestSuite
// Copyright : Florian Humblot - me@florianhumblot.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================
#ifndef MBR_TEST
#define MBR_TEST
#include "test.hpp"
#include <iostream>
#include "../phat/Partition.hpp"
#include "../phat/MasterBootRecord.hpp"
#include "hwlib.hpp"

class MBR_test : public MasterBootRecord {
public:
	MBR_test( Partition partitions[4] ) : MasterBootRecord( partitions[0], partitions[1], partitions[2], partitions[3] ) {}
	void testMBR(TestSuite & TestSuite) {
		TestSuite.assertEquals( isUsableDrive(), true, "MBR_test - isUsableDrive()" );
		TestSuite.assertEquals( getFatAddress( 0 ), ( uint32_t ) 8192, "MBR_test - FatAddress for 0 partition" );
		TestSuite.assertEquals( getFatAddress( 1 ), ( uint32_t ) 0, "MBR_test - FatAddress for 0 partition" );
		TestSuite.assertEquals( getFatAddress( 2 ), ( uint32_t ) 0, "MBR_test - FatAddress for 0 partition" );
		TestSuite.assertEquals( getFatAddress( 3 ), ( uint32_t ) 0, "MBR_test - FatAddress for 0 partition" );
	}
};
class Empty_MBR_test : public MasterBootRecord {
public:
	Empty_MBR_test() : MasterBootRecord() {}
	void testMBR( TestSuite & TestSuite ) {
		TestSuite.assertEquals( isUsableDrive(), false, "Empty_MBR_test - isUsableDrive()" );
	}
};
#endif
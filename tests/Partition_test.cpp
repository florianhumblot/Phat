// ==========================================================================
//
// File      : Partition_test.cpp
// Part of   : Phat - FAT32 reader - TestSuite
// Copyright : Florian Humblot - me@florianhumblot.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================
#ifndef PARTITION_TEST
#define PARTITION_TEST
#include "test.hpp"
#include <iostream>
#include "../phat/MasterBootRecord.hpp"
#include "hwlib.hpp"

class Partition_test : public Partition {
public:
	Partition_test( uint8_t data[16] ) : Partition( data ) {}
	void testPartition( TestSuite & TestSuite ) {
		TestSuite.assertEquals( TestSuite.as_integer( bootable ), TestSuite.as_integer( BootIndicator::NOBOOT ), "Partition - bootable" );
		TestSuite.assertEquals( StartingHead, ( uint8_t ) 130, "Partition - StartingHead" );
		TestSuite.assertEquals( StartingSectCylinder, ( uint16_t ) 3, "Partition - StartingSectCylinder" );
		TestSuite.assertEquals( TestSuite.as_integer( Sys_ID ), TestSuite.as_integer( SystemID::EXT_FAT32_LBA ), "Partition - Sys_ID" );
		TestSuite.assertEquals( EndingHead, ( uint8_t ) 254, "Partition - EndingHead" );
		TestSuite.assertEquals( EndingSectCylinder, ( uint16_t ) 57727, "Partition - EndingSectCylinder" );
		TestSuite.assertEquals( RelativeSector, ( uint32_t ) 8192, "Partition - RelativeSector" );
		TestSuite.assertEquals( TotalSectors, ( uint32_t ) 7736320, "Partition - TotalSectors" );
		TestSuite.assertEquals( isFAT32(), true, "Partition - isFAT32()" );
		TestSuite.assertEquals( getFatTableLocation(), ( uint32_t ) 8192, "Partition - getFatTableLocation()" );
		TestSuite.assertEquals( getAmountOfSectors(), ( uint32_t ) 7736320, "Partition - getAmountOfSectors()" );
	}
};

#endif
// ==========================================================================
//
// File      : MasterBootRecord.cpp
// Part of   : Phat - FAT32 reader
// Copyright : Florian Humblot - me@florianhumblot.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================
#include "MasterBootRecord.hpp"

MasterBootRecord::MasterBootRecord( Partition partition_table, Partition partition_table2, Partition partition_table3, Partition partition_table4 ) {
	partition_tables[0] = partition_table;
	partition_tables[1] = partition_table2;
	partition_tables[2] = partition_table3;
	partition_tables[3] = partition_table4;
	usableDrive = ( partition_tables[0].isFAT32() || partition_tables[1].isFAT32() || partition_tables[2].isFAT32() || partition_tables[3].isFAT32() );

}

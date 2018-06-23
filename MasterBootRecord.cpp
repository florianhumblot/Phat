#include "MasterBootRecord.hpp"

MasterBootRecord::MasterBootRecord( Partition partition_table, Partition partition_table2, Partition partition_table3, Partition partition_table4 ) {
	partition_tables[0] = partition_table;
	partition_tables[1] = partition_table2;
	partition_tables[2] = partition_table3;
	partition_tables[3] = partition_table4;
	usableDrive = ( partition_tables[0].isFAT32() || partition_tables[1].isFAT32() || partition_tables[2].isFAT32() || partition_tables[3].isFAT32() );

}

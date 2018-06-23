#include "DirectoryEntry.hpp"



DirectoryEntry::DirectoryEntry() {}

DirectoryEntry::DirectoryEntry( uint8_t data[32] ) {
	FileName = "";
	FileName.append( data[0] );
	FileName.append( data[1] );
	FileName.append( data[2] );
	FileName.append( data[3] );
	FileName.append( data[4] );
	FileName.append( data[5] );
	FileName.append( data[6] );
	FileName.append( data[7] );
	Extention = "";
	Extention.append( data[8] );
	Extention.append( data[9] );
	Extention.append( data[10] );
	FAT_ATTR_TYPE = data[11];
	CreatedTime10ms = data[13];
	CreatedTime = ( data[15] << 8 ) + data[14];
	CreatedDate = ( data[17] << 8 ) + data[16];
	AccessDate = ( data[19] << 8 ) + data[18];
	UpdateTime = ( data[23] << 8 ) + data[22];
	UpdateDate = ( data[25] << 8 ) + data[24];
	FirstLogicalCluster = ( data[21] << 24 ) + (data[20] << 16) + (data[27] << 8) + data[26];
	FileSize = ( data[31] << 24 ) + ( data[30] << 16 ) + ( data[29] << 8 ) + data[28];
}

DirectoryEntry::DirectoryEntry( uint8_t data[32], hwlib::string<0> & lfn ){
	FileName = lfn;
	Extention = "";
	Extention.append( data[8] );
	Extention.append( data[9] );
	Extention.append( data[10] );
	FAT_ATTR_TYPE = data[11];
	CreatedTime10ms = data[13];
	CreatedTime = ( data[15] << 8 ) + data[14];
	CreatedDate = ( data[17] << 8 ) + data[16];
	AccessDate = ( data[19] << 8 ) + data[18];
	UpdateTime = ( data[23] << 8 ) + data[22];
	UpdateDate = ( data[25] << 8 ) + data[24];
	FirstLogicalCluster = ( data[21] << 24 ) + ( data[20] << 16 ) + ( data[27] << 8 ) + data[26];
	FileSize = ( data[31] << 24 ) + ( data[30] << 16 ) + ( data[29] << 8 ) + data[28];
}


DirectoryEntry::~DirectoryEntry() {}

#pragma once

#include "hwlib.hpp"
class DirectoryEntry {
private:
	hwlib::string<255> FileName;
	hwlib::string<3> Extention;
	uint8_t attributes;
	uint8_t FAT_ATTR_TYPE;
	uint8_t CreatedTime10ms;
	uint16_t CreatedTime;
	uint16_t CreatedDate;
	uint16_t AccessDate;
	uint16_t UpdateTime;
	uint16_t UpdateDate;
	uint32_t FirstLogicalCluster;
	uint32_t FileSize;

public:
	DirectoryEntry();
	DirectoryEntry( uint8_t data[32] );
	DirectoryEntry( uint8_t data[32], hwlib::string<0> & lfn );
	~DirectoryEntry();
	uint32_t getFirstLogicalCluster() {
		return FirstLogicalCluster;
	}
	uint32_t getFileSize() {
		return FileSize;
	}
	bool isFile() {
		return FAT_ATTR_TYPE == 32;
	}
	friend hwlib::ostream & operator<< ( hwlib::ostream & stream, DirectoryEntry DE ) {
		stream << hwlib::dec;
		stream << "Directory: " << DE.FileName << "\n";
		stream << "Extention: " << DE.Extention << "\n";
		stream << "attributes: " << DE.attributes << "\n";
		stream << "FAT_ATTR_TYPE: " << DE.FAT_ATTR_TYPE << "\n";
		stream << "CreatedTime10ms: " << DE.CreatedTime10ms << "\n";
		stream << "CreatedTime: " << ( ( DE.CreatedTime & 0x7800 ) >> 11 ) << ':' << ( ( DE.CreatedTime & 0x7E0 ) >> 5 ) << ':' << ( DE.CreatedTime & 0x1f ) << "\n";
		stream << "CreatedDate: " << ( DE.CreatedDate & 0x1f ) << '-' << ( ( DE.CreatedDate & 0x1E0 ) >> 5 ) << '-' << ( ( ( DE.CreatedDate & 0xFE00 ) >> 9 ) + 1980 ) << "\n";
		stream << "AccessDate: " << ( DE.AccessDate & 0x1f ) << '-' << ( ( DE.AccessDate & 0x1E0 ) >> 5 ) << '-' << ( ( ( DE.AccessDate & 0xFE00 ) >> 9 ) + 1980 ) << "\n";
		stream << "UpdateTime: " << ( ( DE.UpdateTime & 0x7800 ) >> 11 ) << ':' << ( ( DE.UpdateTime & 0x7E0 ) >> 5 ) << ':' << ( DE.UpdateTime & 0x1f ) << "\n";
		stream << "UpdateDate: " << ( DE.UpdateDate & 0x1f ) << '-' << ( ( DE.UpdateDate & 0x1E0 ) >> 5 ) << '-' << ( ( ( DE.UpdateDate & 0xFE00 ) >> 9 ) + 1980 ) << "\n";
		stream << "FirstLogicalCluster: " << DE.FirstLogicalCluster << "\n";
		stream << "FileSize: " << DE.FileSize << "\n";
		return stream;
	}
};


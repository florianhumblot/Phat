#pragma once
#ifndef PHAT_DirectoryEntry_HPP
#define PHAT_DirectoryEntry_HPP
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
	bool LFN;
	uint8_t parent;

public:
	DirectoryEntry();
	DirectoryEntry( uint8_t data[32], uint8_t parentListingIndex = 0 );
	DirectoryEntry( uint8_t data[32], hwlib::string<0> & lfn, uint8_t parentListingIndex = 0 );
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

	void print_table_headers();
	uint8_t getParentIndex() {
		return parent;
	}
	bool isNotARealDir() {
		return ( FileName == ".." || FileName == "." );
	}

	bool isADirectory() {
		return FAT_ATTR_TYPE == 16;
	}

	friend hwlib::ostream & operator<< ( hwlib::ostream & stream, DirectoryEntry DE ) {
		stream << hwlib::dec;
		hwlib::string <22> Pad = "";

		if ( DE.LFN ) {
			for ( int8_t i = 21; ( i - ( DE.FileName.length() ) ) > 0; i-- ) {
				Pad.append( ' ' );
			}
			stream << '/' << DE.FileName << Pad;
		} else {
			for ( int8_t i = 21; ( i - ( DE.FileName.length() + DE.Extention.length() ) ) > 0; i-- ) {
				Pad.append( ' ' );
			}
			stream << '/' << DE.FileName << '.' << DE.Extention << Pad;
		}

		stream << ( ( DE.FAT_ATTR_TYPE == 32 ) ? "File  " : "Folder" ) << "\t\t   ";
		stream << ( DE.CreatedDate & 0x1f ) << '-' << ( ( DE.CreatedDate & 0x1E0 ) >> 5 ) << '-' << ( ( ( DE.CreatedDate & 0xFE00 ) >> 9 ) + 1980 ) << " ";
		stream << ( ( DE.CreatedTime & 0xF800 ) >> 11 ) << ':' << ( ( DE.CreatedTime & 0x7E0 ) >> 5 ) << ':' << ( DE.CreatedTime & 0x1f ) << " ";
		stream << "   " << ( DE.UpdateDate & 0x1f ) << '-' << ( ( DE.UpdateDate & 0x1E0 ) >> 5 ) << '-' << ( ( ( DE.UpdateDate & 0xFE00 ) >> 9 ) + 1980 ) << " ";
		stream << ( ( DE.UpdateTime & 0xF800 ) >> 11 ) << ':' << ( ( DE.UpdateTime & 0x7E0 ) >> 5 ) << ':' << ( DE.UpdateTime & 0x1f ) << " \t  ";
		stream << ( DE.FileSize / 1024 ) << "KB" << " ";
		stream << " (parent: " << DE.parent << ")";
		return stream;
	}
};

#endif
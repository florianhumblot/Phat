// ==========================================================================
//
// File      : DirectoryEntry.hpp
// Part of   : Phat - FAT32 reader
// Copyright : Florian Humblot - me@florianhumblot.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
///file DirectoryEntry.hpp
/// \brief Datastructure for storing directory entries.
/// \details
/// This class contains all necessary information about a directory on a FAT32 system.
/// It parses a 16 byte directory entry on the  sd card.

#ifndef PHAT_DirectoryEntry_HPP
#define PHAT_DirectoryEntry_HPP
#include "hwlib.hpp"
class DirectoryEntry {
protected:
	hwlib::string<255> FileName;
	hwlib::string<3> Extension;
	uint8_t FAT_ATTR_TYPE;
	uint8_t CreatedTime10ms;
	uint16_t CreatedTime;
	uint16_t CreatedDate;
	uint16_t AccessDate;
	uint16_t UpdateTime;
	uint16_t UpdateDate;
	uint32_t FirstLogicalCluster;
	uint32_t FileSize;
	/// \brief Set to true if the file has a Long File Name or not.
	bool LFN;
	/// \brief FileNumbe index of the parent in directoryListing of SDDevice
	uint8_t parent;

public:
	/// \brief Empty constructor
	DirectoryEntry();
	/// \brief Constructor for entries with a small (or standard) sized filename
	DirectoryEntry( uint8_t data[32], uint8_t parentListingIndex = 0 );
	/// \brief Constructor for entries with a long file name.
	DirectoryEntry( uint8_t data[32], hwlib::string<0> & lfn, uint8_t parentListingIndex = 0 );

	/// \brief Gets the first logical cluster number of the entry.
	/// \details this number is not usable on it's own, you need to call BootSector::GetFirstSectorForCluster() to get a block address on the SD card.
	uint32_t getFirstLogicalCluster() {
		return FirstLogicalCluster;
	}
	
	/// \brief Getter for the filesize in bytes
	uint32_t getFileSize() {
		return FileSize;
	}

	/// \brief Checks whether this entry is a file or not
	bool isFile() {
		return FAT_ATTR_TYPE == 32;
	}


	/// \brief prints table headers
	/// \details This function should be called before a cout of one or more directory entries to better understand what the data represents.
	void print_table_headers();

	/// \brief Gets the parent index
	/// \details the index is the index of the parent entry in the SDDevice::directoryListing array
	uint8_t getParentIndex() {
		return parent;
	}

	/// \brief Checks whether this entry is a directory or not
	bool isADirectory() {
		return FAT_ATTR_TYPE == 16;
	}


	/// \brief ostream operator for printing a directory entry.
	/// \details formats some of the data and prints it in a readable format
	friend hwlib::ostream & operator<< ( hwlib::ostream & stream, DirectoryEntry DE ) {
		stream << hwlib::dec;
		hwlib::string <22> Pad = "";

		if ( DE.LFN ) {
			for ( int8_t i = 21; ( i - ( DE.FileName.length() ) ) > 0; i-- ) {
				Pad.append( ' ' );
			}
			stream << '/' << DE.FileName << Pad;
		} else {
			for ( int8_t i = 21; ( i - ( DE.FileName.length() + DE.Extension.length() ) ) > 0; i-- ) {
				Pad.append( ' ' );
			}
			stream << '/' << DE.FileName << '.' << DE.Extension << Pad;
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
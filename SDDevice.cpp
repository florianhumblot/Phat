//
// Created by djrel on 5/26/2018.
//

#include "hwlib.hpp"
#include "SDDevice.hpp"
#include "MasterBootRecord.hpp"
#include "Partition.hpp"
#include "BootSector.hpp"
#include "DirectoryEntry.hpp"
#include <array>

int SDDevice::init_card() {
	std::array<uint8_t, 32> buffer = {};
	GenerateCRCTable();
	SS.set( true );
	wait_bytes( 10 );
	SS.set( false );
	hwlib::wait_ms( 20 );
	SS.set( false );
	hwlib::wait_ms( 1 );
	execute_command( CMD_GO_IDLE_STATE, 0x00000000, false );
	readBytes( 8, true, buffer.begin() );
	SS.set( true );
	wait_bytes( 1 );
	SS.set( false );
	execute_command( CMD_SEND_IF_COND, 0x000001AA, false );
	readBytes( 8, true, buffer.begin() );
	SS.set( true );
	wait_bytes( 1 );
	do {
		SS.set( false );
		execute_command( CMD_APP_CMD, 0x0, false );
		readBytes( 8, true, buffer.begin() );
		execute_command( ACMD_SD_SEND_OP_COND, 0x40000000, false );
		readBytes( 8, true, buffer.begin() );
		SS.set( true );
	} while ( buffer[0] != 0x0 );
	SS.set( true );
	execute_command( CMD_READ_OCR, 0x0000000, false );
	readBytes( 8, true, buffer.begin() );
	SS.set( false );
	execute_command( CMD_SET_BLOCKLEN, 0x0000200, false );
	readBytes( 8, true, buffer.begin() );
	SS.set( true );
	std::array<uint8_t, 512> block = {};
	uint32_t address = 0x00000000;
	SS.set( false );
	readBlock( block, address );
	SS.set( true );
	uint8_t data_[16];
	for ( auto & i : data_ ) { i = 0; }
	Partition Partitions[4];
	uint8_t j = 0;
	for ( uint_fast8_t i = 446; i < 462; i++ ) {
		data_[j] = block[i];
		j++;
	}
	Partitions[0] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }
	j = 0;
	for ( uint_fast8_t i = 462; i < 478; i++ ) {
		data_[j] = block[i];
	}
	Partitions[1] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }
	j = 0;
	for ( uint_fast8_t i = 478; i < 494; i++ ) {
		data_[j] = block[i];
	}
	Partitions[2] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }
	j = 0;
	for ( uint_fast8_t i = 494; i < 510; i++ ) {
		data_[j] = block[i];
	}
	Partitions[3] = Partition( data_ );
	for ( auto & i : data_ ) { i = 0; }

	hwlib::wait_ms( 1 );
	MBR = MasterBootRecord( Partitions[0], Partitions[1], Partitions[2], Partitions[3] );
	address = MBR.getFatAddress( 0 );
	SS.set( false );
	readBlock( block, address );
	SS.set( true );
	uint8_t BPBinfo[16];
	BPBinfo[0] = block[12];
	BPBinfo[1] = block[11];
	BPBinfo[2] = block[13];
	BPBinfo[3] = block[15];
	BPBinfo[4] = block[14];
	BPBinfo[5] = block[16];
	BPBinfo[6] = block[39];
	BPBinfo[7] = block[38];
	BPBinfo[8] = block[37];
	BPBinfo[9] = block[36];
	BPBinfo[10] = block[47];
	BPBinfo[11] = block[46];
	BPBinfo[12] = block[45];
	BPBinfo[13] = block[44];
	BPBinfo[14] = block[18];
	BPBinfo[15] = block[17];
	BPB = BootSector( BPBinfo, address );
	hwlib::cout << "Got BPB: " << BPB << hwlib::endl;
	return 0;
}

void SDDevice::wait_bytes( uint_fast8_t wait_amount = 1 ) {
	readBytes( wait_amount, true, nullptr );
}

//int SDDevice::getDirectoryListing( uint8_t filenumber ) {
//	hwlib::cout << "Getting Directory listing! " << hwlib::endl;
//	std::array<uint8_t, 512> block;
//	uint32_t address = 0x00;
//	hwlib::string<255> lfn = "";
//	bool finishedReading = false;
//	if ( filenumber == 0 ) {
//		address = BPB.GetRootDirAddress();
//	} else {
//		address = BPB.GetFirstSectorForCluster( directoryListing[filenumber].getFirstLogicalCluster() );
//		if ( directoryListing[filenumber].isFile() ) {
//			//print the file and return;
//			printTextFile( address, directoryListing[filenumber].getFileSize() );
//			getDirectoryListing();
//			return 0;
//		}
//
//	}
//
//	bool readingLFN = false;
//	while ( finishedReading == false && currentDirectoryIndex <= 100 && addressHasBeenProcessedAlready( address ) == false ) {
//		SS.set( false );
//		readBlock( block, address );
//
//		SS.set( true );
//		for ( uint_fast8_t i = 0; i < 16; i++ ) {
//			if ( filenumber != 0 ) {
//				//hwlib::cout << "Reading entry number: " << +(i*36) << hwlib::endl;
//				//hwlib::cout << "Attr type:" << block[( 36 * i ) + 11] << hwlib::endl;
//			}
//			if ( block[( 32 * i )] == 0x00 &&
//				 block[( 32 * i ) + 1] == 0x00 &&
//				 block[( 32 * i ) + 2] == 0x00 &&
//				 block[( 32 * i ) + 3] == 0x00 &&
//				 block[( 32 * i ) + 4] == 0x00 &&
//				 block[( 32 * i ) + 5] == 0x00 &&
//				 block[( 32 * i ) + 6] == 0x00 &&
//				 block[( 32 * i ) + 7] == 0x00
//				 ) {
//				finishedReading = true;
//				break;
//			}
//			if ( block[( 32 * i ) + 11] == 15 ) {
//				//hwlib::cout << "Reading LFN: true" << hwlib::endl;
//				readingLFN = true;
//
//				uint8_t AmountOfLFN = block[( 32 * i )] & 0x3F;
//				for ( uint8_t j = AmountOfLFN - 1; j > 0; j-- ) {
//					//this is a LFN entry;
//					//Get the first part of the (long) name
//					lfn.append( block[( 32 * ( i + j ) ) + 1] );
//					lfn.append( block[( 32 * ( i + j ) ) + 3] );
//					lfn.append( block[( 32 * ( i + j ) ) + 5] );
//					lfn.append( block[( 32 * ( i + j ) ) + 7] );
//					lfn.append( block[( 32 * ( i + j ) ) + 9] );
//					//Get the second part of the (long) name
//					lfn.append( block[( 32 * ( i + j ) ) + 14] );
//					lfn.append( block[( 32 * ( i + j ) ) + 16] );
//					lfn.append( block[( 32 * ( i + j ) ) + 18] );
//					lfn.append( block[( 32 * ( i + j ) ) + 20] );
//					lfn.append( block[( 32 * ( i + j ) ) + 22] );
//					lfn.append( block[( 32 * ( i + j ) ) + 24] );
//					// Get the last part of the (long) name
//					lfn.append( block[( 32 * ( i + j ) ) + 28] );
//					lfn.append( block[( 32 * ( i + j ) ) + 30] );
//
//				}
//
//				//this is a LFN entry;
//				//Get the first part of the (long) name
//				lfn.append( block[( 32 * i ) + 1] );
//				lfn.append( block[( 32 * i ) + 3] );
//				lfn.append( block[( 32 * i ) + 5] );
//				lfn.append( block[( 32 * i ) + 7] );
//				lfn.append( block[( 32 * i ) + 9] );
//				//Get the second part of the (long) name
//				lfn.append( block[( 32 * i ) + 14] );
//				lfn.append( block[( 32 * i ) + 16] );
//				lfn.append( block[( 32 * i ) + 18] );
//				lfn.append( block[( 32 * i ) + 20] );
//				lfn.append( block[( 32 * i ) + 22] );
//				lfn.append( block[( 32 * i ) + 24] );
//				// Get the last part of the (long) name
//				lfn.append( block[( 32 * i ) + 28] );
//				lfn.append( block[( 32 * i ) + 30] );
//
//				//Skip the LFN blocks;
//				i += AmountOfLFN - 1;
//			}
//			if ( block[( 32 * i ) + 11] == 16 || block[( 32 * i ) + 11] == 32 ) {
//				// We found the directory entry
//				uint8_t data_[32];
//				for ( uint_fast8_t j = 0; j < 32; j++ ) {
//					data_[j] = block[( 32 * i ) + j];
//				}
//				if ( readingLFN ) {
//					readingLFN = false;
//					directoryListing[currentDirectoryIndex] = DirectoryEntry( data_, lfn );
//					lfn = "";
//				} else {
//					directoryListing[currentDirectoryIndex] = DirectoryEntry( data_ );
//				}
//				currentDirectoryIndex++;
//			} else if ( block[( 32 * i ) + 11] == 22 ) {
//				readingLFN = false;
//				lfn = "";
//			}
//		}
//		//Jump to the next block;
//		address++;
//	}
//	directoryListing[0].print_table_headers();
//	uint16_t startIndex = 0;
//	if ( filenumber != 0 ) {
//		startIndex = filenumber + 1;
//	}
//	for ( uint16_t i = startIndex; i < currentDirectoryIndex; i++ ) {
//		hwlib::cout << i << ' ' << directoryListing[i] << hwlib::endl;
//
//	}
//	return 0;
//
//}

void SDDevice::printTextFile( uint32_t address, uint32_t size ) {
	std::array<uint8_t, 512> block;

	hwlib::cout << "---------------------------------------START TEXT FILE---------------------------------------\n\n" << hwlib::endl;


	for ( uint_fast16_t h = 0; h < ( ( size / 512 ) + 1 ); h++ ) {
		for ( auto & i : block ) { i = 0; }
		SS.set( false );
		readBlock( block, address );
		printBlock( block, address );
		SS.set( true );
		address++;
	}


	hwlib::cout << "\n\n---------------------------------------END TEXT FILE---------------------------------------" << hwlib::endl;
}



int SDDevice::execute_command( SDDevice::SupportedCommands command, uint32_t arguments, bool isAcmd ) {

	uint8_t CRC = 0xff;
	uint8_t data_out[6] = {
			static_cast< uint8_t >( ( command | 0x40 ) ),
			static_cast< uint8_t >( ( arguments >> 24 ) ),
			static_cast< uint8_t >( ( arguments >> 16 ) ),
			static_cast< uint8_t >( ( arguments >> 8 ) ),
			static_cast< uint8_t >( ( arguments ) ),
			CRC
	};
	data_out[5] = ( getCRC( data_out, 5 ) << 1 ) | 1;
	if ( isAcmd ) {
		execute_command( CMD_APP_CMD, 0x0, false );
	}
	spi_bus.write_and_read( hwlib::pin_out_dummy, 6, data_out, data.begin() );
	wait_bytes( 1 );
	return 0;
}
void SDDevice::readBlock( std::array<uint8_t, 512>& block, uint32_t address ) {
	//reset block
	for ( uint_fast16_t i = 0; i < 512; i++ ) {
		block[i] = 0x00;
	}
	wait_bytes( 8 );
	execute_command( CMD_READ_SINGLE_BLOCK, address, false );
	do {
		// Wait for the command to be accepted
		readBytes( 1, true, block.begin() );
	} while ( block[0] != 0x00 );

	do {
		// Wait for the transfer to begin, which is when we receive 0xFE
		readBytes( 1, true, block.begin() );
	} while ( block[0] != 0xFE );

	for ( uint_fast16_t i = 0; i <= 496; i += 16 ) {
		readBytes( 16, true, block.begin() + i );
		address += 0x10;
	}
	// Read CRC bits
	uint8_t CRC_BITS[2] = { 0x00 };
	readBytes( 2, true, CRC_BITS );
	// And finish off by waiting 10 bytes.
	wait_bytes( 10 );
}

void SDDevice::printBlock( std::array<uint8_t, 512>& block, uint32_t start_address ) {
	for ( uint_fast16_t i = 0; i <= 496; i += 16 ) {
		print_text( 16, block.begin() + ( start_address % 512 ), start_address );
		start_address += 0x10;
	}
}

SDDevice::SDDevice( hwlib::pin_out &MOSI, hwlib::pin_out &SS,
					hwlib::pin_out &SCLK, hwlib::pin_in &MISO )
	: MOSI( MOSI ),
	SS( SS ),
	SCLK( SCLK ),
	MISO( MISO ),
	spi_bus( hwlib::spi_bus_bit_banged_sclk_mosi_miso( SCLK, MOSI, MISO ) ) {}

void SDDevice::readBytes( uint8_t amount, bool sendOnes, uint8_t *buffer ) {
	if ( sendOnes ) {
		hwlib::wait_us( 15 );
		uint8_t data_out[amount];
		for ( auto &c : data_out ) { c = 0xFF; }
		spi_bus.write_and_read( hwlib::pin_out_dummy, amount, data_out, buffer );
		return;
	}
	spi_bus.write_and_read( hwlib::pin_out_dummy, amount, nullptr, buffer );
}

void SDDevice::print_binary( uint8_t size, uint8_t *data, bool command = false ) {
	if ( command ) {
		hwlib::cout << hwlib::setw( 8 ) << hwlib::setfill( ' ' ) << "COMMAND(CMD" << hwlib::dec << +( data[0] & ~( 1UL << 6 ) ) << "): ";
	} else {
		hwlib::cout << hwlib::setw( 9 ) << hwlib::setfill( ' ' ) << "DATA: ";
	}


	for ( uint_fast8_t i = 0; i < size; i++ ) {
		hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << "0x" << +data[i] << " ";
	}
	hwlib::cout << hwlib::endl << hwlib::endl;
}

void SDDevice::print_shifted_value( uint32_t arguments ) {

	hwlib::cout << hwlib::bin << "The shifted values of: " << +arguments << " are: " << hwlib::endl;

	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( ( arguments >> 24 ) ) << " ";
	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( ( arguments >> 16 ) ) << " ";
	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( ( arguments >> 8 ) ) << " ";
	hwlib::cout << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << hwlib::hex << +static_cast< uint8_t >( arguments ) << hwlib::endl << hwlib::endl;
}

void SDDevice::print_text( uint16_t size, uint8_t * data, uint32_t address ) {
	for ( uint_fast8_t j = 0; j < size; j++ ) {
		if ( data[j] != 0x00 ) {
			hwlib::cout << ( char ) data[j];
		}
	}
}

void SDDevice::debug_block( uint16_t size, uint8_t * data ) {
	hwlib::cout << "The text: " << hwlib::endl;
	for ( uint16_t i = 0; i < size; i += 8 ) {
		hwlib::cout << "Hex: ";
		for ( uint8_t j = 0; j < 8; j++ ) {
			hwlib::cout << "0x" << hwlib::hex << hwlib::setw( 2 ) << hwlib::setfill( '0' ) << +data[i + j] << " ";
		}
		hwlib::cout << "\tChar value: ";
		for ( uint8_t j = 0; j < 8; j++ ) {
			hwlib::cout << ( char ) data[i + j] << " ";

		}
		hwlib::cout << hwlib::endl;
	}
	hwlib::cout << hwlib::endl << hwlib::endl;
}

void SDDevice::generateDirectoryListing( uint16_t parent ) {
	std::array<uint8_t, 512> block;
	uint32_t address = 0x00;
	hwlib::string <255> lfn = "";
	bool finishedReading = false;
	bool readingLFN = false;

	if ( parent == 0 ) {
		address = BPB.GetRootDirAddress();
	} else {
		address = BPB.GetFirstSectorForCluster( directoryListing[parent].getFirstLogicalCluster() );
	}

	while ( finishedReading == false && currentDirectoryIndex <= 100 ) {
		SS.set( false );
		readBlock( block, address );
		SS.set( true );

		for ( uint_fast8_t i = 0; i < 16; i++ ) {
			// Check if the next 8 bytes are 0, if they are, the cluster contains no more data.
			if ( block[( 32 * i )] == 0x00 &&
				 block[( 32 * i ) + 1] == 0x00 &&
				 block[( 32 * i ) + 2] == 0x00 &&
				 block[( 32 * i ) + 3] == 0x00 &&
				 block[( 32 * i ) + 4] == 0x00 &&
				 block[( 32 * i ) + 5] == 0x00 &&
				 block[( 32 * i ) + 6] == 0x00 &&
				 block[( 32 * i ) + 7] == 0x00
				 ) {
				finishedReading = true;
				break;
			}

			// if the 11th byte has a value of 15, this is a long file name entry.
			if ( block[( 32 * i ) + 11] == 15 ) {
				readingLFN = true;
				// Check the amount of LFN entries.
				uint8_t AmountOfLFN = block[( 32 * i )] & 0x3F;

				// Get each entry, from the last part back to where we are now in the cluster
				for ( uint8_t j = AmountOfLFN - 1; j > 0; j-- ) {
					// Get the first part of the (long) name
					lfn.append( block[( 32 * ( i + j ) ) + 1] );
					lfn.append( block[( 32 * ( i + j ) ) + 3] );
					lfn.append( block[( 32 * ( i + j ) ) + 5] );
					lfn.append( block[( 32 * ( i + j ) ) + 7] );
					lfn.append( block[( 32 * ( i + j ) ) + 9] );
					// Get the second part of the (long) name
					lfn.append( block[( 32 * ( i + j ) ) + 14] );
					lfn.append( block[( 32 * ( i + j ) ) + 16] );
					lfn.append( block[( 32 * ( i + j ) ) + 18] );
					lfn.append( block[( 32 * ( i + j ) ) + 20] );
					lfn.append( block[( 32 * ( i + j ) ) + 22] );
					lfn.append( block[( 32 * ( i + j ) ) + 24] );
					// Get the last part of the (long) name
					lfn.append( block[( 32 * ( i + j ) ) + 28] );
					lfn.append( block[( 32 * ( i + j ) ) + 30] );

				}
				// Get our current entry, which is always the last part of the entries
				// Get the first part of the (long) name
				lfn.append( block[( 32 * i ) + 1] );
				lfn.append( block[( 32 * i ) + 3] );
				lfn.append( block[( 32 * i ) + 5] );
				lfn.append( block[( 32 * i ) + 7] );
				lfn.append( block[( 32 * i ) + 9] );
				// Get the second part of the (long) name
				lfn.append( block[( 32 * i ) + 14] );
				lfn.append( block[( 32 * i ) + 16] );
				lfn.append( block[( 32 * i ) + 18] );
				lfn.append( block[( 32 * i ) + 20] );
				lfn.append( block[( 32 * i ) + 22] );
				lfn.append( block[( 32 * i ) + 24] );
				// Get the last part of the (long) name
				lfn.append( block[( 32 * i ) + 28] );
				lfn.append( block[( 32 * i ) + 30] );

				// Skip the LFN blocks we just processed;
				i += AmountOfLFN - 1;
			}
			// if the 11th byte has a value of 16, it is a directory, if it has a value of 32, it is a file.
			// Both are considered files, so they're processed the same.
			if ( block[( 32 * i ) + 11] == 16 || block[( 32 * i ) + 11] == 32 ) {
				uint8_t data_[32];
				for ( uint_fast8_t j = 0; j < 32; j++ ) {
					data_[j] = block[( 32 * i ) + j];
				}
				if ( readingLFN ) {
					readingLFN = false;
					directoryListing[currentDirectoryIndex] = DirectoryEntry( data_, lfn, parent );
					lfn = "";
				} else {
					directoryListing[currentDirectoryIndex] = DirectoryEntry( data_, parent );
				}
				currentDirectoryIndex++;
			}
			// if the 11th byte has a value of 22, it is a system partition, no need to process it and so we skip it.
			else if ( block[( 32 * i ) + 11] == 22 ) {

				readingLFN = false;
				lfn = "";
			}
		}
		//We're done with the current block, go to the next.
		address++;
	}
	hwlib::cout << "Generated root listing" << hwlib::endl;

	for ( uint16_t i = parent + 1; i < currentDirectoryIndex + 1; i++ ) {
		if ( !directoryListing[i].isFile() && directoryListing[i].isNotARealDir() ) {
			//hwlib::cout << "Found a directory, generating now." << hwlib::endl;
			hwlib::cout << directoryListing[i] << hwlib::endl;
			generateDirectoryListing( i );
		}
	}
	if ( parent == 0 ) {
		for ( uint16_t i = 1; i < (currentDirectoryIndex + 1); i++ ) {
			hwlib::cout << "I : " << i << "\n" << directoryListing[i]<<hwlib::endl;
		}
	}
}

void SDDevice::printDirectoryListing( uint16_t ofDirectory ) {
	directoryListing[0].print_table_headers();
	for ( uint16_t i = 0; i <= currentDirectoryIndex; i++ ) {
		if ( directoryListing[i].getParentIndex() == ofDirectory ) {
			hwlib::cout << "\t" << i << "\t\t" << directoryListing[i] << hwlib::endl;
		}
	}
}

bool SDDevice::filenumberIsADirectory( uint16_t filenumber ) {
	return !directoryListing[filenumber].isFile();
}
